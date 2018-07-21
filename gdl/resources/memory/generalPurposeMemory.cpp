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

    DEV_EXCEPTION(!IsInitialized(), "General purpose memory is not initialized.");
    DEV_EXCEPTION(address == nullptr, "Can't free a nullptr");
    DEV_EXCEPTION(static_cast<U8*>(address) < mMemory.get() || static_cast<U8*>(address) > mMemory.get() + mMemorySize,
                  "Memory address is not part of the general purpose memory");

    DeallocationData deallocationData(*this, address);

    DEV_EXCEPTION(!IsDeallocatedAddressValid(deallocationData),
                  "Deallocated address is not an allocated memory block or was already freed");

    FindEnclosingFreeMemoryBlocks(deallocationData);

    MergeUpdateLinkedListDeallocation(deallocationData);
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



void GeneralPurposeMemory::FindEnclosingFreeMemoryBlocks(DeallocationData& data) const
{
    while (data.nextFreeMemoryPtr != nullptr && data.nextFreeMemoryPtr < data.currentMemoryPtr)
    {
        data.prevFreeMemoryPtr = data.nextFreeMemoryPtr;
        data.nextFreeMemoryPtr = ReadLinkToNextFreeBlock(data.nextFreeMemoryPtr);
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



bool GeneralPurposeMemory::IsDeallocatedAddressValid(const GeneralPurposeMemory::DeallocationData& data) const
{
    // restored address is out of bounds
    if (data.currentMemoryPtr < mMemory.get() || data.currentMemoryPtr > mMemory.get() + mMemorySize)
        return false;


    // Traverse free memory blocks
    U8* currentTraversalPtr = mFirstFreeMemoryPtr;
    while (currentTraversalPtr != nullptr && currentTraversalPtr < data.currentMemoryPtr)
    {

        U8* nextFreeMemoryPtr = ReadLinkToNextFreeBlock(currentTraversalPtr);
        DEV_EXCEPTION(nextFreeMemoryPtr <= currentTraversalPtr && nextFreeMemoryPtr != nullptr,
                      "Internal linked list of free memory blocks is not ordered.");
        currentTraversalPtr = nextFreeMemoryPtr;
    }
    // Block is a free block
    if (currentTraversalPtr == data.currentMemoryPtr)
        return false;

    // Traverse all blocks
    currentTraversalPtr = mMemory.get();
    while (currentTraversalPtr < data.currentMemoryPtr)
    {
        size_t blockSize = ReadSizeFromMemory(currentTraversalPtr);
        DEV_EXCEPTION(blockSize <= 0, "Read block size <= 0. Internal structure corrupted");
        currentTraversalPtr += blockSize;
    }
    // memory pointer is not a valid memory block
    if (currentTraversalPtr > data.currentMemoryPtr)
        return false;

    return true;
}



void GeneralPurposeMemory::MergeUpdateLinkedListDeallocation(DeallocationData& data)
{
    size_t freedMemorySize = ReadSizeFromMemory(data.currentMemoryPtr);

    // Mergeable with previous?
    if (data.prevFreeMemoryPtr != nullptr &&
        data.prevFreeMemoryPtr + ReadSizeFromMemory(data.prevFreeMemoryPtr) == data.currentMemoryPtr)
    {
        // Mergeable with next? ---> Also fails if nextFreeMemoryPtr == nullptr (last free block)
        if (data.currentMemoryPtr + freedMemorySize == data.nextFreeMemoryPtr)
        {
            freedMemorySize += ReadSizeFromMemory(data.nextFreeMemoryPtr);
            WriteLinkToNextFreeBlock(data.prevFreeMemoryPtr, ReadLinkToNextFreeBlock(data.nextFreeMemoryPtr));
        }

        AddToWrittenSize(data.prevFreeMemoryPtr, freedMemorySize);
    }
    else
    {
        // Mergeable with next? ---> Also fails if nextFreeMemoryPtr == nullptr (last free block)
        if (data.currentMemoryPtr + freedMemorySize == data.nextFreeMemoryPtr)
        {
            AddToWrittenSize(data.currentMemoryPtr, ReadSizeFromMemory(data.nextFreeMemoryPtr));
            data.nextFreeMemoryPtr = ReadLinkToNextFreeBlock(data.nextFreeMemoryPtr);
        }

        WriteLinkToNextFreeBlock(data.currentMemoryPtr, data.nextFreeMemoryPtr);

        if (data.prevFreeMemoryPtr == nullptr)
            mFirstFreeMemoryPtr = data.currentMemoryPtr;
        else
            WriteLinkToNextFreeBlock(data.prevFreeMemoryPtr, data.currentMemoryPtr);
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



U8* GeneralPurposeMemory::RestoreAllocatedPtr(U8* currentMemoryPtr) const
{
    U8 alignmentCorrection = currentMemoryPtr[-1];
    DEV_EXCEPTION(alignmentCorrection < 1 || alignmentCorrection > 128, "Invalid alignment correction read");
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



GeneralPurposeMemory::AllocationData::AllocationData(const GeneralPurposeMemory& gpm, size_t allocationSize,
                                                     size_t alignment)
    : currentMemoryPtr{gpm.mFirstFreeMemoryPtr}
    , prevFreeMemoryPtr{nullptr}
    , nextFreeMemoryPtr{gpm.ReadLinkToNextFreeBlock(currentMemoryPtr)}
    , freeMemorySize{gpm.ReadSizeFromMemory(currentMemoryPtr)}
    , totalAllocationSize{allocationSize + alignment + sizeof(size_t)}
{
}



GeneralPurposeMemory::DeallocationData::DeallocationData(const GeneralPurposeMemory& gpm, void* address)
    : currentMemoryPtr{gpm.RestoreAllocatedPtr(static_cast<U8*>(address))}
    , prevFreeMemoryPtr{nullptr}
    , nextFreeMemoryPtr{gpm.mFirstFreeMemoryPtr}
{
}

} // namespace GDL
