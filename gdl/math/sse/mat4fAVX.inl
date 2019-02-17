#pragma once

#ifdef __AVX2__

#include "gdl/math/sse/mat4fAVX.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/base/sse/transpose.h"
#include "gdl/math/vec4.h"



#include <cassert>
#include <cstring>
#include <type_traits>



namespace GDL
{



Mat4fAVX::Mat4fAVX()
    : mData({{{_mmx_setzero_p<__m256>()}, {_mmx_setzero_p<__m256>()}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fAVX are not 32 byte aligned");
}



Mat4fAVX::Mat4fAVX(std::array<F32, 16> data)
    : mData({{{_mmx_setr_p<__m256>(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7])},
              {_mmx_setr_p<__m256>(data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15])}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fAVX are not 32 byte aligned");
}



Mat4fAVX::Mat4fAVX(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10, F32 v11,
                   F32 v12, F32 v13, F32 v14, F32 v15)
    : mData({{{_mmx_setr_p<__m256>(v0, v1, v2, v3, v4, v5, v6, v7)},
              {_mmx_setr_p<__m256>(v8, v9, v10, v11, v12, v13, v14, v15)}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fAVX are not 32 byte aligned");
}



Mat4fAVX::Mat4fAVX(__m256 col01, __m256 col23)
    : mData({{col01, col23}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fAVX are not 32 byte aligned");
}



Mat4fAVX::Mat4fAVX(const Mat4fAVX& other)
    : mData(other.mData)
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fAVX are not 32 byte aligned");
}



F32 Mat4fAVX::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 3, "row - invalid value! [0..3]");
    DEV_EXCEPTION(col > 3, "col - invalid value! [0..3]");

    return sse::GetValue(mData[col / 2], (col % 2) * 4 + row);
}



bool Mat4fAVX::operator==(const Mat4fAVX& rhs) const
{
    return mData[0] == Approx(rhs.mData[0]) && mData[1] == Approx(rhs.mData[1]);
}



bool Mat4fAVX::operator!=(const Mat4fAVX& rhs) const
{
    return !(operator==(rhs));
}



Mat4fAVX& Mat4fAVX::operator+=(const Mat4fAVX& other)

{
    mData[0] = _mmx_add_p(mData[0], other.mData[0]);
    mData[1] = _mmx_add_p(mData[1], other.mData[1]);
    return *this;
}



Mat4fAVX Mat4fAVX::operator+(const Mat4fAVX& other)

{
    return Mat4fAVX(_mmx_add_p(mData[0], other.mData[0]), _mmx_add_p(mData[1], other.mData[1]));
}



Mat4fAVX Mat4fAVX::operator*(const Mat4fAVX& rhs) const
{
    const __m256 tmpBA = sse::Permute2F128<0, 1, 0, 0>(mData[0], mData[0]);
    const __m256 tmpDC = sse::Permute2F128<0, 1, 0, 0>(mData[1], mData[1]);

    const __m256i mask01 = _mm256_setr_epi32(0, 0, 0, 0, 1, 1, 1, 1);
    const __m256i mask10 = _mm256_setr_epi32(1, 1, 1, 1, 0, 0, 0, 0);
    const __m256i mask23 = _mm256_setr_epi32(2, 2, 2, 2, 3, 3, 3, 3);
    const __m256i mask32 = _mm256_setr_epi32(3, 3, 3, 3, 2, 2, 2, 2);

    return Mat4fAVX(
            _mmx_fmadd_p(mData[0], _mm256_permutevar_ps(rhs.mData[0], mask01),
                         _mmx_fmadd_p(tmpBA, _mm256_permutevar_ps(rhs.mData[0], mask10),
                                      _mmx_fmadd_p(mData[1], _mm256_permutevar_ps(rhs.mData[0], mask23),
                                                   _mmx_mul_p(tmpDC, _mm256_permutevar_ps(rhs.mData[0], mask32))))),

            _mmx_fmadd_p(mData[0], _mm256_permutevar_ps(rhs.mData[1], mask01),
                         _mmx_fmadd_p(tmpBA, _mm256_permutevar_ps(rhs.mData[1], mask10),
                                      _mmx_fmadd_p(mData[1], _mm256_permutevar_ps(rhs.mData[1], mask23),
                                                   _mmx_mul_p(tmpDC, _mm256_permutevar_ps(rhs.mData[1], mask32))))));
}



Vec4fSSE<true> Mat4fAVX::operator*(const Vec4fSSE<true>& rhs) const
{
    __m256 tmp = _mm256_broadcast_ps(&rhs.mData);

    const __m256i mask01 = _mm256_setr_epi32(0, 0, 0, 0, 1, 1, 1, 1);
    const __m256i mask23 = _mm256_setr_epi32(2, 2, 2, 2, 3, 3, 3, 3);

    __m256 tmp2 = _mmx_fmadd_p(mData[0], _mm256_permutevar_ps(tmp, mask01),
                               _mmx_mul_p(mData[1], _mm256_permutevar_ps(tmp, mask23)));

    return Vec4fSSE<true>(_mmx_add_p(_mm256_extractf128_ps(tmp2, 0), _mm256_extractf128_ps(tmp2, 1)));
}


const std::array<F32, 16> Mat4fAVX::Data() const
{
    std::array<F32, 16> data;
    assert(sizeof(mData) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}



F32 Mat4fAVX::Det() const
{
    return sse::Determinant4x4(mData[0], mData[1]);
}



Mat4fAVX Mat4fAVX::Transpose() const
{
    __m256 tmp0 = sse::Shuffle<0, 2, 0, 2>(mData[0], mData[1]);
    __m256 tmp1 = sse::Shuffle<1, 3, 1, 3>(mData[0], mData[1]);

    __m256 tmp2 = sse::Blend<0, 0, 0, 0, 1, 1, 1, 1>(tmp0, tmp1);
    __m256 tmp3 = sse::Permute2F128<0, 1, 1, 0>(tmp0, tmp1);

    tmp0 = sse::Shuffle<0, 2, 0, 2>(tmp2, tmp3);
    tmp1 = sse::Shuffle<1, 3, 1, 3>(tmp2, tmp3);

    tmp2 = _mm256_permutevar_ps(tmp0, _mm256_setr_epi32(0, 2, 1, 3, 2, 0, 3, 1));
    tmp3 = _mm256_permutevar_ps(tmp1, _mm256_setr_epi32(0, 2, 1, 3, 2, 0, 3, 1));

    return Mat4fAVX(tmp2, tmp3);
}



bool Mat4fAVX::IsDataAligned() const
{
    return (IsAligned(&mData[0], sse::alignmentBytes<__m256>) && IsAligned(&mData[1], sse::alignmentBytes<__m256>));
}



inline std::ostream& operator<<(std::ostream& os, const Mat4fAVX& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " " << mat(i, 2) << " " << mat(i, 3) << " |" << std::endl;
    return os;
}



} // namespace GDL
#endif //__AVX2__
