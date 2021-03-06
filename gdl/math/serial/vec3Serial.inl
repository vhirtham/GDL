#pragma once

#include "gdl/math/serial/vec3Serial.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"


namespace GDL
{

template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol>::Vec3Serial()
    : mData{{0, 0, 0}}
{
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol>::Vec3Serial(std::array<_type, 3> data)
    : mData{data}
{
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol>::Vec3Serial(_type v0, _type v1, _type v2)
    : mData{{v0, v1, v2}}
{
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol>::Vec3Serial(const Vec4Serial<_type, _isCol>& other)
    : mData{{other.mData[0], other.mData[1], other.mData[2]}}
{
}



template <typename _type, bool _isCol>
_type Vec3Serial<_type, _isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 2, "Invalid index value! [0..2]");
    return mData[index];
}



template <typename _type, bool _isCol>
bool Vec3Serial<_type, _isCol>::operator==(const Vec3Serial& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 3; ++i)
        result = result && mData[i] == Approx(rhs.mData[i]);
    return result;
}



template <typename _type, bool _isCol>
bool Vec3Serial<_type, _isCol>::operator!=(const Vec3Serial& rhs) const
{
    return !(operator==(rhs));
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol>& Vec3Serial<_type, _isCol>::operator+=(const Vec3Serial& rhs)
{
    mData[0] += rhs.mData[0];
    mData[1] += rhs.mData[1];
    mData[2] += rhs.mData[2];
    return *this;
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol>& Vec3Serial<_type, _isCol>::operator-=(const Vec3Serial& rhs)
{
    mData[0] -= rhs.mData[0];
    mData[1] -= rhs.mData[1];
    mData[2] -= rhs.mData[2];
    return *this;
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol> Vec3Serial<_type, _isCol>::operator+(const Vec3Serial& rhs) const
{
    return Vec3Serial<_type, _isCol>(mData[0] + rhs.mData[0], mData[1] + rhs.mData[1], mData[2] + rhs.mData[2]);
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol> Vec3Serial<_type, _isCol>::operator-(const Vec3Serial& rhs) const
{
    return Vec3Serial<_type, _isCol>(mData[0] - rhs.mData[0], mData[1] - rhs.mData[1], mData[2] - rhs.mData[2]);
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol> Vec3Serial<_type, _isCol>::operator*(F32 rhs) const
{
    return Vec3Serial<_type, _isCol>(mData[0] * rhs, mData[1] * rhs, mData[2] * rhs);
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol> Vec3Serial<_type, _isCol>::Cross(Vec3Serial rhs) const
{
    DEV_EXCEPTION(*this == Vec3Serial(), "Length of this vector is 0. Can't calculate the cross product.");
    DEV_EXCEPTION(rhs == Vec3Serial(), "Length of rhs vector is 0. Can't calculate the cross product.");

    return Vec3Serial<_type, _isCol>{mData[1] * rhs.mData[2] - mData[2] * rhs.mData[1],
                                     mData[2] * rhs.mData[0] - mData[0] * rhs.mData[2],
                                     mData[0] * rhs.mData[1] - mData[1] * rhs.mData[0]};
}



template <typename _type, bool _isCol>
const std::array<_type, 3> Vec3Serial<_type, _isCol>::Data() const
{
    return mData;
}



template <typename _type, bool _isCol>
template <bool _isColRhs>
F32 Vec3Serial<_type, _isCol>::Dot(Vec3Serial<_type, _isColRhs> rhs) const
{
    return mData[0] * rhs.mData[0] + mData[1] * rhs.mData[1] + mData[2] * rhs.mData[2];
}



template <typename _type, bool _isCol>
F32 Vec3Serial<_type, _isCol>::Length() const
{
    return std::sqrt(Dot(*this));
}



template <typename _type, bool _isCol>
Vec3Serial<_type, _isCol>& Vec3Serial<_type, _isCol>::Normalize()
{
    const F32 length = Length();
    DEV_EXCEPTION(length == ApproxZero<F32>(), "Vector length is 0. Can't normalize the vector.");
    for (U32 i = 0; i < 3; ++i)
        mData[i] /= length;

    return *this;
}



template <typename _type, bool _isCol>
[[nodiscard]] inline Vec3Serial<_type, _isCol> operator*(F32 lhs, Vec3Serial<_type, _isCol> rhs)
{
    return rhs * lhs;
}



// LCOV_EXCL_START

template <typename _type>
std::ostream& operator<<(std::ostream& os, const Vec3Serial<_type, true>& vec)
{
    os << "| " << vec[0] << " |\n| " << vec[1] << " |\n| " << vec[2] << " |" << std::endl;
    return os;
}



template <typename _type>
std::ostream& operator<<(std::ostream& os, const Vec3Serial<_type, false>& vec)
{
    os << "| " << vec[0] << " " << vec[1] << " " << vec[2] << " |" << std::endl;
    return os;
}



// LCOV_EXCL_STOP

} // namespace GDL
