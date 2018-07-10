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

    U8* currentMemoryPtr = mFirstFreeMemoryPtr;
    U8* prevFreeMemoryPtr = nullptr;
    U8* nextFreeMemoryPtr = ReadAddressFromMemory(currentMemoryPtr + sizeof(size_t));
    size_t freeMemorySize = ReadSizeFromMemory(currentMemoryPtr);
    size_t totalAllocationSize = size + alignment + sizeof(size_t);

    // ---- free memory size > totalAllocationSize
    // ---- adjust total allocation size if left memory is not big enough for free memory info (size + ptr to next free
    // header)

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
        if (prevFreeMemoryPtr != nullptr)
            WriteAddressToMemory(prevFreeMemoryPtr + sizeof(size_t), leftFreeMemoryPtr);
        WriteSizeToMemory(leftFreeMemoryPtr, leftMemorySize);
        WriteAddressToMemory(leftFreeMemoryPtr + sizeof(size_t), nextFreeMemoryPtr);

        if (prevFreeMemoryPtr == nullptr)
            mFirstFreeMemoryPtr = leftFreeMemoryPtr;
        if (nextFreeMemoryPtr == nullptr)
            mLastFreeMemoryPtr = leftFreeMemoryPtr;
    }


    // Store allocation size in memory in front of returned pointer
    WriteSizeToMemory(currentMemoryPtr, totalAllocationSize);
    currentMemoryPtr += sizeof(size_t);

    // Align memory and store alignment correction in preceding byte
    size_t misalignment = Misalignment(currentMemoryPtr, alignment);
    size_t correction = alignment - misalignment;
    U8* allocatedMemoryPtr = currentMemoryPtr + correction;
    allocatedMemoryPtr[-1] = static_cast<U8>(correction);


    std::cout << (void*)allocatedMemoryPtr << std::endl;

    return allocatedMemoryPtr;
}

void GeneralPurposeMemory::Deallocate(void* address)
{
    std::lock_guard<std::mutex> lock{mMutex};
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
