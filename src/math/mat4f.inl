#include "mat4f.h"

#include "base/Exception.h"
#include "GDLTypedefs.h"

#include <cassert>
#include <type_traits>

GDL::mat4f::mat4f()
    : mCol0(_mm_set1_ps(0.))
    , mCol1(_mm_set1_ps(0.))
    , mCol2(_mm_set1_ps(0.))
    , mCol3(_mm_set1_ps(0.))
{
#ifndef NDEBUG
    if (!(is_aligned(&mCol0, 16) && is_aligned(&mCol1, 16) && is_aligned(&mCol2, 16) && is_aligned(&mCol3, 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4f are not 16 byte aligned");
#endif
}

GDL::mat4f::mat4f(__m128 col0, __m128 col1, __m128 col2, __m128 col3)
    : mCol0(col0)
    , mCol1(col1)
    , mCol2(col2)
    , mCol3(col3)
{
    if (!(is_aligned(&mCol0, 16) && is_aligned(&mCol1, 16) && is_aligned(&mCol2, 16) && is_aligned(&mCol3, 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4f are not 16 byte aligned");
}

GDL::mat4f GDL::mat4f::operator*(const GDL::mat4f &pOther_cr) const
{
    __attribute__((aligned (16))) __m128 Col0     = _mm_add_ps(
                                                    _mm_add_ps(
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol0[0]), mCol0),
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol0[1]), mCol1)),
                                                    _mm_add_ps(
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol0[2]), mCol2),
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol0[3]), mCol3)));

    __attribute__((aligned (16))) __m128 Col1     = _mm_add_ps(
                                                    _mm_add_ps(
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol1[0]), mCol0),
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol1[1]), mCol1)),
                                                    _mm_add_ps(
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol1[2]), mCol2),
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol1[3]), mCol3)));

    __attribute__((aligned (16))) __m128 Col2     = _mm_add_ps(
                                                    _mm_add_ps(
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol2[0]), mCol0),
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol2[1]), mCol1)),
                                                    _mm_add_ps(
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol2[2]), mCol2),
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol2[3]), mCol3)));

    __attribute__((aligned (16)))__m128 Col3     =  _mm_add_ps(
                                                    _mm_add_ps(
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol3[0]), mCol0),
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol3[1]), mCol1)),
                                                    _mm_add_ps(
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol3[2]), mCol2),
                                                        _mm_mul_ps(_mm_set1_ps(pOther_cr.mCol3[3]), mCol3)));

    return mat4f(Col0,Col1,Col2,Col3);
}

#ifndef NDEBUG
namespace GDL
{
std::ostream& operator<<(std::ostream& os, const GDL::mat4f& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat.mCol0[i] << " " << mat.mCol1[i] << " " << mat.mCol2[i] << " " << mat.mCol3[i] << " |"
           << std::endl;
    return os;
}
}
#endif
