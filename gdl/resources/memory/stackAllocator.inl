#pragma once

#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/memoryManager.h"
#include "gdl/base/Exception.h"

namespace GDL
{

template <class _type>
StackAllocator<_type>::StackAllocator() noexcept
{
}



template <class _type>
template <class _typeOther>
StackAllocator<_type>::StackAllocator(const StackAllocator<_typeOther>&) noexcept
{
}



template <class _type>
_type* StackAllocator<_type>::allocate(std::size_t n)
{
    static MemoryInterface* memory = GetMemoryModel();
    return static_cast<_type*>(memory->Allocate(n * sizeof(_type), alignof(_type)));
}



template <class _type>
void StackAllocator<_type>::deallocate(_type* p, [[maybe_unused]] std::size_t n)
{
    static MemoryInterface* memory = GetMemoryModel();
    memory->Deallocate(p, alignof(_type));
}



template <class _type>
MemoryInterface* StackAllocator<_type>::GetMemoryModel()
{
    MemoryInterface* memory = MemoryManager::Instance().GetMemoryStack();
    if (memory == nullptr)
    {
        memory = MemoryManager::Instance().GetGeneralPurposeMemory();
        if (memory == nullptr)
            return MemoryManager::Instance().GetHeapMemory();
    }
    return memory;
}



template <class _type, class _typeOther>
constexpr bool operator==(const StackAllocator<_type>&, const StackAllocator<_typeOther>&) noexcept
{
    return true;
}



template <class _type, class _typeOther>
constexpr bool operator!=(const StackAllocator<_type>&, const StackAllocator<_typeOther>&) noexcept
{
    return false;
}

} // namespace GDL
