#pragma once


#include "gdl/GDLTypedefs.h"

#include <memory>
#include <thread>

namespace GDL
{


//! @brief Memory system that stores equally sized memory blocks. Free memory blocks are used to keep a linked list
//! of free elements. This way allocations and deallocations are constant time operations.
class threadPrivateMemoryStack
{
    size_t mMemorySize;
    size_t mAlignment;
    U32 mNumAllocations;
    std::thread::id mOwningTreadId;
    U8* mMemoryStart;
    std::unique_ptr<U8[]> mMemory;

public:
    //! @brief Creates the thread private memory pool with <memorySize> bytes of memory
    //! @param elementSize: total amount of memory
    //! @param alignment: Memory alignment
    threadPrivateMemoryStack(size_t memorySize, size_t alignment = 1);

    threadPrivateMemoryStack() = delete;
    threadPrivateMemoryStack(const threadPrivateMemoryStack&) = delete;
    threadPrivateMemoryStack(threadPrivateMemoryStack&&) = delete;
    threadPrivateMemoryStack& operator=(const threadPrivateMemoryStack&) = delete;
    threadPrivateMemoryStack& operator=(threadPrivateMemoryStack&&) = delete;
    ~threadPrivateMemoryStack();


    //! @brief Deinitializes the memory stack
    void Deinitialize();

    //! @brief Initializes the memory stack and sets up the internal linked list of free memory blocks
    void Initialize();

private:

    //! @brief Aligns the memory.
    void AlignMemory();

    //! @brief Checks if the memory stack is constructed with valid parameters. Throws if not.
    void CheckConstructionParameters() const;

    //! @brief Returns if the memory stack is initialized
    //! @return TRUE / FALSE
    bool IsInitialized() const;

    //! @brief Returns the memory size including the extra alignment bytes
    //! @return Memory size including the extra alignment bytes
    size_t TotalMemorySize() const;
};

}  //namespace GDL
