#pragma once

#include "gdl/base/simd/_transpose/transpose_m128.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/_transpose/transpose_utility.h"

namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x1(__m128 in, __m128& out)
{
    __m128 tout;

    if constexpr (_firstRowIn == _firstRowOut)
        tout = in;
    else if (_overwriteUnused && _unusedSetZero)
    {
        constexpr bool z0 = (_firstRowOut == 0) ? false : true;
        constexpr bool z1 = (_firstRowOut == 1) ? false : true;
        constexpr bool z2 = (_firstRowOut == 2) ? false : true;
        constexpr bool z3 = (_firstRowOut == 3) ? false : true;

        out = Insert<_firstRowIn, _firstRowOut, z0, z1, z2, z3>(in, in);
        return;
    }
    else
        tout = Broadcast<_firstRowIn>(in);

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m128 in0, __m128 in1, __m128& out0)
{
    __m128 tout;

    constexpr bool setZero = _overwriteUnused && _unusedSetZero;
    if constexpr (setZero && not(_firstRowOut && (_firstRowIn == 0 || _firstRowIn == 3)))
    {
        if constexpr (_firstRowOut % 2 == 0)
        {
            constexpr bool b0 = _firstRowIn > 1;
            constexpr bool b1 = !b0;

            if constexpr (_firstRowIn % 2 == 0)
                out0 = Insert<_firstRowIn, _firstRowIn + 1, b0, b0, b1, b1>(in1, in0);
            else
                out0 = Insert<_firstRowIn, _firstRowIn - 1, b0, b0, b1, b1>(in0, in1);

            constexpr bool perm_con = (_firstRowOut == 0) ? _firstRowIn > 1 : _firstRowIn < 2;
            if constexpr (perm_con)
                out0 = Permute<2, 3, 0, 1>(out0);
        }
        else
        {
            if constexpr (_firstRowIn == 1)
                out0 = Insert<1, 2, true, false, false, true>(in1, in0);
            else
                out0 = Insert<2, 1, true, false, false, true>(in0, in1);
        }
        return;
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {

            if constexpr (_firstRowIn == 0)
                tout = _mm_unpacklo(in0, in1);
            else if constexpr (_firstRowIn == 1)
                tout = Insert<1, 0>(in0, in1);
            else if constexpr (_firstRowIn == 2)
                tout = _mm_unpackhi(in0, in1);
            else
            {
                __m128 tmp1 = _mm_unpackhi(in0, in1);
                tout = _mm_movehl_ps(tmp1, tmp1);
            }
        }
        else if constexpr (_firstRowOut == 1)
        {
            tout = Shuffle<_firstRowIn, _firstRowIn, _firstRowIn, _firstRowIn>(in0, in1);
        }
        else
        {
            if constexpr (_firstRowIn == 0)
            {
                __m128 tmp1 = _mm_unpacklo(in0, in1);
                tout = _mm_movelh_ps(tmp1, tmp1);
            }
            else if constexpr (_firstRowIn == 1)
                tout = _mm_unpacklo(in0, in1);
            else if constexpr (_firstRowIn == 2)
                tout = Insert<2, 3>(in1, in0);
            else
                tout = _mm_unpackhi(in0, in1);
        }
    }


    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x3(__m128 in0, __m128 in1, __m128 in2, __m128& out0)
{
    [[maybe_unused]] constexpr bool setZero = _overwriteUnused && _unusedSetZero;

    __m128 tmp0;


    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m128 tmp1 = _mm_unpacklo(in0, in1);

            tmp0 = Insert<0, 2, false, false, false, true>(in2, tmp1);
        }
        else
        {
            __m128 tmp1 = Shuffle<0, 0, 0, 0>(in0, in1);

            tmp0 = Insert<0, 3, true, false, false, false>(in2, tmp1);
        }
    }

    else if constexpr (_firstRowIn == 1)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m128 tmp1 = Shuffle<1, 1, 1, 1>(in0, in2);

            if constexpr (setZero)
                tmp0 = Insert<1, 1, false, false, false, true>(in1, tmp1);
            else
                tmp0 = BlendIndex<1>(tmp1, in1);
        }
        else
        {
            __m128 tmp1 = _mm_unpacklo(in1, in2);

            if constexpr (setZero)
                tmp0 = Insert<1, 1, true, false, false, false>(in0, tmp1);
            else
                tmp0 = BlendIndex<1>(tmp1, in0);
        }
    }
    else if constexpr (_firstRowIn == 2)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m128 tmp1 = _mm_unpackhi(in0, in1);

            if constexpr (setZero)
                tmp0 = Insert<2, 2, false, false, false, true>(in2, tmp1);
            else
                tmp0 = BlendIndex<2>(tmp1, in2);
        }
        else
        {
            __m128 tmp1 = Shuffle<2, 2, 2, 2>(in0, in2);

            if constexpr (setZero)
                tmp0 = Insert<2, 2, true, false, false, false>(in1, tmp1);
            else
                tmp0 = BlendIndex<2>(tmp1, in1);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            __m128 tmp1 = Shuffle<3, 3, 3, 3>(in1, in2);

            tmp0 = Insert<3, 0, false, false, false, true>(in0, tmp1);
        }
        else
        {
            __m128 tmp1 = _mm_unpackhi(in1, in2);

            tmp0 = Insert<3, 1, true, false, false, false>(in0, tmp1);
        }
    }

    // Write to output registers
    if constexpr (_overwriteUnused)
        out0 = tmp0;
    else
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x4(__m128 in0, __m128 in1, __m128 in2, __m128 in3, __m128& out0)
{
    if constexpr (_firstRowIn == 0)
    {
        __m128 tmp0 = _mm_unpacklo(in0, in1);
        __m128 tmp1 = _mm_unpacklo(in2, in3);

        out0 = _mm_movelh(tmp0, tmp1);
    }
    else if constexpr (_firstRowIn == 1)
    {
        __m128 tmp0 = _mm_unpacklo(in0, in1);
        __m128 tmp1 = _mm_unpacklo(in2, in3);

        out0 = _mm_movehl(tmp1, tmp0);
    }
    else if constexpr (_firstRowIn == 2)
    {
        __m128 tmp0 = _mm_unpackhi(in0, in1);
        __m128 tmp1 = _mm_unpackhi(in2, in3);

        out0 = _mm_movelh(tmp0, tmp1);
    }
    else
    {
        __m128 tmp0 = _mm_unpackhi(in0, in1);
        __m128 tmp1 = _mm_unpackhi(in2, in3);

        out0 = _mm_movehl(tmp1, tmp0);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m128 in0, __m128& out0, __m128& out1)
{
    [[maybe_unused]] constexpr bool setZero = _overwriteUnused && _unusedSetZero;

    constexpr bool z0 = (_firstRowOut == 0) ? false : true;
    constexpr bool z1 = (_firstRowOut == 1) ? false : true;
    constexpr bool z2 = (_firstRowOut == 2) ? false : true;
    constexpr bool z3 = (_firstRowOut == 3) ? false : true;

    __m128 tmp0, tmp1;

    if constexpr (_firstRowIn == _firstRowOut)
    {
        if constexpr (setZero)
            tmp0 = BlendIndex<_firstRowOut>(_mm_setzero<__m128>(), in0);
        else
            tmp0 = in0;
        tmp1 = Insert<_firstRowIn + 1, _firstRowOut, z0, z1, z2, z3>(in0, in0);
    }
    else if constexpr (_firstRowIn + 1 == _firstRowOut)
    {
        if constexpr (setZero)
            tmp1 = BlendIndex<_firstRowOut>(_mm_setzero<__m128>(), in0);
        else
            tmp1 = in0;
        tmp0 = Insert<_firstRowIn, _firstRowOut, z0, z1, z2, z3>(in0, in0);
    }
    else
    {
        tmp0 = Insert<_firstRowIn, _firstRowOut, z0, z1, z2, z3>(in0, in0);
        tmp1 = Insert<_firstRowIn + 1, _firstRowOut, z0, z1, z2, z3>(in0, in0);
    }

    if constexpr (not _overwriteUnused)
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
    }
    else
    {
        out0 = tmp0;
        out1 = tmp1;
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m128 in0, __m128 in1, __m128& out0, __m128& out1)
{
    __m128 tmp0, tmp1;

    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = _mm_unpacklo(in0, in1);
            tmp1 = _mm_movehl_ps(tmp0, tmp0);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Shuffle<1, 0, 0, 1>(in0, in1);
            tmp1 = _mm_unpacklo(in1, tmp0);
        }
        else
        {
            tmp1 = _mm_unpacklo(in0, in1);
            tmp0 = _mm_movelh_ps(tmp1, tmp1);
        }
    }
    else if constexpr (_firstRowIn == 1)
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = Insert<1, 0, false, false, true, true>(in0, in1);
            tmp1 = _mm_unpackhi(in0, in1);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Shuffle<1, 1, 1, 1>(in0, in1);
            tmp1 = Shuffle<2, 2, 2, 2>(in0, in1);
        }
        else
        {
            tmp0 = _mm_unpacklo(in0, in1);
            tmp1 = Insert<2, 3, true, true, false, false>(in1, in0);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = _mm_unpackhi(in0, in1);
            tmp1 = _mm_movehl_ps(tmp0, tmp0);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp1 = Shuffle<2, 3, 3, 2>(in0, in1);
            tmp0 = _mm_unpackhi(tmp1, in0);
        }
        else
        {
            tmp1 = _mm_unpackhi(in0, in1);
            tmp0 = _mm_movelh_ps(tmp1, tmp1);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x3(__m128 in0, __m128 in1, __m128 in2, __m128& out0, __m128& out1)
{

    __m128 tmp0, tmp1;

    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m128 tmp2 = _mm_unpacklo(in0, in1);

            tmp0 = Shuffle<0, 1, 0, 1>(tmp2, in2);
            tmp1 = Shuffle<2, 3, 1, 0>(tmp2, in2);
        }
        else
        {
            __m128 tmp2 = _mm_unpacklo(in1, in2);

            tmp0 = Shuffle<1, 0, 0, 1>(in0, tmp2);
            tmp1 = BlendIndex<1>(tmp2, in0);
        }
    }
    else if constexpr (_firstRowIn == 1)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m128 tmp2 = Shuffle<1, 2, 1, 2>(in0, in1);

            tmp0 = Shuffle<0, 2, 1, 3>(tmp2, in2);
            tmp1 = Shuffle<1, 3, 2, 0>(tmp2, in2);
        }
        else
        {
            __m128 tmp2 = Shuffle<1, 2, 1, 2>(in1, in2);

            tmp0 = Shuffle<3, 1, 0, 2>(in0, tmp2);
            tmp1 = Shuffle<0, 2, 1, 3>(in0, tmp2);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            __m128 tmp2 = _mm_unpackhi(in0, in1);

            tmp0 = BlendIndex<2>(tmp2, in2);
            tmp1 = Shuffle<2, 3, 3, 2>(tmp2, in2);
        }
        else
        {
            __m128 tmp2 = _mm_unpackhi(in1, in2);

            tmp0 = Shuffle<3, 2, 0, 1>(in0, tmp2);
            tmp1 = Shuffle<2, 3, 2, 3>(in0, tmp2);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x4(__m128 in0, __m128 in1, __m128 in2, __m128 in3, __m128& out0, __m128& out1)
{
    if constexpr (_firstRowIn == 0)
    {
        __m128 tmp0 = _mm_unpacklo(in0, in1);
        __m128 tmp1 = _mm_unpacklo(in2, in3);

        out0 = _mm_movelh(tmp0, tmp1);
        out1 = _mm_movehl(tmp1, tmp0);
    }
    else if constexpr (_firstRowIn == 1)
    {
        __m128 tmp0 = Shuffle<1, 2, 1, 2>(in0, in1);
        __m128 tmp1 = Shuffle<1, 2, 1, 2>(in2, in3);

        out0 = Shuffle<0, 2, 0, 2>(tmp0, tmp1);
        out1 = Shuffle<1, 3, 1, 3>(tmp0, tmp1);
    }
    else
    {
        __m128 tmp0 = _mm_unpackhi(in0, in1);
        __m128 tmp1 = _mm_unpackhi(in2, in3);

        out0 = _mm_movelh(tmp0, tmp1);
        out1 = _mm_movehl(tmp1, tmp0);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x1(__m128 in0, __m128& out0, __m128& out1, __m128& out2)
{
    [[maybe_unused]] constexpr bool setZero = _overwriteUnused && _unusedSetZero;

    constexpr bool z0 = (_firstRowOut == 0) ? false : true;
    constexpr bool z1 = (_firstRowOut == 1) ? false : true;
    constexpr bool z2 = (_firstRowOut == 2) ? false : true;
    constexpr bool z3 = (_firstRowOut == 3) ? false : true;

    __m128 tmp0, tmp1, tmp2;

    if constexpr (_firstRowIn == _firstRowOut)
    {
        if constexpr (setZero)
            tmp0 = BlendIndex<_firstRowOut>(_mm_setzero<__m128>(), in0);
        else
            tmp0 = in0;
        tmp1 = Insert<_firstRowIn + 1, _firstRowOut, z0, z1, z2, z3>(in0, in0);
        tmp2 = Insert<_firstRowIn + 2, _firstRowOut, z0, z1, z2, z3>(in0, in0);
    }
    else if constexpr (_firstRowIn + 1 == _firstRowOut)
    {
        if constexpr (setZero)
            tmp1 = BlendIndex<_firstRowOut>(_mm_setzero<__m128>(), in0);
        else
            tmp1 = in0;
        tmp0 = Insert<_firstRowIn, _firstRowOut, z0, z1, z2, z3>(in0, in0);
        tmp2 = Insert<_firstRowIn + 2, _firstRowOut, z0, z1, z2, z3>(in0, in0);
    }
    else if constexpr (_firstRowIn + 2 == _firstRowOut)
    {
        if constexpr (setZero)
            tmp2 = BlendIndex<_firstRowOut>(_mm_setzero<__m128>(), in0);
        else
            tmp2 = in0;
        tmp0 = Insert<_firstRowIn, _firstRowOut, z0, z1, z2, z3>(in0, in0);
        tmp1 = Insert<_firstRowIn + 1, _firstRowOut, z0, z1, z2, z3>(in0, in0);
    }
    else
    {
        tmp0 = Insert<_firstRowIn, _firstRowOut, z0, z1, z2, z3>(in0, in0);
        tmp1 = Insert<_firstRowIn + 1, _firstRowOut, z0, z1, z2, z3>(in0, in0);
        tmp2 = Insert<_firstRowIn + 2, _firstRowOut, z0, z1, z2, z3>(in0, in0);
    }

    if constexpr (not _overwriteUnused)
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
        out2 = BlendIndex<_firstRowOut>(out2, tmp2);
    }
    else
    {
        out0 = tmp0;
        out1 = tmp1;
        out2 = tmp2;
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x2(__m128 in0, __m128 in1, __m128& out0, __m128& out1, __m128& out2)
{
    __m128 tmp0, tmp1, tmp2;

    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = _mm_unpacklo(in0, in1);
            tmp1 = _mm_movehl_ps(tmp0, tmp0);
            tmp2 = _mm_unpackhi(in0, in1);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Shuffle<1, 0, 0, 1>(in0, in1);
            tmp1 = _mm_unpacklo(in1, tmp0);
            tmp2 = Shuffle<3, 2, 2, 3>(in0, in1);
        }
        else
        {
            tmp1 = _mm_unpacklo(in0, in1);
            tmp0 = _mm_movelh_ps(tmp1, tmp1);
            tmp2 = Insert<2, 3, true, true, false, false>(in1, in0);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = Insert<1, 0, false, false, true, true>(in0, in1);
            tmp1 = _mm_unpackhi(in0, in1);
            tmp2 = _mm_movehl_ps(tmp1, tmp1);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Shuffle<1, 1, 1, 1>(in0, in1);
            tmp1 = Shuffle<2, 2, 2, 2>(in0, in1);
            tmp2 = Shuffle<3, 3, 3, 3>(in0, in1);
        }
        else
        {
            tmp0 = _mm_unpacklo(in0, in1);
            tmp1 = Insert<2, 3, true, true, false, false>(in1, in0);
            tmp2 = _mm_unpackhi(in0, in1);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x3(__m128 in0, __m128 in1, __m128 in2, __m128& out0, __m128& out1, __m128& out2)
{
    __m128 tmp0, tmp1, tmp2;

    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m128 tmp3 = Shuffle<0, 0, 0, 0>(in1, in2);
            __m128 tmp4 = Shuffle<1, 1, 1, 1>(in0, in2);
            __m128 tmp5 = _mm_unpackhi(in0, in1);

            tmp0 = BlendIndex<0>(tmp3, in0);
            tmp1 = BlendIndex<1>(tmp4, in1);
            tmp2 = BlendIndex<2>(tmp5, in2);
        }
        else
        {
            __m128 tmp3 = _mm_unpacklo(in1, in2);
            __m128 tmp4 = Shuffle<2, 2, 2, 2>(in0, in2);

            tmp0 = Shuffle<1, 0, 0, 1>(in0, tmp3);
            tmp1 = BlendIndex<1>(tmp3, in0);
            tmp2 = BlendIndex<2>(tmp4, in1);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            __m128 tmp3 = _mm_unpackhi(in0, in1);
            __m128 tmp4 = Shuffle<1, 1, 1, 1>(in0, in2);

            tmp2 = Shuffle<2, 3, 3, 2>(tmp3, in2);
            tmp1 = BlendIndex<2>(tmp3, in2);
            tmp0 = BlendIndex<1>(tmp4, in1);
        }
        else
        {
            __m128 tmp3 = _mm_unpacklo(in1, in2);
            __m128 tmp4 = Shuffle<2, 2, 2, 2>(in0, in2);
            __m128 tmp5 = Shuffle<3, 3, 3, 3>(in0, in1);

            tmp0 = BlendIndex<1>(tmp3, in0);
            tmp1 = BlendIndex<2>(tmp4, in1);
            tmp2 = BlendIndex<3>(tmp5, in2);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x4(__m128 in0, __m128 in1, __m128 in2, __m128 in3, __m128& out0, __m128& out1, __m128& out2)
{
    __m128 tmp0 = _mm_unpacklo(in0, in1);
    __m128 tmp1 = _mm_unpackhi(in0, in1);
    __m128 tmp2 = _mm_unpacklo(in2, in3);
    __m128 tmp3 = _mm_unpackhi(in2, in3);

    if constexpr (_firstRowIn == 0)
    {
        out0 = _mm_movelh(tmp0, tmp2);
        out1 = _mm_movehl(tmp2, tmp0);
        out2 = _mm_movelh(tmp1, tmp3);
    }
    else
    {
        out0 = _mm_movehl(tmp2, tmp0);
        out1 = _mm_movelh(tmp1, tmp3);
        out2 = _mm_movehl(tmp3, tmp1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x1(__m128 in0, __m128& out0, __m128& out1, __m128& out2, __m128& out3)
{
    [[maybe_unused]] constexpr bool setZero = _overwriteUnused && _unusedSetZero;

    constexpr bool z0 = (_firstRowOut == 0) ? false : true;
    constexpr bool z1 = (_firstRowOut == 1) ? false : true;
    constexpr bool z2 = (_firstRowOut == 2) ? false : true;
    constexpr bool z3 = (_firstRowOut == 3) ? false : true;

    __m128 tmp0, tmp1, tmp2, tmp3;

    if constexpr (_firstRowOut == 0)
    {
        if constexpr (setZero)
            tmp0 = BlendIndex<0>(_mm_setzero<__m128>(), in0);
        else
            tmp0 = in0;
        tmp1 = Insert<1, 0, z0, z1, z2, z3>(in0, in0);
        tmp2 = Insert<2, 0, z0, z1, z2, z3>(in0, in0);
        tmp3 = Insert<3, 0, z0, z1, z2, z3>(in0, in0);
    }
    else if constexpr (_firstRowOut == 1)
    {
        if constexpr (setZero)
            tmp1 = BlendIndex<1>(_mm_setzero<__m128>(), in0);
        else
            tmp1 = in0;
        tmp0 = Insert<0, 1, z0, z1, z2, z3>(in0, in0);
        tmp2 = Insert<2, 1, z0, z1, z2, z3>(in0, in0);
        tmp3 = Insert<3, 1, z0, z1, z2, z3>(in0, in0);
    }
    else if constexpr (_firstRowOut == 2)
    {
        if constexpr (setZero)
            tmp2 = BlendIndex<2>(_mm_setzero<__m128>(), in0);
        else
            tmp2 = in0;

        tmp0 = Insert<0, 2, z0, z1, z2, z3>(in0, in0);
        tmp1 = Insert<1, 2, z0, z1, z2, z3>(in0, in0);
        tmp3 = Insert<3, 2, z0, z1, z2, z3>(in0, in0);
    }
    else
    {
        if constexpr (setZero)
            tmp3 = BlendIndex<3>(_mm_setzero<__m128>(), in0);
        else
            tmp3 = in0;

        tmp0 = Insert<0, 3, z0, z1, z2, z3>(in0, in0);
        tmp1 = Insert<1, 3, z0, z1, z2, z3>(in0, in0);
        tmp2 = Insert<2, 3, z0, z1, z2, z3>(in0, in0);
    }

    if constexpr (not _overwriteUnused)
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
        out2 = BlendIndex<_firstRowOut>(out2, tmp2);
        out3 = BlendIndex<_firstRowOut>(out3, tmp3);
    }
    else
    {
        out0 = tmp0;
        out1 = tmp1;
        out2 = tmp2;
        out3 = tmp3;
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x2(__m128 in0, __m128 in1, __m128& out0, __m128& out1, __m128& out2, __m128& out3)
{
    constexpr bool setZero = _overwriteUnused && _unusedSetZero;

    __m128 tmp0, tmp1, tmp2, tmp3;


    if constexpr (_firstRowOut == 0)
    {
        tmp0 = Insert<0, 1, false, false, true, true>(in1, in0);
        tmp1 = Insert<1, 0, false, false, true, true>(in0, in1);
        tmp2 = _mm_unpackhi(in0, in1);
        tmp3 = Permute<2, 3, 0, 1>(tmp2);

        if constexpr (setZero)
        {
            const __m128 zero = _mm_setzero<__m128>();
            tmp2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp2);
            tmp3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp3);
        }
    }
    else if constexpr (_firstRowOut == 1)
    {
        tmp0 = Shuffle<0, 0, 0, 0>(in0, in1);
        tmp1 = Insert<1, 2, true, false, false, true>(in1, in0);
        tmp2 = Insert<2, 1, true, false, false, true>(in0, in1);
        tmp3 = Shuffle<3, 3, 3, 3>(in0, in1);

        if constexpr (setZero)
        {
            const __m128 zero = _mm_setzero<__m128>();
            tmp0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            tmp3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp3);
        }
    }
    else
    {
        tmp3 = Insert<3, 2, true, true, false, false>(in0, in1);
        tmp2 = Insert<2, 3, true, true, false, false>(in1, in0);
        tmp1 = _mm_unpacklo(in0, in1);
        tmp0 = Permute<2, 3, 0, 1>(tmp1);

        if constexpr (setZero)
        {
            const __m128 zero = _mm_setzero<__m128>();
            tmp0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            tmp1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x3(__m128 in0, __m128 in1, __m128 in2, __m128& out0, __m128& out1, __m128& out2, __m128& out3)
{
    __m128 tmp0, tmp1, tmp2, tmp3;


    if constexpr (_firstRowOut == 0)
    {
        __m128 tmp4 = _mm_unpacklo(in0, in1);
        __m128 tmp5 = _mm_unpackhi(in0, in1);

        tmp0 = _mm_movelh(tmp4, in2);
        tmp1 = Shuffle<2, 3, 1, 0>(tmp4, in2);
        tmp2 = BlendIndex<2>(tmp5, in2);
        tmp3 = Shuffle<2, 3, 3, 2>(tmp5, in2);
    }
    else
    {
        __m128 tmp4 = _mm_unpacklo(in1, in2);
        __m128 tmp5 = _mm_unpackhi(in1, in2);

        tmp0 = Shuffle<1, 0, 0, 1>(in0, tmp4);
        tmp1 = BlendIndex<1>(tmp4, in0);
        tmp2 = Shuffle<3, 2, 0, 1>(in0, tmp5);
        tmp3 = _mm_movehl(tmp5, in0);
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x4(__m128 in0, __m128 in1, __m128 in2, __m128 in3, __m128& out0, __m128& out1, __m128& out2,
                         __m128& out3)
{
    __m128 tmp0 = _mm_unpacklo(in0, in1);
    __m128 tmp1 = _mm_unpackhi(in0, in1);
    __m128 tmp2 = _mm_unpacklo(in2, in3);
    __m128 tmp3 = _mm_unpackhi(in2, in3);

    out0 = _mm_movelh_ps(tmp0, tmp2);
    out1 = _mm_movehl_ps(tmp2, tmp0);
    out2 = _mm_movelh_ps(tmp1, tmp3);
    out3 = _mm_movehl_ps(tmp3, tmp1);
}



} // namespace GDL::simd
