#pragma once

#include "gdl/GDLTypedefs.h"

#include <atomic>
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
    mutable std::mutex mMutex;
    std::atomic_bool mInitialized;
    std::unique_ptr<U8[]> mMemory;
    U8* mMemoryStart;
    U8* mFirstFreeElement;
    U8* mLastFreeElement;

public:
    //! @brief Creates the memory pool with <numElements> memory slots for elements of size <elementSize>
    //! @param elementSize: Size of a single element
    //! @param numElements: Number of elements that can be stored
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

    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    void Deallocate(void* address);

    //! @brief Checks the internal consistency of the memory pool
    void CheckConsistency() const;

    //! @brief Initializes the memory pool and sets up the internal linked list of free memory blocks
    void Initialize();

    //! @brief Deinitializes the memory pool
    void Deinitialize();

private:
    //! @brief Aligns the memory.
    void AlignMemory();

    //! @brief Returns the memory size including the extra alignment bytes
    //! @return Memory size including the extra alignment bytes
    size_t TotalMemorySize() const;

    //! @brief Returns the memory size without the extra alignment bytes
    //! @return Memory size without the extra alignment bytes
    size_t MemorySize() const;

    //! @brief Checks if the memory pool is constructed with valid parameters: Throws if not.
    void CheckConstructionParameters() const;

    //! @brief Checks if the address that should be freed is valid. Throws if not.
    //! @param address: Adress that should be freed
    void CheckDeallocation(void* address) const;



    //! @brief Reads the value of the internal free memory list at the given position and returns the pointer to the
    //! next element.
    //! @param addressToRead: Adress where the internal free memory list should be read.
    //! @return Pointer to next free memory block (nullptr if there is no free memory)
    U8* ReadListEntry(const U8* addressToRead) const;

    //! @brief Initializes the internal free memory list.
    void InitializeFreeMemoryList();

    //! @brief Writes a new entry to the free memory list
    //! @param positionInMemory: Position in memory where the value should be written
    //! @param addressToWrite: Address to next element that should be written to the current position
    void WriteListEntry(U8* positionInMemory, const void* addressToWrite);
};
}
