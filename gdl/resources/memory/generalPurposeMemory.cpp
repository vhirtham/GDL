#include "gdl/resources/memory/generalPurposeMemory.h"

#include "gdl/base/Exception.h"
#include "gdl/base/SSESupportFunctions.h"
#include "gdl/base/functions/isPowerOf2.h"
#include "gdl/resources/memory/sharedFunctions.h"

#include <cassert>


namespace GDL
{

GeneralPurposeMemory::GeneralPurposeMemory(size_t memorySize)
    : mMemorySize{memorySize}
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
    std::lock_guard<std::mutex> lock{mMutex};

    return CountAllocatedMemoryBlocksPrivate();
}



U32 GeneralPurposeMemory::CountFreeMemoryBlocks() const
{
    std::lock_guard<std::mutex> lock{mMutex};

    return CountFreeMemoryBlocksPrivate();
}



void GeneralPurposeMemory::Deallocate(void* address, [[maybe_unused]] size_t alignment)
{
    std::lock_guard<std::mutex> lock{mMutex};

    DEV_EXCEPTION(!IsInitialized(), "General purpose memory is not initialized.");
    DEV_EXCEPTION(address == nullptr, "Can't free a nullptr");
    DEV_EXCEPTION(!IsAddressInsideMemory(static_cast<U8*>(address)),
                  "Memory address is not part of the general purpose memory");

    DeallocationData deallocationData(*this, address);

    DEV_EXCEPTION(!IsDeallocatedAddressValid(deallocationData.currentMemoryPtr),
                  "Deallocated address is not an allocated memory block or was already freed");

    FindEnclosingFreeMemoryBlocks(deallocationData);

    MergeUpdateLinkedListDeallocation(deallocationData);
}



void GeneralPurposeMemory::Deinitialize()
{
    std::lock_guard<std::mutex> lock{mMutex};

    EXCEPTION(!IsInitialized(), "General purpose memory is already deinitialized.");
    EXCEPTION(CountAllocatedMemoryBlocksPrivate() != 0, "Can't deinitialize. Memory still in use.");

    mFirstFreeMemoryPtr = nullptr;
    mMemory.reset(nullptr);
}



void GeneralPurposeMemory::Initialize()
{
    std::lock_guard<std::mutex> lock{mMutex};

    EXCEPTION(IsInitialized(), "General purpose memory is already initialized.");

    mMemory.reset(new U8[mMemorySize]);
    mFirstFreeMemoryPtr = GetStartOfMemory();

    WriteSizeToMemory(mFirstFreeMemoryPtr, mMemorySize);
    WriteLinkToNextFreeBlock(mFirstFreeMemoryPtr, nullptr);
}



void GeneralPurposeMemory::AddToWrittenSize(void* positionInMemory, const size_t value)
{
    assert(positionInMemory != nullptr);

    size_t* valuePtr = static_cast<size_t*>(positionInMemory);
    *valuePtr += value;
}



void* GeneralPurposeMemory::AlignAllocatedMemory(U8* currentMemoryPtr, size_t alignment)
{
    assert(currentMemoryPtr != nullptr);
    assert(alignment > 0);

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



U32 GeneralPurposeMemory::CountFreeMemoryBlocksPrivate() const
{
    U8* currentMemoryPtr = mFirstFreeMemoryPtr;
    U32 numFreeMemoryBlocks = 0;

    while (currentMemoryPtr != nullptr)
    {
        assert(IsAddressInsideMemory(currentMemoryPtr));
        assert(IsAddressStartOfMemoryBlock(currentMemoryPtr));

        U8* nextFreeMemoryPtr = ReadLinkToNextFreeBlock(currentMemoryPtr);
        assert(nextFreeMemoryPtr > currentMemoryPtr || nextFreeMemoryPtr == nullptr);

        ++numFreeMemoryBlocks;
        currentMemoryPtr = nextFreeMemoryPtr;
    }

    return numFreeMemoryBlocks;
}



U32 GeneralPurposeMemory::CountMemoryBlocks() const
{
    U8* currentMemoryPtr = GetStartOfMemory();

    const U8* const endOfMemoryPtr = GetEndOfMemory();

    U32 numBlocks = 0;
    while (currentMemoryPtr != endOfMemoryPtr)
    {
        assert(ReadSizeFromMemory(currentMemoryPtr) > 0);
        assert(currentMemoryPtr < endOfMemoryPtr);

        ++numBlocks;
        currentMemoryPtr += ReadSizeFromMemory(currentMemoryPtr);
    }

    return numBlocks;
}



U32 GeneralPurposeMemory::CountAllocatedMemoryBlocksPrivate() const
{
    return CountMemoryBlocks() - CountFreeMemoryBlocksPrivate();
}



void GeneralPurposeMemory::FindEnclosingFreeMemoryBlocks(DeallocationData& data) const
{
    assert(data.currentMemoryPtr != nullptr);

    while (data.nextFreeMemoryPtr != nullptr && data.nextFreeMemoryPtr < data.currentMemoryPtr)
    {
        assert(data.prevFreeMemoryPtr < data.nextFreeMemoryPtr);

        data.prevFreeMemoryPtr = data.nextFreeMemoryPtr;
        data.nextFreeMemoryPtr = ReadLinkToNextFreeBlock(data.nextFreeMemoryPtr);
    }
}



void GeneralPurposeMemory::FindFreeMemoryBlock(AllocationData& data) const
{
    while (data.freeMemorySize < data.totalAllocationSize)
    {
        assert(data.prevFreeMemoryPtr < data.nextFreeMemoryPtr || data.nextFreeMemoryPtr == nullptr);
        EXCEPTION(data.nextFreeMemoryPtr == nullptr, "No properly sized memory block available.");

        // Traverse to next memory block
        data.prevFreeMemoryPtr = data.currentMemoryPtr;
        data.currentMemoryPtr = data.nextFreeMemoryPtr;
        data.nextFreeMemoryPtr = ReadLinkToNextFreeBlock(data.currentMemoryPtr);
        data.freeMemorySize = ReadSizeFromMemory(data.currentMemoryPtr);
    }
}



U8* GeneralPurposeMemory::GetEndOfMemory() const
{
    return mMemory.get() + mMemorySize;
}



U8* GeneralPurposeMemory::GetStartOfMemory() const
{
    return mMemory.get();
}



bool GeneralPurposeMemory::IsInitialized() const
{
    return mMemory != nullptr;
}



bool GeneralPurposeMemory::IsAddressInsideMemory(U8* address) const
{
    if (address < GetStartOfMemory() || address > GetEndOfMemory())
        return false;
    return true;
}



bool GeneralPurposeMemory::IsAddressStartOfFreeMemoryBlock(U8* address) const
{
    assert(address != nullptr);

    U8* currentMemoryPtr = mFirstFreeMemoryPtr;
    while (currentMemoryPtr != nullptr && currentMemoryPtr < address)
    {

        U8* nextFreeMemoryPtr = ReadLinkToNextFreeBlock(currentMemoryPtr);
        assert(IsAddressStartOfMemoryBlock(currentMemoryPtr));
        assert(nextFreeMemoryPtr > currentMemoryPtr || nextFreeMemoryPtr == nullptr);

        currentMemoryPtr = nextFreeMemoryPtr;
    }
    // Block is a free block
    if (currentMemoryPtr == address)
        return true;
    return false;
}



bool GeneralPurposeMemory::IsAddressStartOfMemoryBlock(U8* address) const
{
    assert(address != nullptr);

    U8* currentMemoryPtr = GetStartOfMemory();
    while (currentMemoryPtr < address)
    {
        size_t blockSize = ReadSizeFromMemory(currentMemoryPtr);
        assert(blockSize > 0);

        currentMemoryPtr += blockSize;
    }

    if (currentMemoryPtr > address)
        return false;
    return true;
}



bool GeneralPurposeMemory::IsDeallocatedAddressValid(U8* address) const
{
    return IsAddressInsideMemory(address) && !IsAddressStartOfFreeMemoryBlock(address) &&
           IsAddressStartOfMemoryBlock(address);
}



void GeneralPurposeMemory::MergeUpdateLinkedListDeallocation(DeallocationData& data)
{
    size_t freedMemorySize = ReadSizeFromMemory(data.currentMemoryPtr);
    assert(freedMemorySize > 0);

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



void GeneralPurposeMemory::WriteLinkToNextFreeBlock(U8* currentFreeBlockPtr, const void* nextFreeBlockPtr)
{
    WriteAddressToMemory(currentFreeBlockPtr + sizeof(size_t), nextFreeBlockPtr);
}



GeneralPurposeMemory::AllocationData::AllocationData(const GeneralPurposeMemory& gpm, size_t allocationSize,
                                                     size_t alignment)
    : currentMemoryPtr{gpm.mFirstFreeMemoryPtr}
    , prevFreeMemoryPtr{nullptr}
    , nextFreeMemoryPtr{gpm.ReadLinkToNextFreeBlock(currentMemoryPtr)}
    , freeMemorySize{ReadSizeFromMemory(currentMemoryPtr)}
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
