#pragma once

#include "gdl/resources/memory/memoryInterface.h"

#include <memory>

namespace GDL
{

template <class _type>
struct PoolAllocator
{
    typedef _type value_type;
    PoolAllocator() noexcept;

    template <class _typeOther>
    PoolAllocator(const PoolAllocator<_typeOther>&) noexcept;

    _type* allocate(std::size_t n);

    void deallocate(_type* p, std::size_t n);

private:
    static MemoryInterface* GetMemoryModel();
};

template <class _type, class _typeOther>
constexpr bool operator==(const PoolAllocator<_type>&, const PoolAllocator<_typeOther>&) noexcept;

template <class _type, class _typeOther>
constexpr bool operator!=(const PoolAllocator<_type>&, const PoolAllocator<_typeOther>&) noexcept;


} // namespace GDL


#include "gdl/resources/memory/poolAllocator.inl"
