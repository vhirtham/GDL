#pragma once

#include "gdl/math/single/vecSingle.h"


namespace GDL
{

template <typename _type, I32 _size, bool _isCol>
VecSingle<_type, _size, _isCol>::VecSingle()
    : mData{{0}}
{
}



template <typename _type, I32 _size, bool _isCol>
template <typename... _args>
VecSingle<_type, _size, _isCol>::VecSingle(_args... args)
    : mData{{static_cast<_type>(args)...}}
{
    static_assert(_size == sizeof...(args), "Number of parameters must be equal to the vectors size");
}



template <typename _type, I32 _size, bool _isCol>
VecSingle<_type, _size, _isCol>::VecSingle(const std::array<_type, _size>& data)
    : mData(data)
{
}



template <typename _type, I32 _size, bool _isCol>
const std::array<_type, _size>& VecSingle<_type, _size, _isCol>::Data() const
{
    return mData;
}


} // namespace GDL
