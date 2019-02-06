#pragma once

#include "gdl/math/single/vec2Single.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"

#include <cmath>

namespace GDL
{

template <typename _type, bool _isCol>
Vec2Single<_type, _isCol>::Vec2Single()
    : mData{{0, 0}}
{
}



template <typename _type, bool _isCol>
Vec2Single<_type, _isCol>::Vec2Single(std::array<_type, 2> data)
    : mData{data}
{
}



template <typename _type, bool _isCol>
Vec2Single<_type, _isCol>::Vec2Single(_type v0, _type v1)
    : mData{{v0, v1}}
{
}



template <typename _type, bool _isCol>
_type Vec2Single<_type, _isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 1, "Invalid index value! [0..1]");
    return mData[index];
}



template <typename _type, bool _isCol>
bool Vec2Single<_type, _isCol>::operator==(const Vec2Single& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 2; ++i)
        result = result && mData[i] == Approx(rhs.mData[i]);
    return result;
}



template <typename _type, bool _isCol>
bool Vec2Single<_type, _isCol>::operator!=(const Vec2Single& rhs) const
{
    return !(operator==(rhs));
}



template <typename _type, bool _isCol>
Vec2Single<_type, _isCol>& Vec2Single<_type, _isCol>::operator+=(const Vec2Single& rhs)
{
    mData[0] += rhs.mData[0];
    mData[1] += rhs.mData[1];
    return *this;
}



template <typename _type, bool _isCol>
Vec2Single<_type, _isCol>& Vec2Single<_type, _isCol>::operator-=(const Vec2Single& rhs)
{
    mData[0] -= rhs.mData[0];
    mData[1] -= rhs.mData[1];
    return *this;
}



template <typename _type, bool _isCol>
Vec2Single<_type, _isCol> Vec2Single<_type, _isCol>::operator+(const Vec2Single& rhs) const
{
    return Vec2Single<_type, _isCol>(mData[0] + rhs.mData[0], mData[1] + rhs.mData[1]);
}



template <typename _type, bool _isCol>
Vec2Single<_type, _isCol> Vec2Single<_type, _isCol>::operator-(const Vec2Single& rhs) const
{
    return Vec2Single<_type, _isCol>(mData[0] - rhs.mData[0], mData[1] - rhs.mData[1]);
}



template <typename _type, bool _isCol>
Vec2Single<_type, _isCol> Vec2Single<_type, _isCol>::operator*(F32 rhs) const
{
    return Vec2Single<_type, _isCol>(mData[0] * rhs, mData[1] * rhs);
}



template <typename _type, bool _isCol>
const std::array<_type, 2> Vec2Single<_type, _isCol>::Data() const
{
    return mData;
}



template <typename _type, bool _isCol>
template <bool _isColRhs>
F32 Vec2Single<_type, _isCol>::Dot(Vec2Single<_type, _isColRhs> rhs) const
{
    return mData[0] * rhs.mData[0] + mData[1] * rhs.mData[1];
}



template <typename _type, bool _isCol>
F32 Vec2Single<_type, _isCol>::Length() const
{
    return std::sqrt(Dot(*this));
}



template <typename _type, bool _isCol>
Vec2Single<_type, _isCol>& Vec2Single<_type, _isCol>::Normalize()
{
    const F32 length = Length();
    DEV_EXCEPTION(length == ApproxZero<F32>(), "Vector length is 0. Can't normalize the vector.");
    for (U32 i = 0; i < 2; ++i)
        mData[i] /= length;

    return *this;
}



template <typename _type, bool _isCol>
[[nodiscard]] inline Vec2Single<_type, _isCol> operator*(F32 lhs, Vec2Single<_type, _isCol> rhs)
{
    return rhs * lhs;
}



// LCOV_EXCL_START

template <typename _type>
std::ostream& operator<<(std::ostream& os, const Vec2Single<_type, true>& vec)
{
    os << "| " << vec[0] << " |\n| " << vec[1] << " |" << std::endl;
    return os;
}



template <typename _type>
std::ostream& operator<<(std::ostream& os, const Vec2Single<_type, false>& vec)
{
    os << "| " << vec[0] << " " << vec[1] << " |" << std::endl;
    return os;
}

// LCOV_EXCL_STOP



} // namespace GDL
