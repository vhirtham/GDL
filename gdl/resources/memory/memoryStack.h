#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/resources/cpu/spinlock.h"
#include "gdl/resources/memory/memoryInterface.h"
#include "gdl/resources/memory/memorySize.h"
#include <memory>
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

    typedef typename std::conditional<_ThreadPrivate, std::thread::id, SpinLock>::type SpinlockOrThreadId;



    //! @brief Class which stores the current state of the memory stack and restores it when the object is destroyed
    class MemoryStackDeallocator
    {
        friend class MemoryStackTemplate<_ThreadPrivate>;

        MemoryStackTemplate& mMemoryStack;
        U32 mStoredNumAllocations = 0;
        U8* mStoredPointer = nullptr;

        //! @brief Ctor
        //! @param memoryStack: Memory stack that should be restored after destruction of an instance of this class
        MemoryStackDeallocator(MemoryStackTemplate& memoryStack);

    public:
        MemoryStackDeallocator() = delete;
        MemoryStackDeallocator(const MemoryStackDeallocator&) = delete;
        MemoryStackDeallocator(MemoryStackDeallocator&&) = delete;
        MemoryStackDeallocator& operator=(const MemoryStackDeallocator&) = delete;
        MemoryStackDeallocator& operator=(MemoryStackDeallocator&&) = delete;
        ~MemoryStackDeallocator();
    };



    MemorySize mMemorySize;
    U32 mNumAllocations;
    U8* mCurrentMemoryPtr;
    std::unique_ptr<U8[]> mMemory;
    SpinlockOrThreadId mSpinlockOrThreadId;

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

    //! @brief Creates a memory stack deallocator for the memory stack
    //! @return New memory stack allocator instance
    [[nodiscard]] MemoryStackDeallocator CreateMemoryStackDeallocator();

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

    //! @brief Sets the number of allocations and the current memory pointer
    //! @param numAllocations: New number of allocations
    //! @param memoryPointer: New position of the memory pointer
    //! @remark This function is only ment to be used by the stack deallocator class
    void SetState(U32 numAllocations, U8* memoryPointer);
};


using ThreadPrivateMemoryStack = MemoryStackTemplate<true>;
using MemoryStack = MemoryStackTemplate<false>;

} // namespace GDL
