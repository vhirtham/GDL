#pragma once

#include "gdl/math/sse/vec4fSSE.h"

#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/dotProduct.h"

#include <cassert>



namespace GDL
{

template <bool _isCol>
Vec4fSSE<_isCol>::Vec4fSSE()
    : mData{_mm_setzero<__m128>()}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec4fSSE is not aligned");
}



template <bool _isCol>
Vec4fSSE<_isCol>::Vec4fSSE(const Vec4fSSE& other)
    : mData{other.mData}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec4fSSE is not aligned");
}



template <bool _isCol>
Vec4fSSE<_isCol>::Vec4fSSE(std::array<F32, 4> data)
    : mData{_mm_setr<__m128>(data[0], data[1], data[2], data[3])}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec4fSSE is not aligned");
}



template <bool _isCol>
Vec4fSSE<_isCol>::Vec4fSSE(F32 v0, F32 v1, F32 v2, F32 v3)
    : mData{_mm_setr<__m128>(v0, v1, v2, v3)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec4fSSE is not aligned");
}



template <bool _isCol>
Vec4fSSE<_isCol>::Vec4fSSE(__m128 data)
    : mData{data}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec4fSSE is not aligned");
}



template <bool _isCol>
F32 Vec4fSSE<_isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 3, "Invalid index value! [0..3]");

    return simd::GetValue(mData, index);
}



template <bool _isCol>
bool Vec4fSSE<_isCol>::operator==(const Vec4fSSE& rhs) const
{
    return mData == Approx(rhs.mData);
}



template <bool _isCol>
bool Vec4fSSE<_isCol>::operator!=(const Vec4fSSE& rhs) const
{
    return !(operator==(rhs));
}



template <bool _isCol>
Vec4fSSE<_isCol>& Vec4fSSE<_isCol>::operator+=(const Vec4fSSE& rhs)
{
    mData = _mm_add(mData, rhs.mData);
    return *this;
}



template <bool _isCol>
Vec4fSSE<_isCol>& Vec4fSSE<_isCol>::operator-=(const Vec4fSSE& rhs)
{
    mData = _mm_sub(mData, rhs.mData);
    return *this;
}



template <bool _isCol>
Vec4fSSE<_isCol> Vec4fSSE<_isCol>::operator*(F32 rhs) const
{
    return Vec4fSSE<_isCol>(_mm_mul(mData, _mm_set1<__m128>(rhs)));
}



template <bool _isCol>
const std::array<F32, 4> Vec4fSSE<_isCol>::Data() const
{
    std::array<F32, 4> data;
    assert(sizeof(mData) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}



template <bool _isCol>
inline __m128 Vec4fSSE<_isCol>::DataSSE() const
{
    return mData;
}



template <bool _isCol>
template <bool _isColRhs>
F32 Vec4fSSE<_isCol>::Dot(Vec4fSSE<_isColRhs> rhs) const
{
    return simd::DotProductF32(mData, rhs.mData);
}



template <bool _isCol>
F32 Vec4fSSE<_isCol>::Length() const
{
    return _mm_cvtsF<__m128>(_mm_sqrt<__m128>(simd::DotProduct(mData, mData)));
}



template <bool _isCol>
Vec4fSSE<_isCol>& Vec4fSSE<_isCol>::Normalize()
{
    DEV_EXCEPTION(*this == Vec4fSSE(), "Vector length is 0. Can't normalize the vector.");
    mData = _mm_div(mData, _mm_sqrt(simd::DotProduct(mData, mData)));

    return *this;
}



template <bool _isCol>
bool Vec4fSSE<_isCol>::IsDataAligned() const
{
    return IsAligned(&mData, IsAligned(&mData, simd::alignmentBytes<__m128>));
}



template <bool _isCol>
inline Vec4fSSE<_isCol> operator*(F32 lhs, Vec4fSSE<_isCol> rhs)
{
    return rhs * lhs;
}



// LCOV_EXCL_START

template <>
inline std::ostream& operator<<(std::ostream& os, const Vec4fSSE<true>& vec)
{
    os << "| " << vec[0] << " |\n| " << vec[1] << " |\n| " << vec[2] << " |\n| " << vec[3] << " |" << std::endl;
    return os;
}



template <>
inline std::ostream& operator<<(std::ostream& os, const Vec4fSSE<false>& vec)
{
    os << "| " << vec[0] << " " << vec[1] << " " << vec[2] << " " << vec[3] << " |" << std::endl;
    return os;
}

// LCOV_EXCL_STOP

} // namespace GDL
