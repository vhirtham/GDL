#include "gdl/resources/memory/memoryPool.h"


#include "gdl/base/Exception.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>

namespace GDL
{

bool is_power_of_2(size_t x)
{
    return x > 0 && !(x & (x - 1));
}

MemoryPool::MemoryPool(size_t elementSize, U32 numElements, size_t alignment)
    : mElementSize{elementSize}
    , mAlignment{alignment}
    , mNumElements{numElements}
    , mNumFreeElements{numElements}
    , mInitialized{false}
    , mMemory{std::make_unique<U8[]>(TotalMemorySize())}
    , mMemoryStart{nullptr}
    , mFirstFreeElement{nullptr}
    , mLastFreeElement{nullptr}
{
    std::lock_guard<std::mutex> lock(mMutex);
    CheckConstructionParameters();
}

MemoryPool::~MemoryPool()
{
    assert(mInitialized == false && "Deinitialize the memory pool before destruction");
    // assert(mNumFreeElements == mNumElements && "Memory still in use");
}



void* MemoryPool::Allocate(size_t size)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mInitialized == false)
        throw Exception(__PRETTY_FUNCTION__, "Memory pool not initialized");
    if (size > mElementSize)
        throw Exception(__PRETTY_FUNCTION__, "Allocation size is larger than a pool element.");
    if (mFirstFreeElement == nullptr)
        throw Exception(__PRETTY_FUNCTION__, "No more memory available.");

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

    CheckDeallocation(address);

    if (mLastFreeElement != nullptr)
        WriteListEntry(mLastFreeElement, address);
    else
        mFirstFreeElement = static_cast<U8*>(address);

    mLastFreeElement = static_cast<U8*>(address);
    WriteListEntry(mLastFreeElement, nullptr);
    ++mNumFreeElements;
}



void MemoryPool::CheckConsistency() const
{
    if (mInitialized == false)
        throw Exception(__PRETTY_FUNCTION__, "Memory pool not initialized");

    U32 freeElementsCount = 0;

    std::lock_guard<std::mutex> lock(mMutex);
    U8* currentPosition = mFirstFreeElement;

    while (currentPosition != nullptr)
    {
        currentPosition = ReadListEntry(currentPosition);
        ++freeElementsCount;
        if (freeElementsCount > mNumFreeElements)
            throw Exception(__PRETTY_FUNCTION__, "Found more free elements than expected. Check for loops in the list "
                                                 "of free elements or if the free memory counter is set correctly");
    }

    if (mNumFreeElements != freeElementsCount)
        throw Exception(__PRETTY_FUNCTION__,
                        "Free memory count is not as expected. Check if it is set correctly in allocation routine.");
}

void MemoryPool::AlignMemory()
{
    void* memoryStart = mMemory.get();
    size_t memorySize = TotalMemorySize();
    std::align(mAlignment, mNumElements, memoryStart, memorySize);

    // LCOV_EXCL_START
    // should never happen and can't be enforcedi n a test from outside of the class -> ignored by LCOV
    if (memorySize < MemorySize())
        throw Exception(__PRETTY_FUNCTION__, "Memory alignment results in a smaller pool size than desired.");
    // LCOV_EXCL_STOP

    mMemoryStart = static_cast<U8*>(memoryStart);
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
    if (mElementSize < minimalElementSize)
        throw Exception(__PRETTY_FUNCTION__,
                        "Element size must be " + std::to_string(minimalElementSize) + " or higher.");
    if (mNumElements == 0)
        throw Exception(__PRETTY_FUNCTION__, "Number of elements must be larger than 0");
    if (!is_power_of_2(mAlignment))
        throw Exception(__PRETTY_FUNCTION__, "Alignment must be a power of 2");
    if (mElementSize % mAlignment > 0)
        throw Exception(__PRETTY_FUNCTION__, "Pool element size must be a multiple of alignment");
}


void MemoryPool::CheckDeallocation(void* address) const
{
    if (mInitialized == false)
        throw Exception(__PRETTY_FUNCTION__, "memory pool not initialized");
    if (address == nullptr)
        throw Exception(__PRETTY_FUNCTION__, "Can't free a nullptr");
    if (static_cast<U8*>(address) < mMemoryStart || static_cast<U8*>(address) > mMemoryStart + MemorySize())
        throw Exception(__PRETTY_FUNCTION__, "Memory address is not part of the pool allocators memory");
    if ((static_cast<U8*>(address) - mMemoryStart) % mElementSize > 0)
        throw Exception(__PRETTY_FUNCTION__, "Memory address is not start of a valid memory block");

// Only in debug mode, since it is expensive
#ifndef NDEBUG
    U8* currentPosition = mFirstFreeElement;
    while (currentPosition != nullptr)
    {
        if (static_cast<U8*>(address) == currentPosition)
            throw Exception(__PRETTY_FUNCTION__, "Memory block already freed.");
        currentPosition = ReadListEntry(currentPosition);
    }
#endif
}



void MemoryPool::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutex);
    AlignMemory();
    InitializeFreeMemoryList();
    mNumFreeElements = mNumElements;
    mInitialized = true;
}

void MemoryPool::Deinitialize()
{
    if (mInitialized == false)
        throw Exception(__PRETTY_FUNCTION__, "Memory pool already deinitialized.");
    if (mNumElements != mNumFreeElements)
        throw Exception(__PRETTY_FUNCTION__, "Can't deinitialize. Memory still in use.");
    CheckConsistency();
    mInitialized = false;
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
