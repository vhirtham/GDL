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
    : mData{_mmx_setzero_p<__m128>()}
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
    : mData{_mmx_setr_p<__m128>(data[0], data[1], data[2], data[3])}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec4fSSE is not aligned");
}



template <bool _isCol>
Vec4fSSE<_isCol>::Vec4fSSE(F32 v0, F32 v1, F32 v2, F32 v3)
    : mData{_mmx_setr_p<__m128>(v0, v1, v2, v3)}
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

    return sse::GetValue(mData, index);
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
    mData = _mmx_add_p(mData, rhs.mData);
    return *this;
}



template <bool _isCol>
Vec4fSSE<_isCol>& Vec4fSSE<_isCol>::operator-=(const Vec4fSSE& rhs)
{
    mData = _mmx_sub_p(mData, rhs.mData);
    return *this;
}



template <bool _isCol>
Vec4fSSE<_isCol> Vec4fSSE<_isCol>::operator*(F32 rhs)
{
    return Vec4fSSE<_isCol>(_mmx_mul_p(mData, _mmx_set1_p<__m128>(rhs)));
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
F32 Vec4fSSE<_isCol>::Length() const
{
    return _mmx_cvtsx_fx<__m128>(_mmx_sqrt_p<__m128>(sse::DotProduct<__m128, 4, true, 0>(mData, mData)));
}



template <bool _isCol>
Vec4fSSE<_isCol>& Vec4fSSE<_isCol>::Normalize()
{
    DEV_EXCEPTION(*this == Vec4fSSE(), "Vector length is 0. Can't normalize the vector.");
    mData = _mmx_div_p(mData, _mmx_sqrt_p(sse::DotProduct<__m128, 4, true>(mData, mData)));

    return *this;
}



template <bool _isCol>
template <bool _isColRhs>
F32 Vec4fSSE<_isCol>::Dot(Vec4fSSE<_isColRhs> rhs) const
{
    return sse::DotProduct(mData, rhs.mData);
}



template <bool _isCol>
bool Vec4fSSE<_isCol>::IsDataAligned() const
{
    return IsAligned(&mData, IsAligned(&mData, sse::alignmentBytes<__m128>));
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
