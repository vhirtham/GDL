#pragma once

#include "gdl/math/serial/vecSerial.h"

#include "gdl/base/exception.h"

#include <iostream>

namespace GDL
{

template <typename _type, U32 _size, bool _isCol>
VecSerial<_type, _size, _isCol>::VecSerial()
    : mData{{0}}
{
}



template <typename _type, U32 _size, bool _isCol>
template <typename... _args>
VecSerial<_type, _size, _isCol>::VecSerial(_args... args)
    : mData{{static_cast<_type>(args)...}}
{
    static_assert(_size == sizeof...(args), "Number of parameters must be equal to the vectors size");
}



template <typename _type, U32 _size, bool _isCol>
VecSerial<_type, _size, _isCol>::VecSerial(const std::array<_type, _size>& data)
    : mData(data)
{
}



template <typename _type, U32 _size, bool _isCol>
[[nodiscard]] inline _type VecSerial<_type, _size, _isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index >= _size, "Invalid index");
    return mData[index];
}



template <typename _type, U32 _size, bool _isCol>
const std::array<_type, _size>& VecSerial<_type, _size, _isCol>::Data() const
{
    return mData;
}



template <typename _type, U32 _size, bool _isCol>
inline std::ostream& operator<<(std::ostream& os, const VecSerial<_type, _size, _isCol>& vec)
{
    if constexpr (_isCol)
    {
        for (U32 i = 0; i < _size; ++i)
            os << "| " << vec[i] << " |" << std::endl;
    }
    else
    {
        os << "| ";
        for (U32 i = 0; i < _size; ++i)
            os << vec[i] << " ";
        os << "|" << std::endl;
    }
    return os;
}



} // namespace GDL
