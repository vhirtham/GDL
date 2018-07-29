#pragma once

#include "gdl/resources/memory/generalPurposeAllocator.h"

namespace GDL
{

template<class _type>
GeneralPurposeAllocator<_type>::GeneralPurposeAllocator() noexcept
{
}

template<class _type>
template<class _typeOther>
GeneralPurposeAllocator<_type>::GeneralPurposeAllocator(const GeneralPurposeAllocator<_typeOther>&) noexcept
{
}


template<class _type>
_type* GeneralPurposeAllocator<_type>::allocate(std::size_t n)
{
    static GeneralPurposeMemory& memory = MemoryManager::Instance().GetGeneralPurposeMemory();
    return static_cast<_type*>(memory.Allocate(n * sizeof(_type), alignof(_type)));
}

template<class _type>
void GeneralPurposeAllocator<_type>::deallocate(_type *p, std::size_t n)
{
    static GeneralPurposeMemory& memory = MemoryManager::Instance().GetGeneralPurposeMemory();
    memory.Deallocate(p);
}


template <class _type, class _typeOther>
constexpr bool operator==(const GeneralPurposeAllocator<_type>&, const GeneralPurposeAllocator<_typeOther>&) noexcept
{
    return true;
}

template <class _type, class _typeOther>
constexpr bool operator!=(const GeneralPurposeAllocator<_type>&, const GeneralPurposeAllocator<_typeOther>&) noexcept
{
    return false;
}

} // namespace GDL
