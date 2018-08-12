#include "gdl/resources/memory/memoryStack.h"


#include "gdl/base/Exception.h"
#include "gdl/base/functions/isPowerOf2.h"
#include "gdl/base/SSESupportFunctions.h"


namespace GDL
{

template <>
memoryStackTemplate<true>::memoryStackTemplate(MemorySize memorySize)
    : mMemorySize{memorySize}
    , mNumAllocations{0}
    , mCurrentMemoryPtr{nullptr}
    , mMemory{nullptr}
    , mMutexOrThreadId{std::this_thread::get_id()}
{
    CheckConstructionParameters();
}

template <>
memoryStackTemplate<false>::memoryStackTemplate(MemorySize memorySize)
    : mMemorySize{memorySize}
    , mNumAllocations{0}
    , mCurrentMemoryPtr{nullptr}
    , mMemory{nullptr}
{
    CheckConstructionParameters();
}

template <bool _ThreadPrivate>
memoryStackTemplate<_ThreadPrivate>::~memoryStackTemplate()
{
}



template <>
void* memoryStackTemplate<true>::Allocate(size_t size, size_t alignment)
{
    DEV_EXCEPTION(mMutexOrThreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");

    return AllocatePrivate(size, alignment);
}



template <>
void* memoryStackTemplate<false>::Allocate(size_t size, size_t alignment)
{
    std::lock_guard<std::mutex> lock(mMutexOrThreadId);
    return AllocatePrivate(size, alignment);
}



template <>
void memoryStackTemplate<true>::Deallocate(void* address, [[maybe_unused]] size_t alignment)
{
    DEV_EXCEPTION(mMutexOrThreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");

    DeallocatePrivate(address);
}



template <>
void memoryStackTemplate<false>::Deallocate(void* address, [[maybe_unused]] size_t alignment)
{
    std::lock_guard<std::mutex> lock(mMutexOrThreadId);
    DeallocatePrivate(address);
}



template <>
void memoryStackTemplate<true>::Deinitialize()
{
    EXCEPTION(mMutexOrThreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    DeinitializePrivate();
}



template <>
void memoryStackTemplate<false>::Deinitialize()
{
    std::lock_guard<std::mutex> lock(mMutexOrThreadId);
    DeinitializePrivate();
}



template <>
void memoryStackTemplate<true>::Initialize()
{
    EXCEPTION(mMutexOrThreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    InitializePrivate();
}



template <>
void memoryStackTemplate<false>::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutexOrThreadId);
    InitializePrivate();
}


template <bool _ThreadPrivate>
void* memoryStackTemplate<_ThreadPrivate>::AllocatePrivate(size_t size, size_t alignment)
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
void memoryStackTemplate<_ThreadPrivate>::DeallocatePrivate(void* address)
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
void memoryStackTemplate<_ThreadPrivate>::DeinitializePrivate()
{

    EXCEPTION(IsInitialized() == false, "Memory stack already deinitialized.");
    EXCEPTION(mNumAllocations != 0, "Can't deinitialize. Memory still in use.");

    mMemory.reset(nullptr);
    mCurrentMemoryPtr = {nullptr};
}

template <bool _ThreadPrivate>
void memoryStackTemplate<_ThreadPrivate>::InitializePrivate()
{
    EXCEPTION(IsInitialized(), "Memory stack is already initialized");

    mMemory.reset(new U8[mMemorySize.GetNumBytes()]);
    mNumAllocations = 0;
    mCurrentMemoryPtr = mMemory.get();
}



template <bool _ThreadPrivate>
void memoryStackTemplate<_ThreadPrivate>::CheckConstructionParameters() const
{
    EXCEPTION(mMemorySize < 1, "Memory size must be bigger than 0");
}



template <bool _ThreadPrivate>
bool memoryStackTemplate<_ThreadPrivate>::IsInitialized() const
{
    return mMemory != nullptr;
}



template class memoryStackTemplate<true>;
template class memoryStackTemplate<false>;

} // namespace GDL
