#pragma once
#include "mat4SIMD.h"

#include "gdl/base/Exception.h"


#include <cassert>
#include <cstring>
#include <type_traits>

GDL::mat4SIMD::mat4SIMD()
    : mData({{{_mm_set1_ps(0.)}, {_mm_set1_ps(0.)}, {_mm_set1_ps(0.)}, {_mm_set1_ps(0.)}}})
{
#ifndef NDEBUG
    if (!(is_aligned(&mData[0], 16) && is_aligned(&mData[1], 16) && is_aligned(&mData[2], 16) && is_aligned(&mData[3], 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}

GDL::mat4SIMD::mat4SIMD(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10,
                        F32 v11, F32 v12, F32 v13, F32 v14, F32 v15)
    : mData({{{_mm_setr_ps(v0, v1, v2, v3)},
              {_mm_setr_ps(v4, v5, v6, v7)},
              {_mm_setr_ps(v8, v9, v10, v11)},
              {_mm_setr_ps(v12, v13, v14, v15)}}})
{
#ifndef NDEBUG
    if (!(is_aligned(&mData[0], 16) && is_aligned(&mData[1], 16) && is_aligned(&mData[2], 16) && is_aligned(&mData[3], 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}

GDL::mat4SIMD::mat4SIMD(__m128 col0, __m128 col1, __m128 col2, __m128 col3)
    : mData({{col0,col1,col2,col3}})
{
#ifndef NDEBUG
    if (!(is_aligned(&mData[0], 16) && is_aligned(&mData[1], 16) && is_aligned(&mData[2], 16) && is_aligned(&mData[3], 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}

GDL::mat4SIMD::mat4SIMD(const GDL::mat4SIMD& other)
    : mData(other.mData)
{
#ifndef NDEBUG
    if (!(is_aligned(&mData[0], 16) && is_aligned(&mData[1], 16) && is_aligned(&mData[2], 16) && is_aligned(&mData[3], 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}


GDL::mat4SIMD GDL::mat4SIMD::operator*(const GDL::mat4SIMD& other) const
{
#ifdef ENABLE_SSE4
    __attribute__((aligned(16))) __m128 Col0 =
            _mm_fmadd_ps(_mm_set1_ps(other.mData[0][0]), mData[0],
                         _mm_fmadd_ps(_mm_set1_ps(other.mData[0][1]), mData[1],
                                      _mm_fmadd_ps(_mm_set1_ps(other.mData[0][2]), mData[2],
                                                   _mm_mul_ps(_mm_set1_ps(other.mData[0][3]), mData[3]))));

    __attribute__((aligned(16))) __m128 Col1 =
            _mm_fmadd_ps(_mm_set1_ps(other.mData[1][0]), mData[0],
                         _mm_fmadd_ps(_mm_set1_ps(other.mData[1][1]), mData[1],
                                      _mm_fmadd_ps(_mm_set1_ps(other.mData[1][2]), mData[2],
                                                   _mm_mul_ps(_mm_set1_ps(other.mData[1][3]), mData[3]))));

    __attribute__((aligned(16))) __m128 Col2 =
            _mm_fmadd_ps(_mm_set1_ps(other.mData[2][0]), mData[0],
                         _mm_fmadd_ps(_mm_set1_ps(other.mData[2][1]), mData[1],
                                      _mm_fmadd_ps(_mm_set1_ps(other.mData[2][2]), mData[2],
                                                   _mm_mul_ps(_mm_set1_ps(other.mData[2][3]), mData[3]))));

    __attribute__((aligned(16))) __m128 Col3 =
            _mm_fmadd_ps(_mm_set1_ps(other.mData[3][0]), mData[0],
                         _mm_fmadd_ps(_mm_set1_ps(other.mData[3][1]), mData[1],
                                      _mm_fmadd_ps(_mm_set1_ps(other.mData[3][2]), mData[2],
                                                   _mm_mul_ps(_mm_set1_ps(other.mData[3][3]), mData[3]))));
#else
    __attribute__((aligned(16))) __m128 Col0 = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mData[0][0]), mData[0]), _mm_mul_ps(_mm_set1_ps(other.mData[0][1]), mData[1])),
            _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mData[0][2]), mData[2]), _mm_mul_ps(_mm_set1_ps(other.mData[0][3]), mData[3])));

    __attribute__((aligned(16))) __m128 Col1 = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mData[1][0]), mData[0]), _mm_mul_ps(_mm_set1_ps(other.mData[1][1]), mData[1])),
            _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mData[1][2]), mData[2]), _mm_mul_ps(_mm_set1_ps(other.mData[1][3]), mData[3])));

    __attribute__((aligned(16))) __m128 Col2 = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mData[2][0]), mData[0]), _mm_mul_ps(_mm_set1_ps(other.mData[2][1]), mData[1])),
            _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mData[2][2]), mData[2]), _mm_mul_ps(_mm_set1_ps(other.mData[2][3]), mData[3])));

    __attribute__((aligned(16))) __m128 Col3 = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mData[3][0]), mData[0]), _mm_mul_ps(_mm_set1_ps(other.mData[3][1]), mData[1])),
            _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mData[3][2]), mData[2]), _mm_mul_ps(_mm_set1_ps(other.mData[3][3]), mData[3])));

#endif
    return mat4SIMD(Col0, Col1, Col2, Col3);
}

GDL::mat4SIMD& GDL::mat4SIMD::operator+=(const mat4SIMD& other)
{
    mData[0] = _mm_add_ps(mData[0], other.mData[0]);
    mData[1] = _mm_add_ps(mData[1], other.mData[1]);
    mData[2] = _mm_add_ps(mData[2], other.mData[2]);
    mData[3] = _mm_add_ps(mData[3], other.mData[3]);
    return *this;
}

GDL::F32 GDL::mat4SIMD::operator()(const U32 row, const U32 col) const
{
    assert(row < 4 && "row - invalid value! [0..3]");
    switch (col)
    {
    case 0:
        return mData[0][row];
    case 1:
        return mData[1][row];
    case 2:
        return mData[2][row];
    case 3:
        return mData[3][row];
    default:
        throw std::out_of_range("col - invalid value! [0..3]");
    }
}

const std::array<GDL::F32, 16> GDL::mat4SIMD::Data() const
{
    std::array<F32, 16> data;
    assert(sizeof(mData) == sizeof(data));
    std::memcpy(&data, &mData, sizeof(data));
    return data;
}


#ifndef NDEBUG
namespace GDL
{
inline std::ostream& operator<<(std::ostream& os, const GDL::mat4SIMD& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat.mData[0][i] << " " << mat.mData[1][i] << " " << mat.mData[2][i] << " " << mat.mData[3][i] << " |"
           << std::endl;
    return os;
}
}
#endif
