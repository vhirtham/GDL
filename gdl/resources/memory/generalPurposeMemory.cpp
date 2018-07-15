#include "gdl/resources/memory/generalPurposeMemory.h"

#include "gdl/base/Exception.h"
#include "gdl/base/SSESupportFunctions.h"

#include <cstring>
#include <iostream>


namespace GDL
{

GeneralPurposeMemory::GeneralPurposeMemory(size_t memorySize)
    : mMemorySize{memorySize}
    , mNumAllocations{0}
    , mFirstFreeMemoryPtr{nullptr}
    , mLastFreeMemoryPtr{nullptr}
    , mMemory{nullptr}
{
    CheckConstructionParameters();
}

GeneralPurposeMemory::~GeneralPurposeMemory()
{
}

void* GeneralPurposeMemory::Allocate(size_t size, size_t alignment)
{
    std::lock_guard<std::mutex> lock{mMutex};

    DEV_EXCEPTION(mMemory.get() == nullptr, "General purpose memory is not initialized.");
    DEV_EXCEPTION(alignment > 255, "Alignment must be smaller than 256");
    EXCEPTION(mFirstFreeMemoryPtr == nullptr, "No more memory left");

    // Data from first free memory block - maybe collect them in a private struct
    U8* currentMemoryPtr = mFirstFreeMemoryPtr;
    U8* prevFreeMemoryPtr = nullptr;
    U8* nextFreeMemoryPtr = ReadAddressFromMemory(currentMemoryPtr + sizeof(size_t));
    size_t freeMemorySize = ReadSizeFromMemory(currentMemoryPtr);

    size_t totalAllocationSize = size + alignment + sizeof(size_t);


    FindFreeMemoryBlock(currentMemoryPtr, prevFreeMemoryPtr, nextFreeMemoryPtr, freeMemorySize, totalAllocationSize);

    UpdateLinkedListAllocation(currentMemoryPtr, prevFreeMemoryPtr, nextFreeMemoryPtr, freeMemorySize,
                               totalAllocationSize);


    WriteSizeToMemory(currentMemoryPtr, totalAllocationSize);
    currentMemoryPtr += sizeof(size_t);

    return AlignAllocatedMemory(currentMemoryPtr, alignment);
}



void GeneralPurposeMemory::CheckMemoryConsistency() const
{
    U8* currentMemoryPtr = mFirstFreeMemoryPtr;
    U8* nextFreeMemoryPtr = nullptr;

    // Memory not full
    if (currentMemoryPtr != nullptr)
        ReadAddressFromMemory(currentMemoryPtr + sizeof(size_t));

    U8* totalMemoryEndPtr = mMemory.get() + mMemorySize;

    // Traverse free memory blocks to the last one
    while (nextFreeMemoryPtr != nullptr)
    {
        EXCEPTION(nextFreeMemoryPtr <= currentMemoryPtr, "Internal linked list of free memory blocks is not ordered.");

        U8* currentMemoryPtr = nextFreeMemoryPtr;
        U8* nextFreeMemoryPtr = ReadAddressFromMemory(currentMemoryPtr + sizeof(size_t));

        EXCEPTION(nextFreeMemoryPtr != nullptr &&
                          (nextFreeMemoryPtr < mMemory.get() || nextFreeMemoryPtr > totalMemoryEndPtr),
                  "Internal linked list of free memory blocks points to a value that is not inside the managed memory "
                  "section.");
    }

    // Traverse occupied memory blocks at the end, if there are some
    while (currentMemoryPtr != totalMemoryEndPtr && currentMemoryPtr != nullptr)
    {
        size_t blockSize = ReadSizeFromMemory(currentMemoryPtr);
        EXCEPTION(blockSize <= 0, "Read block size <= 0.");

        currentMemoryPtr += blockSize;
        EXCEPTION(currentMemoryPtr > totalMemoryEndPtr,
                  "Read block size bigger than the distance to the memories end.");
    }

    // Start again at the beginning and traverse all blocks
    currentMemoryPtr = mMemory.get();
    while (currentMemoryPtr != totalMemoryEndPtr && currentMemoryPtr != nullptr)
    {
        size_t blockSize = ReadSizeFromMemory(currentMemoryPtr);
        EXCEPTION(blockSize <= 0, "Read block size <= 0.");

        currentMemoryPtr += blockSize;
        EXCEPTION(currentMemoryPtr > totalMemoryEndPtr,
                  "Read block size bigger than the distance to the memories end.");
    }
}



void GeneralPurposeMemory::Deallocate(void* address)
{
    std::lock_guard<std::mutex> lock{mMutex};

    DEV_EXCEPTION(mMemory.get() == nullptr, "General purpose memory is not initialized.");

    U8* currentMemoryPtr = RestoreAllocatedPtr(static_cast<U8*>(address));
    U8* nextFreeMemoryPtr = mFirstFreeMemoryPtr;
    U8* prevFreeMemoryPtr = nullptr;

    FindEnclosingFreeMemoryBlocks(currentMemoryPtr, prevFreeMemoryPtr, nextFreeMemoryPtr);

    MergeUpdateLinkedListDeallocation(currentMemoryPtr, prevFreeMemoryPtr, nextFreeMemoryPtr);
}



void GeneralPurposeMemory::Deinitialize()
{
    std::lock_guard<std::mutex> lock{mMutex};

    EXCEPTION(!IsInitialized(), "General purpose memory is already deinitialized.");
    EXCEPTION(ReadSizeFromMemory(mMemory.get()) != mMemorySize, "Can't deinitialize. Memory still in use.");

    mFirstFreeMemoryPtr = nullptr;
    mLastFreeMemoryPtr = nullptr;
    mMemory.reset(nullptr);
}



void GeneralPurposeMemory::Initialize()
{
    std::lock_guard<std::mutex> lock{mMutex};

    EXCEPTION(IsInitialized(), "General purpose memory is already initialized.");

    mMemory.reset(new U8[mMemorySize]);
    mNumAllocations = 0;
    mFirstFreeMemoryPtr = mMemory.get();
    mLastFreeMemoryPtr = mFirstFreeMemoryPtr;

    WriteSizeToMemory(mFirstFreeMemoryPtr, mMemorySize);
    WriteAddressToMemory(mFirstFreeMemoryPtr + sizeof(size_t), nullptr);
}



void GeneralPurposeMemory::AddToWrittenSize(void* positionInMemory, const size_t value)
{
    size_t* valuePtr = static_cast<size_t*>(positionInMemory);
    *valuePtr += value;
}



void* GeneralPurposeMemory::AlignAllocatedMemory(U8* currentMemoryPtr, size_t alignment)
{
    size_t misalignment = Misalignment(currentMemoryPtr, alignment);
    size_t correction = alignment - misalignment;

    currentMemoryPtr += correction;
    currentMemoryPtr[-1] = static_cast<U8>(correction);

    return currentMemoryPtr;
}



void GeneralPurposeMemory::CheckConstructionParameters() const
{
    EXCEPTION(mMemorySize < 1, "Memory size must be bigger than 1");
}



void GeneralPurposeMemory::FindEnclosingFreeMemoryBlocks(U8*& currentMemoryPtr, U8*& prevFreeMemoryPtr,
                                                         U8*& nextFreeMemoryPtr) const
{
    while (nextFreeMemoryPtr != nullptr && nextFreeMemoryPtr < currentMemoryPtr)
    {
        prevFreeMemoryPtr = nextFreeMemoryPtr;
        nextFreeMemoryPtr = ReadAddressFromMemory(nextFreeMemoryPtr + sizeof(size_t));
    }
}



void GeneralPurposeMemory::FindFreeMemoryBlock(U8*& currentMemoryPtr, U8*& prevFreeMemoryPtr, U8*& nextFreeMemoryPtr,
                                               size_t& freeMemorySize, size_t totalAllocationSize) const
{
    while (freeMemorySize < totalAllocationSize)
    {
        EXCEPTION(nextFreeMemoryPtr == nullptr, "No properly sized memory block available.");

        // Traverse to next memory block
        prevFreeMemoryPtr = currentMemoryPtr;
        currentMemoryPtr = nextFreeMemoryPtr;
        nextFreeMemoryPtr = ReadAddressFromMemory(currentMemoryPtr + sizeof(size_t));
        freeMemorySize = ReadSizeFromMemory(currentMemoryPtr);
    }
}



bool GeneralPurposeMemory::IsInitialized() const
{
    return mMemory != nullptr;
}



void GeneralPurposeMemory::MergeUpdateLinkedListDeallocation(U8*& currentMemoryPtr, U8*& prevFreeMemoryPtr,
                                                             U8*& nextFreeMemoryPtr)
{
    if (prevFreeMemoryPtr == nullptr)
    {
        mFirstFreeMemoryPtr = currentMemoryPtr;
        if (nextFreeMemoryPtr != nullptr)
        {
            size_t freedMemorySize = ReadSizeFromMemory(currentMemoryPtr);

            // Mergeable with next?
            if (currentMemoryPtr + freedMemorySize == nextFreeMemoryPtr)
            {
                size_t nextFreeMemorySize = ReadSizeFromMemory(nextFreeMemoryPtr);
                nextFreeMemoryPtr = ReadAddressFromMemory(nextFreeMemoryPtr + sizeof(size_t));
                AddToWrittenSize(currentMemoryPtr, nextFreeMemorySize);
                WriteAddressToMemory(currentMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);

                if (nextFreeMemoryPtr == nullptr)
                    mLastFreeMemoryPtr = currentMemoryPtr;
            }
            else
            {
                WriteAddressToMemory(currentMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
            }
        }
        else
        {
            WriteAddressToMemory(currentMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
            mLastFreeMemoryPtr = currentMemoryPtr;
        }
    }
    else
    {
        // is new last?
        if (nextFreeMemoryPtr == nullptr)
        {
            size_t prevFreeMemorySize = ReadSizeFromMemory(prevFreeMemoryPtr);
            // Mergeable with previous?
            if (prevFreeMemoryPtr + prevFreeMemorySize == currentMemoryPtr)
            {
                size_t freedMemorySize = ReadSizeFromMemory(currentMemoryPtr);
                AddToWrittenSize(prevFreeMemoryPtr, freedMemorySize);
            }
            else
            {
                WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), currentMemoryPtr);
                WriteAddressToMemory(currentMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
                mLastFreeMemoryPtr = currentMemoryPtr;
            }
        }
        else
        {
            size_t freedMemorySize = ReadSizeFromMemory(currentMemoryPtr);
            size_t prevFreeMemorySize = ReadSizeFromMemory(prevFreeMemoryPtr);

            // Mergeable with next?
            if (currentMemoryPtr + freedMemorySize == nextFreeMemoryPtr)
            {
                // Mergeable with previous?
                if (prevFreeMemoryPtr + prevFreeMemorySize == currentMemoryPtr)
                {
                    size_t nextFreeMemorySize = ReadSizeFromMemory(nextFreeMemoryPtr);
                    nextFreeMemoryPtr = ReadAddressFromMemory(nextFreeMemoryPtr + sizeof(size_t));
                    AddToWrittenSize(prevFreeMemoryPtr, freedMemorySize + nextFreeMemorySize);
                    WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
                    if (nextFreeMemoryPtr == nullptr)
                        mLastFreeMemoryPtr = prevFreeMemoryPtr;
                }
                else
                {
                    size_t nextFreeMemorySize = ReadSizeFromMemory(nextFreeMemoryPtr);
                    nextFreeMemoryPtr = ReadAddressFromMemory(nextFreeMemoryPtr + sizeof(size_t));
                    AddToWrittenSize(currentMemoryPtr, nextFreeMemorySize);
                    WriteAddressToMemory(currentMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
                    WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), currentMemoryPtr);
                    if (nextFreeMemoryPtr == nullptr)
                        mLastFreeMemoryPtr = currentMemoryPtr;
                }
            }
            else
            {
                // Mergeable with previous?
                if (prevFreeMemoryPtr + prevFreeMemorySize == currentMemoryPtr)
                {
                    AddToWrittenSize(prevFreeMemoryPtr, freedMemorySize);
                }
                else
                {
                    WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), currentMemoryPtr);
                    WriteAddressToMemory(currentMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
                }
            }
        }
    }
}



void GeneralPurposeMemory::UpdateLinkedListAllocation(U8*& currentMemoryPtr, U8*& prevFreeMemoryPtr,
                                                      U8*& nextFreeMemoryPtr, size_t freeMemorySize,
                                                      size_t& totalAllocationSize)
{
    constexpr size_t minimalFreeBlockSize = sizeof(size_t) + sizeof(void*);

    size_t leftMemorySize = freeMemorySize - totalAllocationSize;

    if (leftMemorySize < minimalFreeBlockSize)
    {
        totalAllocationSize = freeMemorySize;

        if (currentMemoryPtr == mFirstFreeMemoryPtr)
        {
            if (nextFreeMemoryPtr == nullptr)
            {
                mFirstFreeMemoryPtr = nullptr;
                mLastFreeMemoryPtr = nullptr;
            }
            else
            {
                mFirstFreeMemoryPtr = nextFreeMemoryPtr;
            }
        }
        else
        {
            if (nextFreeMemoryPtr == nullptr)
            {
                WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), nullptr);
                mLastFreeMemoryPtr = prevFreeMemoryPtr;
            }
            else
            {
                WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
            }
        }
    }
    else
    {
        if (currentMemoryPtr == mFirstFreeMemoryPtr)
        {
            if (nextFreeMemoryPtr == nullptr)
            {
                U8* leftFreeMemoryPtr = currentMemoryPtr + totalAllocationSize;
                WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
                WriteAddressToMemory(leftFreeMemoryPtr + sizeof(size_t), nullptr);
                mFirstFreeMemoryPtr = leftFreeMemoryPtr;
                mLastFreeMemoryPtr = leftFreeMemoryPtr;
            }
            else
            {
                U8* leftFreeMemoryPtr = currentMemoryPtr + totalAllocationSize;
                WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
                WriteAddressToMemory(leftFreeMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
                mFirstFreeMemoryPtr = leftFreeMemoryPtr;
            }
        }
        else
        {
            if (nextFreeMemoryPtr == nullptr)
            {
                U8* leftFreeMemoryPtr = currentMemoryPtr + totalAllocationSize;
                WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
                WriteAddressToMemory(leftFreeMemoryPtr + sizeof(size_t), nullptr);
                WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), leftFreeMemoryPtr);
                mLastFreeMemoryPtr = leftFreeMemoryPtr;
            }
            else
            {
                U8* leftFreeMemoryPtr = currentMemoryPtr + totalAllocationSize;
                WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
                WriteAddressToMemory(leftFreeMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
                WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), leftFreeMemoryPtr);
            }
        }
    }
}



U8* GeneralPurposeMemory::ReadAddressFromMemory(const U8* positionInMemory) const
{
    U8* address = nullptr;
    std::memcpy(&address, positionInMemory, sizeof(void*));
    return address;
}



U8* GeneralPurposeMemory::RestoreAllocatedPtr(U8* currentMemoryPtr)
{
    U8 alignmentCorrection = currentMemoryPtr[-1];
    return currentMemoryPtr - (alignmentCorrection + sizeof(size_t));
}



size_t GeneralPurposeMemory::ReadSizeFromMemory(const void* positionInMemory) const
{
    return *static_cast<const size_t*>(positionInMemory);
}

void GeneralPurposeMemory::WriteAddressToMemory(U8* positionInMemory, const void* addressToWrite)
{
    std::memcpy(positionInMemory, &addressToWrite, sizeof(void*));
}

void GeneralPurposeMemory::WriteSizeToMemory(void* positionInMemory, const size_t value)
{
    size_t* valuePtr = static_cast<size_t*>(positionInMemory);
    *valuePtr = value;
}

} // namespace GDL
