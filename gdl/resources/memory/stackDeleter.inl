#pragma once

#include "gdl/resources/memory/stackDeleter.h"
#include "gdl/resources/memory/stackAllocator.h"

namespace GDL

{

template <typename _type>
template <typename _typeOther>
StackDeleter<_type>::StackDeleter(const StackDeleter<_typeOther>&)
{
}



template <typename _type>
template <typename _typeOther>
StackDeleter<_type>::StackDeleter(StackDeleter<_typeOther>&&)
{
}



template <typename _type>
template <typename _typeOther>
StackDeleter<_type>& StackDeleter<_type>::operator=(const StackDeleter<_typeOther>&)
{
    return *this;
}



template <typename _type>
template <typename _typeOther>
StackDeleter<_type>& StackDeleter<_type>::operator=(StackDeleter<_typeOther>&&)
{
    return *this;
}



template <typename _type>
void StackDeleter<_type>::operator()(_type* pointer)
{
    pointer->~_type();
    StackAllocator<_type>::GetMemoryAllocationPattern()->Deallocate(pointer, alignof(_type));
}

} // namespace GDL
