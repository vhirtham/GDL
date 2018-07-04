#include "gdl/resources/memory/memoryStack.h"


#include "gdl/base/Exception.h"
#include "gdl/base/functions/isPowerOf2.h"

#include <cassert>

namespace GDL
{

template <bool _ThreadPrivate>
memoryStackTemplate<_ThreadPrivate>::memoryStackTemplate(size_t memorySize, size_t alignment)
    : mMemorySize{memorySize}
    , mAlignment{alignment}
    , mNumAllocations{0}
    , mMemoryStart{nullptr}
    , mCurrentMemoryPtr{nullptr}
    , mMemory{nullptr}
    , mOwningTreadId{std::this_thread::get_id()}
{
    CheckConstructionParameters();
}



template <bool _ThreadPrivate>
memoryStackTemplate<_ThreadPrivate>::~memoryStackTemplate()
{
    assert(IsInitialized() == false && "Deinitialize the thread private memory stack before destruction - If you did, "
                                       "there might have been an exception");
}



template <>
void* memoryStackTemplate<true>::Allocate(size_t size)
{
    DEV_EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");

    return AllocateInternal(size);
}



template <>
void* memoryStackTemplate<false>::Allocate(size_t size)
{
    return AllocateInternal(size);
}



template <>
void memoryStackTemplate<true>::Deallocate(void* address)
{
    DEV_EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");

    DeallocateInternal(address);
}



template <>
void memoryStackTemplate<false>::Deallocate(void* address)
{
    DeallocateInternal(address);
}



template <>
void memoryStackTemplate<true>::Deinitialize()
{
    EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    DeinitializeInternal();
}



template <>
void memoryStackTemplate<false>::Deinitialize()
{
    DeinitializeInternal();
}



template <>
void memoryStackTemplate<true>::Initialize()
{
    EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    InitializeInternal();
}



template <>
void memoryStackTemplate<false>::Initialize()
{
    InitializeInternal();
}


template <bool _ThreadPrivate>
void memoryStackTemplate<_ThreadPrivate>::AlignMemory()
{
    void* memoryStartBefAlign = mMemory.get();
    size_t memorySizeBefAlign = TotalMemorySize();

    mMemoryStart = static_cast<U8*>(std::align(mAlignment, mMemorySize, memoryStartBefAlign, memorySizeBefAlign));

    EXCEPTION(mMemoryStart == nullptr, "Memory alignment failed.");
}

template <bool _ThreadPrivate>
void* memoryStackTemplate<_ThreadPrivate>::AllocateInternal(size_t size)
{
    // https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
    size_t consumedMemorySize = ((size + mAlignment - 1) / mAlignment) * mAlignment;

    DEV_EXCEPTION(consumedMemorySize == 0, "Allocated memory size is 0.");
    DEV_EXCEPTION(IsInitialized() == false, "Memory pool not initialized");
    EXCEPTION(mCurrentMemoryPtr + consumedMemorySize > mMemoryStart + mMemorySize, "No more memory available.");

    void* allocatedMemoryPtr = mCurrentMemoryPtr;
    mCurrentMemoryPtr += consumedMemorySize;

    ++mNumAllocations;

    return allocatedMemoryPtr;
}

template <bool _ThreadPrivate>
void memoryStackTemplate<_ThreadPrivate>::DeallocateInternal(void* address)
{
    DEV_EXCEPTION(address == nullptr, "Can't free a nullptr");
    DEV_EXCEPTION(static_cast<U8*>(address) < mMemoryStart || static_cast<U8*>(address) > mMemoryStart + mMemorySize,
                  "Memory address is not part of the pool allocators memory");
    DEV_EXCEPTION(mNumAllocations == 0, "No memory allocated that can be deallocated");

    --mNumAllocations;
    if (mNumAllocations == 0)
        mCurrentMemoryPtr = mMemoryStart;
}

template <bool _ThreadPrivate>
void memoryStackTemplate<_ThreadPrivate>::DeinitializeInternal()
{

    EXCEPTION(IsInitialized() == false, "Memory pool already deinitialized.");
    EXCEPTION(mNumAllocations != 0, "Can't deinitialize. Memory still in use.");

    mMemory.reset(nullptr);
    mMemoryStart = nullptr;
    mCurrentMemoryPtr = {nullptr};
}

template <bool _ThreadPrivate>
void memoryStackTemplate<_ThreadPrivate>::InitializeInternal()
{
    mMemory.reset(new U8[TotalMemorySize()]);
    AlignMemory();
    mNumAllocations = 0;
    mCurrentMemoryPtr = mMemoryStart;
}



template <bool _ThreadPrivate>
void memoryStackTemplate<_ThreadPrivate>::CheckConstructionParameters() const
{
    EXCEPTION(mMemorySize < 1, "Memory size must be bigger than 0");
    EXCEPTION(!IsPowerOf2(mAlignment), "Alignment must be a power of 2");
}



template <bool _ThreadPrivate>
bool memoryStackTemplate<_ThreadPrivate>::IsInitialized() const
{
    return mMemory != nullptr;
}



template <bool _ThreadPrivate>
size_t memoryStackTemplate<_ThreadPrivate>::TotalMemorySize() const
{
    return mMemorySize + mAlignment;
}

template class memoryStackTemplate<true>;
template class memoryStackTemplate<false>;

} // namespace GDL
