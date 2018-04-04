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
    mInitialized = true;
}

MemoryManager& MemoryManager::Instance()
{
    static MemoryManager memoryManager;
    return memoryManager;
}

void MemoryManager::SetInternalThreadStackSize(U32 size)
{
    ThrowOnInitialized();
    mInternalStackSizeThread = size;
}



void* MemoryManager::AllocateInternalThreadStack(U32 size)
{

    const std::thread::id id = std::this_thread::get_id();
    auto memoryStackIterator = mInternalStack.find(id);
    if (memoryStackIterator == mInternalStack.end())
        memoryStackIterator = mInternalStack.emplace(id, ThreadPrivateMemoryStack(mInternalStackSizeThread)).first;

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
