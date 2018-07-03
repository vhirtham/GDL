#include "gdl/resources/memory/threadPrivateMemoryStack.h"


#include "gdl/base/Exception.h"
#include "gdl/base/functions/isPowerOf2.h"

#include <cassert>

namespace GDL
{

threadPrivateMemoryStack::threadPrivateMemoryStack(size_t memorySize, size_t alignment)
    : mMemorySize{memorySize}
    , mAlignment{alignment}
    , mNumAllocations{0}
    , mOwningTreadId{std::this_thread::get_id()}
    , mMemoryStart{nullptr}
    , mCurrentMemoryPtr{nullptr}
    , mMemory{nullptr}
{
    CheckConstructionParameters();
}

threadPrivateMemoryStack::~threadPrivateMemoryStack()
{
    assert(IsInitialized() == false && "Deinitialize the thread private memory stack before destruction - If you did, "
                                       "there might have been an exception");
}

void* threadPrivateMemoryStack::Allocate(size_t size)
{
    // https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
    size_t consumedMemorySize = ((size + mAlignment - 1) / mAlignment) * mAlignment;


    DEV_EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");
    DEV_EXCEPTION(consumedMemorySize == 0, "Allocated memory size is 0.");
    DEV_EXCEPTION(IsInitialized() == false, "Memory pool not initialized");
    EXCEPTION(mCurrentMemoryPtr + consumedMemorySize > mMemoryStart + mMemorySize, "No more memory available.");


    void* allocatedMemoryPtr = mCurrentMemoryPtr;
    mCurrentMemoryPtr += consumedMemorySize;

    ++mNumAllocations;

    return allocatedMemoryPtr;
}

void threadPrivateMemoryStack::Deallocate(void* address)
{
    DEV_EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
                  "Thread private memory stack can only be accessed by owning thread");
    DEV_EXCEPTION(address == nullptr, "Can't free a nullptr");
    DEV_EXCEPTION(static_cast<U8*>(address) < mMemoryStart || static_cast<U8*>(address) > mMemoryStart + mMemorySize,
                  "Memory address is not part of the pool allocators memory");
    DEV_EXCEPTION(mNumAllocations == 0, "No memory allocated that can be deallocated");

    --mNumAllocations;
    if (mNumAllocations == 0)
        mCurrentMemoryPtr = mMemoryStart;
}

void threadPrivateMemoryStack::Deinitialize()
{
    EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    //! ---- CONTINUE Test if exceptions are triggered correctly!

    EXCEPTION(IsInitialized() == false, "Memory pool already deinitialized.");
    EXCEPTION(mNumAllocations != 0, "Can't deinitialize. Memory still in use.");

    mMemory.reset(nullptr);
    mMemoryStart = nullptr;
    mCurrentMemoryPtr = {nullptr};
}

void threadPrivateMemoryStack::Initialize()
{
    EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    mMemory.reset(new U8[TotalMemorySize()]);
    AlignMemory();
    mNumAllocations = 0;
    mCurrentMemoryPtr = mMemoryStart;
}



void threadPrivateMemoryStack::AlignMemory()
{
    void* memoryStartBefAlign = mMemory.get();
    size_t memorySizeBefAlign = TotalMemorySize();

    mMemoryStart = static_cast<U8*>(std::align(mAlignment, mMemorySize, memoryStartBefAlign, memorySizeBefAlign));

    EXCEPTION(mMemoryStart == nullptr, "Memory alignment failed.");
}



void threadPrivateMemoryStack::CheckConstructionParameters() const
{
    EXCEPTION(mMemorySize < 1, "Memory size must be bigger than 0");
    EXCEPTION(!IsPowerOf2(mAlignment), "Alignment must be a power of 2");
}

bool threadPrivateMemoryStack::IsInitialized() const
{
    return mMemory != nullptr;
}

size_t threadPrivateMemoryStack::TotalMemorySize() const
{
    return mMemorySize + mAlignment;
}

} // namespace GDL
