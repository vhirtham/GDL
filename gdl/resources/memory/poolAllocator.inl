#pragma once

#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/memoryManager.h"
#include "gdl/base/Exception.h"

namespace GDL
{

template <class _type>
PoolAllocator<_type>::PoolAllocator() noexcept
{
}



template <class _type>
template <class _typeOther>
PoolAllocator<_type>::PoolAllocator(const PoolAllocator<_typeOther>&) noexcept
{
}



template <class _type>
_type* PoolAllocator<_type>::allocate(std::size_t numInstances)
{
    DEV_EXCEPTION(numInstances > 1, "Pool allocator is not compatible with array like data types");
    return static_cast<_type*>(GetMemoryAllocationPattern()->Allocate(numInstances * sizeof(_type), alignof(_type)));
}



template <class _type>
void PoolAllocator<_type>::deallocate(_type* p, std::size_t)
{
    GetMemoryAllocationPattern()->Deallocate(p, alignof(_type));
}

template <class _type>
MemoryInterface* PoolAllocator<_type>::GetMemoryAllocationPattern()
{
    static MemoryInterface* memoryAP = InitializeMemoryAllocationPattern();
    return memoryAP;
}



template <class _type>
MemoryInterface* PoolAllocator<_type>::InitializeMemoryAllocationPattern()
{
    MemoryInterface* memoryAP = MemoryManager::Instance().GetMemoryPool(sizeof(_type), alignof(_type));
    if (memoryAP == nullptr)
    {
        memoryAP = MemoryManager::Instance().GetGeneralPurposeMemory();
        if (memoryAP == nullptr)
            return MemoryManager::Instance().GetHeapMemory();
    }
    return memoryAP;
}



template <class _type, class _typeOther>
constexpr bool operator==(const PoolAllocator<_type>&, const PoolAllocator<_typeOther>&) noexcept
{
    return true;
}



template <class _type, class _typeOther>
constexpr bool operator!=(const PoolAllocator<_type>&, const PoolAllocator<_typeOther>&) noexcept
{
    return false;
}

} // namespace GDL
