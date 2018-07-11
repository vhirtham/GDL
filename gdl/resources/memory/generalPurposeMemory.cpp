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

    DEV_EXCEPTION(alignment > 255, "Alignment must be smaller than 256");

    // Data from first free memory block - maybe collect them in a private struct
    U8* currentMemoryPtr = mFirstFreeMemoryPtr;
    U8* prevFreeMemoryPtr = nullptr;
    U8* nextFreeMemoryPtr = ReadAddressFromMemory(currentMemoryPtr + sizeof(size_t));
    size_t freeMemorySize = ReadSizeFromMemory(currentMemoryPtr);

    size_t totalAllocationSize = size + alignment + sizeof(size_t);

    while (freeMemorySize < totalAllocationSize)
    {
        EXCEPTION(nextFreeMemoryPtr == nullptr, "No properly sized memory block available.");

        // Traverse to next memory block
        prevFreeMemoryPtr = currentMemoryPtr;
        currentMemoryPtr = nextFreeMemoryPtr;
        nextFreeMemoryPtr = ReadAddressFromMemory(currentMemoryPtr + sizeof(size_t));
        freeMemorySize = ReadSizeFromMemory(currentMemoryPtr);
    }


    // Adjust allocation size if not enough space for a free memory header is left and update pointer of previous free
    // memory header
    size_t leftMemorySize = freeMemorySize - totalAllocationSize;
    if (leftMemorySize < sizeof(size_t) + sizeof(void*))
    {
        totalAllocationSize = freeMemorySize;
        leftMemorySize = 0;
        if (prevFreeMemoryPtr != nullptr)
            WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);
    }
    else
    {
        U8* leftFreeMemoryPtr = currentMemoryPtr + totalAllocationSize;
        WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
        WriteAddressToMemory(leftFreeMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);

        if (prevFreeMemoryPtr == nullptr)
            mFirstFreeMemoryPtr = leftFreeMemoryPtr;
        else
            WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), leftFreeMemoryPtr);

        if (nextFreeMemoryPtr == nullptr)
            mLastFreeMemoryPtr = leftFreeMemoryPtr;
    }


    // Store allocation size in memory in front of returned pointer. This is needed for deallocation
    WriteSizeToMemory(currentMemoryPtr, totalAllocationSize);
    currentMemoryPtr += sizeof(size_t);

    // Align memory and store alignment correction in preceding byte
    size_t misalignment = Misalignment(currentMemoryPtr, alignment);
    size_t correction = alignment - misalignment;
    U8* allocatedMemoryPtr = currentMemoryPtr + correction;
    allocatedMemoryPtr[-1] = static_cast<U8>(correction);


    std::cout << (void*)allocatedMemoryPtr << std::endl;

    // return pointer
    return allocatedMemoryPtr;
}

void GeneralPurposeMemory::CheckMemoryConsistency() const
{
    U8* currentMemoryPtr = mFirstFreeMemoryPtr;
    U8* nextFreeMemoryPtr = ReadAddressFromMemory(currentMemoryPtr + sizeof(size_t));
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
    size_t blockSize = ReadSizeFromMemory(currentMemoryPtr);
    while (currentMemoryPtr != totalMemoryEndPtr)
    {
        currentMemoryPtr += blockSize;
        blockSize = ReadSizeFromMemory(currentMemoryPtr);

        EXCEPTION(blockSize <= 0, "Read block size <= 0.");
        EXCEPTION(currentMemoryPtr > totalMemoryEndPtr,
                  "Read block size bigger than the distance to the memories end.");
    }
}

void GeneralPurposeMemory::Deallocate(void* address)
{
    std::lock_guard<std::mutex> lock{mMutex};

    // find previous and next free memory block of freed address. Therefore the free memory blocks is traversed until
    // the both enclosing blocks are found (special case before the first element and behind the last)

    // Merge the freed block with its enclosing blocks if possible.

    // Update pointer of previous block (or lastFree elementPointer) if necessary
}

void GeneralPurposeMemory::Deinitialize()
{
    std::lock_guard<std::mutex> lock{mMutex};

    EXCEPTION(!IsInitialized(), "General purpose memory is already deinitialized.");
    EXCEPTION(mNumAllocations != 0, "Can't deinitialize. Memory still in use.");

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

void GeneralPurposeMemory::CheckConstructionParameters() const
{
    EXCEPTION(mMemorySize < 1, "Memory size must be bigger than 1");
}

bool GeneralPurposeMemory::IsInitialized() const
{
    return mMemory != nullptr;
}

U8* GeneralPurposeMemory::ReadAddressFromMemory(const U8* positionInMemory) const
{
    U8* address = nullptr;
    std::memcpy(&address, positionInMemory, sizeof(void*));
    return address;
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
