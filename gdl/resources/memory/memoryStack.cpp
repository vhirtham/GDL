#include "gdl/resources/memory/memoryStack.h"


#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/functions/isPowerOf2.h"

#include <mutex>

namespace GDL
{

template <>
MemoryStackTemplate<true>::MemoryStackTemplate(MemorySize memorySize)
    : mMemorySize{memorySize}
    , mNumAllocations{0}
    , mCurrentMemoryPtr{nullptr}
    , mMemory{nullptr}
    , mSpinlockOrThreadId{std::this_thread::get_id()}
{
    CheckConstructionParameters();
}

template <>
MemoryStackTemplate<false>::MemoryStackTemplate(MemorySize memorySize)
    : mMemorySize{memorySize}
    , mNumAllocations{0}
    , mCurrentMemoryPtr{nullptr}
    , mMemory{nullptr}
{
    CheckConstructionParameters();
}

template <bool _ThreadPrivate>
MemoryStackTemplate<_ThreadPrivate>::~MemoryStackTemplate()
{
}



template <>
void* MemoryStackTemplate<true>::Allocate(size_t size, size_t alignment)
{
    DEV_EXCEPTION(mSpinlockOrThreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");

    return AllocatePrivate(size, alignment);
}



template <>
void* MemoryStackTemplate<false>::Allocate(size_t size, size_t alignment)
{
    std::lock_guard<SpinLock> lock(mSpinlockOrThreadId);
    return AllocatePrivate(size, alignment);
}



template <>
MemoryStackTemplate<false>::MemoryStackDeallocator MemoryStackTemplate<false>::CreateMemoryStackDeallocator()
{
    std::lock_guard<SpinLock> lock(mSpinlockOrThreadId);

    return MemoryStackDeallocator{*this};
}



template <>
MemoryStackTemplate<true>::MemoryStackDeallocator MemoryStackTemplate<true>::CreateMemoryStackDeallocator()
{
    DEV_EXCEPTION(mSpinlockOrThreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");

    return MemoryStackDeallocator{*this};
}



template <>
void MemoryStackTemplate<false>::SetState(U32 numAllocations, U8* memoryPointer)
{
    std::lock_guard<SpinLock> lock(mSpinlockOrThreadId);

    mNumAllocations = numAllocations;
    mCurrentMemoryPtr = memoryPointer;
}



template <>
void MemoryStackTemplate<true>::SetState(U32 numAllocations, U8* memoryPointer)
{
    DEV_EXCEPTION(mSpinlockOrThreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");

    mNumAllocations = numAllocations;
    mCurrentMemoryPtr = memoryPointer;
}



template <>
void MemoryStackTemplate<true>::Deallocate(void* address, [[maybe_unused]] size_t alignment)
{
    DEV_EXCEPTION(mSpinlockOrThreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");

    DeallocatePrivate(address);
}



template <>
void MemoryStackTemplate<false>::Deallocate(void* address, [[maybe_unused]] size_t alignment)
{
    std::lock_guard<SpinLock> lock(mSpinlockOrThreadId);
    DeallocatePrivate(address);
}



template <>
void MemoryStackTemplate<true>::Deinitialize()
{
    EXCEPTION(mSpinlockOrThreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    DeinitializePrivate();
}



template <>
void MemoryStackTemplate<false>::Deinitialize()
{
    std::lock_guard<SpinLock> lock(mSpinlockOrThreadId);
    DeinitializePrivate();
}



template <>
void MemoryStackTemplate<true>::Initialize()
{
    EXCEPTION(mSpinlockOrThreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    InitializePrivate();
}



template <>
void MemoryStackTemplate<false>::Initialize()
{
    std::lock_guard<SpinLock> lock(mSpinlockOrThreadId);
    InitializePrivate();
}


template <bool _ThreadPrivate>
void* MemoryStackTemplate<_ThreadPrivate>::AllocatePrivate(size_t size, size_t alignment)
{
    size_t misalignment = Misalignment(mCurrentMemoryPtr, alignment);
    size_t correction = ((misalignment + alignment - 1) / alignment) * alignment - misalignment;

    U8* allocatedMemoryPtr = mCurrentMemoryPtr + correction;

    DEV_EXCEPTION(size == 0, "Allocated memory size is 0.");
    DEV_EXCEPTION(!IsInitialized(), "Memory stack not initialized.");
    DEV_EXCEPTION(!IsPowerOf2(alignment), "Alignment must be a power of 2.");
    EXCEPTION(allocatedMemoryPtr + size > mMemory.get() + mMemorySize.GetNumBytes(), "No more memory available.");

    mCurrentMemoryPtr = allocatedMemoryPtr + size;

    ++mNumAllocations;

    return allocatedMemoryPtr;
}

template <bool _ThreadPrivate>
void MemoryStackTemplate<_ThreadPrivate>::DeallocatePrivate(void* address)
{
    DEV_EXCEPTION(address == nullptr, "Can't free a nullptr");
    DEV_EXCEPTION(static_cast<U8*>(address) < mMemory.get() ||
                          static_cast<U8*>(address) > mMemory.get() + mMemorySize.GetNumBytes(),
                  "Memory address is not part of the stack allocators memory");
    DEV_EXCEPTION(mNumAllocations == 0, "No memory allocated that can be deallocated");

    --mNumAllocations;
    if (mNumAllocations == 0)
        mCurrentMemoryPtr = mMemory.get();
}

template <bool _ThreadPrivate>
void MemoryStackTemplate<_ThreadPrivate>::DeinitializePrivate()
{

    EXCEPTION(IsInitialized() == false, "Memory stack already deinitialized.");
    EXCEPTION(mNumAllocations != 0, "Can't deinitialize. Memory still in use.");

    mMemory.reset(nullptr);
    mCurrentMemoryPtr = {nullptr};
}

template <bool _ThreadPrivate>
void MemoryStackTemplate<_ThreadPrivate>::InitializePrivate()
{
    EXCEPTION(IsInitialized(), "Memory stack is already initialized");

    mMemory.reset(new U8[mMemorySize.GetNumBytes()]);
    mNumAllocations = 0;
    mCurrentMemoryPtr = mMemory.get();
}



template <bool _ThreadPrivate>
void MemoryStackTemplate<_ThreadPrivate>::CheckConstructionParameters() const
{
    EXCEPTION(mMemorySize < 1, "Memory size must be bigger than 0");
}



template <bool _ThreadPrivate>
bool MemoryStackTemplate<_ThreadPrivate>::IsInitialized() const
{
    return mMemory != nullptr;
}



template <bool _ThreadPrivate>
MemoryStackTemplate<_ThreadPrivate>::MemoryStackDeallocator::MemoryStackDeallocator(MemoryStackTemplate& memoryStack)
    : mMemoryStack{memoryStack}
    , mStoredNumAllocations{memoryStack.mNumAllocations}
    , mStoredPointer{memoryStack.mCurrentMemoryPtr}
{
}



template <bool _ThreadPrivate>
MemoryStackTemplate<_ThreadPrivate>::MemoryStackDeallocator::~MemoryStackDeallocator()
{
    mMemoryStack.SetState(mStoredNumAllocations, mStoredPointer);
}



template class MemoryStackTemplate<true>;
template class MemoryStackTemplate<false>;

} // namespace GDL
