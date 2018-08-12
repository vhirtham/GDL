#include "gdl/resources/memory/memoryPool.h"


#include "gdl/base/Exception.h"
#include "gdl/base/functions/isPowerOf2.h"
#include "gdl/resources/memory/sharedFunctions.h"

#include <cstring>


namespace GDL
{

MemoryPool::MemoryPool(MemorySize elementSize, U32 numElements, size_t alignment)
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
}



void* MemoryPool::Allocate(size_t size, size_t alignment)
{
    std::lock_guard<std::mutex> lock(mMutex);
    DEV_EXCEPTION(!IsPowerOf2(alignment), "Alignment must be a power of 2.");
    DEV_EXCEPTION((alignment - 1) / mAlignment > 0, "Alignment must be a power of 2.");
    DEV_EXCEPTION(IsInitialized() == false, "Memory pool not initialized");
    DEV_EXCEPTION(size > mElementSize, "Allocation size is larger than a pool element.");
    EXCEPTION(mFirstFreeElement == nullptr, "No more memory available.");

    void* allocatedMemoryPtr = mFirstFreeElement;

    mFirstFreeElement = ReadAddressFromMemory(mFirstFreeElement);

    // no more memory left, so here is no last free either
    if (mFirstFreeElement == nullptr)
        mLastFreeElement = nullptr;

    --mNumFreeElements;
    return allocatedMemoryPtr;
}



void MemoryPool::Deallocate(void* address, [[maybe_unused]] size_t alignment)
{
    std::lock_guard<std::mutex> lock(mMutex);

#if !(defined(NDEBUG) && defined(NDEVEXCEPTION))
    CheckDeallocation(address);
#endif

    if (mLastFreeElement != nullptr)
        WriteAddressToMemory(mLastFreeElement, address);
    else
        mFirstFreeElement = static_cast<U8*>(address);

    mLastFreeElement = static_cast<U8*>(address);
    WriteAddressToMemory(mLastFreeElement, nullptr);
    ++mNumFreeElements;
}



void MemoryPool::CheckMemoryConsistency() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    CheckMemoryConsistencyPrivate();
}



void MemoryPool::AlignMemory()
{
    void* memoryStartBefAlign = mMemory.get();
    size_t memorySizeBefAlign = TotalMemorySize();

    mMemoryStart =
            static_cast<U8*>(std::align(mAlignment, EffectiveMemorySize(), memoryStartBefAlign, memorySizeBefAlign));

    EXCEPTION(mMemoryStart == nullptr, "Memory alignment failed.");
}



size_t MemoryPool::TotalMemorySize() const
{
    return EffectiveMemorySize() + mAlignment;
}



size_t MemoryPool::EffectiveMemorySize() const
{
    return mNumElements * mElementSize.GetNumBytes();
}



void MemoryPool::CheckConstructionParameters() const
{
    constexpr size_t minimalElementSize = sizeof(void*);
    EXCEPTION(mElementSize < minimalElementSize,
              "Element size must be " + std::to_string(minimalElementSize) + " or higher.");
    EXCEPTION(mNumElements == 0, "Number of elements must be larger than 0");
    EXCEPTION(!IsPowerOf2(mAlignment), "Alignment must be a power of 2");
    EXCEPTION(mElementSize.GetNumBytes() % mAlignment > 0, "Pool element size must be a multiple of alignment");
}



void MemoryPool::CheckDeallocation(void* address) const
{
    DEV_EXCEPTION(IsInitialized() == false, "memory pool not initialized");
    DEV_EXCEPTION(address == nullptr, "Can't free a nullptr");
    DEV_EXCEPTION(static_cast<U8*>(address) < mMemoryStart ||
                          static_cast<U8*>(address) > mMemoryStart + EffectiveMemorySize(),
                  "Memory address is not part of the pool allocators memory");
    DEV_EXCEPTION(static_cast<size_t>(static_cast<U8*>(address) - mMemoryStart) % mElementSize.GetNumBytes() > 0,
                  "Memory address is not start of a valid memory block");

// Only in debug mode, since it is expensive
#ifndef NDEBUG
    U8* currentPosition = mFirstFreeElement;
    while (currentPosition != nullptr)
    {
        DEBUG_EXCEPTION(static_cast<U8*>(address) == currentPosition, "Memory block already freed.");
        currentPosition = ReadAddressFromMemory(currentPosition);
    }
#endif
}



void MemoryPool::CheckMemoryConsistencyPrivate() const
{
    EXCEPTION(IsInitialized() == false, "Memory pool not initialized");

    U32 freeElementsCount = 0;
    U8* currentPosition = mFirstFreeElement;

    while (currentPosition != nullptr)
    {
        currentPosition = ReadAddressFromMemory(currentPosition);
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

    EXCEPTION(IsInitialized(), "Memory pool is already initialized.");

    mMemory.reset(new U8[TotalMemorySize()]);

    AlignMemory();
    InitializeFreeMemoryList();
    mNumFreeElements = mNumElements;
}

size_t MemoryPool::GetAlignment() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mAlignment;
}

MemorySize MemoryPool::GetElementSize() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mElementSize;
}



void MemoryPool::Deinitialize()
{
    std::lock_guard<std::mutex> lock(mMutex);

    EXCEPTION(IsInitialized() == false, "Memory pool already deinitialized.");
    EXCEPTION(mNumElements != mNumFreeElements, "Can't deinitialize. Memory still in use.");

    CheckMemoryConsistencyPrivate();

    // reset internal variables and free memory
    mMemoryStart = nullptr;
    mFirstFreeElement = nullptr;
    mLastFreeElement = nullptr;
    mMemory.reset(nullptr);
}



void MemoryPool::InitializeFreeMemoryList()
{
    mFirstFreeElement = mMemoryStart;

    U8* currentPosition = mFirstFreeElement;

    // Set up linked list in free memory
    for (U32 i = 0; i < mNumElements - 1; ++i)
    {
        void* nextAddressPtr = static_cast<void*>(currentPosition + mElementSize.GetNumBytes());
        WriteAddressToMemory(currentPosition, nextAddressPtr);
        currentPosition += mElementSize.GetNumBytes();
    }

    mLastFreeElement = currentPosition;
    WriteAddressToMemory(mLastFreeElement, nullptr);
}
}
