#pragma once

#include "gdl/GDLTypedefs.h"

#include <memory>
#include <mutex>



namespace GDL
{

//! @brief Memory system that stores equally sized memory blocks. Free memory blocks are used to keep a linked list
//! of free elements. This way allocations and deallocations are constant time operations.
class MemoryPool
{
    size_t mElementSize;
    size_t mAlignment;
    U32 mNumElements;
    U32 mNumFreeElements;
    U8* mMemoryStart;
    U8* mFirstFreeElement;
    U8* mLastFreeElement;
    std::unique_ptr<U8[]> mMemory;
    mutable std::mutex mMutex;

public:
    //! @brief Creates the memory pool with <numElements> memory slots for elements of size <elementSize>
    //! @param elementSize: Size of a single element
    //! @param numElements: Number of elements that can be stored
    //! @param alignment: Memory alignment
    MemoryPool(size_t elementSize, U32 numElements, size_t alignment = 1);

    MemoryPool() = delete;
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    MemoryPool& operator=(MemoryPool&&) = delete;
    ~MemoryPool();

    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @return Pointer to memory
    void* Allocate(size_t size);

    //! @brief Checks the internal consistency of the memory pool
    void CheckMemoryConsistency() const;

    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    void Deallocate(void* address);

    //! @brief Deinitializes the memory pool
    void Deinitialize();

    //! @brief Initializes the memory pool and sets up the internal linked list of free memory blocks
    void Initialize();

private:
    //! @brief Aligns the memory.
    void AlignMemory();

    //! @brief Checks if the memory pool is constructed with valid parameters. Throws if not.
    void CheckConstructionParameters() const;

    //! @brief Checks if the address that should be freed is valid. Throws if not.
    //! @param address: Adress that should be freed
    void CheckDeallocation(void* address) const;

    //! @brief Checks the internal consistency of the memory pool
    //! @remark In contrast to the public CheckConsistency, this version does not aquire a lock. This way it can be used
    //! in functions that already aquiered a lock without introducing any further locking complexity.
    void CheckMemoryConsistencyPrivate() const;

    //! @brief Returns if the memory pool is initialized
    //! @return TRUE / FALSE
    bool IsInitialized() const;

    //! @brief Initializes the internal free memory list.
    void InitializeFreeMemoryList();

    //! @brief Returns the memory size without the extra alignment bytes
    //! @return Memory size without the extra alignment bytes
    size_t MemorySize() const;

    //! @brief Returns the memory size including the extra alignment bytes
    //! @return Memory size including the extra alignment bytes
    size_t TotalMemorySize() const;
};
}
