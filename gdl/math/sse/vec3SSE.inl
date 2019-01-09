#pragma once

#include "gdl/math/sse/vec3SSE.h"

#include "gdl/base/approx.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/maskMacros.h"

#include <cassert>



namespace GDL
{

template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE()
    : mData{_mmx_setzero_p<__m128>()}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE(const Vec3SSE& other)
    : mData{other.mData}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE(std::array<F32, 3> data)
    : mData{_mmx_setr_p<__m128>(data[0], data[1], data[2], 0)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE(F32 v0, F32 v1, F32 v2)
    : mData{_mmx_setr_p<__m128>(v0, v1, v2, 0)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE(__m128 data)
    : mData{data}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
bool Vec3SSE<_isCol>::operator==(const Vec3SSE& rhs) const
{
    return mData == Approx<__m128, 3>(rhs.mData);
}



template <bool _isCol>
bool Vec3SSE<_isCol>::operator!=(const Vec3SSE& rhs) const
{
    return !(operator==(rhs));
}



template <bool _isCol>
F32 Vec3SSE<_isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 2, "Invalid index value! [0..2]");

    return sse::GetValue(mData, index);
}



template <bool _isCol>
Vec3SSE<_isCol> Vec3SSE<_isCol>::Cross(Vec3SSE rhs) const
{
    DEV_EXCEPTION(*this == Vec3SSE(), "Length of this vector is 0. Can't calculate the cross product.");
    DEV_EXCEPTION(rhs == Vec3SSE(), "Length of rhs vector is 0. Can't calculate the cross product.");

    // source: http://threadlocalmutex.com/?p=8
    __m128 lhs_yzx = _mm_shuffle_ps(mData, mData, SHUFFLE_4_MASK(1, 2, 0, 3));
    __m128 rhs_yzx = _mm_shuffle_ps(rhs.mData, rhs.mData, SHUFFLE_4_MASK(1, 2, 0, 3));

    __m128 tmp = _mmx_sub_p(_mmx_mul_p(mData, rhs_yzx), _mmx_mul_p(lhs_yzx, rhs.mData));

    return Vec3SSE<_isCol>(_mm_shuffle_ps(tmp, tmp, SHUFFLE_4_MASK(1, 2, 0, 3)));
}



template <bool _isCol>
const std::array<F32, 3> Vec3SSE<_isCol>::Data() const
{
    std::array<F32, 3> data;
    assert(sizeof(mData) - sizeof(F32) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}



template <bool _isCol>
F32 Vec3SSE<_isCol>::Length() const
{
    return _mmx_cvtsx_fx(_mmx_sqrt_p(sse::DotProduct<__m128, 3, true, 0>(mData, mData)));
}



template <bool _isCol>
Vec3SSE<_isCol>& Vec3SSE<_isCol>::Normalize()
{
    DEV_EXCEPTION(*this == Vec3SSE(), "Vector length is 0. Can't normalize the vector.");
    mData = _mmx_div_p(mData, _mmx_sqrt_p(sse::DotProduct<__m128, 3, true>(mData, mData)));

    return *this;
}



template <bool _isCol>
template <bool _isColRhs>
F32 Vec3SSE<_isCol>::Dot(Vec3SSE<_isColRhs> rhs) const
{
    return sse::DotProduct<__m128, 3>(mData, rhs.mData);
}



template <bool _isCol>
bool Vec3SSE<_isCol>::IsDataAligned() const
{
    return IsAligned(&mData, 16);
}



// LCOV_EXCL_START

template <>
std::ostream& operator<<(std::ostream& os, const Vec3SSE<true>& vec)
{
    os << "| " << vec[0] << " |\n| " << vec[1] << " |\n| " << vec[2] << " |" << std::endl;
    return os;
}



template <>
std::ostream& operator<<(std::ostream& os, const Vec3SSE<false>& vec)
{
    os << "| " << vec[0] << " " << vec[1] << " " << vec[2] << " |" << std::endl;
    return os;
}


// LCOV_EXCL_STOP


} // namespace GDL
