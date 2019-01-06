#pragma once

#include "gdl/math/single/vec4Single.h"

#include "gdl/base/exception.h"


namespace GDL
{

template <typename _type, bool _isCol>
Vec4Single<_type, _isCol>::Vec4Single()
    : mData{{0, 0, 0, 0}}
{
}



template <typename _type, bool _isCol>
Vec4Single<_type, _isCol>::Vec4Single(std::array<_type, 4> data)
    : mData{data}
{
}



template <typename _type, bool _isCol>
Vec4Single<_type, _isCol>::Vec4Single(_type v0, _type v1, _type v2, _type v3)
    : mData{{v0, v1, v2, v3}}
{
}



template <typename _type, bool _isCol>
_type Vec4Single<_type, _isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 4, "Invalid index value! [0..3]");
    return mData[index];
}



template <typename _type, bool _isCol>
const std::array<_type, 4> Vec4Single<_type, _isCol>::Data() const
{
    return mData;
}



template <typename _type>
std::ostream& operator<<(std::ostream& os, const Vec4Single<_type, true>& vec)
{
    os << "| " << vec[0] << " |\n| " << vec[1] << " |\n| " << vec[2] << " |\n| " << vec[3] << " |" << std::endl;
    return os;
}



template <typename _type>
std::ostream& operator<<(std::ostream& os, const Vec4Single<_type, false>& vec)
{
    os << "| " << vec[0] << " " << vec[1] << " " << vec[2] << " " << vec[3] << " |" << std::endl;
    return os;
}


} // namespace GDL
