#include "gdl/resources/memory/memoryPool.h"


#include "gdl/base/Exception.h"

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

void* MemoryPool::Allocate(U32 size)
{
    if (size > mElementSize)
        throw Exception(__PRETTY_FUNCTION__, "Allocation size is larger than a pool element.");
    if (mFirstFreeElement == nullptr)
        throw Exception(__PRETTY_FUNCTION__, "No more memory available.");

    void* allocatedMemory = mFirstFreeElement;
    std::memcpy(&mFirstFreeElement, mFirstFreeElement, sizeof(void*));
    --mFreeMemorySize;

    // no more memory left, so here is no last free either
    if (mFirstFreeElement == nullptr)
        mLastFreeElement = nullptr;

    std::cout << allocatedMemory << std::endl;
    return allocatedMemory;
}


void MemoryPool::Deallocate(void* address)
{
    // Check if pointer has a valid address -> address - memoryStart / mElementsize = abs( ... )
    // Check if there is any way to check if the memory is already freed!

    if (mLastFreeElement != nullptr)
        std::memcpy(mLastFreeElement, &address, sizeof(void*));
    else
        mFirstFreeElement = static_cast<U8*>(address);

    mLastFreeElement = static_cast<U8*>(address);
    address = nullptr;
    std::memcpy(mLastFreeElement, &address, sizeof(void*));
    ++mFreeMemorySize;
}



void MemoryPool::CheckConsistency() const
{
    U32 freeElementsCount = 0;
    U8* currentPosition = mFirstFreeElement;

    while (currentPosition != nullptr)
    {
        std::memcpy(&currentPosition, currentPosition, sizeof(void*));
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
    constexpr U32 minimalElementSize = sizeof(void*);
    if (mElementSize < minimalElementSize)
        throw Exception(__PRETTY_FUNCTION__,
                        "Element size must be " + std::to_string(minimalElementSize) + " or higher.");


    U8* currentPosition = mMemory.get();
    void* nextAddressPtr = nullptr;

    // Set up linked list in free memory
    for (U32 i = 0; i < mMemorySize - 1; ++i)
    {
        nextAddressPtr = static_cast<void*>(currentPosition + mElementSize);
        std::memcpy(currentPosition, &nextAddressPtr, sizeof(void*));
        currentPosition += mElementSize;
    }
    mLastFreeElement = currentPosition;

    // Set last entry to nullptr
    nextAddressPtr = nullptr;
    std::memcpy(currentPosition, &nextAddressPtr, sizeof(void*));
}
}
