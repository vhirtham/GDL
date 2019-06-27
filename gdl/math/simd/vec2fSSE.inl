#pragma once

#include "gdl/math/simd/vec2fSSE.h"

#include "gdl/base/approx.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/dotProduct.h"
//#include "gdl/base/simd/maskMacros.h"

#include <cassert>



namespace GDL
{

template <bool _isCol>
Vec2fSSE<_isCol>::Vec2fSSE()
    : mData{_mm_setzero<__m128>()}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec2fSSE is not aligned");
}



template <bool _isCol>
Vec2fSSE<_isCol>::Vec2fSSE(const Vec2fSSE& other)
    : mData{other.mData}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec2fSSE is not aligned");
}



template <bool _isCol>
Vec2fSSE<_isCol>::Vec2fSSE(std::array<F32, 2> data)
    : mData{_mm_setr<__m128>(data[0], data[1], 0, 0)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec2fSSE is not aligned");
}



template <bool _isCol>
Vec2fSSE<_isCol>::Vec2fSSE(F32 v0, F32 v1)
    : mData{_mm_setr<__m128>(v0, v1, 0, 0)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec2fSSE is not aligned");
}



template <bool _isCol>
Vec2fSSE<_isCol>::Vec2fSSE(__m128 data)
    : mData{data}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec2fSSE is not aligned");
}



template <bool _isCol>
F32 Vec2fSSE<_isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 1, "Invalid index value! [0..1]");

    return simd::GetValue(mData, index);
}



template <bool _isCol>
bool Vec2fSSE<_isCol>::operator==(const Vec2fSSE& rhs) const
{
    return mData == Approx<__m128, 2>(rhs.mData);
}



template <bool _isCol>
bool Vec2fSSE<_isCol>::operator!=(const Vec2fSSE& rhs) const
{
    return !(operator==(rhs));
}



template <bool _isCol>
Vec2fSSE<_isCol>& Vec2fSSE<_isCol>::operator+=(const Vec2fSSE& rhs)
{
    mData = _mm_add(mData, rhs.mData);
    return *this;
}



template <bool _isCol>
Vec2fSSE<_isCol>& Vec2fSSE<_isCol>::operator-=(const Vec2fSSE& rhs)
{
    mData = _mm_sub(mData, rhs.mData);
    return *this;
}



template <bool _isCol>
Vec2fSSE<_isCol> Vec2fSSE<_isCol>::operator+(const Vec2fSSE& rhs) const
{
    return _mm_add(mData, rhs.mData);
}



template <bool _isCol>
Vec2fSSE<_isCol> Vec2fSSE<_isCol>::operator-(const Vec2fSSE& rhs) const
{
    return _mm_sub(mData, rhs.mData);
}



template <bool _isCol>
Vec2fSSE<_isCol> Vec2fSSE<_isCol>::operator*(F32 rhs) const
{
    return _mm_mul(mData, _mm_setr<__m128>(rhs, rhs, 0.f, 0.f));
}



template <bool _isCol>
const std::array<F32, 2> Vec2fSSE<_isCol>::Data() const
{
    std::array<F32, 2> data;
    assert(sizeof(mData) - 2 * sizeof(F32) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}



template <bool _isCol>
inline __m128 Vec2fSSE<_isCol>::DataSSE() const
{
    return mData;
}



template <bool _isCol>
template <bool _isColRhs>
F32 Vec2fSSE<_isCol>::Dot(Vec2fSSE<_isColRhs> rhs) const
{
    return simd::DotProductF32<1, 1, 0, 0>(mData, rhs.mData);
}



template <bool _isCol>
F32 Vec2fSSE<_isCol>::Length() const
{
    return _mm_cvtsF(_mm_sqrt(simd::DotProduct<1, 1, 0, 0>(mData, mData)));
}



template <bool _isCol>
Vec2fSSE<_isCol>& Vec2fSSE<_isCol>::Normalize()
{
    DEV_EXCEPTION(*this == Vec2fSSE(), "Vector length is 0. Can't normalize the vector.");
    mData = _mm_div(mData, _mm_sqrt(simd::DotProduct<1, 1, 0, 0>(mData, mData)));

    return *this;
}



template <bool _isCol>
bool Vec2fSSE<_isCol>::IsDataAligned() const
{
    return IsAligned(&mData, simd::alignmentBytes<__m128>);
}



template <bool _isCol>
[[nodiscard]] inline Vec2fSSE<_isCol> operator*(F32 lhs, Vec2fSSE<_isCol> rhs)
{
    return rhs * lhs;
}



// LCOV_EXCL_START

template <>
inline std::ostream& operator<<(std::ostream& os, const Vec2fSSE<true>& vec)
{
    os << "| " << vec[0] << " |\n| " << vec[1] << " |" << std::endl;
    return os;
}



template <>
inline std::ostream& operator<<(std::ostream& os, const Vec2fSSE<false>& vec)
{
    os << "| " << vec[0] << " " << vec[1] << " |" << std::endl;
    return os;
}

// LCOV_EXCL_STOP



} // namespace GDL
