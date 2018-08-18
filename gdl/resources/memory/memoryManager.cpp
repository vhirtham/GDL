#include "gdl/resources/memory/memoryManager.h"

#include "gdl/base/exception.h"
#include "gdl/resources/memory/heapMemory.h"


namespace GDL
{

MemoryManager::MemoryManager()
    : mInitialized{false}
    , mThreadPrivateMemoryEnabled{false}
    , mSetupFinished{false}
    , mMemoryRequestedUninitialized{false}
    , mGeneralPurposeMemory{nullptr}
    , mMemoryStack{nullptr}
{
}



void MemoryManager::Initialize()
{
    std::lock_guard<std::shared_mutex> lock(mMutex);

    EXCEPTION(mMemoryRequestedUninitialized,
              "Can't initialize. There was a request for memory before the initialization.");
    EXCEPTION(mGeneralPurposeMemory == nullptr && mMemoryPools.empty(),
              "Can't initialize. No memory added to memory manager.");

    if (mGeneralPurposeMemory != nullptr)
        mGeneralPurposeMemory->Initialize();

    for (auto& memoryPool : mMemoryPools)
        memoryPool.second.Initialize();

    if (mMemoryStack != nullptr)
        mMemoryStack->Initialize();

    mSetupFinished = true;
    mInitialized = true;
}



void MemoryManager::EnableThreadPrivateMemory()
{
    std::lock_guard<std::shared_mutex> lock(mMutex);

    EXCEPTION(mSetupFinished == true, "Setup process already finished.");
    EXCEPTION(mThreadPrivateMemoryEnabled, "Thread private memory is already enabled.");

    mThreadPrivateMemoryEnabled = true;
}

bool MemoryManager::IsThreadPrivateMemoryEnabled() const
{
    return mThreadPrivateMemoryEnabled;
}



MemoryManager& MemoryManager::Instance()
{
    static MemoryManager memoryManager;
    return memoryManager;
}



void MemoryManager::Deinitialize()
{
    std::lock_guard<std::shared_mutex> lock(mMutex);

    EXCEPTION(mInitialized == false, "Can't deinitialize. Memory manager was not initialized.");
    EXCEPTION(mThreadPrivateMemoryEnabled && !mThreadPrivateMemoryStacks.empty(),
              "Can't deinitialize. All thread private memory needs to be deleted first.");

    if (mGeneralPurposeMemory != nullptr)
        mGeneralPurposeMemory->Deinitialize();

    for (auto& memoryPool : mMemoryPools)
        memoryPool.second.Deinitialize();

    if (mMemoryStack != nullptr)
        mMemoryStack->Deinitialize();

    mInitialized = false;
}



MemoryInterface* GDL::MemoryManager::GetGeneralPurposeMemory() const
{
    std::shared_lock<std::shared_mutex> lock(mMutex);
    if (mInitialized == false)
    {
        mSetupFinished = true;
        mMemoryRequestedUninitialized = true;
        return nullptr;
    }
    return mGeneralPurposeMemory.get();
}



MemoryInterface* MemoryManager::GetHeapMemory() const
{
    std::shared_lock<std::shared_mutex> lock(mMutex);

    if (mInitialized == false)
    {
        mSetupFinished = true;
        mMemoryRequestedUninitialized = true;
    }

    static HeapMemory memory;
    return &memory;
}

MemoryInterface* MemoryManager::GetMemoryStack() const
{
    std::shared_lock<std::shared_mutex> lock(mMutex);
    if (mInitialized == false)
    {
        mSetupFinished = true;
        mMemoryRequestedUninitialized = true;
        return nullptr;
    }
    return mMemoryStack.get();
}



MemoryInterface* MemoryManager::GetMemoryPool(size_t elementSize, size_t alignment) const
{
    std::shared_lock<std::shared_mutex> lock(mMutex);
    if (mInitialized == false)
    {
        mSetupFinished = true;
        mMemoryRequestedUninitialized = true;
        return nullptr;
    }
    for (auto& it : mMemoryPools)
        if (it.second.GetElementSize() >= elementSize && it.second.GetAlignment() >= alignment)
            return &const_cast<MemoryPool&>(it.second);

    return nullptr;
}

MemoryInterface* MemoryManager::GetThreadPrivateMemoryStack() const
{
    std::shared_lock<std::shared_mutex> lock(mMutex);
    if (mInitialized == false)
    {
        mSetupFinished = true;
        mMemoryRequestedUninitialized = true;
        return nullptr;
    }

    auto it = mThreadPrivateMemoryStacks.find(std::this_thread::get_id());
    if (it != mThreadPrivateMemoryStacks.end())
        return &const_cast<ThreadPrivateMemoryStack&>(it->second);
    return nullptr;
}



void MemoryManager::CreatePrivateMemoryStackForThisThread(MemorySize memorySize)
{
    std::lock_guard<std::shared_mutex> lock(mMutex);
    EXCEPTION(mInitialized == false, "Memory manager needs to be initialized to add thread private memory.");
    EXCEPTION(mThreadPrivateMemoryEnabled == false,
              "Thread private memory needs to be enabled during setup of the memory manager.");
    EXCEPTION(mThreadPrivateMemoryStacks.find(std::this_thread::get_id()) != mThreadPrivateMemoryStacks.end(),
              "Current thread already has a thread private memory stack.");

    auto[it, success] = mThreadPrivateMemoryStacks.emplace(std::this_thread::get_id(), memorySize);

    EXCEPTION(success == false, "Thread private memory stack could not be inserted into map.");
    it->second.Initialize();
}



void MemoryManager::CreateGeneralPurposeMemory(MemorySize memorySize)
{
    std::lock_guard<std::shared_mutex> lock(mMutex);

    EXCEPTION(mSetupFinished == true, "Setup process already finished.");
    EXCEPTION(mGeneralPurposeMemory != nullptr, "Genaral purpose memory already created");

    mGeneralPurposeMemory.reset(new GeneralPurposeMemory{memorySize});
}



void MemoryManager::CreateMemoryStack(MemorySize memorySize)
{
    std::lock_guard<std::shared_mutex> lock(mMutex);

    EXCEPTION(mSetupFinished == true, "Setup process already finished.");
    EXCEPTION(mMemoryStack != nullptr, "Memory stack already created");

    mMemoryStack.reset(new MemoryStack{memorySize});
}



void MemoryManager::CreateMemoryPool(MemorySize elementSize, U32 numElements, size_t alignment)
{
    std::lock_guard<std::shared_mutex> lock(mMutex);
    if (alignment == 0)
        alignment = elementSize.GetNumBytes();
    EXCEPTION(mSetupFinished == true, "Setup process already finished.");
    EXCEPTION(mMemoryPools.find(elementSize.GetNumBytes()) != mMemoryPools.end(),
              "There already is a memory pool with size " + std::to_string(elementSize.GetNumBytes()));

    mMemoryPools.emplace(std::piecewise_construct, std::forward_as_tuple(elementSize.GetNumBytes()),
                         std::forward_as_tuple(elementSize, numElements, alignment));
}



void MemoryManager::DeletePrivateMemoryStackForThisThread()
{
    std::lock_guard<std::shared_mutex> lock(mMutex);

    auto it = mThreadPrivateMemoryStacks.find(std::this_thread::get_id());

    EXCEPTION(mInitialized == false, "Memory manager needs to be initialized to delete thread private memory.");
    EXCEPTION(mThreadPrivateMemoryEnabled == false,
              "Thread private memory needs to be enabled during setup of the memory manager.");
    EXCEPTION(it == mThreadPrivateMemoryStacks.end(),
              "There is no thread private memory stack for the current thread.");

    it->second.Deinitialize();
    mThreadPrivateMemoryStacks.erase(std::this_thread::get_id());
}

} // namespace GDL
