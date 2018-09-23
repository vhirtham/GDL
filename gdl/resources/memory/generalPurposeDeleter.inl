#pragma once

#include "gdl/resources/memory/generalPurposeDeleter.h"
#include "gdl/resources/memory/generalPurposeAllocator.h"


namespace GDL

{

template <typename _type>
template <typename _typeOther>
GeneralPurposeDeleter<_type>::GeneralPurposeDeleter(const GeneralPurposeDeleter<_typeOther>&)
{
}



template <typename _type>
template <typename _typeOther>
GeneralPurposeDeleter<_type>::GeneralPurposeDeleter(GeneralPurposeDeleter<_typeOther>&&)
{
}



template <typename _type>
template <typename _typeOther>
GeneralPurposeDeleter<_type>& GeneralPurposeDeleter<_type>::operator=(const GeneralPurposeDeleter<_typeOther>&)
{
    return *this;
}



template <typename _type>
template <typename _typeOther>
GeneralPurposeDeleter<_type>& GeneralPurposeDeleter<_type>::operator=(GeneralPurposeDeleter<_typeOther>&&)
{
    return *this;
}



template <typename _type>
void GeneralPurposeDeleter<_type>::operator()(_type* pointer)
{
    pointer->~_type();
    GeneralPurposeAllocator<_type>::GetMemoryAllocationPattern()->Deallocate(pointer, alignof(_type));
}

} // namespace GDL
