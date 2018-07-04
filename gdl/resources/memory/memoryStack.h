#pragma once


#include "gdl/GDLTypedefs.h"

#include <memory>
#include <thread>

namespace GDL
{


//! @brief Memory system that can only allocate memory from its end. The memory is only freed all at once if the number
//! of allocations equals the number of deallocations. The template bool defines how multithreading is handled.
//! @tparam _ThreadPrivate: TRUE if the memory stack can only be accessed by a single thread. FALSE if the memory stack
//! is accessable by multiple threads and needs to be protected by a mutex
template <bool _ThreadPrivate>
class memoryStackTemplate
{
    size_t mMemorySize;
    size_t mAlignment;
    U32 mNumAllocations;
    U8* mMemoryStart;
    U8* mCurrentMemoryPtr;
    std::unique_ptr<U8[]> mMemory;
    std::thread::id mOwningTreadId;

public:
    //! @brief Creates the thread private memory pool with <memorySize> bytes of memory
    //! @param elementSize: total amount of memory
    //! @param alignment: Memory alignment
    memoryStackTemplate(size_t memorySize, size_t alignment = 1);

    memoryStackTemplate() = delete;
    memoryStackTemplate(const memoryStackTemplate&) = delete;
    memoryStackTemplate(memoryStackTemplate&&) = delete;
    memoryStackTemplate& operator=(const memoryStackTemplate&) = delete;
    memoryStackTemplate& operator=(memoryStackTemplate&&) = delete;
    ~memoryStackTemplate();


    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @return Pointer to memory
    void* Allocate(size_t size);

    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    void Deallocate(void* address);

    //! @brief Deinitializes the memory stack
    void Deinitialize();

    //! @brief Initializes the memory stack
    void Initialize();

private:
    //! @brief Aligns the memory.
    void AlignMemory();

    //! @brief Class internal function which does the memory allocation
    //! @param size: Size of the memory that should be allocated
    //! @return Pointer to memory
    void* AllocateInternal(size_t size);

    //! @brief Checks if the memory stack is constructed with valid parameters. Throws if not.
    void CheckConstructionParameters() const;

    //! @brief Class internal function which does the deallocation
    //! @param address: Adress that should be freed
    void DeallocateInternal(void* address);

    //! @brief Class internal function that deinitializes the memory stack
    void DeinitializeInternal();

    //! @brief Class internal function that initializes the memory stack
    void InitializeInternal();

    //! @brief Returns if the memory stack is initialized
    //! @return TRUE / FALSE
    bool IsInitialized() const;

    //! @brief Returns the memory size including the extra alignment bytes
    //! @return Memory size including the extra alignment bytes
    size_t TotalMemorySize() const;
};


using threadPrivateMemoryStack = memoryStackTemplate<true>;
using memoryStack = memoryStackTemplate<true>;

} // namespace GDL
