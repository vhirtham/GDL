#pragma once

#include "gdl/math/sse/vec3fSSE.h"

#include "gdl/base/approx.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/swizzle.h"

#include <cassert>



namespace GDL
{

template <bool _isCol>
Vec3fSSE<_isCol>::Vec3fSSE()
    : mData{_mmx_setzero_p<__m128>()}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3fSSE is not aligned");
}



template <bool _isCol>
Vec3fSSE<_isCol>::Vec3fSSE(const Vec3fSSE& other)
    : mData{other.mData}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3fSSE is not aligned");
}



template <bool _isCol>
Vec3fSSE<_isCol>::Vec3fSSE(std::array<F32, 3> data)
    : mData{_mmx_setr_p<__m128>(data[0], data[1], data[2], 0)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3fSSE is not aligned");
}



template <bool _isCol>
Vec3fSSE<_isCol>::Vec3fSSE(F32 v0, F32 v1, F32 v2)
    : mData{_mmx_setr_p<__m128>(v0, v1, v2, 0)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3fSSE is not aligned");
}



template <bool _isCol>
Vec3fSSE<_isCol>::Vec3fSSE(const Vec4fSSE<_isCol>& other)
    : mData{simd::Blend<0, 0, 0, 1>(other.mData, _mmx_setzero_p<__m128>())}
{
}



template <bool _isCol>
Vec3fSSE<_isCol>::Vec3fSSE(__m128 data)
    : mData{data}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3fSSE is not aligned");
}



template <bool _isCol>
F32 Vec3fSSE<_isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 2, "Invalid index value! [0..2]");

    return simd::GetValue(mData, index);
}



template <bool _isCol>
bool Vec3fSSE<_isCol>::operator==(const Vec3fSSE& rhs) const
{
    return mData == Approx<__m128, 3>(rhs.mData);
}



template <bool _isCol>
bool Vec3fSSE<_isCol>::operator!=(const Vec3fSSE& rhs) const
{
    return !(operator==(rhs));
}



template <bool _isCol>
Vec3fSSE<_isCol>& Vec3fSSE<_isCol>::operator+=(const Vec3fSSE& rhs)
{
    mData = _mmx_add_p(mData, rhs.mData);
    return *this;
}



template <bool _isCol>
Vec3fSSE<_isCol>& Vec3fSSE<_isCol>::operator-=(const Vec3fSSE& rhs)
{
    mData = _mmx_sub_p(mData, rhs.mData);
    return *this;
}



template <bool _isCol>
Vec3fSSE<_isCol> Vec3fSSE<_isCol>::operator+(const Vec3fSSE& rhs) const
{
    return _mmx_add_p(mData, rhs.mData);
}



template <bool _isCol>
Vec3fSSE<_isCol> Vec3fSSE<_isCol>::operator-(const Vec3fSSE& rhs) const
{
    return _mmx_sub_p(mData, rhs.mData);
}



template <bool _isCol>
Vec3fSSE<_isCol> Vec3fSSE<_isCol>::operator*(F32 rhs) const
{
    return _mmx_mul_p(mData, _mmx_setr_p<__m128>(rhs, rhs, rhs, 0.f));
}



template <bool _isCol>
Vec3fSSE<_isCol> Vec3fSSE<_isCol>::Cross(Vec3fSSE rhs) const
{
    DEV_EXCEPTION(*this == Vec3fSSE(), "Length of this vector is 0. Can't calculate the cross product.");
    DEV_EXCEPTION(rhs == Vec3fSSE(), "Length of rhs vector is 0. Can't calculate the cross product.");

    return simd::CrossProduct(mData, rhs.mData);
}



template <bool _isCol>
const std::array<F32, 3> Vec3fSSE<_isCol>::Data() const
{
    std::array<F32, 3> data;
    assert(sizeof(mData) - sizeof(F32) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}



template <bool _isCol>
inline __m128 Vec3fSSE<_isCol>::DataSSE() const
{
    return mData;
}



template <bool _isCol>
template <bool _isColRhs>
F32 Vec3fSSE<_isCol>::Dot(Vec3fSSE<_isColRhs> rhs) const
{
    return simd::DotProductF32<1, 1, 1, 0>(mData, rhs.mData);
}



template <bool _isCol>
F32 Vec3fSSE<_isCol>::Length() const
{
    return _mmx_cvtsx_fx(_mmx_sqrt_p(simd::DotProduct<1, 1, 1, 0>(mData, mData)));
}



template <bool _isCol>
Vec3fSSE<_isCol>& Vec3fSSE<_isCol>::Normalize()
{
    DEV_EXCEPTION(*this == Vec3fSSE(), "Vector length is 0. Can't normalize the vector.");
    mData = _mmx_div_p(mData, _mmx_sqrt_p(simd::DotProduct<1, 1, 1, 0>(mData, mData)));

    return *this;
}



template <bool _isCol>
bool Vec3fSSE<_isCol>::IsDataAligned() const
{
    return IsAligned(&mData, IsAligned(&mData, simd::alignmentBytes<__m128>));
}



template <bool _isCol>
[[nodiscard]] inline Vec3fSSE<_isCol> operator*(F32 lhs, Vec3fSSE<_isCol> rhs)
{
    return rhs * lhs;
}



// LCOV_EXCL_START

template <>
inline std::ostream& operator<<(std::ostream& os, const Vec3fSSE<true>& vec)
{
    os << "| " << vec[0] << " |\n| " << vec[1] << " |\n| " << vec[2] << " |" << std::endl;
    return os;
}



template <>
inline std::ostream& operator<<(std::ostream& os, const Vec3fSSE<false>& vec)
{
    os << "| " << vec[0] << " " << vec[1] << " " << vec[2] << " |" << std::endl;
    return os;
}

// LCOV_EXCL_STOP


} // namespace GDL
