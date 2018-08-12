#pragma once

#include "gdl/resources/memory/generalPurposeMemory.h"
#include "gdl/resources/memory/memoryPool.h"
#include "gdl/resources/memory/memorySize.h"
#include "gdl/resources/memory/memoryStack.h"

#include <atomic>
#include <map>
#include <memory>
#include <mutex>

namespace GDL
{

class MemoryManager
{
    std::atomic_bool mInitialized;
    mutable std::mutex mMutex;
    mutable bool mSetupFinished;
    mutable bool mMemoryRequestedUninitialized;

    std::unique_ptr<GeneralPurposeMemory> mGeneralPurposeMemory;
    std::unique_ptr<memoryStack> mMemoryStack;
    std::map<size_t, MemoryPool> mMemoryPools;


    //! @brief Private ctor since this class should only be used as a singleton
    MemoryManager();

public:
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager& operator=(MemoryManager&&) = delete;
    ~MemoryManager() = default;



    //! @brief Gets the memory manager instance
    //! @return Memory manager Instance
    static MemoryManager& Instance();

    //! @brief Deinitializes the memory manager
    void Deinitialize();

    //! @brief Initializes the memory manager
    void Initialize();

    //! @brief Returns an memory interface pointer to the general purpose memory
    //! @return Pointer to the general purpose memory if it exists. Otherwise nullptr
    MemoryInterface* GetGeneralPurposeMemory() const;


    //! @brief Returns an memory interface pointer to the heap memory
    //! @return Pointer to the heap memory
    MemoryInterface* GetHeapMemory() const;

    //! @brief Returns an memory interface pointer to the memory stack
    //! @return Pointer to the memory stack if it exists. Otherwise nullptr
    MemoryInterface* GetMemoryStack() const;

    //! @brief Returns an memory interface pointer to a fitting memory pool.
    //! @param elementSize: Size of the data type which should fit into the memory pool.
    //! @param alignment: Alignment of the data type which should fit into the memory pool.
    //! @return Pointer to a fitting memory pool if it exist. Otherwise nullptr
    MemoryInterface* GetMemoryPool(size_t elementSize, size_t alignment) const;

    //! @brief Creates a general purpose memory
    //! @param memorySize: Size of the general purpose memory
    void CreateGeneralPurposeMemory(MemorySize memorySize);

    //! @brief Creates a memory stack
    //! @param memorySize: Size of the memory stack
    void CreateMemoryStack(MemorySize memorySize);

    //! @brief Creates a memory pool
    //! @param elementSize: Size of a single element of the memory pool
    //! @param numElements: Number of elements that can be stored in the memory pool
    //! @param alignment: Alignment of the memory pool (default: alignment=elementSize)
    //! @remark If the alignment value is set to 0, the alignment is set to the element size
    void CreateMemoryPool(MemorySize elementSize, U32 numElements, size_t alignment = 0);
};
}
