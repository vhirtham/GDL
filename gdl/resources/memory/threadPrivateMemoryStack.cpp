#include "gdl/resources/memory/threadPrivateMemoryStack.h"


#include "gdl/base/Exception.h"
#include "gdl/base/functions/isPowerOf2.h"

namespace GDL
{

threadPrivateMemoryStack::threadPrivateMemoryStack(size_t memorySize, size_t alignment)
    : mMemorySize{memorySize}
    , mAlignment{alignment}
    , mNumAllocations{0}
    , mOwningTreadId{std::this_thread::get_id()}
    , mMemoryStart{nullptr}
    , mMemory{nullptr}
{
    CheckConstructionParameters();
}

threadPrivateMemoryStack::~threadPrivateMemoryStack()
{
}

void threadPrivateMemoryStack::Deinitialize()
{
    EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    //! ---- CONTINUE Test if exceptions are triggered correctly!

    EXCEPTION(IsInitialized() == false, "Memory pool already deinitialized.");
    EXCEPTION(mNumAllocations != 0, "Can't deinitialize. Memory still in use.");
}

void threadPrivateMemoryStack::Initialize()
{
    EXCEPTION(mOwningTreadId != std::this_thread::get_id(),
              "Thread private memory stack can only be accessed by owning thread");

    mMemory.reset(new U8[TotalMemorySize()]);
    AlignMemory();
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
