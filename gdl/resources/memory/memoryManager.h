#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"
#include "gdl/resources/memory/memoryStack.h"

#include <atomic>
#include <cassert>
#include <map>
#include <memory>
#include <thread>

namespace GDL
{
class MemoryManager
{
    // U64 mMemorySize;
    // std::unique_ptr<U8[]> mMemory;
    // std::atomic<U8*> mCurrentPosition;

    std::map<std::thread::id, ThreadPrivateMemoryStack> mInternalStack;

    MemoryManager() = default;

public:
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager& operator=(MemoryManager&&) = delete;
    ~MemoryManager() = default;

    static MemoryManager& Instance()
    {
        static MemoryManager memoryManager;
        return memoryManager;
    }

    void* Allocate();

    void* AllocateInternalStack(U32 size)
    {
        constexpr U32 memorySize = 1024 * 1024;
        const std::thread::id id = std::this_thread::get_id();
        auto memoryStackIterator = mInternalStack.find(id);
        if (memoryStackIterator == mInternalStack.end())
            memoryStackIterator = mInternalStack.emplace(id, ThreadPrivateMemoryStack(memorySize)).first; // second new

        assert(mInternalStack.find(id) != mInternalStack.end());
        assert(mInternalStack.find(id) == memoryStackIterator);
        return memoryStackIterator->second.Allocate(size);
    }

    void DeallocateInternalStack()
    {
        const std::thread::id id = std::this_thread::get_id();
        assert(mInternalStack.find(id) != mInternalStack.end());
        mInternalStack.find(id)->second.Deallocate();
    }
};

// MemoryManager::MemoryManager(U64 memorySize)
//    : mMemorySize(memorySize)
//    , mMemory(std::make_unique<U8[]>(mMemorySize))
//    , mCurrentPosition(mMemory.get())
//{
//    if (memorySize < 1)
//        throw Exception(__PRETTY_FUNCTION__,
//                        "Can't construct memory Manager with a memory size of 0! You need to call "
//                        "MemoryManager::Instance(U64 memorySize) with the desired memory size before the first "
//                        "allocation takes place!");
//}
}
