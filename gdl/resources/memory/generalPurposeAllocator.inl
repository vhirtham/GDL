#pragma once

#include "gdl/resources/memory/generalPurposeAllocator.h"


namespace GDL
{

template <class _type>
GeneralPurposeAllocator<_type>::GeneralPurposeAllocator() noexcept
{
}

template <class _type>
template <class _typeOther>
GeneralPurposeAllocator<_type>::GeneralPurposeAllocator(const GeneralPurposeAllocator<_typeOther>&) noexcept
{
}


template <class _type>
_type* GeneralPurposeAllocator<_type>::allocate(std::size_t n)
{
    static MemoryInterface* memory = MemoryManager::Instance().GetGeneralPurposeMemory();
    if (memory == nullptr)
    {
        static std::allocator<_type> stdAlloc;
        return stdAlloc.allocate(n);
    }
    return static_cast<_type*>(memory->Allocate(n * sizeof(_type), alignof(_type)));
}

template <class _type>
void GeneralPurposeAllocator<_type>::deallocate(_type* p, std::size_t n)
{
    static MemoryInterface* memory = MemoryManager::Instance().GetGeneralPurposeMemory();
    if (memory == nullptr)
    {
        static std::allocator<_type> stdAlloc;
        stdAlloc.deallocate(p, n);
    }
    else
        memory->Deallocate(p);
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
