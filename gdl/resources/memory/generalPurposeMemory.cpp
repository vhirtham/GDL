#include "gdl/resources/memory/generalPurposeMemory.h"

#include "gdl/base/Exception.h"

namespace GDL
{

GeneralPurposeMemory::GeneralPurposeMemory(size_t memorySize)
    : mMemorySize{memorySize}
    , mNumAllocations{0}
    , mFirstFreeMemory{nullptr}
    , mLastFreeMemory{nullptr}
    , mMemory{nullptr}
{
    CheckConstructionParameters();
}

GeneralPurposeMemory::~GeneralPurposeMemory()
{
}

void GeneralPurposeMemory::Deinitialize()
{
    std::lock_guard<std::mutex> lock{mMutex};

    EXCEPTION(!IsInitialized(), "General purpose memory is already deinitialized.");
    EXCEPTION(mNumAllocations != 0, "Can't deinitialize. Memory still in use.");

    mFirstFreeMemory = nullptr;
    mLastFreeMemory = nullptr;
    mMemory.reset(nullptr);
}

void GeneralPurposeMemory::Initialize()
{
    std::lock_guard<std::mutex> lock{mMutex};

    EXCEPTION(IsInitialized(), "General purpose memory is already initialized.");

    mMemory.reset(new U8[mMemorySize]);
    mNumAllocations = 0;
    mFirstFreeMemory = mMemory.get();
    mLastFreeMemory = mFirstFreeMemory;
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
