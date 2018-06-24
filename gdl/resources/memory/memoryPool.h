#pragma once

#include "gdl/GDLTypedefs.h"

#include "memory"

namespace GDL
{

class MemoryPool
{
    U32 mElementSize;
    U32 mMemorySize;
    U32 mFreeMemorySize;
    std::unique_ptr<U8[]> mMemory;
    U8* mFirstFreeElement;
    U8* mLastFreeElement;

public:
    //! @brief Creates the memory pool with <memorySize> memory slots for elements of size <elementSize>
    //! @param elementSize: Size of a single element
    //! @param memorySize: Number of elements that can be stored
    MemoryPool(U32 elementSize, U32 memorySize);

    MemoryPool() = delete;
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    MemoryPool& operator=(MemoryPool&&) = delete;
    ~MemoryPool() = default;

    void* Allocate(U32 size);

    void Deallocate(void* address);

    void CheckConsistency() const;

private:

    void Initialize();
};
}
