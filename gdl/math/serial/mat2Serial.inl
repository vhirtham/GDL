#pragma once

#include "gdl/math/serial/mat2Serial.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/math/serial/vec2Serial.h"

namespace GDL
{

template <typename _type>
inline Mat2Serial<_type>::Mat2Serial()
    : mD({{0.0, 0.0, 0.0, 0.0}})
{
}



template <typename _type>
Mat2Serial<_type>::Mat2Serial(_type val)
    : mD({val, 0.0, 0.0, val})
{
}



template <typename _type>
Mat2Serial<_type>::Mat2Serial(std::array<_type, 4> data)
    : mD{data}
{
}



template <typename _type>
Mat2Serial<_type>::Mat2Serial(_type v0, _type v1, _type v2, _type v3)
    : mD({{v0, v1, v2, v3}})
{
}



template <typename _type>
_type Mat2Serial<_type>::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 1, "row - invalid value! [0..1]");
    DEV_EXCEPTION(col > 1, "col - invalid value! [0..1]");

    return mD[row + col * 2];
}



template <typename _type>
bool Mat2Serial<_type>::operator==(const Mat2Serial<_type>& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 4; ++i)
        result = result && mD[i] == Approx(rhs.mD[i]);
    return result;
}



template <typename _type>
bool Mat2Serial<_type>::operator!=(const Mat2Serial<_type>& rhs) const
{
    return !(operator==(rhs));
}



template <typename _type>
Mat2Serial<_type> Mat2Serial<_type>::operator+(const Mat2Serial<_type>& rhs) const
{
    Mat2Serial<_type> result;
    for (U32 i = 0; i < 4; ++i)
        result.mD[i] = mD[i] + rhs.mD[i];
    return result;
}



template <typename _type>
Mat2Serial<_type>& Mat2Serial<_type>::operator+=(const Mat2Serial<_type>& rhs)
{
    for (U32 i = 0; i < 4; ++i)
        mD[i] += rhs.mD[i];
    return *this;
}



template <typename _type>
Mat2Serial<_type> Mat2Serial<_type>::operator*(const Mat2Serial<_type>& rhs) const
{
    return Mat2Serial<_type>(mD[0] * rhs.mD[0] + mD[2] * rhs.mD[1], mD[1] * rhs.mD[0] + mD[3] * rhs.mD[1],
                             mD[0] * rhs.mD[2] + mD[2] * rhs.mD[3], mD[1] * rhs.mD[2] + mD[3] * rhs.mD[3]);
}



template <typename _type>
Vec2Serial<_type, true> Mat2Serial<_type>::operator*(const Vec2Serial<_type, true>& rhs) const
{
    return Vec2Serial<_type, true>(mD[0] * rhs.mData[0] + mD[2] * rhs.mData[1],
                                   mD[1] * rhs.mData[0] + mD[3] * rhs.mData[1]);
}



template <typename _type>
const std::array<_type, 4> Mat2Serial<_type>::Data() const
{
    return mD;
}



template <typename _type>
F32 Mat2Serial<_type>::Det() const
{
    return mD[0] * mD[3] - mD[1] * mD[2];
}



template <typename _type>
Mat2Serial<_type> Mat2Serial<_type>::Transpose() const
{
    return Mat2Serial<_type>(mD[0], mD[2], mD[1], mD[3]);
}



template <typename _type2>
inline std::ostream& operator<<(std::ostream& os, const Mat2Serial<_type2>& mat)
{
    for (U32 i = 0; i < 2; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " |" << std::endl;
    return os;
}



} // namespace GDL
