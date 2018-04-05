#include "gdl/resources/memory/memoryManager.h"

namespace GDL
{
// static variable initialization
bool MemoryManager::mInitialized = false;
U32 MemoryManager::mMaxNumThreads = 8;
U32 MemoryManager::mInternalStackSizeThread = 1024 * 1024;
U64 MemoryManager::mFreeMemorySize = 0;


MemoryManager::MemoryManager()
    : mMemory(std::make_unique<U8[]>(mFreeMemorySize + mMaxNumThreads * mInternalStackSizeThread))
{
    // lock with global/static class mutex. Then check if already true - if yes throw with info that two threads were
    // initializing the memory manager via instance. User should always initialize the memory manager first, before
    // multiple threads are spawned.
    mInitialized = true;
}

MemoryManager& MemoryManager::Instance()
{
    static MemoryManager memoryManager;
    return memoryManager;
}

void MemoryManager::Initialize()
{
    MemoryManager::Instance();
}

void MemoryManager::SetInternalThreadStackSize(U32 size)
{
    ThrowOnInitialized();
    mInternalStackSizeThread = size;
}



void* MemoryManager::AllocateInternalThreadStack(U32 size)
{

    // shared lock
    const std::thread::id id = std::this_thread::get_id();
    auto memoryStackIterator = mInternalStack.find(id);
    // make it != and call allocate.
    if (memoryStackIterator == mInternalStack.end())
        memoryStackIterator = mInternalStack.emplace(id, ThreadPrivateMemoryStack(mInternalStackSizeThread)).first;
    // if the function makes it past the if - which should returnand unlock the shared lock, the entry is not in the
    // map.
    // lock a unique lock, create the entry, allocate and return
    assert(mInternalStack.find(id) != mInternalStack.end());
    assert(mInternalStack.find(id) == memoryStackIterator);
    return memoryStackIterator->second.Allocate(size);
}



void MemoryManager::DeallocateInternalThreadStack()
{
    const std::thread::id id = std::this_thread::get_id();
    assert(mInternalStack.find(id) != mInternalStack.end());
    mInternalStack.find(id)->second.Deallocate();
}

void MemoryManager::ThrowOnInitialized()
{
    if (mInitialized)
        throw Exception(__PRETTY_FUNCTION__,
                        "You can set the memory managers parameters only before the first call to Instance()!");
}

} // namespace GDL
