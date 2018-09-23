#pragma once

#include "gdl/resources/memory/poolDeleter.h"
#include "gdl/resources/memory/poolAllocator.h"


namespace GDL

{

template <typename _type>
template <typename _typeOther>
PoolDeleter<_type>::PoolDeleter(const PoolDeleter<_typeOther>&)
{
}



template <typename _type>
template <typename _typeOther>
PoolDeleter<_type>::PoolDeleter(PoolDeleter<_typeOther>&&)
{
}



template <typename _type>
template <typename _typeOther>
PoolDeleter<_type>& PoolDeleter<_type>::operator=(const PoolDeleter<_typeOther>&)
{
    return *this;
}



template <typename _type>
template <typename _typeOther>
PoolDeleter<_type>& PoolDeleter<_type>::operator=(PoolDeleter<_typeOther>&&)
{
    return *this;
}



template <typename _type>
void PoolDeleter<_type>::operator()(_type* pointer)
{
    pointer->~_type();
    PoolAllocator<_type>::GetMemoryAllocationPattern()->Deallocate(pointer, alignof(_type));
}

} // namespace GDL
