#include "gdl/resources/memory/memoryPool.h"


#include "gdl/base/Exception.h"
#include "gdl/base/functions/isPowerOf2.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>

namespace GDL
{

MemoryPool::MemoryPool(size_t elementSize, U32 numElements, size_t alignment)
    : mElementSize{elementSize}
    , mAlignment{alignment}
    , mNumElements{numElements}
    , mNumFreeElements{numElements}
    , mMemoryStart{nullptr}
    , mFirstFreeElement{nullptr}
    , mLastFreeElement{nullptr}
    , mMemory{nullptr}
{
    std::lock_guard<std::mutex> lock(mMutex);
    CheckConstructionParameters();
}



MemoryPool::~MemoryPool()
{
    std::lock_guard<std::mutex> lock(mMutex);
    assert(IsInitialized() == false &&
           "Deinitialize the memory pool before destruction - If you did, there might have been an exception");
}



void* MemoryPool::Allocate(size_t size)
{
    std::lock_guard<std::mutex> lock(mMutex);
    DEV_EXCEPTION(IsInitialized() == false, "Memory pool not initialized");
    DEV_EXCEPTION(size > mElementSize, "Allocation size is larger than a pool element.");
    EXCEPTION(mFirstFreeElement == nullptr, "No more memory available.");

    void* allocatedMemory = mFirstFreeElement;

    mFirstFreeElement = ReadListEntry(mFirstFreeElement);

    // no more memory left, so here is no last free either
    if (mFirstFreeElement == nullptr)
        mLastFreeElement = nullptr;

    --mNumFreeElements;
    return allocatedMemory;
}



void MemoryPool::Deallocate(void* address)
{
    std::lock_guard<std::mutex> lock(mMutex);

#if !(defined(NDEBUG) && defined(NDEVEXCEPTION))
    CheckDeallocation(address);
#endif

    if (mLastFreeElement != nullptr)
        WriteListEntry(mLastFreeElement, address);
    else
        mFirstFreeElement = static_cast<U8*>(address);

    mLastFreeElement = static_cast<U8*>(address);
    WriteListEntry(mLastFreeElement, nullptr);
    ++mNumFreeElements;
}



void MemoryPool::CheckMemoryConsistency() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    CheckMemoryConsistencyLockFree();
}



void MemoryPool::AlignMemory()
{
    void* memoryStartBefAlign = mMemory.get();
    size_t memorySizeBefAlign = TotalMemorySize();

    mMemoryStart = static_cast<U8*>(std::align(mAlignment, MemorySize(), memoryStartBefAlign, memorySizeBefAlign));

    EXCEPTION(mMemoryStart == nullptr, "Memory alignment failed.");
}



size_t MemoryPool::TotalMemorySize() const
{
    return MemorySize() + mAlignment;
}



size_t MemoryPool::MemorySize() const
{
    return mNumElements * mElementSize;
}



void MemoryPool::CheckConstructionParameters() const
{
    constexpr size_t minimalElementSize = sizeof(void*);
    EXCEPTION(mElementSize < minimalElementSize,
              "Element size must be " + std::to_string(minimalElementSize) + " or higher.");
    EXCEPTION(mNumElements == 0, "Number of elements must be larger than 0");
    EXCEPTION(!IsPowerOf2(mAlignment), "Alignment must be a power of 2");
    EXCEPTION(mElementSize % mAlignment > 0, "Pool element size must be a multiple of alignment");
}



void MemoryPool::CheckDeallocation(void* address) const
{
    DEV_EXCEPTION(IsInitialized() == false, "memory pool not initialized");
    DEV_EXCEPTION(address == nullptr, "Can't free a nullptr");
    DEV_EXCEPTION(static_cast<U8*>(address) < mMemoryStart || static_cast<U8*>(address) > mMemoryStart + MemorySize(),
                  "Memory address is not part of the pool allocators memory");
    DEV_EXCEPTION((static_cast<U8*>(address) - mMemoryStart) % mElementSize > 0,
                  "Memory address is not start of a valid memory block");

// Only in debug mode, since it is expensive
#ifndef NDEBUG
    U8* currentPosition = mFirstFreeElement;
    while (currentPosition != nullptr)
    {
        DEBUG_EXCEPTION(static_cast<U8*>(address) == currentPosition, "Memory block already freed.");
        currentPosition = ReadListEntry(currentPosition);
    }
#endif
}



void MemoryPool::CheckMemoryConsistencyLockFree() const
{
    EXCEPTION(IsInitialized() == false, "Memory pool not initialized");

    U32 freeElementsCount = 0;
    U8* currentPosition = mFirstFreeElement;

    while (currentPosition != nullptr)
    {
        currentPosition = ReadListEntry(currentPosition);
        ++freeElementsCount;
        EXCEPTION(freeElementsCount > mNumFreeElements, "Found more free elements than expected. Check for loops in "
                                                        "the list of free elements or if the free memory counter is "
                                                        "set correctly");
    }

    EXCEPTION(mNumFreeElements != freeElementsCount,
              "Free memory count is not as expected. Check if it is set correctly in allocation routine.");
}



bool MemoryPool::IsInitialized() const
{
    return mMemory != nullptr;
}



void MemoryPool::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutex);

    mMemory.reset(new U8[TotalMemorySize()]);

    AlignMemory();
    InitializeFreeMemoryList();
    mNumFreeElements = mNumElements;
}



void MemoryPool::Deinitialize()
{
    std::lock_guard<std::mutex> lock(mMutex);

    EXCEPTION(IsInitialized() == false, "Memory pool already deinitialized.");
    EXCEPTION(mNumElements != mNumFreeElements, "Can't deinitialize. Memory still in use.");

    CheckMemoryConsistencyLockFree();

    // reset internal variables and free memory
    mMemoryStart = nullptr;
    mFirstFreeElement = nullptr;
    mLastFreeElement = nullptr;
    mMemory.reset(nullptr);
}



U8* MemoryPool::ReadListEntry(const U8* addressToRead) const
{
    U8* entry = nullptr;
    std::memcpy(&entry, addressToRead, sizeof(void*));
    return entry;
}

void MemoryPool::InitializeFreeMemoryList()
{
    mFirstFreeElement = mMemoryStart;

    U8* currentPosition = mFirstFreeElement;

    // Set up linked list in free memory
    for (U32 i = 0; i < mNumElements - 1; ++i)
    {
        void* nextAddressPtr = static_cast<void*>(currentPosition + mElementSize);
        WriteListEntry(currentPosition, nextAddressPtr);
        currentPosition += mElementSize;
    }

    mLastFreeElement = currentPosition;
    WriteListEntry(mLastFreeElement, nullptr);
}



void MemoryPool::WriteListEntry(U8* positionInMemory, const void* addressToWrite)
{
    std::memcpy(positionInMemory, &addressToWrite, sizeof(void*));
}
}
