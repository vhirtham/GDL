#pragma once
#include "gdl/math/mat4Single.h"

#include "gdl/base/exception.h"

namespace GDL
{


template <typename _type>
Mat4Single<_type>::Mat4Single()
    : mD({{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}})
{
}



template <typename _type>
Mat4Single<_type>::Mat4Single(_type val)
    : mD({val, 0.0, 0.0, 0.0, 0.0, val, 0.0, 0.0, 0.0, 0.0, val, 0.0, 0.0, 0.0, 0.0, val})
{
}



template <typename _type>
Mat4Single<_type>::Mat4Single(_type v0, _type v1, _type v2, _type v3, _type v4, _type v5, _type v6, _type v7, _type v8,
                              _type v9, _type v10, _type v11, _type v12, _type v13, _type v14, _type v15)
    : mD({{v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15}})
{
}



template <typename _type>
Mat4Single<_type> Mat4Single<_type>::operator*(const Mat4Single<_type>& other) const
{
    return Mat4Single<_type>(
            mD[0] * other.mD[0] + mD[4] * other.mD[1] + mD[8] * other.mD[2] + mD[12] * other.mD[3],
            mD[1] * other.mD[0] + mD[5] * other.mD[1] + mD[9] * other.mD[2] + mD[13] * other.mD[3],
            mD[2] * other.mD[0] + mD[6] * other.mD[1] + mD[10] * other.mD[2] + mD[14] * other.mD[3],
            mD[3] * other.mD[0] + mD[7] * other.mD[1] + mD[11] * other.mD[2] + mD[15] * other.mD[3],

            mD[0] * other.mD[4] + mD[4] * other.mD[5] + mD[8] * other.mD[6] + mD[12] * other.mD[7],
            mD[1] * other.mD[4] + mD[5] * other.mD[5] + mD[9] * other.mD[6] + mD[13] * other.mD[7],
            mD[2] * other.mD[4] + mD[6] * other.mD[5] + mD[10] * other.mD[6] + mD[14] * other.mD[7],
            mD[3] * other.mD[4] + mD[7] * other.mD[5] + mD[11] * other.mD[6] + mD[15] * other.mD[7],

            mD[0] * other.mD[8] + mD[4] * other.mD[9] + mD[8] * other.mD[10] + mD[12] * other.mD[11],
            mD[1] * other.mD[8] + mD[5] * other.mD[9] + mD[9] * other.mD[10] + mD[13] * other.mD[11],
            mD[2] * other.mD[8] + mD[6] * other.mD[9] + mD[10] * other.mD[10] + mD[14] * other.mD[11],
            mD[3] * other.mD[8] + mD[7] * other.mD[9] + mD[11] * other.mD[10] + mD[15] * other.mD[11],

            mD[0] * other.mD[12] + mD[4] * other.mD[13] + mD[8] * other.mD[14] + mD[12] * other.mD[15],
            mD[1] * other.mD[12] + mD[5] * other.mD[13] + mD[9] * other.mD[14] + mD[13] * other.mD[15],
            mD[2] * other.mD[12] + mD[6] * other.mD[13] + mD[10] * other.mD[14] + mD[14] * other.mD[15],
            mD[3] * other.mD[12] + mD[7] * other.mD[13] + mD[11] * other.mD[14] + mD[15] * other.mD[15]);
}



template <typename _type>
Mat4Single<_type>& Mat4Single<_type>::operator+=(const Mat4Single<_type>& rhs)
{
    for (U32 i = 0; i < 16; ++i)
        mD[i] += rhs.mD[i];
    return *this;
}



template <typename _type>
_type Mat4Single<_type>::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 3, "row - invalid value! [0..3]");
    DEV_EXCEPTION(col > 3, "col - invalid value! [0..3]");

    return mD[row + col * 4];
}



template <typename _type>
const std::array<_type, 16> Mat4Single<_type>::Data() const
{
    return mD;
}



template <typename _type2>
inline std::ostream& operator<<(std::ostream& os, const Mat4Single<_type2>& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " " << mat(i, 2) << " " << mat(i, 3) << " |" << std::endl;
    return os;
}

} // namespace GDL
