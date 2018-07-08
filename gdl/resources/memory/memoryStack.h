#pragma once


#include "gdl/GDLTypedefs.h"

#include <memory>
#include <mutex>
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

    typedef typename std::conditional<_ThreadPrivate, std::thread::id, std::mutex>::type MutexOrThreadId;

    size_t mMemorySize;
    U32 mNumAllocations;
    U8* mCurrentMemoryPtr;
    std::unique_ptr<U8[]> mMemory;
    MutexOrThreadId mMutexOrThreadId;

public:
    //! @brief Creates the thread private memory stack with <memorySize> bytes of memory
    //! @param memorySize: total amount of memory
    memoryStackTemplate(size_t memorySize);

    memoryStackTemplate() = delete;
    memoryStackTemplate(const memoryStackTemplate&) = delete;
    memoryStackTemplate(memoryStackTemplate&&) = delete;
    memoryStackTemplate& operator=(const memoryStackTemplate&) = delete;
    memoryStackTemplate& operator=(memoryStackTemplate&&) = delete;
    ~memoryStackTemplate();


    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @return Pointer to memory
    //! @param alignment: Memory alignment
    void* Allocate(size_t size, size_t alignment = 1);

    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    void Deallocate(void* address);

    //! @brief Deinitializes the memory stack
    void Deinitialize();

    //! @brief Initializes the memory stack
    void Initialize();

private:
    //! @brief Class internal function which does the memory allocation
    //! @param size: Size of the memory that should be allocated
    //! @return Pointer to memory
    //! @param alignment: Memory alignment
    void* AllocateInternal(size_t size, size_t alignment);

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
};


using threadPrivateMemoryStack = memoryStackTemplate<true>;
using memoryStack = memoryStackTemplate<false>;

} // namespace GDL
