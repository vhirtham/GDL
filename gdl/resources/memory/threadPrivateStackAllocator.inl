#pragma once

#include "gdl/base/exception.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"
#include "gdl/resources/memory/memoryManager.h"

namespace GDL
{

template <class _type>
ThreadPrivateStackAllocator<_type>::ThreadPrivateStackAllocator() noexcept
    : mMemoryAllocationPattern{*GetMemoryAllocationPattern()}
{
}



template <class _type>
template <class _typeOther>
ThreadPrivateStackAllocator<_type>::ThreadPrivateStackAllocator(
        const ThreadPrivateStackAllocator<_typeOther>& other) noexcept
    : mMemoryAllocationPattern{other.mMemoryAllocationPattern}
{
}



template <class _type>
_type* ThreadPrivateStackAllocator<_type>::allocate(std::size_t numInstances)
{
    return static_cast<_type*>(mMemoryAllocationPattern.Allocate(numInstances * sizeof(_type), alignof(_type)));
}



template <class _type>
void ThreadPrivateStackAllocator<_type>::deallocate(_type* pointer, std::size_t)
{
    mMemoryAllocationPattern.Deallocate(pointer, alignof(_type));
}



template <class _type>
MemoryInterface* ThreadPrivateStackAllocator<_type>::GetMemoryAllocationPattern()
{
    MemoryInterface* memoryAP = MemoryManager::Instance().GetThreadPrivateMemoryStack();
    if (memoryAP == nullptr)
    {
        DEV_EXCEPTION(MemoryManager::Instance().IsThreadPrivateMemoryEnabled(),
                      "No thread private memory created for the calling thread.");
        static MemoryInterface* alternativeMemoryAP = InitializeAlternativeMemoryAllocationPattern();
        return alternativeMemoryAP;
    }
    return memoryAP;
}

template <class _type>
MemoryInterface* ThreadPrivateStackAllocator<_type>::InitializeAlternativeMemoryAllocationPattern()
{
    MemoryInterface* memoryAP = MemoryManager::Instance().GetGeneralPurposeMemory();
    if (memoryAP == nullptr)
        return MemoryManager::Instance().GetHeapMemory();
    return memoryAP;
}



template <class _type, class _typeOther>
constexpr bool operator==(const ThreadPrivateStackAllocator<_type>&,
                          const ThreadPrivateStackAllocator<_typeOther>&) noexcept
{
    return true;
}



template <class _type, class _typeOther>
constexpr bool operator!=(const ThreadPrivateStackAllocator<_type>&,
                          const ThreadPrivateStackAllocator<_typeOther>&) noexcept
{
    return false;
}


} // namespace GDL
