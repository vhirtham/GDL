#pragma once

#include "gdl/resources/memory/memoryManager.h"

namespace GDL
{

//! @remark // http://www.cplusplus.com/reference/memory/allocator_traits/
template <class T>
struct InternalStackAllocator
{
    typedef T value_type;
    InternalStackAllocator() noexcept
    {
    }
    template <class U>
    InternalStackAllocator(const InternalStackAllocator<U>&) noexcept
    {
    }

    T* allocate(std::size_t n)
    {
        MemoryManager& memoryManager = MemoryManager::Instance();
        return static_cast<T*>(memoryManager.AllocateInternalStack(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t n)
    {
        MemoryManager::Instance().DeallocateInternalStack();
    }
};

template <class T, class U>
constexpr bool operator==(const InternalStackAllocator<T>&, const InternalStackAllocator<U>&) noexcept
{
    return true;
}

template <class T, class U>
constexpr bool operator!=(const InternalStackAllocator<T>&, const InternalStackAllocator<U>&) noexcept
{
    return false;
}
}
