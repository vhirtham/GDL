#pragma once

#include "gdl/math/single/vec4Single.h"

#include "gdl/base/approx.h"
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
bool Vec4Single<_type, _isCol>::operator==(const Vec4Single& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 4; ++i)
        result = result && mData[i] == Approx(rhs.mData[i]);
    return result;
}



template <typename _type, bool _isCol>
bool Vec4Single<_type, _isCol>::operator!=(const Vec4Single& rhs) const
{
    return !(operator==(rhs));
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



template <typename _type, bool _isCol>
F32 Vec4Single<_type, _isCol>::Length() const
{
    return std::sqrt(Dot(*this));
}



template <typename _type, bool _isCol>
Vec4Single<_type, _isCol>& Vec4Single<_type, _isCol>::Normalize()
{
    const F32 length = Length();
    DEV_EXCEPTION(length == ApproxZero<F32>(), "Vector length is 0. Can't normalize the vector.");
    for (U32 i = 0; i < 4; ++i)
        mData[i] /= length;

    return *this;
}



template <typename _type, bool _isCol>
template <bool _isColRhs>
F32 Vec4Single<_type, _isCol>::Dot(Vec4Single<_type, _isColRhs> rhs) const
{
    return mData[0] * rhs.mData[0] + mData[1] * rhs.mData[1] + mData[2] * rhs.mData[2] + mData[3] * rhs.mData[3];
}



// LCOV_EXCL_START

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

// LCOV_EXCL_STOP

} // namespace GDL
