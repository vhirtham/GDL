#pragma once

#include "gdl/base/simd/_transpose/transpose_m128d.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/_transpose/transpose_utility.h"



namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x1(__m128d in, __m128d& out)
{
    __m128d tout;

    if constexpr (_firstRowIn == _firstRowOut)
        tout = in;
    else
    {
        if constexpr (_overwriteUnused == true && _unusedSetZero == true)
        {
            __m128d zero = _mm_setzero<__m128d>();
            if constexpr (_firstRowIn == 0)
                out = _mm_unpacklo_pd(zero, in);
            else
                out = _mm_unpackhi(in, zero);
            return;
        }
        else
        {
            if constexpr (_firstRowIn == 0)
                tout = _mm_moveldup(in);
            else
                tout = _mm_unpackhi(in, in);
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m128d in0, __m128d in1, __m128d& out0)
{
    if constexpr (_firstRowIn == 0)
        out0 = _mm_unpacklo(in0, in1);
    else
        out0 = _mm_unpackhi(in0, in1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m128d in0, __m128d& out0, __m128d& out1)
{

    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
        {
            __m128d zero = _mm_setzero<__m128d>();
            if constexpr (_firstRowOut == 0)
            {
                out0 = BlendIndex<_firstRowOut>(zero, in0);
                out1 = _mm_unpackhi(in0, zero);
            }
            else
            {
                out0 = _mm_unpacklo(zero, in0);
                out1 = BlendIndex<_firstRowOut>(zero, in0);
            }
        }
        else
        {
            if constexpr (_firstRowOut == 0)
            {
                out0 = in0;
                out1 = _mm_unpackhi(in0, in0);
            }
            else
            {
                out0 = _mm_unpacklo(in0, in0);
                out1 = in0;
            }
        }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            out0 = BlendIndex<_firstRowOut>(out0, in0);
            out1 = _mm_unpackhi(in0, out1);
        }
        else
        {
            out0 = _mm_unpacklo(out0, in0);
            out1 = BlendIndex<_firstRowOut>(out1, in0);
        }
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m128d in0, __m128d in1, __m128d& out0, __m128d& out1)
{
    out0 = _mm_unpacklo(in0, in1);
    out1 = _mm_unpackhi(in0, in1);
}



} // namespace GDL::simd
