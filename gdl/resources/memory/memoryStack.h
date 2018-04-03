#pragma once

#include "gdl/GDLTypedefs.h"

#include <cassert>
#include <memory>

#include <iostream>

namespace GDL
{
class ThreadPrivateMemoryStack
{
    I32 mNumAllocations = 0;
    U32 mMemorySize;
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
        , mMemory{std::make_unique<U8[]>(mMemorySize)} // first new call
        , mCurrentPosition{mMemory.get()}
    {
    }

    ~ThreadPrivateMemoryStack()
    {
        assert(mNumAllocations == 0);
    }

    void* Allocate(U32 size)
    {
        // std::cout << static_cast<void*>(mCurrentPosition) << std::endl;
        assert(mCurrentPosition + size < mMemory.get() + mMemorySize);
        U8* position = mCurrentPosition;
        mCurrentPosition += size;
        // std::cout << static_cast<void*>(mCurrentPosition) << std::endl;
        ++mNumAllocations;
        return position;
    }

    void Deallocate()
    {
        assert(mNumAllocations > 0);
        --mNumAllocations;
        if (mNumAllocations == 0)
            mCurrentPosition = mMemory.get();
    }
};
}
