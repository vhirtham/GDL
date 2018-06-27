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
    U32 mElementSize;
    U32 mMemorySize;
    U32 mFreeMemorySize;
    mutable std::mutex mMutex;
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
    ~MemoryPool();

    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @return Pointer to memory
    void* Allocate(U32 size);

    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    void Deallocate(void* address);

    //! @brief Checks the internal consistency of the memory pool
    void CheckConsistency() const;

private:
    //! @brief Initializes the memory pool and sets up the internal linked list of free memory blocks
    void Initialize();

    //! @brief Reads the value of the internal free memory list at the given position and returns the pointer to the
    //! next element.
    //! @param addressToRead: Adress where the internal free memory list should be read.
    //! @return Pointer to next free memory block (nullptr if there is no free memory)
    U8* ReadListEntry(const U8* addressToRead) const;

    //! @brief Writes a new entry to the free memory list
    //! @param positionInMemory: Position in memory where the value should be written
    //! @param addressToWrite: Address to next element that should be written to the current position
    void WriteListEntry(U8* positionInMemory, const void* addressToWrite);
};
}
