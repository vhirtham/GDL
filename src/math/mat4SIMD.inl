#pragma once
#include "mat4SIMD.h"

#include "base/Exception.h"

#include <cassert>
#include <type_traits>

GDL::mat4SIMD::mat4SIMD()
    : mCol0(_mm_set1_ps(0.))
    , mCol1(_mm_set1_ps(0.))
    , mCol2(_mm_set1_ps(0.))
    , mCol3(_mm_set1_ps(0.))
{
#ifndef NDEBUG
    if (!(is_aligned(&mCol0, 16) && is_aligned(&mCol1, 16) && is_aligned(&mCol2, 16) && is_aligned(&mCol3, 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}

GDL::mat4SIMD::mat4SIMD(__m128 col0, __m128 col1, __m128 col2, __m128 col3)
    : mCol0(col0)
    , mCol1(col1)
    , mCol2(col2)
    , mCol3(col3)
{
#ifndef NDEBUG
    if (!(is_aligned(&mCol0, 16) && is_aligned(&mCol1, 16) && is_aligned(&mCol2, 16) && is_aligned(&mCol3, 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}

GDL::mat4SIMD::mat4SIMD(const GDL::mat4SIMD& other)
    : mCol0(other.mCol0)
    , mCol1(other.mCol1)
    , mCol2(other.mCol2)
    , mCol3(other.mCol3)
{
#ifndef NDEBUG
    if (!(is_aligned(&mCol0, 16) && is_aligned(&mCol1, 16) && is_aligned(&mCol2, 16) && is_aligned(&mCol3, 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}


GDL::mat4SIMD GDL::mat4SIMD::operator*(const GDL::mat4SIMD& other) const
{
    __attribute__((aligned(16))) __m128 Col0 =
            _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mCol0[0]), mCol0),
                                  _mm_mul_ps(_mm_set1_ps(other.mCol0[1]), mCol1)),
                       _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mCol0[2]), mCol2),
                                  _mm_mul_ps(_mm_set1_ps(other.mCol0[3]), mCol3)));

    __attribute__((aligned(16))) __m128 Col1 =
            _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mCol1[0]), mCol0),
                                  _mm_mul_ps(_mm_set1_ps(other.mCol1[1]), mCol1)),
                       _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mCol1[2]), mCol2),
                                  _mm_mul_ps(_mm_set1_ps(other.mCol1[3]), mCol3)));

    __attribute__((aligned(16))) __m128 Col2 =
            _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mCol2[0]), mCol0),
                                  _mm_mul_ps(_mm_set1_ps(other.mCol2[1]), mCol1)),
                       _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mCol2[2]), mCol2),
                                  _mm_mul_ps(_mm_set1_ps(other.mCol2[3]), mCol3)));

    __attribute__((aligned(16))) __m128 Col3 =
            _mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mCol3[0]), mCol0),
                                  _mm_mul_ps(_mm_set1_ps(other.mCol3[1]), mCol1)),
                       _mm_add_ps(_mm_mul_ps(_mm_set1_ps(other.mCol3[2]), mCol2),
                                  _mm_mul_ps(_mm_set1_ps(other.mCol3[3]), mCol3)));

    return mat4SIMD(Col0, Col1, Col2, Col3);
}

GDL::F32 GDL::mat4SIMD::operator()(const U32 row, const U32 col) const
{
    assert(row < 4 && "row - invalid value! [0..3]");
    switch (col)
    {
    case 0:
        return mCol0[row];
    case 1:
        return mCol1[row];
    case 2:
        return mCol2[row];
    case 3:
        return mCol3[row];
    default:
        throw std::out_of_range("col - invalid value! [0..3]");
    }
}


#ifndef NDEBUG
namespace GDL
{
std::ostream& operator<<(std::ostream& os, const GDL::mat4SIMD& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat.mCol0[i] << " " << mat.mCol1[i] << " " << mat.mCol2[i] << " " << mat.mCol3[i] << " |"
           << std::endl;
    return os;
}
}
#endif
