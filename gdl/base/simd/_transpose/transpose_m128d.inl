#pragma once

#include "gdl/base/simd/_transpose/transpose_m128d.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/_transpose/transpose_utility.h"



namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x1(__m128d in, __m128d& out) noexcept
{
    intern::transpose::StaticAssertions<1, 1, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m128d>();
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

    intern::transpose::SetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m128d in_0, __m128d in_1, __m128d& out_0) noexcept
{
    intern::transpose::StaticAssertions<1, 2, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m128d>();

    if constexpr (_firstRowIn == 0)
        out_0 = _mm_unpacklo(in_0, in_1);
    else
        out_0 = _mm_unpackhi(in_0, in_1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m128d in_0, __m128d& out_0, __m128d& out_1) noexcept
{
    intern::transpose::StaticAssertions<2, 1, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m128d>();

    Transpose1x1<0, _firstRowOut, _overwriteUnused, _unusedSetZero>(in_0, out_0);
    Transpose1x1<1, _firstRowOut, _overwriteUnused, _unusedSetZero>(in_0, out_1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m128d in_0, __m128d in_1, __m128d& out_0, __m128d& out_1) noexcept
{
    intern::transpose::StaticAssertions<2, 2, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m128d>();

    out_0 = _mm_unpacklo(in_0, in_1);
    out_1 = _mm_unpackhi(in_0, in_1);
}



} // namespace GDL::simd
