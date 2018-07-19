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
    U8* nextFreeMemoryPtr = ReadLinkToNextFreeBlock(currentMemoryPtr);
    size_t freeMemorySize = ReadSizeFromMemory(currentMemoryPtr);

    size_t totalAllocationSize = size + alignment + sizeof(size_t);


    FindFreeMemoryBlock(currentMemoryPtr, prevFreeMemoryPtr, nextFreeMemoryPtr, freeMemorySize, totalAllocationSize);

    UpdateLinkedListAllocation(currentMemoryPtr, prevFreeMemoryPtr, nextFreeMemoryPtr, freeMemorySize,
                               totalAllocationSize);


    WriteSizeToMemory(currentMemoryPtr, totalAllocationSize);
    currentMemoryPtr += sizeof(size_t);

    return AlignAllocatedMemory(currentMemoryPtr, alignment);
}



U32 GeneralPurposeMemory::CountAllocatedMemoryBlocks() const
{
    U8* currentMemoryPtr = mMemory.get();
    U8* totalMemoryEndPtr = mMemory.get() + mMemorySize;
    U32 numTotalBlocks = 0;
    while (currentMemoryPtr != totalMemoryEndPtr && currentMemoryPtr != nullptr)
    {
        ++numTotalBlocks;
        size_t blockSize = ReadSizeFromMemory(currentMemoryPtr);
        EXCEPTION(blockSize <= 0, "Read block size <= 0.");

        currentMemoryPtr += blockSize;
        EXCEPTION(currentMemoryPtr > totalMemoryEndPtr,
                  "Read block size bigger than the distance to the memories end.");
    }

    return numTotalBlocks - CountFreeMemoryBlocks();
}



U32 GeneralPurposeMemory::CountFreeMemoryBlocks() const
{
    U8* currentMemoryPtr = mFirstFreeMemoryPtr;
    U8* nextFreeMemoryPtr = nullptr;

    U32 numFreeMemoryBlocks = 0;

    // Memory not full
    if (currentMemoryPtr != nullptr)
    {
        nextFreeMemoryPtr = ReadLinkToNextFreeBlock(currentMemoryPtr);
        ++numFreeMemoryBlocks;
    }

    // Traverse free memory blocks to the last one
    while (nextFreeMemoryPtr != nullptr)
    {
        EXCEPTION(nextFreeMemoryPtr <= currentMemoryPtr, "Internal linked list of free memory blocks is not ordered.");

        ++numFreeMemoryBlocks;
        currentMemoryPtr = nextFreeMemoryPtr;
        nextFreeMemoryPtr = ReadLinkToNextFreeBlock(currentMemoryPtr);

        EXCEPTION(nextFreeMemoryPtr != nullptr &&
                          (nextFreeMemoryPtr < mMemory.get() || nextFreeMemoryPtr > mMemory.get() + mMemorySize),
                  "Internal linked list of free memory blocks points to a value that is not inside the managed memory "
                  "section.");
    }

    return numFreeMemoryBlocks;
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
    mMemory.reset(nullptr);
}



void GeneralPurposeMemory::Initialize()
{
    std::lock_guard<std::mutex> lock{mMutex};

    EXCEPTION(IsInitialized(), "General purpose memory is already initialized.");

    mMemory.reset(new U8[mMemorySize]);
    mNumAllocations = 0;
    mFirstFreeMemoryPtr = mMemory.get();

    WriteSizeToMemory(mFirstFreeMemoryPtr, mMemorySize);
    WriteLinkToNextFreeBlock(mFirstFreeMemoryPtr, nullptr);
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
        nextFreeMemoryPtr = ReadLinkToNextFreeBlock(nextFreeMemoryPtr);
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
        nextFreeMemoryPtr = ReadLinkToNextFreeBlock(currentMemoryPtr);
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
                nextFreeMemoryPtr = ReadLinkToNextFreeBlock(nextFreeMemoryPtr);
                AddToWrittenSize(currentMemoryPtr, nextFreeMemorySize);
                WriteLinkToNextFreeBlock(currentMemoryPtr, nextFreeMemoryPtr);
            }
            else
            {
                WriteLinkToNextFreeBlock(currentMemoryPtr, nextFreeMemoryPtr);
            }
        }
        else
        {
            WriteLinkToNextFreeBlock(currentMemoryPtr, nextFreeMemoryPtr);
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
                WriteLinkToNextFreeBlock(prevFreeMemoryPtr, currentMemoryPtr);
                WriteLinkToNextFreeBlock(currentMemoryPtr, nextFreeMemoryPtr);
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
                    nextFreeMemoryPtr = ReadLinkToNextFreeBlock(nextFreeMemoryPtr);
                    AddToWrittenSize(prevFreeMemoryPtr, freedMemorySize + nextFreeMemorySize);
                    WriteLinkToNextFreeBlock(prevFreeMemoryPtr, nextFreeMemoryPtr);
                }
                else
                {
                    size_t nextFreeMemorySize = ReadSizeFromMemory(nextFreeMemoryPtr);
                    nextFreeMemoryPtr = ReadLinkToNextFreeBlock(nextFreeMemoryPtr);
                    AddToWrittenSize(currentMemoryPtr, nextFreeMemorySize);
                    WriteLinkToNextFreeBlock(currentMemoryPtr, nextFreeMemoryPtr);
                    WriteLinkToNextFreeBlock(prevFreeMemoryPtr, currentMemoryPtr);
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
                    WriteLinkToNextFreeBlock(prevFreeMemoryPtr, currentMemoryPtr);
                    WriteLinkToNextFreeBlock(currentMemoryPtr, nextFreeMemoryPtr);
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
                WriteLinkToNextFreeBlock(prevFreeMemoryPtr, nullptr);
            }
            else
            {
                WriteLinkToNextFreeBlock(prevFreeMemoryPtr, nextFreeMemoryPtr);
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
                WriteLinkToNextFreeBlock(leftFreeMemoryPtr, nullptr);
                mFirstFreeMemoryPtr = leftFreeMemoryPtr;
            }
            else
            {
                U8* leftFreeMemoryPtr = currentMemoryPtr + totalAllocationSize;
                WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
                WriteLinkToNextFreeBlock(leftFreeMemoryPtr, nextFreeMemoryPtr);
                mFirstFreeMemoryPtr = leftFreeMemoryPtr;
            }
        }
        else
        {
            if (nextFreeMemoryPtr == nullptr)
            {
                U8* leftFreeMemoryPtr = currentMemoryPtr + totalAllocationSize;
                WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
                WriteLinkToNextFreeBlock(leftFreeMemoryPtr, nullptr);
                WriteLinkToNextFreeBlock(prevFreeMemoryPtr, leftFreeMemoryPtr);
            }
            else
            {
                U8* leftFreeMemoryPtr = currentMemoryPtr + totalAllocationSize;
                WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
                WriteLinkToNextFreeBlock(leftFreeMemoryPtr, nextFreeMemoryPtr);
                WriteLinkToNextFreeBlock(prevFreeMemoryPtr, leftFreeMemoryPtr);
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

U8* GeneralPurposeMemory::ReadLinkToNextFreeBlock(U8* currentFreeBlockPtr) const
{
    return ReadAddressFromMemory(currentFreeBlockPtr + sizeof(size_t));
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

void GeneralPurposeMemory::WriteLinkToNextFreeBlock(U8* currentFreeBlockPtr, const void* nextFreeBlockPtr)
{
    WriteAddressToMemory(currentFreeBlockPtr + sizeof(size_t), nextFreeBlockPtr);
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
