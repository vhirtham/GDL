#pragma once
#include "gdl/math/sse/mat3fSSE.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/vec3.h"



#include <cassert>
#include <cstring>
#include <type_traits>



namespace GDL
{



Mat3fSSE::Mat3fSSE()
    : mData({{{_mmx_setzero_p<__m128>()}, {_mmx_setzero_p<__m128>()}, {_mmx_setzero_p<__m128>()}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat3fSSE are not 16 byte aligned");
}


Mat3fSSE::Mat3fSSE(std::array<F32, 9> data)
    : mData({{{_mmx_setr_p<__m128>(data[0], data[1], data[2], 0)},
              {_mmx_setr_p<__m128>(data[3], data[4], data[5], 0)},
              {_mmx_setr_p<__m128>(data[6], data[7], data[8], 0)}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat3fSSE are not 16 byte aligned");
}



Mat3fSSE::Mat3fSSE(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8)
    : mData({{{_mmx_setr_p<__m128>(v0, v1, v2, 0)},
              {_mmx_setr_p<__m128>(v3, v4, v5, 0)},
              {_mmx_setr_p<__m128>(v6, v7, v8, 0)}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat3fSSE are not 16 byte aligned");
}



Mat3fSSE::Mat3fSSE(__m128 col0, __m128 col1, __m128 col2)
    : mData({{col0, col1, col2}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat3fSSE are not 16 byte aligned");
}



Mat3fSSE::Mat3fSSE(const Mat3fSSE& other)
    : mData(other.mData)
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat3fSSE are not 16 byte aligned");
}



F32 Mat3fSSE::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 2, "row - invalid value! [0..2]");
    DEV_EXCEPTION(col > 2, "col - invalid value! [0..2]");

    return sse::GetValue(mData[col], row);
}



bool Mat3fSSE::operator==(const Mat3fSSE& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 3; ++i)
        result = result && mData[i] == Approx<__m128, 3>(rhs.mData[i]);
    return result;
}



bool Mat3fSSE::operator!=(const Mat3fSSE& rhs) const
{
    return !(operator==(rhs));
}



Mat3fSSE& Mat3fSSE::operator+=(const Mat3fSSE& other)
{
    mData[0] = _mmx_add_p(mData[0], other.mData[0]);
    mData[1] = _mmx_add_p(mData[1], other.mData[1]);
    mData[2] = _mmx_add_p(mData[2], other.mData[2]);
    return *this;
}



Mat3fSSE Mat3fSSE::operator+(const Mat3fSSE& other)

{
    return Mat3fSSE(_mmx_add_p(mData[0], other.mData[0]), _mmx_add_p(mData[1], other.mData[1]),
                   _mmx_add_p(mData[2], other.mData[2]));
}



Mat3fSSE Mat3fSSE::operator*(const Mat3fSSE& rhs) const
{
    using namespace GDL::sse;
    return Mat3fSSE(_mmx_fmadd_p(sse::Swizzle1<0>(rhs.mData[0]), mData[0],
                                _mmx_fmadd_p(Swizzle1<1>(rhs.mData[0]), mData[1],
                                             _mmx_mul_p(Swizzle1<2>(rhs.mData[0]), mData[2]))),
                   _mmx_fmadd_p(Swizzle1<0>(rhs.mData[1]), mData[0],
                                _mmx_fmadd_p(Swizzle1<1>(rhs.mData[1]), mData[1],
                                             _mmx_mul_p(Swizzle1<2>(rhs.mData[1]), mData[2]))),
                   _mmx_fmadd_p(Swizzle1<0>(rhs.mData[2]), mData[0],
                                _mmx_fmadd_p(Swizzle1<1>(rhs.mData[2]), mData[1],
                                             _mmx_mul_p(Swizzle1<2>(rhs.mData[2]), mData[2]))));
}



Vec3fSSE<true> Mat3fSSE::operator*(const Vec3fSSE<true>& rhs) const
{
    using namespace GDL::sse;
    return Vec3fSSE<true>(
            _mmx_fmadd_p(Swizzle1<0>(rhs.mData), mData[0],
                         _mmx_fmadd_p(Swizzle1<1>(rhs.mData), mData[1], _mmx_mul_p(Swizzle1<2>(rhs.mData), mData[2]))));
}



const std::array<F32, 9> Mat3fSSE::Data() const
{
    std::array<F32, 9> data;
    assert(sizeof(mData) - 3 * sizeof(F32) == sizeof(data));

    std::memcpy(&data[0], &mData[0], 3 * sizeof(F32));
    std::memcpy(&data[3], &mData[1], 3 * sizeof(F32));
    std::memcpy(&data[6], &mData[2], 3 * sizeof(F32));
    return data;
}



F32 Mat3fSSE::Det() const
{
    __m128 r1_yzx = _mm_shuffle_ps(mData[1], mData[1], SHUFFLE_4_MASK(1, 2, 0, 3));
    __m128 r2_yzx = _mm_shuffle_ps(mData[2], mData[2], SHUFFLE_4_MASK(1, 2, 0, 3));

    __m128 tmp = _mmx_sub_p(_mmx_mul_p(mData[1], r2_yzx), _mmx_mul_p(r1_yzx, mData[2]));

    return sse::DotProduct<__m128, 3>(mData[0], _mm_shuffle_ps(tmp, tmp, SHUFFLE_4_MASK(1, 2, 0, 3)));
}



Mat3fSSE Mat3fSSE::Transpose() const
{
    __m128 tmp0 = _mm_unpacklo_ps(mData[0], mData[1]);
    __m128 tmp1 = _mm_unpackhi_ps(mData[0], mData[1]);

    return Mat3fSSE(_mm_movelh_ps(tmp0, mData[2]), _mm_shuffle_ps(tmp0, mData[2], SHUFFLE_4_MASK(2, 3, 1, 3)),
                   _mm_shuffle_ps(tmp1, mData[2], SHUFFLE_4_MASK(0, 1, 2, 3)));
}



bool Mat3fSSE::IsDataAligned() const
{
    return (IsAligned(&mData[0], sse::alignmentBytes<__m128>) && IsAligned(&mData[1], sse::alignmentBytes<__m128>) &&
            IsAligned(&mData[2], sse::alignmentBytes<__m128>));
}



inline std::ostream& operator<<(std::ostream& os, const Mat3fSSE& mat)
{
    for (U32 i = 0; i < 3; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " " << mat(i, 2) << " |" << std::endl;
    return os;
}



} // namespace GDL
