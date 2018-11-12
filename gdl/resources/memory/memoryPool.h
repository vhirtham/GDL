#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/resources/cpu/spinlock.h"
#include "gdl/resources/memory/memoryInterface.h"
#include "gdl/resources/memory/memorySize.h"

#include <memory>



namespace GDL
{

//! @brief Memory system that stores equally sized memory blocks. Free memory blocks are used to keep a linked list
//! of free elements. This way allocations and deallocations are constant time operations.
class MemoryPool : public MemoryInterface
{
    MemorySize mElementSize;
    size_t mAlignment;
    U32 mNumElements;
    U32 mNumFreeElements;
    U8* mMemoryStart;
    U8* mFirstFreeElement;
    U8* mLastFreeElement;
    std::unique_ptr<U8[]> mMemory;
    mutable SpinLock mSpinLock;

public:
    //! @brief Creates the memory pool with <numElements> memory slots for elements of size <elementSize>
    //! @param elementSize: Size of a single element
    //! @param numElements: Number of elements that can be stored
    //! @param alignment: Memory alignment
    MemoryPool(MemorySize elementSize, U32 numElements, size_t alignment = 1);

    MemoryPool() = delete;
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    MemoryPool& operator=(MemoryPool&&) = delete;
    ~MemoryPool() override;

    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @param alignment: Alignment of the memory that should be allocated
    //! @return Pointer to memory
    virtual void* Allocate(size_t size, size_t alignment = 1) override;

    //! @brief Checks the internal consistency of the memory pool
    void CheckMemoryConsistency() const;

    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    //! @param alignment: Alignment of the memory that should be deallocated
    virtual void Deallocate(void* address, size_t alignment = 1) override;

    //! @brief Deinitializes the memory pool
    void Deinitialize();

    //! @brief Initializes the memory pool and sets up the internal linked list of free memory blocks
    void Initialize();

    //! @brief Gets the memory alignment value
    //! @return Memory alignment value
    size_t GetAlignment() const;

    //! @brief Gets the element size
    //! @return Element size
    MemorySize GetElementSize() const;

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
    size_t EffectiveMemorySize() const;

    //! @brief Returns the memory size including the extra alignment bytes
    //! @return Memory size including the extra alignment bytes
    size_t TotalMemorySize() const;
};
}
