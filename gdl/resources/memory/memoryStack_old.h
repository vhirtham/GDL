#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"

#include <cassert>
#include <memory>
#include <thread>
#include <iostream>

namespace GDL
{
class ThreadPrivateMemoryStack
{
    I32 mNumAllocations = 0;
    U32 mMemorySize;
    std::thread::id mThreadId;
    std::unique_ptr<U8[]> mMemory;
    U8* mCurrentPosition;

public:
    ThreadPrivateMemoryStack() = delete;
    ThreadPrivateMemoryStack(const ThreadPrivateMemoryStack&) = delete;
    ThreadPrivateMemoryStack(ThreadPrivateMemoryStack&&) = default;
    ThreadPrivateMemoryStack& operator=(const ThreadPrivateMemoryStack&) = delete;
    ThreadPrivateMemoryStack& operator=(ThreadPrivateMemoryStack&&) = default;

    ThreadPrivateMemoryStack(U32 memorySize)
        : mNumAllocations{0}
        , mMemorySize{memorySize}
        , mThreadId{std::this_thread::get_id()}
        , mMemory{std::make_unique<U8[]>(mMemorySize)}
        , mCurrentPosition{mMemory.get()}
    {
    }

    ~ThreadPrivateMemoryStack()
    {
        assert(mNumAllocations == 0);
    }

    void* Allocate(U32 size)
    {
        assert(std::this_thread::get_id() == mThreadId);
        if (mCurrentPosition + size > mMemory.get() + mMemorySize)
            throw Exception(__PRETTY_FUNCTION__, "Thread private memory stack overflow.");
        U8* position = mCurrentPosition;
        mCurrentPosition += size;
        ++mNumAllocations;
        return position;
    }

    void Deallocate()
    {
        assert(std::this_thread::get_id() == mThreadId);
        assert(mNumAllocations > 0);
        --mNumAllocations;
        if (mNumAllocations == 0)
            mCurrentPosition = mMemory.get();
    }
};
}
