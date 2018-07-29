#pragma once

#include "gdl/resources/memory/generalPurposeMemory.h"
#include "gdl/resources/memory/memoryManager.h"

#include <memory>

namespace GDL
{

template <class _type>
struct GeneralPurposeAllocator
{
    typedef _type value_type;
    GeneralPurposeAllocator() noexcept;

    template <class _typeOther>
    GeneralPurposeAllocator(const GeneralPurposeAllocator<_typeOther>&) noexcept;

    _type* allocate(std::size_t n);

    void deallocate(_type* p, std::size_t n);
};

template <class _type, class _typeOther>
constexpr bool operator==(const GeneralPurposeAllocator<_type>&, const GeneralPurposeAllocator<_typeOther>&) noexcept;

template <class _type, class _typeOther>
constexpr bool operator!=(const GeneralPurposeAllocator<_type>&, const GeneralPurposeAllocator<_typeOther>&) noexcept;


} // namespace GDL


#include "gdl/resources/memory/generalPurposeAllocator.inl"
