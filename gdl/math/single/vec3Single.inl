#pragma once

#include "gdl/math/single/vec3Single.h"

#include "gdl/base/exception.h"


namespace GDL
{

template <typename _type, bool _isCol>
Vec3Single<_type, _isCol>::Vec3Single()
    : mData{{0, 0, 0}}
{
}



template <typename _type, bool _isCol>
Vec3Single<_type, _isCol>::Vec3Single(std::array<_type, 3> data)
    : mData{data}
{
}



template <typename _type, bool _isCol>
Vec3Single<_type, _isCol>::Vec3Single(_type v0, _type v1, _type v2)
    : mData{{v0, v1, v2}}
{
}



template <typename _type, bool _isCol>
_type Vec3Single<_type, _isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 3, "Invalid index value! [0..2]");
    return mData[index];
}



template <typename _type, bool _isCol>
const std::array<_type, 3> Vec3Single<_type, _isCol>::Data() const
{
    return mData;
}



template <typename _type>
std::ostream& operator<<(std::ostream& os, const Vec3Single<_type, true>& vec)
{
    os << "| " << vec[0] << " |\n| " << vec[1] << " |\n| " << vec[2] << " |" << std::endl;
    return os;
}



template <typename _type>
std::ostream& operator<<(std::ostream& os, const Vec3Single<_type, false>& vec)
{
    os << "| " << vec[0] << " " << vec[1] << " " << vec[2] << " |" << std::endl;
    return os;
}


} // namespace GDL
