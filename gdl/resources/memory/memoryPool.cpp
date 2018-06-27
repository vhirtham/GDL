#include "gdl/resources/memory/memoryPool.h"


#include "gdl/base/Exception.h"

#include <cassert>
#include <cstring>
#include <iostream>

namespace GDL
{

MemoryPool::MemoryPool(U32 elementSize, U32 memorySize)
    : mElementSize{elementSize}
    , mMemorySize{memorySize}
    , mFreeMemorySize{mMemorySize}
    , mMemory{std::make_unique<U8[]>(mMemorySize * mElementSize)}
    , mFirstFreeElement{mMemory.get()}
{
    Initialize();
    CheckConsistency();
}

MemoryPool::~MemoryPool()
{
    assert(mFreeMemorySize == mMemorySize && "Memory still in use");
}



void* MemoryPool::Allocate(U32 size)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (size > mElementSize)
        throw Exception(__PRETTY_FUNCTION__, "Allocation size is larger than a pool element.");
    if (mFirstFreeElement == nullptr)
        throw Exception(__PRETTY_FUNCTION__, "No more memory available.");

    void* allocatedMemory = mFirstFreeElement;

    mFirstFreeElement = ReadListEntry(mFirstFreeElement);

    // no more memory left, so here is no last free either
    if (mFirstFreeElement == nullptr)
        mLastFreeElement = nullptr;

    --mFreeMemorySize;
    return allocatedMemory;
}



void MemoryPool::Deallocate(void* address)
{
    std::lock_guard<std::mutex> lock(mMutex);
    // Check if pointer has a valid address -> address - memoryStart / mElementsize = abs( ... )
    // Check if there is any way to check if the memory is already freed!

    if (mLastFreeElement != nullptr)
        WriteListEntry(mLastFreeElement, address);
    else
        mFirstFreeElement = static_cast<U8*>(address);

    mLastFreeElement = static_cast<U8*>(address);
    WriteListEntry(mLastFreeElement, nullptr);
    ++mFreeMemorySize;
}



void MemoryPool::CheckConsistency() const
{
    U32 freeElementsCount = 0;

    std::lock_guard<std::mutex> lock(mMutex);
    U8* currentPosition = mFirstFreeElement;

    while (currentPosition != nullptr)
    {
        currentPosition = ReadListEntry(currentPosition);
        ++freeElementsCount;
        if (freeElementsCount > mFreeMemorySize)
            throw Exception(__PRETTY_FUNCTION__, "Found more free elements than expected. Check for loops in the list "
                                                 "of free elements or if the free memory counter is set correctly");
    }

    if (mFreeMemorySize != freeElementsCount)
        throw Exception(__PRETTY_FUNCTION__,
                        "Free memory count is not as expected. Check if it is set correctly in allocation routine.");
}



void MemoryPool::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutex);
    constexpr U32 minimalElementSize = sizeof(void*);
    if (mElementSize < minimalElementSize)
        throw Exception(__PRETTY_FUNCTION__,
                        "Element size must be " + std::to_string(minimalElementSize) + " or higher.");


    U8* currentPosition = mMemory.get();

    // Set up linked list in free memory
    for (U32 i = 0; i < mMemorySize - 1; ++i)
    {
        void* nextAddressPtr = static_cast<void*>(currentPosition + mElementSize);
        WriteListEntry(currentPosition, nextAddressPtr);
        currentPosition += mElementSize;
    }

    mLastFreeElement = currentPosition;
    WriteListEntry(mLastFreeElement, nullptr);
}



U8* MemoryPool::ReadListEntry(const U8* addressToRead) const
{
    U8* entry = nullptr;
    std::memcpy(&entry, addressToRead, sizeof(void*));
    return entry;
}



void MemoryPool::WriteListEntry(U8* positionInMemory, const void* addressToWrite)
{
    std::memcpy(positionInMemory, &addressToWrite, sizeof(void*));
}
}
