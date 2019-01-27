#pragma once

#include "gdl/math/single/mat3Single.h"

#include "gdl/base/exception.h"
#include "gdl/math/single/vec3Single.h"

namespace GDL
{
template <typename _type>
inline Mat3Single<_type>::Mat3Single()
    : mD({{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}})
{
}



template <typename _type>
Mat3Single<_type>::Mat3Single(_type val)
    : mD({val, 0.0, 0.0, 0.0, val, 0.0, 0.0, 0.0, val})
{
}



template <typename _type>
Mat3Single<_type>::Mat3Single(std::array<_type, 9> data)
    : mD{data}
{
}



template <typename _type>
Mat3Single<_type>::Mat3Single(_type v0, _type v1, _type v2, _type v3, _type v4, _type v5, _type v6, _type v7, _type v8)
    : mD({{v0, v1, v2, v3, v4, v5, v6, v7, v8}})
{
}



template <typename _type>
_type Mat3Single<_type>::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 2, "row - invalid value! [0..2]");
    DEV_EXCEPTION(col > 2, "col - invalid value! [0..2]");

    return mD[row + col * 3];
}



template <typename _type>
bool Mat3Single<_type>::operator==(const Mat3Single<_type>& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 9; ++i)
        result = result && mD[i] == Approx(rhs.mD[i]);
    return result;
}



template <typename _type>
bool Mat3Single<_type>::operator!=(const Mat3Single<_type>& rhs) const
{
    return !(operator==(rhs));
}



template <typename _type>
Mat3Single<_type> Mat3Single<_type>::operator+(const Mat3Single<_type>& rhs) const
{
    Mat3Single<_type> result;
    for (U32 i = 0; i < 9; ++i)
        result.mD[i] = mD[i] + rhs.mD[i];
    return result;
}



template <typename _type>
Mat3Single<_type>& Mat3Single<_type>::operator+=(const Mat3Single<_type>& rhs)
{
    for (U32 i = 0; i < 9; ++i)
        mD[i] += rhs.mD[i];
    return *this;
}



template <typename _type>
Mat3Single<_type> Mat3Single<_type>::operator*(const Mat3Single<_type>& rhs) const
{
    return Mat3Single<_type>(mD[0] * rhs.mD[0] + mD[3] * rhs.mD[1] + mD[6] * rhs.mD[2],
                             mD[1] * rhs.mD[0] + mD[4] * rhs.mD[1] + mD[7] * rhs.mD[2],
                             mD[2] * rhs.mD[0] + mD[5] * rhs.mD[1] + mD[8] * rhs.mD[2],

                             mD[0] * rhs.mD[3] + mD[3] * rhs.mD[4] + mD[6] * rhs.mD[5],
                             mD[1] * rhs.mD[3] + mD[4] * rhs.mD[4] + mD[7] * rhs.mD[5],
                             mD[2] * rhs.mD[3] + mD[5] * rhs.mD[4] + mD[8] * rhs.mD[5],

                             mD[0] * rhs.mD[6] + mD[3] * rhs.mD[7] + mD[6] * rhs.mD[8],
                             mD[1] * rhs.mD[6] + mD[4] * rhs.mD[7] + mD[7] * rhs.mD[8],
                             mD[2] * rhs.mD[6] + mD[5] * rhs.mD[7] + mD[8] * rhs.mD[8]);
}



template <typename _type>
Vec3Single<_type, true> Mat3Single<_type>::operator*(const Vec3Single<_type, true>& rhs) const
{
    return Vec3Single<_type, true>(mD[0] * rhs.mData[0] + mD[3] * rhs.mData[1] + mD[6] * rhs.mData[2],
                                   mD[1] * rhs.mData[0] + mD[4] * rhs.mData[1] + mD[7] * rhs.mData[2],
                                   mD[2] * rhs.mData[0] + mD[5] * rhs.mData[1] + mD[8] * rhs.mData[2]);
}



template <typename _type>
Mat3Single<_type> Mat3Single<_type>::Transpose() const
{
    return Mat3Single<_type>(mD[0], mD[3], mD[6], mD[1], mD[4], mD[7], mD[2], mD[5], mD[8]);
}



template <typename _type>
const std::array<_type, 9> Mat3Single<_type>::Data() const
{
    return mD;
}



template <typename _type2>
inline std::ostream& operator<<(std::ostream& os, const Mat3Single<_type2>& mat)
{
    for (U32 i = 0; i < 3; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " " << mat(i, 2) << " |" << std::endl;
    return os;
}


} // namespace GDL
