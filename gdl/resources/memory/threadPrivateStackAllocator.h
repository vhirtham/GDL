#pragma once

#include "gdl/resources/memory/memoryInterface.h"

#include <memory>

namespace GDL
{

template <class _type>
struct ThreadPrivateStackAllocator
{
    typedef _type value_type;
    ThreadPrivateStackAllocator() noexcept;

    template <class _typeOther>
    ThreadPrivateStackAllocator(const ThreadPrivateStackAllocator<_typeOther>&) noexcept;

    _type* allocate(std::size_t n);

    void deallocate(_type* p, std::size_t n);

private:
    static MemoryInterface* GetMemoryModel();
};

template <class _type, class _typeOther>
constexpr bool operator==(const ThreadPrivateStackAllocator<_type>&,
                          const ThreadPrivateStackAllocator<_typeOther>&) noexcept;

template <class _type, class _typeOther>
constexpr bool operator!=(const ThreadPrivateStackAllocator<_type>&,
                          const ThreadPrivateStackAllocator<_typeOther>&) noexcept;


} // namespace GDL


#include "gdl/resources/memory/threadPrivateStackAllocator.inl"
