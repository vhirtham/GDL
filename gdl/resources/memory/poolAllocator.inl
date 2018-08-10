#pragma once

#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/memoryInterface.h"
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
_type* PoolAllocator<_type>::allocate(std::size_t n)
{
    DEV_EXCEPTION(n > 1, "Pool allocator is not compatible with array like data types");
    static MemoryInterface* memory = MemoryManager::Instance().GetMemoryPool(sizeof(_type), alignof(_type));
    if (memory == nullptr)
    {
        static std::allocator<_type> stdAlloc;
        return stdAlloc.allocate(n);
    }
    return static_cast<_type*>(memory->Allocate(n * sizeof(_type), alignof(_type)));
}

template <class _type>
void PoolAllocator<_type>::deallocate(_type* p, std::size_t n)
{
    DEV_EXCEPTION(n > 1, "Pool allocator is not compatible with array like data types");
    static MemoryInterface* memory = MemoryManager::Instance().GetMemoryPool(sizeof(_type), alignof(_type));
    if (memory == nullptr)
    {
        static std::allocator<_type> stdAlloc;
        stdAlloc.deallocate(p, n);
    }
    else
        memory->Deallocate(p);
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
