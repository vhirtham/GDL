#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"
#include "gdl/resources/memory/memoryStack_old.h"

#include <atomic>
#include <map>
#include <memory>
#include <thread>

namespace GDL
{

//! @brief Singleton class that manages a single block of memory. Different kind of allocators can be used to access
//! that memory in different locations and allocation patterns.
class MemoryManager
{
    template <class T>
    friend struct InternalStackAllocator;

    // std::atomic<U8*> mCurrentPosition;

    static bool mInitialized;
    static U32 mMaxNumThreads;
    static U32 mInternalStackSizeThread;
    static U64 mFreeMemorySize;

    std::unique_ptr<U8[]> mMemory;

    std::map<std::thread::id, ThreadPrivateMemoryStack> mInternalStack;

    //! @brief Constructor that allocates a single big single block of memory to suffice the programs need for memory
    MemoryManager();

public:
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager& operator=(MemoryManager&&) = delete;
    ~MemoryManager() = default;


    //! Initializes the memory manager
    static void Initialize();

    //! @brief Sets the size of the internal thread private memory stacks
    //! @param size: Size of the internal thread private memory stacks
    //! @remark The call must happen before the first call of Instance(). Otherwise this function throws.
    static void SetInternalThreadStackSize(U32 size);

private:
    //! @brief Gets the memory manager instance
    //! @return Memory manager Instance
    static MemoryManager& Instance();

    //! @brief Allocates memory on the internal thread private stack.
    //! @param size: Size of the memory that should be allocated
    //! @return Pointer to the memory
    void* AllocateInternalThreadStack(U32 size);

    //! @brief Deallocates memory on the internal thread private stack.
    void DeallocateInternalThreadStack();

    //! @brief Throws an exception if the memory manager is already initialized
    static void ThrowOnInitialized();
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
