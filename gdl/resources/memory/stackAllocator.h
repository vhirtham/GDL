#pragma once

#include "gdl/resources/memory/memoryInterface.h"

#include <memory>

namespace GDL
{

template <class _type>
struct StackAllocator
{
    typedef _type value_type;
    StackAllocator() noexcept;

    template <class _typeOther>
    StackAllocator(const StackAllocator<_typeOther>&) noexcept;

    _type* allocate(std::size_t n);

    void deallocate(_type* p, std::size_t n);

private:
    static MemoryInterface* GetMemoryModel();
};

template <class _type, class _typeOther>
constexpr bool operator==(const StackAllocator<_type>&, const StackAllocator<_typeOther>&) noexcept;

template <class _type, class _typeOther>
constexpr bool operator!=(const StackAllocator<_type>&, const StackAllocator<_typeOther>&) noexcept;


} // namespace GDL


#include "gdl/resources/memory/stackAllocator.inl"
