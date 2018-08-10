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
    EXCEPTION(mGeneralPurposeMemory == nullptr && mMemoryPools.empty(),
              "Can't initialize. No memory added to memory manager.");

    if (mGeneralPurposeMemory != nullptr)
        mGeneralPurposeMemory->Initialize();

    for (auto& memoryPool : mMemoryPools)
        memoryPool.second.Initialize();

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

    if (mGeneralPurposeMemory != nullptr)
        mGeneralPurposeMemory->Deinitialize();

    for (auto& memoryPool : mMemoryPools)
        memoryPool.second.Deinitialize();

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



MemoryInterface* MemoryManager::GetMemoryPool(size_t elementSize, size_t alignment) const
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mInitialized == false)
    {
        mSetupFinished = true;
        mMemoryRequestedUninitialized = true;
    }
    for (auto& it : mMemoryPools)
        if (it.second.GetElementSize() >= elementSize && it.second.GetAlignment() >= alignment)
            return &const_cast<MemoryPool&>(it.second);

    return mGeneralPurposeMemory.get();
}



void MemoryManager::CreateGeneralPurposeMemory(size_t memorySize)
{
    std::lock_guard<std::mutex> lock(mMutex);

    EXCEPTION(mSetupFinished == true, "Setup process already finished.");
    EXCEPTION(mGeneralPurposeMemory != nullptr, "Genaral purpose memory already created");

    mGeneralPurposeMemory.reset(new GeneralPurposeMemory{memorySize});
}

void MemoryManager::CreateMemoryPool(size_t elementSize, U32 numElements, size_t alignment)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (alignment == 0)
        alignment = elementSize;
    EXCEPTION(mSetupFinished == true, "Setup process already finished.");
    EXCEPTION(mMemoryPools.find(elementSize) != mMemoryPools.end(),
              "There already is a memory pool with size " + std::to_string(elementSize));

    mMemoryPools.emplace(std::piecewise_construct, std::forward_as_tuple(elementSize),
                         std::forward_as_tuple(elementSize, numElements, alignment));
}
}
