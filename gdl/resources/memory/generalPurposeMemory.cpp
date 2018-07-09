#include "gdl/resources/memory/generalPurposeMemory.h"

#include "gdl/base/Exception.h"
#include "gdl/base/SSESupportFunctions.h"

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

    U8* currentMemoryPtr = mFirstFreeMemoryPtr;
    size_t freeMemorySize = *reinterpret_cast<size_t*>(currentMemoryPtr);
    size_t totalAllocationSize = size + alignment + sizeof(size_t);

    // Add header size (header stores size of the allocation for deallocation)
    currentMemoryPtr += sizeof(size_t);
    size_t misalignment = Misalignment(currentMemoryPtr, alignment);
    size_t correction = alignment - misalignment;

    DEV_EXCEPTION(alignment > 255, "Alignment must be smaller than 256");

    U8* allocatedMemoryPtr = currentMemoryPtr + correction;

    // Write number of correction bytes to preceding byte
    allocatedMemoryPtr[-1] = static_cast<U8>(correction);


    // ----------- Write allocated memory size to original pointer address


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

    // Writes the first free memory header
    size_t* freeMemoryHeader = reinterpret_cast<size_t*>(mFirstFreeMemoryPtr);
    *freeMemoryHeader = mMemorySize - sizeof(size_t);
    // ---------- also needs to store 1 or 2 nullptrs for size_t* (linked list to next free blocks)
}

void GeneralPurposeMemory::CheckConstructionParameters() const
{
    EXCEPTION(mMemorySize < 1, "Memory size must be bigger than 1");
}

bool GeneralPurposeMemory::IsInitialized() const
{
    return mMemory != nullptr;
}

} // namespace GDL
