#pragma once

#include "gdl/base/Exception.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"
#include "gdl/resources/memory/memoryManager.h"

namespace GDL
{

template <class _type>
ThreadPrivateStackAllocator<_type>::ThreadPrivateStackAllocator() noexcept
{
}



template <class _type>
template <class _typeOther>
ThreadPrivateStackAllocator<_type>::ThreadPrivateStackAllocator(const ThreadPrivateStackAllocator<_typeOther>&) noexcept
{
}



template <class _type>
_type* ThreadPrivateStackAllocator<_type>::allocate(std::size_t n)
{
    return static_cast<_type*>(GetMemoryModel()->Allocate(n * sizeof(_type), alignof(_type)));
}



template <class _type>
void ThreadPrivateStackAllocator<_type>::deallocate(_type* p, [[maybe_unused]] std::size_t n)
{
    GetMemoryModel()->Deallocate(p, alignof(_type));
}



template <class _type>
MemoryInterface* ThreadPrivateStackAllocator<_type>::GetMemoryModel()
{
    MemoryInterface* memory = MemoryManager::Instance().GetThreadPrivateMemoryStack();
    DEV_EXCEPTION(memory == nullptr && MemoryManager::Instance().IsThreadPrivateMemoryEnabled(),
                  "No thread private memory created for the calling thread.");
    if (memory == nullptr)
        memory = MemoryManager::Instance().GetGeneralPurposeMemory();
    if (memory == nullptr)
        return MemoryManager::Instance().GetHeapMemory();
    return memory;
}



template <class _type, class _typeOther>
constexpr bool operator==(const ThreadPrivateStackAllocator<_type>&,
                          const ThreadPrivateStackAllocator<_typeOther>&) noexcept
{
    return true;
}



template <class _type, class _typeOther>
constexpr bool operator!=(const ThreadPrivateStackAllocator<_type>&,
                          const ThreadPrivateStackAllocator<_typeOther>&) noexcept
{
    return false;
}

} // namespace GDL
