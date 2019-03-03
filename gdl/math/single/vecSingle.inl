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
const std::array<_type, _size>& VecSingle<_type, _size, _isCol>::Data() const
{
    return mData;
}


} // namespace GDL
