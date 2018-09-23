#pragma once

#include "gdl/resources/memory/threadPrivateStackDeleter.h"

#include <type_traits>



namespace GDL

{

template <typename _type>
ThreadPrivateStackDeleter<_type>::ThreadPrivateStackDeleter()
    : mMemoryAllocationPattern{*ThreadPrivateStackAllocator<_type>().GetMemoryAllocationPattern()}
{
}


template <typename _type>
template <typename _typeOther>
ThreadPrivateStackDeleter<_type>::ThreadPrivateStackDeleter(const ThreadPrivateStackDeleter<_typeOther>& other)
    : mMemoryAllocationPattern{other.mMemoryAllocationPattern}
{
    static_assert(std::is_same<_type, _typeOther>::value || std::is_base_of<_type, _typeOther>::value,
                  "Types are not compatible.");
}



template <typename _type>
template <typename _typeOther>
ThreadPrivateStackDeleter<_type>::ThreadPrivateStackDeleter(ThreadPrivateStackDeleter<_typeOther>&& other)
    : mMemoryAllocationPattern{other.mMemoryAllocationPattern}
{
    static_assert(std::is_same<_type, _typeOther>::value || std::is_base_of<_type, _typeOther>::value,
                  "Types are not compatible.");
}



template <typename _type>
template <typename _typeOther>
ThreadPrivateStackDeleter<_type>& ThreadPrivateStackDeleter<_type>::
operator=(const ThreadPrivateStackDeleter<_typeOther>&)
{
    static_assert(std::is_same<_type, _typeOther>::value || std::is_base_of<_type, _typeOther>::value,
                  "Types are not compatible.");
    return *this;
}



template <typename _type>
template <typename _typeOther>
ThreadPrivateStackDeleter<_type>& ThreadPrivateStackDeleter<_type>::operator=(ThreadPrivateStackDeleter<_typeOther>&&)
{
    static_assert(std::is_same<_type, _typeOther>::value || std::is_base_of<_type, _typeOther>::value,
                  "Types are not compatible.");
    return *this;
}



template <typename _type>
template <typename _typeAllocator>
ThreadPrivateStackDeleter<_type>::ThreadPrivateStackDeleter(
        ThreadPrivateStackAllocator<_typeAllocator>& threadPrivateStackAllocator)
    : mMemoryAllocationPattern{*threadPrivateStackAllocator.GetMemoryAllocationPattern()}
{
    static_assert(std::is_same<_type, _typeAllocator>::value || std::is_base_of<_type, _typeAllocator>::value,
                  "Types are not compatible.");
}



template <typename _type>
void ThreadPrivateStackDeleter<_type>::operator()(_type* pointer)
{
    pointer->~_type();
    mMemoryAllocationPattern.Deallocate(pointer, alignof(_type));
}

} // namespace GDL
