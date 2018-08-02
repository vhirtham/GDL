#include "gdl/resources/memory/memoryManager.h"

#include "gdl/base/Exception.h"


namespace GDL
{

MemoryManager::MemoryManager()
    : mInitialized{false}
    , mSetupFinished{false}
    , mMemoryRequestedUninitialized{false}
    , mGeneralPurposeMemory{nullptr}
{
}

void MemoryManager::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutex);

    EXCEPTION(mMemoryRequestedUninitialized,
              "Can't initialize. There was a request for memory before the initialization.");
    EXCEPTION(mGeneralPurposeMemory == nullptr, "Can't initialize. No memory added to memory manager.");

    if (mGeneralPurposeMemory != nullptr)
        mGeneralPurposeMemory->Initialize();

    mSetupFinished = true;
    mInitialized = true;
}

MemoryManager& MemoryManager::Instance()
{
    static MemoryManager memoryManager;
    return memoryManager;
}

void MemoryManager::Deinitialize()
{
    std::lock_guard<std::mutex> lock(mMutex);

    EXCEPTION(mInitialized == false, "Can't deinitialize. Memory manager was not initialized.");
    EXCEPTION(mGeneralPurposeMemory == nullptr, "Can't deinitialize. No memory added to memory manager.");

    if (mGeneralPurposeMemory != nullptr)
        mGeneralPurposeMemory->Deinitialize();

    mInitialized = false;
}

MemoryInterface* GDL::MemoryManager::GetGeneralPurposeMemory() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mInitialized == false)
    {
        mSetupFinished = true;
        mMemoryRequestedUninitialized = true;
    }
    return mGeneralPurposeMemory.get();
}

void MemoryManager::CreateGeneralPurposeMemory(size_t memorySize)
{
    std::lock_guard<std::mutex> lock(mMutex);

    EXCEPTION(mSetupFinished == true, "Setup process already finished.");
    EXCEPTION(mGeneralPurposeMemory != nullptr, "Genaral purpose memory already created");
    mGeneralPurposeMemory.reset(new GeneralPurposeMemory{memorySize});
}
}
