#include "gdl/resources/memory/generalPurposeMemory.h"

#include "gdl/base/Exception.h"
#include "gdl/base/SSESupportFunctions.h"
#include "gdl/base/functions/isPowerOf2.h"

#include <cassert>
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

    DEV_EXCEPTION(size == 0, "Allocated memory size is 0.");
    DEV_EXCEPTION(!IsInitialized(), "General purpose memory is not initialized.");
    DEV_EXCEPTION(!IsPowerOf2(alignment), "Alignment must be a power of 2.");
    DEV_EXCEPTION(alignment > 255, "Maximum alignment is 128");
    EXCEPTION(mFirstFreeMemoryPtr == nullptr, "No more memory left");

    AllocationData allocationData{*this, size, alignment};

    FindFreeMemoryBlock(allocationData);

    UpdateLinkedListAllocation(allocationData);

    WriteSizeToMemory(allocationData.currentMemoryPtr, allocationData.totalAllocationSize);
    allocationData.currentMemoryPtr += sizeof(size_t);

    return AlignAllocatedMemory(allocationData.currentMemoryPtr, alignment);
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



void GeneralPurposeMemory::FindFreeMemoryBlock(AllocationData& data) const
{
    while (data.freeMemorySize < data.totalAllocationSize)
    {
        EXCEPTION(data.nextFreeMemoryPtr == nullptr, "No properly sized memory block available.");

        // Traverse to next memory block
        data.prevFreeMemoryPtr = data.currentMemoryPtr;
        data.currentMemoryPtr = data.nextFreeMemoryPtr;
        data.nextFreeMemoryPtr = ReadLinkToNextFreeBlock(data.currentMemoryPtr);
        data.freeMemorySize = ReadSizeFromMemory(data.currentMemoryPtr);
    }
}



bool GeneralPurposeMemory::IsInitialized() const
{
    return mMemory != nullptr;
}



void GeneralPurposeMemory::MergeUpdateLinkedListDeallocation(U8*& currentMemoryPtr, U8*& prevFreeMemoryPtr,
                                                             U8*& nextFreeMemoryPtr)
{
    size_t freedMemorySize = ReadSizeFromMemory(currentMemoryPtr);

    // Mergeable with previous?
    if (prevFreeMemoryPtr != nullptr && prevFreeMemoryPtr + ReadSizeFromMemory(prevFreeMemoryPtr) == currentMemoryPtr)
    {
        // Mergeable with next?
        if (currentMemoryPtr + freedMemorySize == nextFreeMemoryPtr) // Also fails if nextFreeMemoryPtr == nullptr
        {
            freedMemorySize += ReadSizeFromMemory(nextFreeMemoryPtr);
            WriteLinkToNextFreeBlock(prevFreeMemoryPtr, ReadLinkToNextFreeBlock(nextFreeMemoryPtr));
        }

        AddToWrittenSize(prevFreeMemoryPtr, freedMemorySize);
    }
    else
    {
        // Mergeable with next?
        if (currentMemoryPtr + freedMemorySize == nextFreeMemoryPtr) // Also fails if nextFreeMemoryPtr == nullptr
        {
            AddToWrittenSize(currentMemoryPtr, ReadSizeFromMemory(nextFreeMemoryPtr));
            nextFreeMemoryPtr = ReadLinkToNextFreeBlock(nextFreeMemoryPtr);
        }

        WriteLinkToNextFreeBlock(currentMemoryPtr, nextFreeMemoryPtr);

        if (prevFreeMemoryPtr == nullptr)
            mFirstFreeMemoryPtr = currentMemoryPtr;
        else
            WriteLinkToNextFreeBlock(prevFreeMemoryPtr, currentMemoryPtr);
    }
}



void GeneralPurposeMemory::UpdateLinkedListAllocation(AllocationData& data)
{
    constexpr size_t minimalFreeBlockSize = sizeof(size_t) + sizeof(void*);

    size_t leftMemorySize = data.freeMemorySize - data.totalAllocationSize;

    // Create new free memory block if enough memory is left
    if (leftMemorySize < minimalFreeBlockSize)
        data.totalAllocationSize = data.freeMemorySize;
    else
    {
        U8* leftFreeMemoryPtr = data.currentMemoryPtr + data.totalAllocationSize;
        WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
        WriteLinkToNextFreeBlock(leftFreeMemoryPtr, data.nextFreeMemoryPtr);
        data.nextFreeMemoryPtr = leftFreeMemoryPtr;
    }

    // Update previous list entry
    if (data.currentMemoryPtr == mFirstFreeMemoryPtr)
        mFirstFreeMemoryPtr = data.nextFreeMemoryPtr;
    else
        WriteLinkToNextFreeBlock(data.prevFreeMemoryPtr, data.nextFreeMemoryPtr);
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

GeneralPurposeMemory::AllocationData::AllocationData(GeneralPurposeMemory& gpm, size_t allocationSize, size_t alignment)
    : currentMemoryPtr{gpm.mFirstFreeMemoryPtr}
    , prevFreeMemoryPtr{nullptr}
    , nextFreeMemoryPtr{gpm.ReadLinkToNextFreeBlock(currentMemoryPtr)}
    , freeMemorySize{gpm.ReadSizeFromMemory(currentMemoryPtr)}
    , totalAllocationSize{allocationSize + alignment + sizeof(size_t)}
{
}

} // namespace GDL
