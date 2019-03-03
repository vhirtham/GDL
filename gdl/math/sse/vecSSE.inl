#pragma once

#include "gdl/math/sse/vecSSE.h"


namespace GDL
{

template <typename _type, I32 _size, bool _isCol>
VecSSE<_type, _size, _isCol>::VecSSE()
    : mData{{0}}
{
}


template <typename _type, I32 _size, bool _isCol>
inline std::array<_type, _size> VecSSE<_type, _size, _isCol>::Data() const
{
    std::array<_type, _size> data;

    std::memcpy(&data, &mData, sizeof(data));

    return data;
}

} // namespace GDL
