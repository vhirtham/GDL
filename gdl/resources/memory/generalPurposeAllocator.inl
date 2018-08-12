#pragma once

#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/memoryManager.h"

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
    static MemoryInterface* memory = GetMemoryModel();
    return static_cast<_type*>(memory->Allocate(n * sizeof(_type), alignof(_type)));
}



template <class _type>
void GeneralPurposeAllocator<_type>::deallocate(_type* p, [[maybe_unused]] std::size_t n)
{
    static MemoryInterface* memory = GetMemoryModel();
    memory->Deallocate(p, alignof(_type));
}



template <class _type>
MemoryInterface* GeneralPurposeAllocator<_type>::GetMemoryModel()
{
    MemoryInterface* memory = MemoryManager::Instance().GetGeneralPurposeMemory();
    if (memory == nullptr)
        return MemoryManager::Instance().GetHeapMemory();
    return memory;
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
