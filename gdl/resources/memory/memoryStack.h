#pragma once


#include "gdl/GDLTypedefs.h"
#include "gdl/resources/memory/memoryInterface.h"
#include "gdl/resources/memory/memorySize.h"

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
class MemoryStackTemplate : public MemoryInterface
{

    typedef typename std::conditional<_ThreadPrivate, std::thread::id, std::mutex>::type MutexOrThreadId;

    MemorySize mMemorySize;
    U32 mNumAllocations;
    U8* mCurrentMemoryPtr;
    std::unique_ptr<U8[]> mMemory;
    MutexOrThreadId mMutexOrThreadId;

public:
    //! @brief Creates the thread private memory stack with <memorySize> bytes of memory
    //! @param memorySize: total amount of memory
    MemoryStackTemplate(MemorySize memorySize);

    MemoryStackTemplate() = delete;
    MemoryStackTemplate(const MemoryStackTemplate&) = delete;
    MemoryStackTemplate(MemoryStackTemplate&&) = delete;
    MemoryStackTemplate& operator=(const MemoryStackTemplate&) = delete;
    MemoryStackTemplate& operator=(MemoryStackTemplate&&) = delete;
    ~MemoryStackTemplate();


    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @param alignment: Memory alignment
    //! @return Pointer to memory
    virtual void* Allocate(size_t size, size_t alignment = 1) override;

    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    //! @param alignment: Memory alignment
    virtual void Deallocate(void* address, size_t alignment = 1) override;

    //! @brief Deinitializes the memory stack
    void Deinitialize();

    //! @brief Initializes the memory stack
    void Initialize();

private:
    //! @brief Class internal function which does the memory allocation
    //! @param size: Size of the memory that should be allocated
    //! @return Pointer to memory
    //! @param alignment: Memory alignment
    void* AllocatePrivate(size_t size, size_t alignment);

    //! @brief Checks if the memory stack is constructed with valid parameters. Throws if not.
    void CheckConstructionParameters() const;

    //! @brief Class internal function which does the deallocation
    //! @param address: Adress that should be freed
    void DeallocatePrivate(void* address);

    //! @brief Class internal function that deinitializes the memory stack
    void DeinitializePrivate();

    //! @brief Class internal function that initializes the memory stack
    void InitializePrivate();

    //! @brief Returns if the memory stack is initialized
    //! @return TRUE / FALSE
    bool IsInitialized() const;
};


using ThreadPrivateMemoryStack = MemoryStackTemplate<true>;
using MemoryStack = MemoryStackTemplate<false>;

} // namespace GDL
