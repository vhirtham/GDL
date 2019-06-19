#pragma once
#include "gdl/math/serial/mat4Serial.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"

namespace GDL
{


template <typename _type>
Mat4Serial<_type>::Mat4Serial()
    : mD({{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}})
{
}



template <typename _type>
Mat4Serial<_type>::Mat4Serial(_type val)
    : mD({val, 0.0, 0.0, 0.0, 0.0, val, 0.0, 0.0, 0.0, 0.0, val, 0.0, 0.0, 0.0, 0.0, val})
{
}

template <typename _type>
Mat4Serial<_type>::Mat4Serial(std::array<_type, 16> data)
    : mD{data}
{
}

template <typename _type>
Mat4Serial<_type>::Mat4Serial(_type v0, _type v1, _type v2, _type v3, _type v4, _type v5, _type v6, _type v7, _type v8,
                              _type v9, _type v10, _type v11, _type v12, _type v13, _type v14, _type v15)
    : mD({{v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15}})
{
}



template <typename _type>
_type Mat4Serial<_type>::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 3, "row - invalid value! [0..3]");
    DEV_EXCEPTION(col > 3, "col - invalid value! [0..3]");

    return mD[row + col * 4];
}



template <typename _type>
bool Mat4Serial<_type>::operator==(const Mat4Serial<_type>& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 16; ++i)
        result = result && mD[i] == Approx(rhs.mD[i]);
    return result;
}



template <typename _type>
bool Mat4Serial<_type>::operator!=(const Mat4Serial<_type>& rhs) const
{
    return !(operator==(rhs));
}



template <typename _type>
Mat4Serial<_type> Mat4Serial<_type>::operator+(const Mat4Serial<_type>& rhs) const
{
    Mat4Serial<_type> result;
    for (U32 i = 0; i < 16; ++i)
        result.mD[i] = mD[i] + rhs.mD[i];
    return result;
}



template <typename _type>
Mat4Serial<_type>& Mat4Serial<_type>::operator+=(const Mat4Serial<_type>& rhs)
{
    for (U32 i = 0; i < 16; ++i)
        mD[i] += rhs.mD[i];
    return *this;
}



template <typename _type>
Mat4Serial<_type> Mat4Serial<_type>::operator*(const Mat4Serial<_type>& rhs) const
{
    return Mat4Serial<_type>(mD[0] * rhs.mD[0] + mD[4] * rhs.mD[1] + mD[8] * rhs.mD[2] + mD[12] * rhs.mD[3],
                             mD[1] * rhs.mD[0] + mD[5] * rhs.mD[1] + mD[9] * rhs.mD[2] + mD[13] * rhs.mD[3],
                             mD[2] * rhs.mD[0] + mD[6] * rhs.mD[1] + mD[10] * rhs.mD[2] + mD[14] * rhs.mD[3],
                             mD[3] * rhs.mD[0] + mD[7] * rhs.mD[1] + mD[11] * rhs.mD[2] + mD[15] * rhs.mD[3],

                             mD[0] * rhs.mD[4] + mD[4] * rhs.mD[5] + mD[8] * rhs.mD[6] + mD[12] * rhs.mD[7],
                             mD[1] * rhs.mD[4] + mD[5] * rhs.mD[5] + mD[9] * rhs.mD[6] + mD[13] * rhs.mD[7],
                             mD[2] * rhs.mD[4] + mD[6] * rhs.mD[5] + mD[10] * rhs.mD[6] + mD[14] * rhs.mD[7],
                             mD[3] * rhs.mD[4] + mD[7] * rhs.mD[5] + mD[11] * rhs.mD[6] + mD[15] * rhs.mD[7],

                             mD[0] * rhs.mD[8] + mD[4] * rhs.mD[9] + mD[8] * rhs.mD[10] + mD[12] * rhs.mD[11],
                             mD[1] * rhs.mD[8] + mD[5] * rhs.mD[9] + mD[9] * rhs.mD[10] + mD[13] * rhs.mD[11],
                             mD[2] * rhs.mD[8] + mD[6] * rhs.mD[9] + mD[10] * rhs.mD[10] + mD[14] * rhs.mD[11],
                             mD[3] * rhs.mD[8] + mD[7] * rhs.mD[9] + mD[11] * rhs.mD[10] + mD[15] * rhs.mD[11],

                             mD[0] * rhs.mD[12] + mD[4] * rhs.mD[13] + mD[8] * rhs.mD[14] + mD[12] * rhs.mD[15],
                             mD[1] * rhs.mD[12] + mD[5] * rhs.mD[13] + mD[9] * rhs.mD[14] + mD[13] * rhs.mD[15],
                             mD[2] * rhs.mD[12] + mD[6] * rhs.mD[13] + mD[10] * rhs.mD[14] + mD[14] * rhs.mD[15],
                             mD[3] * rhs.mD[12] + mD[7] * rhs.mD[13] + mD[11] * rhs.mD[14] + mD[15] * rhs.mD[15]);
}



template <typename _type>
Vec4Serial<_type, true> Mat4Serial<_type>::operator*(const Vec4Serial<_type, true>& rhs) const
{
    return Vec4Serial<_type, true>(
            mD[0] * rhs.mData[0] + mD[4] * rhs.mData[1] + mD[8] * rhs.mData[2] + mD[12] * rhs.mData[3],
            mD[1] * rhs.mData[0] + mD[5] * rhs.mData[1] + mD[9] * rhs.mData[2] + mD[13] * rhs.mData[3],
            mD[2] * rhs.mData[0] + mD[6] * rhs.mData[1] + mD[10] * rhs.mData[2] + mD[14] * rhs.mData[3],
            mD[3] * rhs.mData[0] + mD[7] * rhs.mData[1] + mD[11] * rhs.mData[2] + mD[15] * rhs.mData[3]);
}



template <typename _type>
const std::array<_type, 16> Mat4Serial<_type>::Data() const
{
    return mD;
}



template <typename _type>
F32 Mat4Serial<_type>::Det() const
{
    F32 ab01 = mD[0] * mD[5] - mD[1] * mD[4];
    F32 cd23 = mD[10] * mD[15] - mD[11] * mD[14];

    F32 ab12 = mD[1] * mD[6] - mD[2] * mD[5];
    F32 cd03 = mD[8] * mD[15] - mD[11] * mD[12];

    F32 ab23 = mD[2] * mD[7] - mD[3] * mD[6];
    F32 cd01 = mD[8] * mD[13] - mD[9] * mD[12];

    F32 ab30 = mD[3] * mD[4] - mD[0] * mD[7];
    F32 cd21 = mD[10] * mD[13] - mD[9] * mD[14];

    F32 ab02 = mD[0] * mD[6] - mD[2] * mD[4];
    F32 cd31 = mD[11] * mD[13] - mD[9] * mD[15];

    F32 ab13 = mD[1] * mD[7] - mD[3] * mD[5];
    F32 cd20 = mD[10] * mD[12] - mD[8] * mD[14];

    return ab01 * cd23 + ab12 * cd03 + ab23 * cd01 + ab30 * cd21 + ab02 * cd31 + ab13 * cd20;
}



template <typename _type>
Mat4Serial<_type> Mat4Serial<_type>::Transpose() const
{
    return Mat4Serial<_type>(mD[0], mD[4], mD[8], mD[12], mD[1], mD[5], mD[9], mD[13], mD[2], mD[6], mD[10], mD[14],
                             mD[3], mD[7], mD[11], mD[15]);
}



template <typename _type2>
inline std::ostream& operator<<(std::ostream& os, const Mat4Serial<_type2>& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " " << mat(i, 2) << " " << mat(i, 3) << " |" << std::endl;
    return os;
}

} // namespace GDL
