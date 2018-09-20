#pragma once

#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/memoryManager.h"
#include "gdl/base/exception.h"

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
_type* StackAllocator<_type>::allocate(std::size_t numInstances)
{
    return static_cast<_type*>(GetMemoryAllocationPattern()->Allocate(numInstances * sizeof(_type), alignof(_type)));
}



template <class _type>
void StackAllocator<_type>::deallocate(_type* pointer, std::size_t)
{
    GetMemoryAllocationPattern()->Deallocate(pointer, alignof(_type));
}


template <class _type>
void StackAllocator<_type>::Deleter(_type* pointer)
{
    GetMemoryAllocationPattern()->Deallocate(pointer, alignof(_type));
}


template <class _type>
MemoryInterface* StackAllocator<_type>::GetMemoryAllocationPattern()
{
    static MemoryInterface* memoryAP = InitializeMemoryAllocationPattern();
    return memoryAP;
}



template <class _type>
MemoryInterface* StackAllocator<_type>::InitializeMemoryAllocationPattern()
{
    MemoryInterface* memoryAP = MemoryManager::Instance().GetMemoryStack();
    if (memoryAP == nullptr)
    {
        memoryAP = MemoryManager::Instance().GetGeneralPurposeMemory();
        if (memoryAP == nullptr)
            return MemoryManager::Instance().GetHeapMemory();
    }
    return memoryAP;
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
