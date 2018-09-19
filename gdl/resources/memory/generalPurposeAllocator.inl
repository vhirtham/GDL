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
_type* GeneralPurposeAllocator<_type>::allocate(std::size_t numInstances)
{
    return static_cast<_type*>(GetMemoryAllocationPattern()->Allocate(numInstances * sizeof(_type), alignof(_type)));
}




template <class _type>
void GeneralPurposeAllocator<_type>::deallocate(_type* pointer, std::size_t)
{
    GetMemoryAllocationPattern()->Deallocate(pointer, alignof(_type));
}




template <class _type>
void GeneralPurposeAllocator<_type>::Deleter(_type* pointer)
{
    GetMemoryAllocationPattern()->Deallocate(pointer, alignof(_type));
}




template <class _type>
MemoryInterface* GeneralPurposeAllocator<_type>::GetMemoryAllocationPattern()
{
    static MemoryInterface* memoryAP = InitializeMemoryAllocationPattern();
    return memoryAP;
}



template<class _type>
MemoryInterface *GeneralPurposeAllocator<_type>::InitializeMemoryAllocationPattern()
{
    MemoryInterface* memoryAP = MemoryManager::Instance().GetGeneralPurposeMemory();
    if (memoryAP == nullptr)
        return MemoryManager::Instance().GetHeapMemory();
    return memoryAP;
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
