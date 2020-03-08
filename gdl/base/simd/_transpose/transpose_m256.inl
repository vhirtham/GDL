#pragma once

#include "gdl/base/simd/_transpose/transpose_m256.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/_transpose/transpose_utility.h"

#include <array>

#ifdef __AVX2__

namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x1(__m256 in, __m256& out)
{
    static_assert(not(_overwriteUnused == false && _unusedSetZero == true), "Option _unusedSetZero has no effect.");

    __m256 tmp;

    if constexpr (_firstRowIn == _firstRowOut)
    {
        tmp = in;
    }
    else
    {
        constexpr U32 numLaneVals = numValuesPerLane<__m256>;
        constexpr U32 laneIn = _firstRowIn / numLaneVals;
        constexpr U32 laneOut = _firstRowOut / numLaneVals;
        constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
        constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;



        if constexpr (laneIn == laneOut)
        {
            if constexpr (laneOffsetIn == 0)
            {
                if constexpr (laneOffsetOut == 1)
                    tmp = _mm_moveldup(in);
                else
                    tmp = Broadcast<0>(in);
            }
            else if constexpr (laneOffsetIn == 1)
            {
                if constexpr (laneOffsetOut == 0)
                    tmp = _mm_movehdup(in);
                else
                    tmp = _mm_unpacklo(in, in);
            }
            else if constexpr (laneOffsetIn == 2)
            {
                if constexpr (laneOffsetOut == 3)
                    tmp = _mm_moveldup(in);
                else
                    tmp = _mm_unpackhi(in, in);
            }
            else
            {
                if constexpr (laneOffsetOut == 2)
                    tmp = _mm_movehdup(in);
                else
                    tmp = Broadcast<3>(in);
            }
        }
        else
        {
            __m256 perm = Permute2F128<1, 0>(in);
            if constexpr (laneOffsetIn == laneOffsetOut)
                tmp = perm;
            else if constexpr (laneOffsetIn == 0)
            {
                if constexpr (laneOffsetOut == 1)
                    tmp = _mm_moveldup(perm);
                else
                    tmp = Broadcast<0>(perm);
            }
            else if constexpr (laneOffsetIn == 1)
            {
                if constexpr (laneOffsetOut == 0)
                    tmp = _mm_movehdup(perm);
                else
                    tmp = _mm_unpacklo(perm, perm);
            }
            else if constexpr (laneOffsetIn == 2)
            {
                if constexpr (laneOffsetOut == 3)
                    tmp = _mm_moveldup(perm);
                else
                    tmp = _mm_unpackhi(perm, perm);
            }
            else
            {
                if constexpr (laneOffsetOut == 2)
                    tmp = _mm_movehdup(perm);
                else
                    tmp = Broadcast<3>(perm);
            }
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out, tmp);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m256 in0, __m256 in1, __m256& out0)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0;


    if constexpr (laneOffsetOut == 1)
    {
        tmp0 = Shuffle<laneOffsetIn, laneOffsetIn, laneOffsetIn, laneOffsetIn>(in0, in1);
        if constexpr (laneIn != laneOut)
            tmp0 = Permute2F128<1, 0>(tmp0);
    }

    else if constexpr (laneOffsetOut == 3)
    {
        if constexpr (laneIn == 0)
        {
            __m256 perm = Permute2F128<1, 0>(in1);
            tmp0 = Shuffle<laneOffsetIn, laneOffsetIn, laneOffsetIn, laneOffsetIn>(perm, in0);
        }
        else
        {
            __m256 perm = Permute2F128<1, 0>(in0);
            tmp0 = Shuffle<laneOffsetIn, laneOffsetIn, laneOffsetIn, laneOffsetIn>(in1, perm);
        }
    }

    else
    {
        if constexpr (laneOffsetIn == 0)
        {
            if constexpr (laneOffsetOut == 0)
            {
                tmp0 = _mm_unpacklo(in0, in1);
            }
            else
            {
                __m256 tmp1 = _mm_unpacklo(in0, in1);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
        }
        else if constexpr (laneOffsetIn == 1)
        {
            if constexpr (laneOffsetOut == 0)
            {
                __m256 tmp1 = _mm_movehdup(in0);
                tmp0 = Blend<0, 1, 0, 0, 0, 1, 0, 0>(tmp1, in1);
            }
            else
            {
                tmp0 = _mm_unpacklo(in0, in1);
            }
        }
        else if constexpr (laneOffsetIn == 2)
        {
            if constexpr (laneOffsetOut == 0)
            {
                tmp0 = _mm_unpackhi(in0, in1);
            }
            else
            {
                __m256 tmp1 = _mm_unpackhi(in0, in1);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
        }
        else
        {
            if constexpr (laneOffsetOut == 0)
            {
                __m256 tmp1 = _mm_unpackhi(in0, in1);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
            else
            {
                tmp0 = _mm_unpackhi(in0, in1);
            }
        }

        if constexpr (laneIn != laneOut)
            tmp0 = Permute2F128<1, 0>(tmp0);
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;


    __m256 tmp0;

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp1 = _mm_unpacklo(in0, in2);

            tmp0 = _mm_unpacklo(tmp1, in1);

            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp1 = _mm_unpacklo(in1, in2);

            tmp0 = Shuffle<1, 0, 0, 1>(in0, tmp1);

            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp1 = _mm_unpacklo(in0, in1);
            __m256 tmp2 = Permute<0, 1, 0, 1>(tmp1);

            tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp2, in2);
        }
        else
        {
            __m256 tmp1 = Broadcast<0>(in0);
            __m256 tmp2 = _mm_unpacklo(in1, in2);

            tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp1, tmp2);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp1 = _mm_unpacklo(in0, in1);

            tmp0 = Shuffle<2, 3, 1, 0>(tmp1, in2);

            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp1 = _mm_unpacklo(in1, in2);

            tmp0 = Blend<0, 1, 0, 0, 0, 1, 0, 0>(tmp1, in0);

            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp1 = _mm_unpacklo(in0, in1);
            __m256 tmp2 = Broadcast<1>(in2);

            tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp1, tmp2);
        }
        else
        {
            __m256 tmp1 = _mm_unpacklo(in1, in2);
            __m256 tmp2 = Shuffle<2, 3, 0, 1>(tmp1, in0);

            tmp0 = Permute2F128<laneIn, laneIn>(tmp2);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp1 = _mm_unpackhi(in0, in1);

            tmp0 = Blend<0, 0, 1, 0, 0, 0, 1, 0>(tmp1, in2);

            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp1 = Shuffle<2, 2, 2, 2>(in0, in2);

            tmp0 = Blend<0, 0, 1, 0, 0, 0, 1, 0>(tmp1, in1);

            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp1 = _mm_unpackhi(in0, in1);
            __m256 tmp2 = Shuffle<2, 3, 0, 1>(in2, tmp1);

            tmp0 = Permute2F128<laneIn, laneIn>(tmp2);
        }
        else
        {
            __m256 tmp1 = Broadcast<2>(in0);
            __m256 tmp2 = _mm_unpackhi(in1, in2);

            tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp1, tmp2);
        }
    }
    else
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp1 = _mm_unpackhi(in0, in1);

            tmp0 = Shuffle<2, 3, 3, 2>(tmp1, in2);

            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp1 = _mm_unpackhi(in0, in2);

            tmp0 = _mm_unpackhi(in1, tmp1);

            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp1 = _mm_unpackhi(in0, in1);
            __m256 tmp2 = Shuffle<3, 2, 2, 3>(in2, tmp1);

            tmp0 = Permute2F128<laneIn, laneIn>(tmp2);
        }
        else
        {
            __m256 tmp1 = _mm_unpackhi(in1, in2);
            __m256 tmp2 = Shuffle<2, 3, 2, 3>(tmp1, in0);

            tmp0 = Permute2F128<laneIn, laneIn>(tmp2);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0)
{

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;


    __m256 tmp0;

    if constexpr (laneOffsetIn == 0)
    {
        __m256 tmp1, tmp2;
        if constexpr (laneOffsetOut == 0)
        {
            tmp1 = _mm_unpacklo(in0, in1);
            tmp2 = _mm_unpacklo(in2, in3);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp1 = _mm_unpacklo(in3, in0);
            tmp2 = _mm_unpacklo(in1, in2);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp1 = _mm_unpacklo(in2, in3);
            tmp2 = _mm_unpacklo(in0, in1);
        }
        else
        {
            tmp1 = _mm_unpacklo(in1, in2);
            tmp2 = _mm_unpacklo(in3, in0);
        }

        tmp0 = _mm_movelh(tmp1, tmp2);
    }
    else if constexpr (laneOffsetIn == 1)
    {
        __m256 tmp1, tmp2;
        if constexpr (laneOffsetOut == 0)
        {
            tmp1 = _mm_unpacklo(in0, in1);
            tmp2 = _mm_unpacklo(in2, in3);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp1 = _mm_unpacklo(in3, in0);
            tmp2 = _mm_unpacklo(in1, in2);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp1 = _mm_unpacklo(in2, in3);
            tmp2 = _mm_unpacklo(in0, in1);
        }
        else
        {
            tmp1 = _mm_unpacklo(in1, in2);
            tmp2 = _mm_unpacklo(in3, in0);
        }

        tmp0 = _mm_movehl(tmp2, tmp1);
    }
    else if constexpr (laneOffsetIn == 2)
    {
        __m256 tmp1, tmp2;
        if constexpr (laneOffsetOut == 0)
        {
            tmp1 = _mm_unpackhi(in0, in1);
            tmp2 = _mm_unpackhi(in2, in3);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp1 = _mm_unpackhi(in3, in0);
            tmp2 = _mm_unpackhi(in1, in2);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp1 = _mm_unpackhi(in2, in3);
            tmp2 = _mm_unpackhi(in0, in1);
        }
        else
        {
            tmp1 = _mm_unpackhi(in1, in2);
            tmp2 = _mm_unpackhi(in3, in0);
        }

        tmp0 = _mm_movelh(tmp1, tmp2);
    }
    else
    {
        __m256 tmp1, tmp2;
        if constexpr (laneOffsetOut == 0)
        {
            tmp1 = _mm_unpackhi(in0, in1);
            tmp2 = _mm_unpackhi(in2, in3);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp1 = _mm_unpackhi(in3, in0);
            tmp2 = _mm_unpackhi(in1, in2);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp1 = _mm_unpackhi(in2, in3);
            tmp2 = _mm_unpackhi(in0, in1);
        }
        else
        {
            tmp1 = _mm_unpackhi(in1, in2);
            tmp2 = _mm_unpackhi(in3, in0);
        }

        tmp0 = _mm_movehl(tmp2, tmp1);
    }

    if constexpr (laneIn != laneOut || laneOffsetOut > 0)
        tmp0 = Permute2F128<laneIn, laneIn>(tmp0);

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0)
{

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;


    __m256 tmp0;


    __m256 tmp1, tmp2;
    if constexpr (laneOffsetOut == 0)
    {
        Transpose1x4<laneOffsetIn, 0>(in0, in1, in2, in3, tmp1);
        Transpose1x1<laneOffsetIn, 0>(in4, tmp2);
    }
    else if constexpr (laneOffsetOut == 1)
    {
        Transpose1x3<laneOffsetIn, 1>(in0, in1, in2, tmp1);
        Transpose1x2<laneOffsetIn, 0>(in3, in4, tmp2);
    }
    else if constexpr (laneOffsetOut == 2)
    {
        Transpose1x2<laneOffsetIn, 2>(in0, in1, tmp1);
        Transpose1x3<laneOffsetIn, 0>(in2, in3, in4, tmp2);
    }
    else
    {
        Transpose1x1<laneOffsetIn, 3>(in0, tmp1);
        Transpose1x4<laneOffsetIn, 0>(in1, in2, in3, in4, tmp2);
    }
    tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp1, tmp2);


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0)
{

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;


    __m256 tmp0;

    __m256 tmp1, tmp2;
    if constexpr (laneOffsetOut == 0)
    {
        Transpose1x4<laneOffsetIn, 0>(in0, in1, in2, in3, tmp1);
        Transpose1x2<laneOffsetIn, 0>(in4, in5, tmp2);
    }
    else if constexpr (laneOffsetOut == 1)
    {
        Transpose1x3<laneOffsetIn, 1>(in0, in1, in2, tmp1);
        Transpose1x3<laneOffsetIn, 0>(in3, in4, in5, tmp2);
    }
    else
    {
        Transpose1x2<laneOffsetIn, 2>(in0, in1, tmp1);
        Transpose1x4<laneOffsetIn, 0>(in2, in3, in4, in5, tmp2);
    }
    tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp1, tmp2);


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0)
{

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;


    __m256 tmp0;

    __m256 tmp1, tmp2;
    if constexpr (laneOffsetOut == 0)
    {
        Transpose1x4<laneOffsetIn, 0>(in0, in1, in2, in3, tmp1);
        Transpose1x3<laneOffsetIn, 0>(in4, in5, in6, tmp2);
    }
    else
    {
        Transpose1x3<laneOffsetIn, 1>(in0, in1, in2, tmp1);
        Transpose1x4<laneOffsetIn, 0>(in3, in4, in5, in6, tmp2);
    }

    tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp1, tmp2);


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0)
{

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;


    __m256 tmp0;

    __m256 tmp1, tmp2;

    Transpose1x4<laneOffsetIn, 0>(in0, in1, in2, in3, tmp1);
    Transpose1x4<laneOffsetIn, 0>(in4, in5, in6, in7, tmp2);


    tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp1, tmp2);


    // Write to output registers
    out0 = tmp0;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m256 in0, __m256& out0, __m256& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1;

    if constexpr (laneOffsetIn == 0)
    {
        __m256 tmp2;
        if constexpr (laneIn == laneOut)
            tmp2 = in0;
        else
            tmp2 = Permute2F128<1, 0>(in0);

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = tmp2;
            tmp1 = _mm_movehdup(tmp2);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = _mm_moveldup(tmp2);
            tmp1 = tmp2;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp0 = Permute<2, 3, 0, 1>(tmp2);
            tmp1 = _mm_unpacklo(tmp2, tmp2);
        }
        else
        {
            tmp1 = _mm_unpacklo(tmp2, tmp2);
            tmp0 = _mm_unpacklo(tmp1, tmp1);
        }
    }

    else if constexpr (laneOffsetIn == 1)
    {
        __m256 tmp2;
        if constexpr (laneIn == laneOut)
            tmp2 = in0;
        else
            tmp2 = Permute2F128<1, 0>(in0);

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = _mm_movehdup(tmp2);
            tmp1 = _mm_unpackhi(tmp2, tmp2);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = tmp2;
            tmp1 = _mm_unpackhi(tmp2, tmp2);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp0 = _mm_unpacklo(tmp2, tmp2);
            tmp1 = tmp2;
        }
        else
        {
            tmp0 = _mm_unpacklo(tmp2, tmp2);
            tmp1 = _mm_moveldup(tmp2);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        __m256 tmp2;
        if constexpr (laneIn == laneOut)
            tmp2 = in0;
        else
            tmp2 = Permute2F128<1, 0>(in0);

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = _mm_unpackhi(tmp2, tmp2);
            tmp1 = _mm_unpackhi(tmp0, tmp0);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = _mm_unpackhi(tmp2, tmp2);
            tmp1 = Permute<2, 3, 0, 1>(tmp2);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp0 = tmp2;
            tmp1 = _mm_movehdup(tmp2);
        }
        else
        {
            tmp0 = _mm_moveldup(tmp2);
            tmp1 = tmp2;
        }
    }
    else
    {
        __m256 tmp2 = Permute2F128<1, 0>(in0);

        if constexpr (_firstRowOut == 0)
        {
            tmp0 = Broadcast<3>(in0);
            tmp1 = tmp2;
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Permute<2, 3, 0, 1>(in0);
            tmp1 = _mm_moveldup(tmp2);
        }
        else if constexpr (_firstRowOut == 2)
        {
            tmp0 = _mm_movehdup(in0);
            tmp1 = Permute<2, 3, 0, 1>(tmp2);
        }
        else if constexpr (_firstRowOut == 3)
        {
            tmp0 = in0;
            tmp1 = Broadcast<0>(tmp2);
        }
        else if constexpr (_firstRowOut == 4)
        {
            tmp0 = Broadcast<3>(tmp2);
            tmp1 = in0;
        }
        else if constexpr (_firstRowOut == 5)
        {
            tmp0 = Permute<2, 3, 0, 1>(tmp2);
            tmp1 = _mm_moveldup(in0);
        }
        else if constexpr (_firstRowOut == 6)
        {
            tmp0 = _mm_movehdup(tmp2);
            tmp1 = Permute<2, 3, 0, 1>(in0);
        }
        else
        {
            tmp0 = tmp2;
            tmp1 = Broadcast<0>(in0);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1;
    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneIn == laneOut)
        {
            if constexpr (laneOffsetOut == 0)
            {
                tmp0 = _mm_unpacklo(in0, in1);
                tmp1 = Permute<2, 3, 0, 1>(tmp0);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                tmp0 = Shuffle<1, 0, 0, 1>(in0, in1);
                tmp1 = _mm_unpacklo(in1, tmp0);
            }
            else if constexpr (laneOffsetOut == 2)
            {
                tmp1 = _mm_unpacklo(in0, in1);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
            else
            {
                __m256 perm = Permute2F128<1, 0>(in1);
                tmp0 = Shuffle<0, 0, 0, 0>(perm, in0);
                tmp1 = Shuffle<1, 1, 1, 1>(perm, in0);
            }
        }
        else
        {
            if constexpr (laneOffsetOut == 0)
            {
                __m256 tmp2 = _mm_unpacklo(in0, in1);
                tmp0 = Permute2F128<1, 0>(tmp2);
                tmp1 = Permute<2, 3, 0, 1>(tmp0);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                __m256 tmp2 = Shuffle<1, 0, 0, 1>(in0, in1);
                tmp0 = Permute2F128<1, 0>(tmp2);
                tmp1 = Permute<0, 0, 3, 3>(tmp0);
            }
            else if constexpr (laneOffsetOut == 2)
            {
                __m256 tmp2 = _mm_unpacklo(in0, in1);
                tmp1 = Permute2F128<1, 0>(tmp2);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
            else
            {
                __m256 perm = Permute2F128<1, 0>(in0);
                tmp0 = Shuffle<0, 0, 0, 0>(in1, perm);
                tmp1 = Shuffle<1, 1, 1, 1>(in1, perm);
            }
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneIn == laneOut)
        {
            if constexpr (laneOffsetOut == 0)
            {
                __m256 tmp2 = _mm_unpacklo(in0, in1);
                tmp0 = Permute<2, 3, 0, 1>(tmp2);
                tmp1 = _mm_unpackhi(in0, in1);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                tmp0 = Shuffle<1, 1, 1, 1>(in0, in1);
                tmp1 = Shuffle<2, 2, 2, 2>(in0, in1);
            }
            else if constexpr (laneOffsetOut == 2)
            {
                __m256 tmp2 = _mm_unpackhi(in0, in1);
                tmp0 = _mm_unpacklo(in0, in1);
                tmp1 = Permute<2, 3, 0, 1>(tmp2);
            }
            else
            {
                __m256 perm = Permute2F128<1, 0>(in1);
                tmp0 = Shuffle<1, 1, 1, 1>(perm, in0);
                tmp1 = Shuffle<2, 2, 2, 2>(perm, in0);
            }
        }
        else
        {
            if constexpr (laneOffsetOut == 0)
            {
                __m256 tmp2 = Shuffle<1, 2, 1, 2>(in0, in1);
                tmp0 = Permute8F32<4, 6, 5, 7, 0, 2, 1, 3>(tmp2);
                tmp1 = Permute<2, 3, 0, 1>(tmp0);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                __m256 tmp2 = Shuffle<2, 1, 1, 2>(in0, in1);
                tmp0 = Permute2F128<1, 0>(tmp2);
                tmp1 = Permute<0, 0, 3, 3>(tmp0);
            }
            else if constexpr (laneOffsetOut == 2)
            {
                __m256 tmp2 = Shuffle<1, 2, 1, 2>(in0, in1);
                tmp1 = Permute8F32<4, 6, 5, 7, 0, 2, 1, 3>(tmp2);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
            else
            {
                __m256 perm = Permute2F128<1, 0>(in0);
                tmp0 = Shuffle<1, 1, 1, 1>(in1, perm);
                tmp1 = Shuffle<2, 2, 2, 2>(in1, perm);
            }
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (laneIn == laneOut)
        {
            if constexpr (laneOffsetOut == 0)
            {
                tmp0 = _mm_unpackhi(in0, in1);
                tmp1 = Permute<2, 3, 0, 1>(tmp0);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                tmp1 = Shuffle<2, 3, 3, 2>(in0, in1);
                tmp0 = _mm_unpackhi(tmp1, in0);
            }
            else if constexpr (laneOffsetOut == 2)
            {
                tmp1 = _mm_unpackhi(in0, in1);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
            else
            {
                __m256 perm = Permute2F128<1, 0>(in1);
                tmp0 = Shuffle<2, 2, 2, 2>(perm, in0);
                tmp1 = Shuffle<3, 3, 3, 3>(perm, in0);
            }
        }
        else
        {
            if constexpr (laneOffsetOut == 0)
            {
                __m256 tmp2 = _mm_unpackhi(in0, in1);
                tmp0 = Permute2F128<1, 0>(tmp2);
                tmp1 = Permute<2, 3, 0, 1>(tmp0);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                __m256 tmp2 = Shuffle<2, 3, 3, 2>(in0, in1);
                tmp1 = Permute2F128<1, 0>(tmp2);
                tmp0 = Permute<0, 0, 3, 3>(tmp1);
            }
            else if constexpr (laneOffsetOut == 2)
            {
                __m256 tmp2 = _mm_unpackhi(in0, in1);
                tmp1 = Permute2F128<1, 0>(tmp2);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
            else
            {
                __m256 perm = Permute2F128<1, 0>(in0);
                tmp0 = Shuffle<2, 2, 2, 2>(in1, perm);
                tmp1 = Shuffle<3, 3, 3, 3>(in1, perm);
            }
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256 tmp2 = _mm_unpackhi(in0, in1);
            __m256 tmp3 = _mm_unpacklo(in0, in1);
            tmp0 = Permute<2, 3, 0, 1>(tmp2);
            tmp1 = Permute2F128<1, 0>(tmp3);
        }
        else if constexpr (_firstRowOut == 1)
        {
            __m256 tmp2 = Shuffle<3, 0, 0, 3>(in0, in1);
            tmp0 = Shuffle<0, 3, 3, 0>(in0, in1);
            tmp1 = Permute2F128<1, 0>(tmp2);
        }
        else if constexpr (_firstRowOut == 2)
        {
            __m256 tmp2 = _mm_unpacklo(in0, in1);
            __m256 tmp3 = Permute<2, 3, 0, 1>(tmp2);
            tmp0 = _mm_unpackhi(in0, in1);
            tmp1 = Permute2F128<1, 0>(tmp3);
        }
        else if constexpr (_firstRowOut == 3)
        {
            __m256 perm = Permute2F128<0, 1, 1, 0>(in0, in1);
            tmp0 = Shuffle<3, 3, 3, 3>(perm, in0);
            tmp1 = Shuffle<0, 0, 0, 0>(in1, perm);
        }
        else if constexpr (_firstRowOut == 4)
        {
            __m256 tmp2 = _mm_unpackhi(in0, in1);
            __m256 tmp3 = Permute<2, 3, 0, 1>(tmp2);
            tmp0 = Permute2F128<1, 0>(tmp3);
            tmp1 = _mm_unpacklo(in0, in1);
        }
        else if constexpr (_firstRowOut == 5)
        {
            __m256 tmp2 = Shuffle<0, 3, 3, 0>(in0, in1);
            tmp0 = Permute2F128<1, 0>(tmp2);
            tmp1 = Shuffle<3, 0, 0, 3>(in0, in1);
        }
        else
        {
            __m256 tmp2 = _mm_unpackhi(in0, in1);
            __m256 tmp3 = _mm_unpacklo(in0, in1);
            tmp0 = Permute2F128<1, 0>(tmp2);
            tmp1 = Permute<2, 3, 0, 1>(tmp3);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1;

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp2 = _mm_unpacklo(in0, in1);

            tmp0 = Shuffle<0, 1, 0, 1>(tmp2, in2);
            tmp1 = Shuffle<2, 3, 1, 0>(tmp2, in2);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp2 = _mm_unpacklo(in1, in2);

            tmp0 = Shuffle<1, 0, 0, 1>(in0, tmp2);
            tmp1 = BlendIndex<_firstRowIn + 1>(tmp2, in0);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp2 = _mm_unpacklo(in0, in1);

            __m256 tmp3;
            if constexpr (laneIn == laneOut)
                tmp3 = Permute2F128<0, 0, 1, 0>(tmp2, in2);
            else
                tmp3 = Permute2F128<0, 1, 1, 1>(tmp2, in2);

            tmp0 = Permute<0, 1, 0, 1>(tmp3);
            tmp1 = Permute<1, 0, 2, 3>(tmp3);
        }
        else
        {
            __m256 tmp2 = _mm_unpacklo(in1, in2);

            __m256 tmp3;
            if constexpr (laneIn == laneOut)
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, tmp2);
            else
                tmp3 = Permute2F128<0, 1, 1, 1>(in0, tmp2);

            tmp0 = Permute<0, 1, 1, 0>(tmp3);
            tmp1 = Permute<2, 3, 0, 1>(tmp3);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp2 = Shuffle<1, 2, 1, 2>(in0, in1);

            tmp0 = Shuffle<0, 2, 1, 3>(tmp2, in2);
            tmp1 = Shuffle<1, 3, 2, 0>(tmp2, in2);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp2 = Shuffle<1, 2, 1, 2>(in1, in2);

            tmp0 = Shuffle<3, 1, 0, 2>(in0, tmp2);
            tmp1 = Shuffle<0, 2, 1, 3>(in0, tmp2);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp2 = Shuffle<1, 2, 1, 2>(in0, in1);

            __m256 tmp3;
            if constexpr (laneIn == laneOut)
                tmp3 = Permute2F128<0, 0, 1, 0>(tmp2, in2);
            else
                tmp3 = Permute2F128<0, 1, 1, 1>(tmp2, in2);

            tmp0 = Permute<1, 3, 0, 2>(tmp3);
            tmp1 = Permute<2, 0, 1, 3>(tmp3);
        }
        else
        {
            __m256 tmp2 = Shuffle<1, 2, 1, 2>(in1, in2);

            __m256 tmp3;
            if constexpr (laneIn == laneOut)
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, tmp2);
            else
                tmp3 = Permute2F128<0, 1, 1, 1>(in0, tmp2);

            tmp0 = Permute<0, 2, 3, 1>(tmp3);
            tmp1 = Permute<1, 3, 0, 2>(tmp3);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp2 = _mm_unpackhi(in0, in1);

            tmp0 = BlendIndex<_firstRowIn>(tmp2, in2);
            tmp1 = Shuffle<2, 3, 3, 2>(tmp2, in2);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp2 = _mm_unpackhi(in1, in2);

            tmp0 = Shuffle<3, 2, 0, 1>(in0, tmp2);
            tmp1 = Shuffle<2, 3, 2, 3>(in0, tmp2);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp2 = _mm_unpackhi(in0, in1);

            __m256 tmp3;
            if constexpr (laneIn == laneOut)
                tmp3 = Permute2F128<0, 0, 1, 0>(tmp2, in2);
            else
                tmp3 = Permute2F128<0, 1, 1, 1>(tmp2, in2);

            tmp0 = Permute<2, 3, 0, 1>(tmp3);
            tmp1 = Permute<3, 2, 2, 3>(tmp3);
        }
        else
        {
            __m256 tmp2 = _mm_unpackhi(in1, in2);

            __m256 tmp3;
            if constexpr (laneIn == laneOut)
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, tmp2);
            else
                tmp3 = Permute2F128<0, 1, 1, 1>(in0, tmp2);

            tmp0 = Permute<0, 1, 3, 2>(tmp3);
            tmp1 = Permute<2, 3, 2, 3>(tmp3);
        }
    }
    else
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp2 = Shuffle<0, 3, 0, 3>(in0, in1);

            tmp0 = Shuffle<1, 3, 3, 1>(tmp2, in2);
            tmp1 = Shuffle<0, 2, 0, 2>(tmp2, in2);

            if constexpr (laneOut == 0)
                tmp1 = Permute2F128<1, 0>(tmp1);
            else
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp2 = Shuffle<0, 3, 0, 3>(in1, in2);

            tmp0 = Shuffle<1, 3, 1, 3>(in0, tmp2);
            tmp1 = Shuffle<2, 0, 0, 2>(in0, tmp2);

            if constexpr (laneOut == 0)
                tmp1 = Permute2F128<1, 0>(tmp1);
            else
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp2 = Shuffle<0, 3, 0, 3>(in0, in1);
            __m256 tmp3 = Permute2F128<0, 1, 1, 0>(tmp2, in2);

            tmp0 = Shuffle<3, 1, 1, 3>(tmp3, tmp2);
            tmp1 = Shuffle<0, 2, 0, 2>(in2, tmp3);
        }
        else
        {
            __m256 tmp2 = Shuffle<0, 3, 0, 3>(in1, in2);
            __m256 tmp3 = Permute2F128<0, 1, 1, 0>(in0, tmp2);

            tmp0 = Shuffle<1, 3, 1, 3>(tmp3, in0);
            tmp1 = Shuffle<0, 2, 2, 0>(tmp2, tmp3);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1;

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp2 = _mm_unpacklo(in0, in1);
            __m256 tmp3 = _mm_unpacklo(in2, in3);

            tmp0 = _mm_movelh(tmp2, tmp3);
            tmp1 = _mm_movehl(tmp3, tmp2);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneIn == 0)
            {
                __m256 tmp2 = Permute2F128<0, 0, 1, 0>(in0, in3);
                __m256 tmp3 = Shuffle<0, 1, 0, 1>(in1, in2);

                tmp0 = Shuffle<0, 0, 0, 2>(tmp2, tmp3);
                tmp1 = Shuffle<1, 1, 1, 3>(tmp2, tmp3);
            }
            else
            {
                __m256 tmp2 = _mm_unpacklo(in1, in2);
                __m256 tmp3 = _mm_unpacklo(in3, in0);

                __m256 tmp4 = _mm_movelh(tmp3, tmp2);
                __m256 tmp5 = _mm_movehl(tmp2, tmp3);

                tmp0 = Permute2F128<1, 1>(tmp4);
                tmp1 = Permute2F128<1, 1>(tmp5);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp2 = _mm_unpacklo(in2, in3);
            __m256 tmp3 = _mm_unpacklo(in0, in1);

            __m256 tmp4, tmp5;
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(tmp2);
                tmp5 = tmp3;
            }
            else
            {
                tmp4 = tmp2;
                tmp5 = Permute2F128<1, 0>(tmp3);
            }

            tmp0 = _mm_movelh(tmp4, tmp5);
            tmp1 = _mm_movehl(tmp5, tmp4);
        }
        else
        {
            if constexpr (laneIn == 0)
            {
                __m256 tmp2 = _mm_unpacklo(in3, in0);
                __m256 tmp3 = _mm_unpacklo(in1, in2);

                __m256 tmp4 = _mm_movelh(tmp3, tmp2);
                __m256 tmp5 = _mm_movehl(tmp2, tmp3);

                tmp0 = Permute2F128<0, 0>(tmp4);
                tmp1 = Permute2F128<0, 0>(tmp5);
            }
            else
            {
                __m256 tmp2 = Shuffle<0, 1, 0, 1>(in1, in2);
                __m256 tmp3 = Permute2F128<0, 1, 1, 1>(in0, in3);

                tmp0 = Shuffle<0, 2, 0, 0>(tmp2, tmp3);
                tmp1 = Shuffle<1, 3, 1, 1>(tmp2, tmp3);
            }
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp2 = Shuffle<1, 2, 1, 2>(in0, in1);
            __m256 tmp3 = Shuffle<1, 2, 1, 2>(in2, in3);

            tmp0 = Shuffle<0, 2, 0, 2>(tmp2, tmp3);
            tmp1 = Shuffle<1, 3, 1, 3>(tmp2, tmp3);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneIn == 0)
            {
                __m256 tmp2 = Permute2F128<0, 0, 1, 0>(in0, in3);
                __m256 tmp3 = Shuffle<1, 2, 1, 2>(in1, in2);

                tmp0 = Shuffle<1, 1, 0, 2>(tmp2, tmp3);
                tmp1 = Shuffle<2, 2, 1, 3>(tmp2, tmp3);
            }
            else
            {
                __m256 tmp2 = Shuffle<1, 2, 1, 2>(in3, in0);
                __m256 tmp3 = Shuffle<1, 2, 1, 2>(in1, in2);

                __m256 tmp4 = Shuffle<0, 2, 0, 2>(tmp2, tmp3);
                __m256 tmp5 = Shuffle<1, 3, 1, 3>(tmp2, tmp3);

                tmp0 = Permute2F128<1, 1>(tmp4);
                tmp1 = Permute2F128<1, 1>(tmp5);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp2 = Shuffle<1, 2, 1, 2>(in2, in3);
            __m256 tmp3 = Shuffle<1, 2, 1, 2>(in0, in1);

            __m256 tmp4, tmp5;
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(tmp2);
                tmp5 = tmp3;
            }
            else
            {
                tmp4 = tmp2;
                tmp5 = Permute2F128<1, 0>(tmp3);
            }

            tmp0 = Shuffle<0, 2, 0, 2>(tmp4, tmp5);
            tmp1 = Shuffle<1, 3, 1, 3>(tmp4, tmp5);
        }
        else
        {
            if constexpr (laneIn == 0)
            {
                __m256 tmp2 = Shuffle<1, 2, 1, 2>(in1, in2);
                __m256 tmp3 = Shuffle<1, 2, 1, 2>(in3, in0);

                __m256 tmp4 = Shuffle<0, 2, 0, 2>(tmp2, tmp3);
                __m256 tmp5 = Shuffle<1, 3, 1, 3>(tmp2, tmp3);

                tmp0 = Permute2F128<0, 0>(tmp4);
                tmp1 = Permute2F128<0, 0>(tmp5);
            }
            else
            {
                __m256 tmp2 = Shuffle<1, 2, 1, 2>(in1, in2);
                __m256 tmp3 = Permute2F128<0, 1, 1, 1>(in0, in3);

                tmp0 = Shuffle<0, 2, 1, 1>(tmp2, tmp3);
                tmp1 = Shuffle<1, 3, 2, 2>(tmp2, tmp3);
            }
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp2 = _mm_unpackhi(in0, in1);
            __m256 tmp3 = _mm_unpackhi(in2, in3);

            tmp0 = _mm_movelh(tmp2, tmp3);
            tmp1 = _mm_movehl(tmp3, tmp2);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneIn == 0)
            {
                __m256 tmp2 = Permute2F128<0, 0, 1, 0>(in0, in3);
                __m256 tmp3 = Shuffle<2, 3, 2, 3>(in1, in2);

                tmp0 = Shuffle<2, 2, 0, 2>(tmp2, tmp3);
                tmp1 = Shuffle<3, 3, 1, 3>(tmp2, tmp3);
            }
            else
            {
                __m256 tmp2 = _mm_unpackhi(in1, in2);
                __m256 tmp3 = _mm_unpackhi(in3, in0);

                __m256 tmp4 = _mm_movelh(tmp3, tmp2);
                __m256 tmp5 = _mm_movehl(tmp2, tmp3);

                tmp0 = Permute2F128<1, 1>(tmp4);
                tmp1 = Permute2F128<1, 1>(tmp5);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp2 = _mm_unpackhi(in2, in3);
            __m256 tmp3 = _mm_unpackhi(in0, in1);


            __m256 tmp4, tmp5;
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(tmp2);
                tmp5 = tmp3;
            }
            else
            {
                tmp4 = tmp2;
                tmp5 = Permute2F128<1, 0>(tmp3);
            }

            tmp0 = _mm_movelh(tmp4, tmp5);
            tmp1 = _mm_movehl(tmp5, tmp4);
        }
        else
        {
            if constexpr (laneIn == 0)
            {
                __m256 tmp2 = _mm_unpackhi(in3, in0);
                __m256 tmp3 = _mm_unpackhi(in1, in2);

                __m256 tmp4 = _mm_movelh(tmp3, tmp2);
                __m256 tmp5 = _mm_movehl(tmp2, tmp3);

                tmp0 = Permute2F128<0, 0>(tmp4);
                tmp1 = Permute2F128<0, 0>(tmp5);
            }
            else
            {
                __m256 tmp2 = Shuffle<2, 3, 2, 3>(in1, in2);
                __m256 tmp3 = Permute2F128<0, 1, 1, 1>(in0, in3);

                tmp0 = Shuffle<0, 2, 2, 2>(tmp2, tmp3);
                tmp1 = Shuffle<1, 3, 3, 3>(tmp2, tmp3);
            }
        }
    }
    else
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp2 = Shuffle<3, 0, 3, 0>(in0, in1);
            __m256 tmp3 = Shuffle<3, 0, 3, 0>(in2, in3);

            tmp0 = Shuffle<0, 2, 0, 2>(tmp2, tmp3);
            tmp1 = Shuffle<1, 3, 1, 3>(tmp2, tmp3);

            if constexpr (laneOut == 0)

                tmp1 = Permute2F128<1, 0>(tmp1);
            else
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp2 = Shuffle<3, 0, 3, 0>(in3, in0);
            __m256 tmp3 = Shuffle<3, 0, 3, 0>(in1, in2);

            __m256 tmp4 = Shuffle<0, 2, 0, 2>(tmp2, tmp3);
            __m256 tmp5 = Shuffle<1, 3, 1, 3>(tmp2, tmp3);

            tmp0 = Permute2F128<0, 0>(tmp4);
            tmp1 = Permute2F128<1, 1>(tmp5);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp2 = Shuffle<3, 0, 3, 0>(in2, in3);
            __m256 tmp3 = Shuffle<3, 0, 3, 0>(in0, in1);
            __m256 tmp4 = Permute2F128<0, 1, 1, 0>(tmp3, tmp2);

            tmp0 = Shuffle<0, 2, 0, 2>(tmp4, tmp3);
            tmp1 = Shuffle<1, 3, 1, 3>(tmp2, tmp4);
        }
        else
        {
            __m256 tmp2 = Shuffle<3, 0, 3, 0>(in1, in2);
            __m256 tmp3 = Shuffle<3, 0, 3, 0>(in3, in0);

            __m256 tmp4 = Shuffle<0, 2, 0, 2>(tmp2, tmp3);
            __m256 tmp5 = Shuffle<1, 3, 1, 3>(tmp2, tmp3);

            tmp0 = Permute2F128<0, 0>(tmp4);
            tmp1 = Permute2F128<1, 1>(tmp5);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1;


    if constexpr (laneOffsetIn < 3)
    {
        __m256 tmp2, tmp3, tmp4, tmp5;

        if constexpr (laneOffsetOut == 0)
        {
            if constexpr (laneIn == 0)
            {
                tmp2 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp3 = in1;
                tmp4 = in2;
                tmp5 = in3;
            }
            else
            {
                tmp2 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp3 = Permute2F128<1, 0>(in1);
                tmp4 = Permute2F128<1, 0>(in2);
                tmp5 = Permute2F128<1, 0>(in3);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneIn == 0)
            {
                tmp2 = Permute2F128<1, 0>(in3);
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp4 = in1;
                tmp5 = in2;
            }
            else
            {
                tmp2 = in3;
                tmp3 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp4 = Permute2F128<1, 0>(in1);
                tmp5 = Permute2F128<1, 0>(in2);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            if constexpr (laneIn == 0)
            {
                tmp2 = Permute2F128<1, 0>(in2);
                tmp3 = Permute2F128<1, 0>(in3);
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp5 = in1;
            }
            else
            {
                tmp2 = in2;
                tmp3 = in3;
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp5 = Permute2F128<1, 0>(in1);
            }
        }
        else
        {
            if constexpr (laneIn == 0)
            {
                tmp2 = Permute2F128<1, 0>(in1);
                tmp3 = Permute2F128<1, 0>(in2);
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = Permute2F128<0, 0, 1, 0>(in0, in4);
            }
            else
            {
                tmp2 = in1;
                tmp3 = in2;
                tmp4 = in3;
                tmp5 = Permute2F128<0, 1, 1, 1>(in0, in4);
            }
        }

        if constexpr (laneOffsetIn == 0)
        {

            __m256 tmp6 = _mm_unpacklo(tmp2, tmp3);
            __m256 tmp7 = _mm_unpacklo(tmp4, tmp5);

            tmp0 = _mm_movelh(tmp6, tmp7);
            tmp1 = _mm_movehl(tmp7, tmp6);
        }
        else if constexpr (laneOffsetIn == 1)
        {
            __m256 tmp6 = Shuffle<1, 2, 1, 2>(tmp2, tmp3);
            __m256 tmp7 = Shuffle<1, 2, 1, 2>(tmp4, tmp5);

            tmp0 = Shuffle<0, 2, 0, 2>(tmp6, tmp7);
            tmp1 = Shuffle<1, 3, 1, 3>(tmp6, tmp7);
        }
        else if constexpr (laneOffsetIn == 2)
        {
            __m256 tmp6 = _mm_unpackhi(tmp2, tmp3);
            __m256 tmp7 = _mm_unpackhi(tmp4, tmp5);

            tmp0 = _mm_movelh(tmp6, tmp7);
            tmp1 = _mm_movehl(tmp7, tmp6);
        }
    }
    else
    {
        __m256 tmp3, tmp4, tmp5, tmp6, tmp7;

        if constexpr (laneOffsetOut == 0)
        {
            tmp3 = Permute2F128<1, 0>(in4);
            tmp4 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in0);
            tmp5 = in1;
            tmp6 = in2;
            tmp7 = in3;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp3 = Permute2F128<1, 0>(in4);
            tmp4 = Permute2F128<1, 0>(in3);
            tmp5 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in0);
            tmp6 = in1;
            tmp7 = in2;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp4 = in2;
            tmp5 = in3;
            tmp7 = Permute2F128<1, 0>(in1);
            tmp3 = Permute2F128<1, 0>(in0);
            tmp6 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in4);
        }
        else
        {
            tmp3 = Permute2F128<1, 0>(in0);
            tmp4 = in1;
            tmp5 = in2;
            tmp6 = in3;
            tmp7 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in4);
        }

        __m256 tmp8 = Shuffle<3, 0, 3, 0>(tmp4, tmp5);
        __m256 tmp9 = Shuffle<3, 0, 3, 0>(tmp6, tmp7);

        tmp0 = Shuffle<0, 2, 0, 2>(tmp8, tmp9);
        tmp1 = Shuffle<1, 3, 1, 3>(tmp8, tmp9);

        if constexpr (laneOffsetOut < 2)
            tmp1 = Permute2F128<1, 0>(tmp1);
        else
            tmp0 = Permute2F128<1, 0>(tmp0);
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;

    __m256 tmp0, tmp1;


    if constexpr (laneOffsetIn < 3)
    {
        __m256 tmp2, tmp3, tmp4, tmp5;
        if constexpr (laneIn == 0)
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp2 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp3 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp4 = in2;
                tmp5 = in3;
            }
            else if constexpr (_firstRowOut == 1)
            {
                tmp2 = Permute2F128<1, 0>(in3);
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp4 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp5 = in2;
            }
            else
            {
                tmp2 = Permute2F128<1, 0>(in2);
                tmp3 = Permute2F128<1, 0>(in3);
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
            }
        }
        else
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp2 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp3 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp4 = Permute2F128<1, 0>(in2);
                tmp5 = Permute2F128<1, 0>(in3);
            }
            else if constexpr (_firstRowOut == 1)
            {
                tmp2 = in3;
                tmp3 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp4 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp5 = Permute2F128<1, 0>(in2);
            }
            else
            {
                tmp2 = in2;
                tmp3 = in3;
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp5 = Permute2F128<0, 1, 1, 1>(in1, in5);
            }
        }

        Transpose2x4<laneOffsetIn, 0>(tmp2, tmp3, tmp4, tmp5, tmp0, tmp1);
    }
    else
    {
        __m256 tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9;

        if constexpr (_firstRowOut == 0)
        {
            tmp2 = Permute2F128<0, 0, 1, 0>(in0, in4);
            tmp3 = Permute2F128<0, 0, 1, 0>(in1, in5);
            tmp4 = in2;
            tmp5 = in3;

            tmp6 = Permute2F128<0, 1, 1, 1>(in0, in4);
            tmp7 = Permute2F128<0, 1, 1, 1>(in1, in5);
            tmp8 = Permute2F128<1, 0>(in2);
            tmp9 = Permute2F128<1, 0>(in3);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp2 = Permute2F128<1, 0>(in3);
            tmp3 = Permute2F128<0, 0, 1, 0>(in0, in4);
            tmp4 = Permute2F128<0, 0, 1, 0>(in1, in5);
            tmp5 = in2;

            tmp6 = in3;
            tmp7 = Permute2F128<0, 1, 1, 1>(in0, in4);
            tmp8 = Permute2F128<0, 1, 1, 1>(in1, in5);
            tmp9 = Permute2F128<1, 0>(in2);
        }
        else
        {
            tmp2 = Permute2F128<1, 0>(in2);
            tmp3 = Permute2F128<1, 0>(in3);
            tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
            tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);

            tmp6 = in2;
            tmp7 = in3;
            tmp8 = Permute2F128<0, 1, 1, 1>(in0, in4);
            tmp9 = Permute2F128<0, 1, 1, 1>(in1, in5);
        }

        Transpose1x4<3, 0>(tmp2, tmp3, tmp4, tmp5, tmp0);
        Transpose1x4<0, 0>(tmp6, tmp7, tmp8, tmp9, tmp1);
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;

    __m256 tmp0, tmp1;

    if constexpr (laneOffsetIn < 3)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;
        if constexpr (laneIn == 0)
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp6 = Permute2F128<0, 0, 1, 0>(in2, in6);
                tmp7 = in3;
            }
            else
            {
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp6 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp7 = Permute2F128<0, 0, 1, 0>(in2, in6);
            }
        }
        else
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp5 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp6 = Permute2F128<0, 1, 1, 1>(in2, in6);
                tmp7 = Permute2F128<1, 0>(in3);
            }
            else
            {
                tmp4 = in3;
                tmp5 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp6 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp7 = Permute2F128<0, 1, 1, 1>(in2, in6);
            }
        }
        Transpose2x4<laneOffsetIn, 0>(tmp4, tmp5, tmp6, tmp7, tmp0, tmp1);
    }
    else
    {
        __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;

        if constexpr (_firstRowOut == 0)
        {
            Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
            Transpose4x3<0, 0>(in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
        }
        else
        {
            Transpose4x3<0, 1>(in0, in1, in2, tmp6, tmp7, tmp8, tmp9);
            Transpose4x4<0, 0>(in3, in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
        }


        tmp0 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp1 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;

    __m256 tmp0, tmp1;

    if constexpr (laneOffsetIn < 3)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;
        if constexpr (laneIn == 0)
        {

            tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
            tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
            tmp6 = Permute2F128<0, 0, 1, 0>(in2, in6);
            tmp7 = Permute2F128<0, 0, 1, 0>(in3, in7);
        }
        else
        {

            tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
            tmp5 = Permute2F128<0, 1, 1, 1>(in1, in5);
            tmp6 = Permute2F128<0, 1, 1, 1>(in2, in6);
            tmp7 = Permute2F128<0, 1, 1, 1>(in3, in7);
        }
        Transpose2x4<laneOffsetIn, 0>(tmp4, tmp5, tmp6, tmp7, tmp0, tmp1);
    }
    else
    {
        __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;

        Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
        Transpose4x4<0, 0>(in4, in5, in6, in7, tmp10, tmp11, tmp12, tmp13);

        tmp0 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp1 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
    }


    // Write to output registers
    out0 = tmp0;
    out1 = tmp1;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2;

    if constexpr (laneOffsetIn == 0)
    {
        __m256 tmp3;

        if constexpr (laneOut != laneIn)
            tmp3 = Permute2F128<1, 0>(in0);
        else
            tmp3 = in0;

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = tmp3;
            tmp1 = _mm_movehdup(tmp3);
            tmp2 = _mm_unpackhi(tmp3, tmp3);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = _mm_moveldup(tmp3);
            tmp1 = tmp3;
            tmp2 = _mm_unpackhi(tmp3, tmp3);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp2 = tmp3;
            tmp1 = _mm_unpacklo(tmp3, tmp3);
            tmp0 = _mm_unpacklo(tmp1, tmp1);
        }
        else
        {
            tmp2 = _mm_moveldup(tmp3);
            tmp1 = _mm_unpacklo(tmp3, tmp3);
            tmp0 = _mm_unpacklo(tmp1, tmp1);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        __m256 tmp3;

        if constexpr (laneOut != laneIn)
            tmp3 = Permute2F128<1, 0>(in0);
        else
            tmp3 = in0;

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = _mm_movehdup(tmp3);
            tmp1 = _mm_unpackhi(tmp3, tmp3);
            tmp2 = _mm_unpackhi(tmp1, tmp1);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = tmp3;
            tmp1 = _mm_unpackhi(tmp3, tmp3);
            tmp2 = _mm_unpackhi(tmp1, tmp1);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp0 = _mm_unpacklo(tmp3, tmp3);
            tmp1 = tmp3;
            tmp2 = _mm_movehdup(tmp3);
        }
        else
        {
            tmp0 = _mm_unpacklo(tmp3, tmp3);
            tmp1 = _mm_moveldup(tmp3);
            tmp2 = tmp3;
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        __m256 tmp3, tmp4;
        if constexpr (laneOut == laneIn)
        {
            tmp3 = in0;
            tmp4 = Permute2F128<1, 0>(in0);
        }
        else
        {
            tmp3 = Permute2F128<1, 0>(in0);
            tmp4 = in0;
        }

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = _mm_unpackhi(tmp3, tmp3);
            tmp1 = _mm_unpackhi(tmp0, tmp0);
            tmp2 = tmp4;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = _mm_unpackhi(tmp3, tmp3);
            tmp1 = _mm_unpackhi(tmp0, tmp0);
            tmp2 = _mm_moveldup(tmp4);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp0 = tmp3;
            tmp1 = _mm_movehdup(tmp3);
            tmp2 = Broadcast<0>(tmp4);
        }
        else
        {
            tmp0 = _mm_moveldup(tmp3);
            tmp1 = tmp3;
            tmp2 = Broadcast<0>(tmp4);
        }
    }
    else
    {
        __m256 tmp3, tmp4;
        if constexpr (laneOut == laneIn)
        {
            tmp3 = in0;
            tmp4 = Permute2F128<1, 0>(in0);
        }
        else
        {
            tmp3 = Permute2F128<1, 0>(in0);
            tmp4 = in0;
        }

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = Broadcast<3>(tmp3);
            tmp1 = tmp4;
            tmp2 = _mm_movehdup(tmp4);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = Broadcast<3>(tmp3);
            tmp1 = _mm_moveldup(tmp4);
            tmp2 = tmp4;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp2 = _mm_unpacklo(tmp4, tmp4);
            tmp1 = _mm_unpacklo(tmp2, tmp2);
            tmp0 = _mm_movehdup(tmp3);
        }
        else
        {
            tmp2 = _mm_unpacklo(tmp4, tmp4);
            tmp1 = _mm_unpacklo(tmp2, tmp2);
            tmp0 = tmp3;
        }
    }



    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2;

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut < 3)
        {
            __m256 tmp3, tmp4;
            if constexpr (laneIn == laneOut)
            {
                tmp3 = in0;
                tmp4 = in1;
            }
            else
            {
                tmp3 = Permute2F128<1, 0>(in0);
                tmp4 = Permute2F128<1, 0>(in1);
            }

            if constexpr (laneOffsetOut == 0)
            {

                tmp0 = _mm_unpacklo(tmp3, tmp4);
                tmp1 = Permute<2, 3, 0, 1>(tmp0);
                tmp2 = _mm_unpackhi(tmp3, tmp4);
            }
            else if constexpr (laneOffsetOut == 1)
            {

                tmp0 = Shuffle<1, 0, 0, 1>(tmp3, tmp4);
                tmp1 = _mm_unpacklo(tmp4, tmp0);
                tmp2 = Shuffle<3, 2, 2, 3>(tmp3, tmp4);
            }
            else if constexpr (laneOffsetOut == 2)
            {
                __m256 tmp5 = _mm_unpackhi(tmp3, tmp4);

                tmp2 = Permute<2, 3, 0, 1>(tmp5);
                tmp1 = _mm_unpacklo(tmp3, tmp4);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
        }
        else
        {
            __m256 tmp3, tmp4;
            if constexpr (laneIn == 0)
            {
                tmp3 = Permute2F128<1, 0>(in1);
                tmp4 = in0;
            }
            else
            {
                tmp3 = in1;
                tmp4 = Permute2F128<1, 0>(in0);
            }

            tmp0 = Shuffle<0, 0, 0, 0>(tmp3, tmp4);
            tmp1 = Shuffle<1, 1, 1, 1>(tmp3, tmp4);
            tmp2 = Shuffle<2, 2, 2, 2>(tmp3, tmp4);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneOffsetOut < 3)
        {
            __m256 tmp3, tmp4;
            if constexpr (laneIn == laneOut)
            {
                tmp3 = in0;
                tmp4 = in1;
            }
            else
            {
                tmp3 = Permute2F128<1, 0>(in0);
                tmp4 = Permute2F128<1, 0>(in1);
            }

            if constexpr (laneOffsetOut == 0)
            {
                __m256 tmp5 = _mm_unpacklo(tmp3, tmp4);

                tmp0 = Permute<2, 3, 0, 1>(tmp5);
                tmp1 = _mm_unpackhi(tmp3, tmp4);
                tmp2 = Permute<2, 3, 0, 1>(tmp1);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                tmp0 = Shuffle<1, 1, 1, 1>(tmp3, tmp4);
                tmp1 = Shuffle<2, 2, 2, 2>(tmp3, tmp4);
                tmp2 = Shuffle<3, 3, 3, 3>(tmp3, tmp4);
            }
            else
            {
                tmp2 = _mm_unpackhi(tmp3, tmp4);
                tmp1 = Permute<2, 3, 0, 1>(tmp2);
                tmp0 = _mm_unpacklo(tmp3, tmp4);
            }
        }
        else
        {
            __m256 tmp3, tmp4;
            if constexpr (laneIn == 0)
            {
                tmp3 = Permute2F128<1, 0>(in1);
                tmp4 = in0;
            }
            else
            {
                tmp3 = in1;
                tmp4 = Permute2F128<1, 0>(in0);
            }

            tmp0 = Shuffle<1, 1, 1, 1>(tmp3, tmp4);
            tmp1 = Shuffle<2, 2, 2, 2>(tmp3, tmp4);
            tmp2 = Shuffle<3, 3, 3, 3>(tmp3, tmp4);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256 tmp3 = _mm_unpacklo(in0, in1);

            tmp0 = _mm_unpackhi(in0, in1);
            tmp1 = Permute<2, 3, 0, 1>(tmp0);
            tmp2 = Permute2F128<1, 0>(tmp3);
        }
        else if constexpr (_firstRowOut == 1)
        {
            __m256 tmp3 = Shuffle<0, 0, 0, 0>(in0, in1);

            tmp0 = Shuffle<2, 2, 2, 2>(in0, in1);
            tmp1 = Shuffle<3, 3, 3, 3>(in0, in1);
            tmp2 = Permute2F128<1, 0>(tmp3);
        }
        else if constexpr (_firstRowOut == 2)
        {
            __m256 tmp3 = _mm_unpacklo(in0, in1);
            __m256 tmp4 = Permute<2, 3, 0, 1>(tmp3);

            tmp2 = Permute2F128<1, 0>(tmp4);
            tmp1 = _mm_unpackhi(in0, in1);
            tmp0 = Permute<2, 3, 0, 1>(tmp1);
        }
        else if constexpr (_firstRowOut == 3)
        {
            __m256 tmp3 = Permute2F128<0, 1, 1, 0>(in0, in1);

            tmp0 = Shuffle<2, 2, 2, 2>(tmp3, in0);
            tmp1 = Shuffle<3, 3, 3, 3>(tmp3, in0);
            tmp2 = Shuffle<0, 0, 0, 0>(in1, tmp3);
        }
        else if constexpr (_firstRowOut == 4)
        {
            __m256 tmp3 = _mm_unpackhi(in0, in1);

            tmp0 = Permute2F128<1, 0>(tmp3);
            tmp1 = Permute<2, 3, 0, 1>(tmp0);
            tmp2 = _mm_unpacklo(in0, in1);
        }
        else if constexpr (_firstRowOut == 5)
        {
            __m256 tmp3 = Shuffle<3, 2, 2, 3>(in0, in1);

            tmp0 = Permute2F128<1, 0>(tmp3);
            tmp1 = Permute<0, 0, 3, 3>(tmp0);
            tmp2 = Shuffle<0, 0, 0, 0>(in0, in1);
        }
        else
        {
            __m256 tmp3 = _mm_unpacklo(in0, in1);
            __m256 tmp4 = _mm_unpackhi(in0, in1);

            tmp2 = Permute<2, 3, 0, 1>(tmp3);
            tmp1 = Permute2F128<1, 0>(tmp4);
            tmp0 = Permute<2, 3, 0, 1>(tmp1);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256 tmp3 = _mm_unpacklo(in0, in1);
            __m256 tmp4 = _mm_unpackhi(in0, in1);

            tmp0 = Permute<2, 3, 0, 1>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp3);
            tmp2 = Permute<2, 3, 0, 1>(tmp1);
        }
        else if constexpr (_firstRowOut == 1)
        {
            __m256 tmp3 = Shuffle<1, 0, 0, 1>(in0, in1);

            tmp0 = Shuffle<3, 3, 3, 3>(in0, in1);
            tmp1 = Permute2F128<1, 0>(tmp3);
            tmp2 = Permute<0, 0, 3, 3>(tmp1);
        }
        else if constexpr (_firstRowOut == 2)
        {
            __m256 tmp3 = _mm_unpacklo(in0, in1);

            tmp2 = Permute2F128<1, 0>(tmp3);
            tmp1 = Permute<2, 3, 0, 1>(tmp2);
            tmp0 = _mm_unpackhi(in0, in1);
        }
        else if constexpr (_firstRowOut == 3)
        {
            __m256 tmp3 = Permute2F128<0, 1, 1, 0>(in0, in1);

            tmp0 = Shuffle<3, 3, 3, 3>(tmp3, in0);
            tmp1 = Shuffle<0, 0, 0, 0>(in1, tmp3);
            tmp2 = Shuffle<1, 1, 1, 1>(in1, tmp3);
        }
        else if constexpr (_firstRowOut == 4)
        {
            __m256 tmp3 = _mm_unpackhi(in0, in1);
            __m256 tmp4 = Permute2F128<1, 0>(tmp3);

            tmp0 = Permute<2, 3, 0, 1>(tmp4);
            tmp1 = _mm_unpacklo(in0, in1);
            tmp2 = Permute<2, 3, 0, 1>(tmp1);
        }
        else if constexpr (_firstRowOut == 5)
        {
            __m256 tmp3 = Shuffle<3, 3, 3, 3>(in0, in1);

            tmp0 = Permute2F128<1, 0>(tmp3);
            tmp1 = Shuffle<0, 0, 0, 0>(in0, in1);
            tmp2 = Shuffle<1, 1, 1, 1>(in0, in1);
        }
        else
        {
            __m256 tmp3 = _mm_unpackhi(in0, in1);

            tmp2 = _mm_unpacklo(in0, in1);
            tmp1 = Permute<2, 3, 0, 1>(tmp2);
            tmp0 = Permute2F128<1, 0>(tmp3);
        }
    }



    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2;

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp3 = Shuffle<0, 0, 0, 0>(in1, in2);
            __m256 tmp4 = Shuffle<1, 1, 1, 1>(in0, in2);
            __m256 tmp5 = _mm_unpackhi(in0, in1);

            tmp0 = BlendIndex<_firstRowIn>(tmp3, in0);
            tmp1 = BlendIndex<_firstRowIn + 1>(tmp4, in1);
            tmp2 = BlendIndex<_firstRowIn + 2>(tmp5, in2);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp3 = _mm_unpacklo(in1, in2);
            __m256 tmp4 = Shuffle<2, 2, 2, 2>(in0, in2);

            tmp0 = Shuffle<1, 0, 0, 1>(in0, tmp3);
            tmp1 = BlendIndex<_firstRowIn + 1>(tmp3, in0);
            tmp2 = BlendIndex<_firstRowIn + 2>(tmp4, in1);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp3, tmp4;

            if constexpr (laneIn == 0)
            {
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, in2);
                tmp4 = in1;
            }
            else
            {
                tmp3 = Permute2F128<0, 1, 1, 1>(in0, in2);
                tmp4 = Permute2F128<0, 1, 1, 1>(in1, in2);
            }

            __m256 tmp5 = _mm_unpacklo(tmp3, tmp4);
            __m256 tmp6 = _mm_unpackhi(tmp3, tmp4);

            tmp0 = Permute<0, 1, 0, 1>(tmp5);
            tmp1 = Permute<2, 3, 2, 3>(tmp5);
            tmp2 = Permute<0, 1, 0, 1>(tmp6);
        }
        else
        {
            __m256 tmp3, tmp4;

            if constexpr (laneIn == 0)
            {
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, in1);
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in2);
            }
            else
            {
                tmp3 = in1;
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in2);
            }

            __m256 tmp5 = _mm_unpacklo(tmp3, tmp4);
            __m256 tmp6 = _mm_unpackhi(tmp3, tmp4);

            tmp0 = Permute<0, 1, 0, 1>(tmp5);
            tmp1 = Permute<2, 3, 2, 3>(tmp5);
            tmp2 = Permute<0, 1, 0, 1>(tmp6);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp3 = _mm_unpackhi(in0, in1);
            __m256 tmp4 = Shuffle<1, 1, 1, 1>(in0, in2);

            tmp2 = Shuffle<2, 3, 3, 2>(tmp3, in2);
            tmp1 = BlendIndex<_firstRowIn + 1>(tmp3, in2);
            tmp0 = BlendIndex<_firstRowIn>(tmp4, in1);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp3 = _mm_unpacklo(in1, in2);
            __m256 tmp4 = Shuffle<2, 2, 2, 2>(in0, in2);
            __m256 tmp5 = Shuffle<3, 3, 3, 3>(in0, in1);

            tmp0 = BlendIndex<_firstRowIn>(tmp3, in0);
            tmp1 = BlendIndex<_firstRowIn + 1>(tmp4, in1);
            tmp2 = BlendIndex<_firstRowIn + 2>(tmp5, in2);

            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp3, tmp4;

            if constexpr (laneIn == 0)
            {
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, in2);
                tmp4 = in1;
            }
            else
            {
                tmp3 = Permute2F128<0, 1, 1, 1>(in0, in2);
                tmp4 = Permute2F128<0, 1, 1, 1>(in1, in2);
            }

            __m256 tmp5 = _mm_unpacklo(tmp3, tmp4);
            __m256 tmp6 = _mm_unpackhi(tmp3, tmp4);

            tmp0 = Permute<2, 3, 2, 3>(tmp5);
            tmp1 = Permute<0, 1, 0, 1>(tmp6);
            tmp2 = Permute<2, 3, 2, 3>(tmp6);
        }
        else
        {
            __m256 tmp3, tmp4;

            if constexpr (laneIn == 0)
            {
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, in1);
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in2);
            }
            else
            {
                tmp3 = in1;
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in2);
            }

            __m256 tmp5 = _mm_unpacklo(tmp3, tmp4);
            __m256 tmp6 = _mm_unpackhi(tmp3, tmp4);

            tmp0 = Permute<2, 3, 2, 3>(tmp5);
            tmp1 = Permute<0, 1, 0, 1>(tmp6);
            tmp2 = Permute<2, 3, 2, 3>(tmp6);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp3 = _mm_unpackhi(in0, in1);
            __m256 tmp4 = Shuffle<0, 0, 0, 0>(in1, in2);

            tmp1 = Shuffle<2, 3, 3, 2>(tmp3, in2);
            tmp0 = BlendIndex<2>(tmp3, in2);
            tmp2 = BlendIndex<4>(tmp4, in0);

            if constexpr (laneOut == 0)
            {
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp3 = _mm_unpackhi(in1, in2);
            __m256 tmp4 = _mm_unpacklo(in1, in2);

            tmp0 = Shuffle<3, 2, 0, 1>(in0, tmp3);
            tmp1 = Shuffle<2, 3, 2, 3>(in0, tmp3);
            tmp2 = Shuffle<1, 0, 0, 1>(in0, tmp4);

            if constexpr (laneOut == 0)
            {
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp3 = _mm_unpackhi(in0, in1);
            __m256 tmp4 = _mm_unpacklo(in0, in1);
            __m256 tmp5 = Permute2F128<0, 1, 1, 0>(tmp4, in2);

            tmp0 = Shuffle<2, 3, 0, 1>(tmp5, tmp3);
            tmp1 = Shuffle<3, 2, 2, 3>(tmp5, tmp3);
            tmp2 = Shuffle<0, 1, 0, 1>(in2, tmp5);
        }
        else
        {
            __m256 tmp3 = _mm_unpackhi(in1, in2);
            __m256 tmp4 = Permute2F128<0, 1, 1, 0>(in0, tmp3);
            __m256 tmp5 = _mm_unpacklo(in1, in2);

            tmp0 = Shuffle<0, 1, 3, 2>(tmp4, in0);
            tmp1 = Shuffle<2, 3, 2, 3>(tmp4, in0);
            tmp2 = Shuffle<0, 1, 1, 0>(tmp5, tmp4);
        }
    }
    else
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp3 = _mm_unpacklo(in0, in1);
            __m256 tmp4 = _mm_unpackhi(in0, in1);

            tmp0 = Shuffle<2, 3, 3, 2>(tmp4, in2);
            tmp1 = Shuffle<0, 1, 0, 1>(tmp3, in2);
            tmp2 = Shuffle<2, 3, 1, 0>(tmp3, in2);

            if constexpr (laneOut == 0)
            {
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp3 = _mm_unpacklo(in1, in2);
            __m256 tmp4 = Shuffle<3, 3, 3, 3>(in0, in1);

            tmp1 = Shuffle<1, 0, 0, 1>(in0, tmp3);
            tmp0 = BlendIndex<3>(tmp4, in2);
            tmp2 = BlendIndex<5>(tmp3, in0);

            if constexpr (laneOut == 0)
            {
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp3 = _mm_unpacklo(in0, in1);
            __m256 tmp4 = Permute2F128<0, 1, 1, 0>(tmp3, in2);
            __m256 tmp5 = _mm_unpackhi(in0, in1);

            tmp0 = Shuffle<3, 2, 2, 3>(tmp4, tmp5);
            tmp1 = Shuffle<0, 1, 0, 1>(in2, tmp4);
            tmp2 = Shuffle<1, 0, 2, 3>(in2, tmp4);
        }
        else
        {
            __m256 tmp3 = _mm_unpacklo(in1, in2);
            __m256 tmp4 = _mm_unpackhi(in1, in2);
            __m256 tmp5 = Permute2F128<0, 1, 1, 0>(in0, tmp4);

            tmp0 = Shuffle<2, 3, 2, 3>(tmp5, in0);
            tmp1 = Shuffle<0, 1, 1, 0>(tmp3, tmp5);
            tmp2 = Shuffle<2, 3, 0, 1>(tmp3, tmp5);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2;

    if constexpr (laneOffsetIn == 0)
    {


        __m256 tmp4, tmp5, tmp6, tmp7;
        if constexpr (laneOffsetOut == 0)
        {
            if constexpr (laneIn == laneOut)
            {
                tmp4 = in0;
                tmp5 = in1;
                tmp6 = in2;
                tmp7 = in3;
            }
            else
            {
                tmp4 = Permute2F128<1, 0>(in0);
                tmp5 = Permute2F128<1, 0>(in1);
                tmp6 = Permute2F128<1, 0>(in2);
                tmp7 = Permute2F128<1, 0>(in3);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = in0;
                tmp6 = in1;
                tmp7 = in2;
            }
            else
            {
                tmp4 = in3;
                tmp5 = Permute2F128<1, 0>(in0);
                tmp6 = Permute2F128<1, 0>(in1);
                tmp7 = Permute2F128<1, 0>(in2);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in2);
                tmp5 = Permute2F128<1, 0>(in3);
                tmp6 = in0;
                tmp7 = in1;
            }
            else
            {
                tmp4 = in2;
                tmp5 = in3;
                tmp6 = Permute2F128<1, 0>(in0);
                tmp7 = Permute2F128<1, 0>(in1);
            }
        }
        else
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in1);
                tmp5 = Permute2F128<1, 0>(in2);
                tmp6 = Permute2F128<1, 0>(in3);
                tmp7 = in0;
            }
            else
            {
                tmp4 = in1;
                tmp5 = in2;
                tmp6 = in3;
                tmp7 = Permute2F128<1, 0>(in0);
            }
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movelh(tmp8, tmp10);
        tmp1 = _mm_movehl(tmp10, tmp8);
        tmp2 = _mm_movelh(tmp9, tmp11);
    }
    else if constexpr (laneOffsetIn == 1)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;

        if constexpr (laneOffsetOut == 0)
        {
            if constexpr (laneIn == laneOut)
            {
                tmp4 = in0;
                tmp5 = in1;
                tmp6 = in2;
                tmp7 = in3;
            }
            else
            {
                tmp4 = Permute2F128<1, 0>(in0);
                tmp5 = Permute2F128<1, 0>(in1);
                tmp6 = Permute2F128<1, 0>(in2);
                tmp7 = Permute2F128<1, 0>(in3);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = in0;
                tmp6 = in1;
                tmp7 = in2;
            }
            else
            {
                tmp4 = in3;
                tmp5 = Permute2F128<1, 0>(in0);
                tmp6 = Permute2F128<1, 0>(in1);
                tmp7 = Permute2F128<1, 0>(in2);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in2);
                tmp5 = Permute2F128<1, 0>(in3);
                tmp6 = in0;
                tmp7 = in1;
            }
            else
            {
                tmp4 = in2;
                tmp5 = in3;
                tmp6 = Permute2F128<1, 0>(in0);
                tmp7 = Permute2F128<1, 0>(in1);
            }
        }
        else
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in1);
                tmp5 = Permute2F128<1, 0>(in2);
                tmp6 = Permute2F128<1, 0>(in3);
                tmp7 = in0;
            }
            else
            {
                tmp4 = in1;
                tmp5 = in2;
                tmp6 = in3;
                tmp7 = Permute2F128<1, 0>(in0);
            }
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movehl(tmp10, tmp8);
        tmp1 = _mm_movelh(tmp9, tmp11);
        tmp2 = _mm_movehl(tmp11, tmp9);
    }
    else if constexpr (laneOffsetIn == 2)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;

        if constexpr (laneOffsetOut == 0)
        {
            tmp4 = in0;
            tmp5 = in1;
            tmp6 = in2;
            tmp7 = in3;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp4 = Permute2F128<1, 0>(in3);
            tmp5 = in0;
            tmp6 = in1;
            tmp7 = in2;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp4 = Permute2F128<1, 0>(in2);
            tmp5 = Permute2F128<1, 0>(in3);
            tmp6 = in0;
            tmp7 = in1;
        }
        else
        {
            tmp4 = in1;
            tmp5 = in2;
            tmp6 = in3;
            tmp7 = Permute2F128<1, 0>(in0);
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movelh(tmp9, tmp11);
        tmp1 = _mm_movehl(tmp11, tmp9);
        tmp2 = _mm_movelh(tmp8, tmp10);

        if constexpr (laneOffsetOut < 3)
        {
            if constexpr (laneOut == 0)
            {
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else
        {
            tmp0 = Permute2F128<1, 0>(tmp0);
            tmp1 = Permute2F128<1, 0>(tmp1);
        }
    }
    else
    {
        __m256 tmp4, tmp5, tmp6, tmp7;

        if constexpr (laneOffsetOut == 0)
        {
            tmp4 = in0;
            tmp5 = in1;
            tmp6 = in2;
            tmp7 = in3;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp4 = Permute2F128<1, 0>(in3);
            tmp5 = in0;
            tmp6 = in1;
            tmp7 = in2;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp4 = in2;
            tmp5 = in3;
            tmp6 = Permute2F128<1, 0>(in0);
            tmp7 = Permute2F128<1, 0>(in1);
        }
        else
        {
            tmp4 = in1;
            tmp5 = in2;
            tmp6 = in3;
            tmp7 = Permute2F128<1, 0>(in0);
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movehl(tmp11, tmp9);
        tmp1 = _mm_movelh(tmp8, tmp10);
        tmp2 = _mm_movehl(tmp10, tmp8);

        if constexpr (laneOffsetOut < 2)
        {
            if constexpr (laneOut == 0)
            {
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
            }
        }
        else
        {
            tmp0 = Permute2F128<1, 0>(tmp0);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2;

    if constexpr (laneOffsetIn == 0)
    {


        __m256 tmp4, tmp5, tmp6, tmp7;
        if constexpr (laneOffsetOut == 0)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp5 = in1;
                tmp6 = in2;
                tmp7 = in3;
            }
            else
            {
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp5 = Permute2F128<1, 0>(in1);
                tmp6 = Permute2F128<1, 0>(in2);
                tmp7 = Permute2F128<1, 0>(in3);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp6 = in1;
                tmp7 = in2;
            }
            else
            {
                tmp4 = in3;
                tmp5 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp6 = Permute2F128<1, 0>(in1);
                tmp7 = Permute2F128<1, 0>(in2);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in2);
                tmp5 = Permute2F128<1, 0>(in3);
                tmp6 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp7 = in1;
            }
            else
            {
                tmp4 = in2;
                tmp5 = in3;
                tmp6 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp7 = Permute2F128<1, 0>(in1);
            }
        }
        else
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in1);
                tmp5 = Permute2F128<1, 0>(in2);
                tmp6 = Permute2F128<1, 0>(in3);
                tmp7 = Permute2F128<0, 0, 1, 0>(in0, in4);
            }
            else
            {
                tmp4 = in1;
                tmp5 = in2;
                tmp6 = in3;
                tmp7 = Permute2F128<0, 1, 1, 1>(in0, in4);
            }
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movelh(tmp8, tmp10);
        tmp1 = _mm_movehl(tmp10, tmp8);
        tmp2 = _mm_movelh(tmp9, tmp11);
    }
    else if constexpr (laneOffsetIn == 1)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;

        if constexpr (laneOffsetOut == 0)
        {
            if constexpr (laneIn == laneOut)
            {
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp5 = in1;
                tmp6 = in2;
                tmp7 = in3;
            }
            else
            {
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp5 = Permute2F128<1, 0>(in1);
                tmp6 = Permute2F128<1, 0>(in2);
                tmp7 = Permute2F128<1, 0>(in3);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp6 = in1;
                tmp7 = in2;
            }
            else
            {
                tmp4 = in3;
                tmp5 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp6 = Permute2F128<1, 0>(in1);
                tmp7 = Permute2F128<1, 0>(in2);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in2);
                tmp5 = Permute2F128<1, 0>(in3);
                tmp6 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp7 = in1;
            }
            else
            {
                tmp4 = in2;
                tmp5 = in3;
                tmp6 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp7 = Permute2F128<1, 0>(in1);
            }
        }
        else
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in1);
                tmp5 = Permute2F128<1, 0>(in2);
                tmp6 = Permute2F128<1, 0>(in3);
                tmp7 = Permute2F128<0, 0, 1, 0>(in0, in4);
            }
            else
            {
                tmp4 = in1;
                tmp5 = in2;
                tmp6 = in3;
                tmp7 = Permute2F128<0, 1, 1, 1>(in0, in4);
            }
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movehl(tmp10, tmp8);
        tmp1 = _mm_movelh(tmp9, tmp11);
        tmp2 = _mm_movehl(tmp11, tmp9);
    }
    else if constexpr (laneOffsetIn == 2)
    {
        __m256 tmp3, tmp4, tmp5, tmp6, tmp7;



        if constexpr (laneOffsetOut == 0)
        {
            tmp3 = Permute2F128<1, 0>(in4);
            tmp4 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in0);
            tmp5 = in1;
            tmp6 = in2;
            tmp7 = in3;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp3 = Permute2F128<1, 0>(in4);
            tmp4 = Permute2F128<1, 0>(in3);
            tmp5 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in0);
            tmp6 = in1;
            tmp7 = in2;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp3 = Permute2F128<1, 0>(in4);
            tmp4 = Permute2F128<1, 0>(in2);
            tmp5 = Permute2F128<1, 0>(in3);
            tmp6 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in0);
            tmp7 = in1;
        }
        else
        {
            tmp3 = Permute2F128<1, 0>(in0);
            tmp4 = in1;
            tmp5 = in2;
            tmp6 = in3;
            tmp7 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in4);
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movelh(tmp9, tmp11);
        tmp1 = _mm_movehl(tmp11, tmp9);
        tmp2 = _mm_movelh(tmp8, tmp10);

        if constexpr (laneOffsetOut < 3)
        {
            tmp2 = Permute2F128<1, 0>(tmp2);
        }
        else
        {
            tmp0 = Permute2F128<1, 0>(tmp0);
            tmp1 = Permute2F128<1, 0>(tmp1);
        }
    }
    else
    {
        __m256 tmp3, tmp4, tmp5, tmp6, tmp7;

        if constexpr (laneOffsetOut == 0)
        {
            tmp3 = Permute2F128<1, 0>(in4);
            tmp4 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in0);
            tmp5 = in1;
            tmp6 = in2;
            tmp7 = in3;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp3 = Permute2F128<1, 0>(in4);
            tmp4 = Permute2F128<1, 0>(in3);
            tmp5 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in0);
            tmp6 = in1;
            tmp7 = in2;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp4 = in2;
            tmp5 = in3;
            tmp7 = Permute2F128<1, 0>(in1);
            tmp3 = Permute2F128<1, 0>(in0);
            tmp6 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in4);
        }
        else
        {
            tmp3 = Permute2F128<1, 0>(in0);
            tmp4 = in1;
            tmp5 = in2;
            tmp6 = in3;
            tmp7 = BlendInRange<_firstRowIn, _firstRowIn + 3>(tmp3, in4);
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movehl(tmp11, tmp9);
        tmp1 = _mm_movelh(tmp8, tmp10);
        tmp2 = _mm_movehl(tmp10, tmp8);

        if constexpr (laneOffsetOut < 2)
        {
            tmp1 = Permute2F128<1, 0>(tmp1);
            tmp2 = Permute2F128<1, 0>(tmp2);
        }
        else
        {
            tmp0 = Permute2F128<1, 0>(tmp0);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;

    __m256 tmp0, tmp1, tmp2;


    if constexpr (laneOffsetIn < 2)
    {
        __m256 tmp3, tmp4, tmp5, tmp6;
        if constexpr (laneIn == 0)
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp3 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp4 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp5 = in2;
                tmp6 = in3;
            }
            else if constexpr (_firstRowOut == 1)
            {
                tmp3 = Permute2F128<1, 0>(in3);
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp6 = in2;
            }
            else
            {
                tmp3 = Permute2F128<1, 0>(in2);
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp6 = Permute2F128<0, 0, 1, 0>(in1, in5);
            }
        }
        else
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp3 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp4 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp5 = Permute2F128<1, 0>(in2);
                tmp6 = Permute2F128<1, 0>(in3);
            }
            else if constexpr (_firstRowOut == 1)
            {
                tmp3 = in3;
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp5 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp6 = Permute2F128<1, 0>(in2);
            }
            else
            {
                tmp3 = in2;
                tmp4 = in3;
                tmp5 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp6 = Permute2F128<0, 1, 1, 1>(in1, in5);
            }
        }

        Transpose3x4<laneOffsetIn, 0>(tmp3, tmp4, tmp5, tmp6, tmp0, tmp1, tmp2);
    }
    else
    {
        __m256 tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;

        if constexpr (_firstRowOut == 0)
        {
            tmp3 = Permute2F128<0, 0, 1, 0>(in0, in4);
            tmp4 = Permute2F128<0, 0, 1, 0>(in1, in5);
            tmp5 = in2;
            tmp6 = in3;

            tmp7 = Permute2F128<0, 1, 1, 1>(in0, in4);
            tmp8 = Permute2F128<0, 1, 1, 1>(in1, in5);
            tmp9 = Permute2F128<1, 0>(in2);
            tmp10 = Permute2F128<1, 0>(in3);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp3 = Permute2F128<1, 0>(in3);
            tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
            tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
            tmp6 = in2;

            tmp7 = in3;
            tmp8 = Permute2F128<0, 1, 1, 1>(in0, in4);
            tmp9 = Permute2F128<0, 1, 1, 1>(in1, in5);
            tmp10 = Permute2F128<1, 0>(in2);
        }
        else
        {
            tmp3 = Permute2F128<1, 0>(in2);
            tmp4 = Permute2F128<1, 0>(in3);
            tmp5 = Permute2F128<0, 0, 1, 0>(in0, in4);
            tmp6 = Permute2F128<0, 0, 1, 0>(in1, in5);

            tmp7 = in2;
            tmp8 = in3;
            tmp9 = Permute2F128<0, 1, 1, 1>(in0, in4);
            tmp10 = Permute2F128<0, 1, 1, 1>(in1, in5);
        }
        if constexpr (_firstRowIn == 2)
        {
            Transpose2x4<2, 0>(tmp3, tmp4, tmp5, tmp6, tmp0, tmp1);
            Transpose1x4<0, 0>(tmp7, tmp8, tmp9, tmp10, tmp2);
        }
        else
        {
            Transpose1x4<3, 0>(tmp3, tmp4, tmp5, tmp6, tmp0);
            Transpose2x4<0, 0>(tmp7, tmp8, tmp9, tmp10, tmp1, tmp2);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;

    __m256 tmp0, tmp1, tmp2;

    if constexpr (laneOffsetIn < 2)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;
        if constexpr (laneIn == 0)
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp6 = Permute2F128<0, 0, 1, 0>(in2, in6);
                tmp7 = in3;
            }
            else
            {
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp6 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp7 = Permute2F128<0, 0, 1, 0>(in2, in6);
            }
        }
        else
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp5 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp6 = Permute2F128<0, 1, 1, 1>(in2, in6);
                tmp7 = Permute2F128<1, 0>(in3);
            }
            else
            {
                tmp4 = in3;
                tmp5 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp6 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp7 = Permute2F128<0, 1, 1, 1>(in2, in6);
            }
        }
        Transpose3x4<laneOffsetIn, 0>(tmp4, tmp5, tmp6, tmp7, tmp0, tmp1, tmp2);
    }
    else
    {
        __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;

        if constexpr (_firstRowOut == 0)
        {
            Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
            Transpose4x3<0, 0>(in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
        }
        else
        {
            Transpose4x3<0, 1>(in0, in1, in2, tmp6, tmp7, tmp8, tmp9);
            Transpose4x4<0, 0>(in3, in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
        }

        if constexpr (_firstRowIn == 2)
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
            tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        }
        else
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp1 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
            tmp2 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;

    __m256 tmp0, tmp1, tmp2;

    if constexpr (laneOffsetIn < 2)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;
        if constexpr (laneIn == 0)
        {

            tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
            tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
            tmp6 = Permute2F128<0, 0, 1, 0>(in2, in6);
            tmp7 = Permute2F128<0, 0, 1, 0>(in3, in7);
        }
        else
        {

            tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
            tmp5 = Permute2F128<0, 1, 1, 1>(in1, in5);
            tmp6 = Permute2F128<0, 1, 1, 1>(in2, in6);
            tmp7 = Permute2F128<0, 1, 1, 1>(in3, in7);
        }
        Transpose3x4<laneOffsetIn, 0>(tmp4, tmp5, tmp6, tmp7, tmp0, tmp1, tmp2);
    }
    else
    {
        __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;


        Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
        Transpose4x4<0, 0>(in4, in5, in6, in7, tmp10, tmp11, tmp12, tmp13);


        if constexpr (_firstRowIn == 2)
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
            tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        }
        else
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp1 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
            tmp2 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        }
    }


    // Write to output registers
    out0 = tmp0;
    out1 = tmp1;
    out2 = tmp2;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3;

    if constexpr (laneOffsetIn == 0)
    {
        __m256 tmp4;

        if constexpr (laneOut != laneIn)
            tmp4 = Permute2F128<1, 0>(in0);
        else
            tmp4 = in0;

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = tmp4;
            tmp1 = _mm_movehdup(tmp4);
            tmp2 = _mm_unpackhi(tmp4, tmp4);
            tmp3 = Broadcast<3>(tmp4);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = _mm_moveldup(tmp4);
            tmp1 = tmp4;
            tmp2 = _mm_unpackhi(tmp4, tmp4);
            tmp3 = _mm_unpackhi(tmp2, tmp2);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp3 = _mm_movehdup(tmp4);
            tmp2 = tmp4;
            tmp1 = _mm_unpacklo(tmp4, tmp4);
            tmp0 = _mm_unpacklo(tmp1, tmp1);
        }
        else
        {
            tmp3 = tmp4;
            tmp2 = _mm_moveldup(tmp4);
            tmp1 = _mm_unpacklo(tmp4, tmp4);
            tmp0 = _mm_unpacklo(tmp1, tmp1);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        __m256 tmp4, tmp5;
        if constexpr (laneOut == laneIn)
        {
            tmp4 = in0;
            tmp5 = Permute2F128<1, 0>(in0);
        }
        else
        {
            tmp4 = Permute2F128<1, 0>(in0);
            tmp5 = in0;
        }

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = _mm_movehdup(tmp4);
            tmp1 = _mm_unpackhi(tmp4, tmp4);
            tmp2 = _mm_unpackhi(tmp1, tmp1);
            tmp3 = tmp5;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = tmp4;
            tmp1 = _mm_unpackhi(tmp4, tmp4);
            tmp2 = _mm_unpackhi(tmp1, tmp1);
            tmp3 = _mm_moveldup(tmp5);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp0 = _mm_unpacklo(tmp4, tmp4);
            tmp1 = tmp4;
            tmp2 = _mm_movehdup(tmp4);
            tmp3 = Permute<2, 3, 0, 1>(tmp5);
        }
        else
        {
            tmp0 = _mm_unpacklo(tmp4, tmp4);
            tmp1 = _mm_moveldup(tmp4);
            tmp2 = tmp4;
            tmp3 = Broadcast<0>(tmp5);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        __m256 tmp4, tmp5;
        if constexpr (laneOut == laneIn)
        {
            tmp4 = in0;
            tmp5 = Permute2F128<1, 0>(in0);
        }
        else
        {
            tmp4 = Permute2F128<1, 0>(in0);
            tmp5 = in0;
        }

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = _mm_unpackhi(tmp4, tmp4);
            tmp1 = _mm_unpackhi(tmp0, tmp0);
            tmp2 = tmp5;
            tmp3 = _mm_movehdup(tmp5);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = _mm_unpackhi(tmp4, tmp4);
            tmp1 = _mm_unpackhi(tmp0, tmp0);
            tmp2 = _mm_moveldup(tmp5);
            tmp3 = tmp5;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp0 = tmp4;
            tmp1 = _mm_movehdup(tmp4);
            tmp2 = Permute<2, 3, 0, 1>(tmp5);
            tmp3 = _mm_movehdup(tmp2);
        }
        else
        {
            tmp0 = _mm_moveldup(tmp4);
            tmp1 = tmp4;
            tmp2 = Permute<0, 1, 1, 0>(tmp5);
            tmp3 = _mm_moveldup(tmp2);
        }
    }
    else
    {
        __m256 tmp4, tmp5;
        if constexpr (laneOut == laneIn)
        {
            tmp4 = in0;
            tmp5 = Permute2F128<1, 0>(in0);
        }
        else
        {
            tmp4 = Permute2F128<1, 0>(in0);
            tmp5 = in0;
        }

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = Broadcast<3>(tmp4);
            tmp1 = tmp5;
            tmp2 = _mm_movehdup(tmp5);
            tmp3 = _mm_unpackhi(tmp5, tmp5);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp0 = Broadcast<3>(tmp4);
            tmp1 = _mm_moveldup(tmp5);
            tmp2 = tmp5;
            tmp3 = _mm_unpackhi(tmp5, tmp5);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp3 = tmp5;
            tmp2 = _mm_unpacklo(tmp5, tmp5);
            tmp1 = _mm_unpacklo(tmp2, tmp2);
            tmp0 = _mm_movehdup(tmp4);
        }
        else
        {
            tmp3 = _mm_moveldup(tmp5);
            tmp2 = _mm_unpacklo(tmp5, tmp5);
            tmp1 = _mm_unpacklo(tmp2, tmp2);
            tmp0 = tmp4;
        }
    }



    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3;


    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut < 3)
        {
            __m256 tmp4, tmp5;

            if constexpr (laneIn == laneOut)
            {
                tmp4 = in0;
                tmp5 = in1;
            }
            else
            {
                tmp4 = Permute2F128<1, 0>(in0);
                tmp5 = Permute2F128<1, 0>(in1);
            }

            if constexpr (laneOffsetOut == 0)
            {
                tmp0 = _mm_unpacklo(tmp4, tmp5);
                tmp1 = Permute<2, 3, 0, 1>(tmp0);
                tmp2 = _mm_unpackhi(tmp4, tmp5);
                tmp3 = Permute<2, 3, 0, 1>(tmp2);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                tmp0 = Shuffle<0, 0, 0, 0>(tmp4, tmp5);
                tmp1 = Shuffle<1, 1, 1, 1>(tmp4, tmp5);
                tmp2 = Shuffle<2, 2, 2, 2>(tmp4, tmp5);
                tmp3 = Shuffle<3, 3, 3, 3>(tmp4, tmp5);
            }
            else
            {
                tmp3 = _mm_unpackhi(tmp4, tmp5);
                tmp2 = Permute<2, 3, 0, 1>(tmp3);
                tmp1 = _mm_unpacklo(tmp4, tmp5);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
            }
        }
        else
        {
            __m256 tmp4;
            if constexpr (laneIn == 0)
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in1);
            else
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in1);

            tmp0 = Broadcast<0>(tmp4);
            tmp1 = Broadcast<1>(tmp4);
            tmp2 = Broadcast<2>(tmp4);
            tmp3 = Broadcast<3>(tmp4);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneOffsetOut < 3)
        {
            if constexpr (laneOffsetOut == 0)
            {
                tmp1 = _mm_unpackhi(in0, in1);
                tmp2 = Permute<2, 3, 0, 1>(tmp1);
                tmp3 = _mm_unpacklo(in0, in1);
                tmp0 = Permute<2, 3, 0, 1>(tmp3);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                tmp3 = Shuffle<0, 0, 0, 0>(in0, in1);
                tmp0 = Shuffle<1, 1, 1, 1>(in0, in1);
                tmp1 = Shuffle<2, 2, 2, 2>(in0, in1);
                tmp2 = Shuffle<3, 3, 3, 3>(in0, in1);
            }
            else
            {
                tmp2 = _mm_unpackhi(in0, in1);
                tmp1 = Permute<2, 3, 0, 1>(tmp2);
                tmp0 = _mm_unpacklo(in0, in1);
                tmp3 = Permute<2, 3, 0, 1>(tmp0);
            }


            if constexpr (laneOut == 0)
            {
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
        }
        else
        {
            __m256 tmp4 = Permute2F128<0, 1, 1, 0>(in0, in1);

            tmp0 = Shuffle<1, 1, 1, 1>(tmp4, in0);
            tmp1 = Shuffle<2, 2, 2, 2>(tmp4, in0);
            tmp2 = Shuffle<3, 3, 3, 3>(tmp4, in0);
            tmp3 = Shuffle<0, 0, 0, 0>(in1, tmp4);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (laneOffsetOut < 3)
        {
            if constexpr (laneOffsetOut == 0)
            {
                tmp0 = _mm_unpackhi(in0, in1);
                tmp1 = Permute<2, 3, 0, 1>(tmp0);
                tmp2 = _mm_unpacklo(in0, in1);
                tmp3 = Permute<2, 3, 0, 1>(tmp2);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                tmp2 = Shuffle<0, 0, 0, 0>(in0, in1);
                tmp3 = Shuffle<1, 1, 1, 1>(in0, in1);
                tmp0 = Shuffle<2, 2, 2, 2>(in0, in1);
                tmp1 = Shuffle<3, 3, 3, 3>(in0, in1);
            }
            else
            {
                tmp1 = _mm_unpackhi(in0, in1);
                tmp0 = Permute<2, 3, 0, 1>(tmp1);
                tmp3 = _mm_unpacklo(in0, in1);
                tmp2 = Permute<2, 3, 0, 1>(tmp3);
            }


            if constexpr (laneOut == 0)
            {
                tmp2 = Permute2F128<1, 0>(tmp2);
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else
        {
            __m256 tmp4 = Permute2F128<0, 1, 1, 0>(in0, in1);

            tmp0 = Shuffle<2, 2, 2, 2>(tmp4, in0);
            tmp1 = Shuffle<3, 3, 3, 3>(tmp4, in0);
            tmp2 = Shuffle<0, 0, 0, 0>(in1, tmp4);
            tmp3 = Shuffle<1, 1, 1, 1>(in1, tmp4);
        }
    }
    else
    {
        if constexpr (laneOffsetOut < 3)
        {
            if constexpr (laneOffsetOut == 0)
            {
                tmp1 = _mm_unpacklo(in0, in1);
                tmp2 = Permute<2, 3, 0, 1>(tmp1);
                tmp3 = _mm_unpackhi(in0, in1);
                tmp0 = Permute<2, 3, 0, 1>(tmp3);
            }
            else if constexpr (laneOffsetOut == 1)
            {
                tmp1 = Shuffle<0, 0, 0, 0>(in0, in1);
                tmp2 = Shuffle<1, 1, 1, 1>(in0, in1);
                tmp3 = Shuffle<2, 2, 2, 2>(in0, in1);
                tmp0 = Shuffle<3, 3, 3, 3>(in0, in1);
            }
            else
            {
                tmp0 = _mm_unpackhi(in0, in1);
                tmp3 = Permute<2, 3, 0, 1>(tmp0);
                tmp2 = _mm_unpacklo(in0, in1);
                tmp1 = Permute<2, 3, 0, 1>(tmp2);
            }


            if constexpr (laneOut == 0)
            {
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
            }
        }
        else
        {
            __m256 tmp4 = Permute2F128<0, 1, 1, 0>(in0, in1);

            tmp0 = Shuffle<3, 3, 3, 3>(tmp4, in0);
            tmp1 = Shuffle<0, 0, 0, 0>(in1, tmp4);
            tmp2 = Shuffle<1, 1, 1, 1>(in1, tmp4);
            tmp3 = Shuffle<2, 2, 2, 2>(in1, tmp4);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3;

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut < 2)
        {
            __m256 tmp4, tmp5, tmp6;

            if constexpr (laneIn == laneOut)
            {
                tmp4 = in0;
                tmp5 = in1;
                tmp6 = in2;
            }
            else
            {
                tmp4 = Permute2F128<1, 0>(in0);
                tmp5 = Permute2F128<1, 0>(in1);
                tmp6 = Permute2F128<1, 0>(in2);
            }

            if constexpr (laneOffsetOut == 0)
            {
                __m256 tmp7 = _mm_unpacklo(tmp4, tmp5);
                __m256 tmp8 = _mm_unpackhi(tmp4, tmp5);

                tmp0 = _mm_movelh(tmp7, tmp6);
                tmp1 = Shuffle<2, 3, 1, 0>(tmp7, tmp6);
                tmp2 = Blend<0, 0, 1, 0, 0, 0, 1, 0>(tmp8, tmp6);
                tmp3 = Shuffle<2, 3, 3, 2>(tmp8, tmp6);
            }
            else
            {
                __m256 tmp7 = _mm_unpacklo(tmp5, tmp6);
                __m256 tmp8 = _mm_unpackhi(tmp5, tmp6);

                tmp0 = Shuffle<1, 0, 0, 1>(tmp4, tmp7);
                tmp1 = Blend<0, 1, 0, 0, 0, 1, 0, 0>(tmp7, tmp4);
                tmp2 = Shuffle<3, 2, 0, 1>(tmp4, tmp8);
                tmp3 = _mm_movehl(tmp8, tmp4);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp4, tmp5;
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in2);
                tmp5 = in1;
            }
            else
            {
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in2);
                tmp5 = Permute2F128<1, 0>(in1);
            }

            __m256 tmp6 = _mm_unpacklo(tmp4, tmp5);
            __m256 tmp7 = _mm_unpackhi(tmp4, tmp5);

            tmp0 = Permute<0, 1, 0, 1>(tmp6);
            tmp1 = Permute<2, 3, 2, 3>(tmp6);
            tmp2 = Permute<0, 1, 0, 1>(tmp7);
            tmp3 = Permute<2, 3, 2, 3>(tmp7);
        }
        else
        {
            __m256 tmp4, tmp5;
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in1);
                tmp5 = Permute2F128<0, 0, 1, 0>(in0, in2);
            }
            else
            {
                tmp4 = in1;
                tmp5 = Permute2F128<0, 1, 1, 1>(in0, in2);
            }

            __m256 tmp6 = _mm_unpacklo(tmp4, tmp5);
            __m256 tmp7 = _mm_unpackhi(tmp4, tmp5);

            tmp0 = Permute<0, 1, 0, 1>(tmp6);
            tmp1 = Permute<2, 3, 2, 3>(tmp6);
            tmp2 = Permute<0, 1, 0, 1>(tmp7);
            tmp3 = Permute<2, 3, 2, 3>(tmp7);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp4 = _mm_unpacklo(in0, in1);
            __m256 tmp5 = _mm_unpackhi(in0, in1);

            tmp0 = Shuffle<2, 3, 1, 0>(tmp4, in2);
            tmp1 = Blend<0, 0, 1, 0, 0, 0, 1, 0>(tmp5, in2);
            tmp2 = Shuffle<2, 3, 3, 2>(tmp5, in2);
            tmp3 = _mm_movelh(tmp4, in2);

            if constexpr (laneOut == 0)
            {
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp4 = _mm_unpacklo(in1, in2);
            __m256 tmp5 = _mm_unpackhi(in1, in2);

            tmp0 = Blend<0, 1, 0, 0, 0, 1, 0, 0>(tmp4, in0);
            tmp1 = Shuffle<3, 2, 0, 1>(in0, tmp5);
            tmp2 = _mm_movehl(tmp5, in0);
            tmp3 = Shuffle<1, 0, 0, 1>(in0, tmp4);
            if constexpr (laneOut == 0)
            {
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp4 = _mm_unpacklo(in0, in1);
            __m256 tmp5 = _mm_unpackhi(in0, in1);
            __m256 tmp6 = Permute2F128<0, 1, 1, 0>(tmp4, in2);

            tmp0 = Shuffle<1, 0, 2, 3>(tmp6, tmp4);
            tmp1 = Shuffle<2, 3, 0, 1>(tmp6, tmp5);
            tmp2 = Shuffle<3, 2, 2, 3>(tmp6, tmp5);
            tmp3 = _mm_movelh(in2, tmp6);
        }
        else
        {
            __m256 tmp4 = _mm_unpacklo(in1, in2);
            __m256 tmp5 = _mm_unpackhi(in1, in2);
            __m256 tmp6 = Permute2F128<1, 0>(tmp4);
            __m256 tmp7 = Permute2F128<0, 1, 1, 0>(in0, tmp5);

            tmp0 = Shuffle<2, 3, 0, 1>(tmp6, in0);
            tmp1 = Shuffle<0, 1, 3, 2>(tmp7, in0);
            tmp2 = _mm_movehl(in0, tmp7);
            tmp3 = Shuffle<0, 1, 1, 0>(tmp4, tmp7);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp4 = _mm_unpacklo(in0, in1);
            __m256 tmp5 = _mm_unpackhi(in0, in1);

            tmp0 = Blend<0, 0, 1, 0, 0, 0, 1, 0>(tmp5, in2);
            tmp1 = Shuffle<2, 3, 3, 2>(tmp5, in2);
            tmp2 = _mm_movelh(tmp4, in2);
            tmp3 = Shuffle<2, 3, 1, 0>(tmp4, in2);

            if constexpr (laneOut == 0)
            {
                tmp2 = Permute2F128<1, 0>(tmp2);
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp4 = _mm_unpacklo(in1, in2);
            __m256 tmp5 = _mm_unpackhi(in1, in2);

            tmp0 = Shuffle<3, 2, 0, 1>(in0, tmp5);
            tmp1 = _mm_movehl(tmp5, in0);
            tmp2 = Shuffle<1, 0, 0, 1>(in0, tmp4);
            tmp3 = Blend<0, 1, 0, 0, 0, 1, 0, 0>(tmp4, in0);

            if constexpr (laneOut == 0)
            {
                tmp2 = Permute2F128<1, 0>(tmp2);
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp4 = _mm_unpacklo(in0, in1);
            __m256 tmp5 = _mm_unpackhi(in0, in1);
            __m256 tmp6 = Permute2F128<0, 1, 1, 0>(tmp4, in2);

            tmp0 = Shuffle<2, 3, 0, 1>(tmp6, tmp5);
            tmp1 = Shuffle<3, 2, 2, 3>(tmp6, tmp5);
            tmp2 = _mm_movelh(in2, tmp6);
            tmp3 = Shuffle<1, 0, 2, 3>(in2, tmp6);
        }
        else
        {
            __m256 tmp4 = _mm_unpacklo(in1, in2);
            __m256 tmp5 = _mm_unpackhi(in1, in2);
            __m256 tmp6 = Permute2F128<0, 1, 1, 0>(in0, tmp5);

            tmp0 = Shuffle<0, 1, 3, 2>(tmp6, in0);
            tmp1 = _mm_movehl(in0, tmp6);
            tmp2 = Shuffle<0, 1, 1, 0>(tmp4, tmp6);
            tmp3 = Shuffle<2, 3, 0, 1>(tmp4, tmp6);
        }
    }
    else
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp4 = _mm_unpacklo(in0, in1);
            __m256 tmp5 = _mm_unpackhi(in0, in1);

            tmp0 = Shuffle<2, 3, 3, 2>(tmp5, in2);
            tmp1 = _mm_movelh(tmp4, in2);
            tmp2 = Shuffle<2, 3, 1, 0>(tmp4, in2);
            tmp3 = Blend<0, 0, 1, 0, 0, 0, 1, 0>(tmp5, in2);

            if constexpr (laneOut == 0)
            {
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp4 = _mm_unpacklo(in1, in2);
            __m256 tmp5 = _mm_unpackhi(in1, in2);

            tmp0 = Shuffle<2, 3, 2, 3>(in0, tmp5);
            tmp1 = Shuffle<1, 0, 0, 1>(in0, tmp4);
            tmp2 = Blend<0, 1, 0, 0, 0, 1, 0, 0>(tmp4, in0);
            tmp3 = Shuffle<3, 2, 0, 1>(in0, tmp5);

            if constexpr (laneOut == 0)
            {
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp4 = _mm_unpacklo(in0, in1);
            __m256 tmp5 = _mm_unpackhi(in0, in1);
            __m256 tmp6 = Permute2F128<1, 0>(tmp4);
            __m256 tmp7 = Permute2F128<0, 1, 1, 0>(tmp5, in2);

            tmp0 = Shuffle<3, 2, 2, 3>(tmp7, tmp5);
            tmp1 = _mm_movelh(in2, tmp6);
            tmp2 = Shuffle<1, 0, 2, 3>(in2, tmp6);
            tmp3 = Shuffle<2, 3, 0, 1>(in2, tmp7);
        }
        else
        {
            __m256 tmp4 = _mm_unpacklo(in1, in2);
            __m256 tmp5 = _mm_unpackhi(in1, in2);
            __m256 tmp6 = Permute2F128<0, 1, 1, 0>(in0, tmp5);

            tmp0 = _mm_movehl(in0, tmp6);
            tmp1 = Shuffle<0, 1, 1, 0>(tmp4, tmp6);
            tmp2 = Shuffle<2, 3, 0, 1>(tmp4, tmp6);
            tmp3 = Shuffle<0, 1, 3, 2>(tmp5, tmp6);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3;

    if constexpr (laneOffsetIn == 0)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;
        if constexpr (laneOffsetOut == 0)
        {
            if constexpr (laneIn == laneOut)
            {
                tmp4 = in0;
                tmp5 = in1;
                tmp6 = in2;
                tmp7 = in3;
            }
            else
            {
                tmp4 = Permute2F128<1, 0>(in0);
                tmp5 = Permute2F128<1, 0>(in1);
                tmp6 = Permute2F128<1, 0>(in2);
                tmp7 = Permute2F128<1, 0>(in3);
            }
        }
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = in0;
                tmp6 = in1;
                tmp7 = in2;
            }
            else
            {
                tmp4 = in3;
                tmp5 = Permute2F128<1, 0>(in0);
                tmp6 = Permute2F128<1, 0>(in1);
                tmp7 = Permute2F128<1, 0>(in2);
            }
        }
        else if constexpr (laneOffsetOut == 2)
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in2);
                tmp5 = Permute2F128<1, 0>(in3);
                tmp6 = in0;
                tmp7 = in1;
            }
            else
            {
                tmp4 = in2;
                tmp5 = in3;
                tmp6 = Permute2F128<1, 0>(in0);
                tmp7 = Permute2F128<1, 0>(in1);
            }
        }
        else
        {
            if constexpr (laneIn == 0)
            {
                tmp4 = Permute2F128<1, 0>(in1);
                tmp5 = Permute2F128<1, 0>(in2);
                tmp6 = Permute2F128<1, 0>(in3);
                tmp7 = in0;
            }
            else
            {
                tmp4 = in1;
                tmp5 = in2;
                tmp6 = in3;
                tmp7 = Permute2F128<1, 0>(in0);
            }
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movelh(tmp8, tmp10);
        tmp1 = _mm_movehl(tmp10, tmp8);
        tmp2 = _mm_movelh(tmp9, tmp11);
        tmp3 = _mm_movehl(tmp11, tmp9);
    }
    else if constexpr (laneOffsetIn == 1)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;

        if constexpr (laneOffsetOut == 0)
        {
            tmp4 = in0;
            tmp5 = in1;
            tmp6 = in2;
            tmp7 = in3;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp4 = Permute2F128<1, 0>(in3);
            tmp5 = in0;
            tmp6 = in1;
            tmp7 = in2;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp4 = Permute2F128<1, 0>(in2);
            tmp5 = Permute2F128<1, 0>(in3);
            tmp6 = in0;
            tmp7 = in1;
        }
        else
        {
            tmp4 = Permute2F128<1, 0>(in1);
            tmp5 = Permute2F128<1, 0>(in2);
            tmp6 = Permute2F128<1, 0>(in3);
            tmp7 = in0;
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movehl(tmp10, tmp8);
        tmp1 = _mm_movelh(tmp9, tmp11);
        tmp2 = _mm_movehl(tmp11, tmp9);
        tmp3 = _mm_movelh(tmp8, tmp10);

        if constexpr (laneOut == 0)
        {
            tmp3 = Permute2F128<1, 0>(tmp3);
        }
        else
        {
            tmp0 = Permute2F128<1, 0>(tmp0);
            tmp1 = Permute2F128<1, 0>(tmp1);
            tmp2 = Permute2F128<1, 0>(tmp2);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;

        if constexpr (laneOffsetOut == 0)
        {
            tmp4 = in0;
            tmp5 = in1;
            tmp6 = in2;
            tmp7 = in3;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp4 = Permute2F128<1, 0>(in3);
            tmp5 = in0;
            tmp6 = in1;
            tmp7 = in2;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp4 = Permute2F128<1, 0>(in2);
            tmp5 = Permute2F128<1, 0>(in3);
            tmp6 = in0;
            tmp7 = in1;
        }
        else
        {
            tmp4 = in1;
            tmp5 = in2;
            tmp6 = in3;
            tmp7 = Permute2F128<1, 0>(in0);
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movelh(tmp9, tmp11);
        tmp1 = _mm_movehl(tmp11, tmp9);
        tmp2 = _mm_movelh(tmp8, tmp10);
        tmp3 = _mm_movehl(tmp10, tmp8);

        if constexpr (laneOffsetOut < 3)
        {
            if constexpr (laneOut == 0)
            {
                tmp2 = Permute2F128<1, 0>(tmp2);
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else
        {
            tmp0 = Permute2F128<1, 0>(tmp0);
            tmp1 = Permute2F128<1, 0>(tmp1);
        }
    }
    else
    {
        __m256 tmp4, tmp5, tmp6, tmp7;

        if constexpr (laneOffsetOut == 0)
        {
            tmp4 = in0;
            tmp5 = in1;
            tmp6 = in2;
            tmp7 = in3;
        }
        else if constexpr (laneOffsetOut == 1)
        {
            tmp4 = Permute2F128<1, 0>(in3);
            tmp5 = in0;
            tmp6 = in1;
            tmp7 = in2;
        }
        else if constexpr (laneOffsetOut == 2)
        {
            tmp4 = in2;
            tmp5 = in3;
            tmp6 = Permute2F128<1, 0>(in0);
            tmp7 = Permute2F128<1, 0>(in1);
        }
        else
        {
            tmp4 = in1;
            tmp5 = in2;
            tmp6 = in3;
            tmp7 = Permute2F128<1, 0>(in0);
        }

        __m256 tmp8 = _mm_unpacklo(tmp4, tmp5);
        __m256 tmp9 = _mm_unpackhi(tmp4, tmp5);
        __m256 tmp10 = _mm_unpacklo(tmp6, tmp7);
        __m256 tmp11 = _mm_unpackhi(tmp6, tmp7);

        tmp0 = _mm_movehl(tmp11, tmp9);
        tmp1 = _mm_movelh(tmp8, tmp10);
        tmp2 = _mm_movehl(tmp10, tmp8);
        tmp3 = _mm_movelh(tmp9, tmp11);

        if constexpr (laneOffsetOut < 2)
        {
            if constexpr (laneOut == 0)
            {
                tmp1 = Permute2F128<1, 0>(tmp1);
                tmp2 = Permute2F128<1, 0>(tmp2);
                tmp3 = Permute2F128<1, 0>(tmp3);
            }
            else
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
            }
        }
        else
        {
            tmp0 = Permute2F128<1, 0>(tmp0);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4;

    if constexpr (_firstRowIn == 0)
    {
        tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
    }
    else if constexpr (_firstRowIn == 1)
    {
        __m256 tmp5 = Permute2F128<1, 0>(in4);
        tmp4 = BlendInRange<1, 4>(tmp5, in0);
    }
    else if constexpr (_firstRowIn == 2)
    {
        __m256 tmp5 = Permute2F128<1, 0>(in4);
        tmp4 = BlendInRange<2, 5>(tmp5, in0);
    }
    else if constexpr (_firstRowIn == 3)
    {
        __m256 tmp5 = Permute2F128<1, 0>(in4);
        tmp4 = BlendInRange<3, 6>(tmp5, in0);
    }
    else
    {
        tmp4 = Permute2F128<0, 1, 1, 1>(in4, in0);
    }


    Transpose4x4<_firstRowIn, _firstRowOut>(tmp4, in1, in2, in3, tmp0, tmp1, tmp2, tmp3);


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    if constexpr (_firstRowIn == 0)
    {
        tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
    }
    else if constexpr (_firstRowIn == 1)
    {
        __m256 tmp6 = Permute2F128<1, 0>(in4);
        __m256 tmp7 = Permute2F128<1, 0>(in5);
        tmp4 = BlendInRange<1, 4>(tmp6, in0);
        tmp5 = BlendInRange<1, 4>(tmp7, in1);
    }
    else if constexpr (_firstRowIn == 2)
    {
        __m256 tmp6 = Permute2F128<1, 0>(in4);
        __m256 tmp7 = Permute2F128<1, 0>(in5);
        tmp4 = BlendInRange<2, 5>(tmp6, in0);
        tmp5 = BlendInRange<2, 5>(tmp7, in1);
    }
    else if constexpr (_firstRowIn == 3)
    {
        __m256 tmp6 = Permute2F128<1, 0>(in4);
        __m256 tmp7 = Permute2F128<1, 0>(in5);
        tmp4 = BlendInRange<3, 6>(tmp6, in0);
        tmp5 = BlendInRange<3, 6>(tmp7, in1);
    }
    else
    {
        tmp4 = Permute2F128<0, 1, 1, 1>(in4, in0);
        tmp5 = Permute2F128<0, 1, 1, 1>(in5, in1);
    }
    Transpose4x4<_firstRowIn, _firstRowOut>(tmp4, tmp5, in2, in3, tmp0, tmp1, tmp2, tmp3);



    // Write to output registers
    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3;

    if constexpr (laneOffsetIn == 0)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;
        if constexpr (laneIn == 0)
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp6 = Permute2F128<0, 0, 1, 0>(in2, in6);
                tmp7 = in3;
            }
            else
            {
                tmp4 = Permute2F128<1, 0>(in3);
                tmp5 = Permute2F128<0, 0, 1, 0>(in0, in4);
                tmp6 = Permute2F128<0, 0, 1, 0>(in1, in5);
                tmp7 = Permute2F128<0, 0, 1, 0>(in2, in6);
            }
        }
        else
        {
            if constexpr (_firstRowOut == 0)
            {
                tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp5 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp6 = Permute2F128<0, 1, 1, 1>(in2, in6);
                tmp7 = Permute2F128<1, 0>(in3);
            }
            else
            {
                tmp4 = in3;
                tmp5 = Permute2F128<0, 1, 1, 1>(in0, in4);
                tmp6 = Permute2F128<0, 1, 1, 1>(in1, in5);
                tmp7 = Permute2F128<0, 1, 1, 1>(in2, in6);
            }
        }
        Transpose4x4(tmp4, tmp5, tmp6, tmp7, tmp0, tmp1, tmp2, tmp3);
    }
    else
    {
        __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;

        if constexpr (_firstRowOut == 0)
        {
            Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
            Transpose4x3<0, 0>(in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
        }
        else
        {
            Transpose4x3<0, 1>(in0, in1, in2, tmp6, tmp7, tmp8, tmp9);
            Transpose4x4<0, 0>(in3, in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
        }


        if constexpr (_firstRowIn == 1)
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
            tmp1 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
            tmp2 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp3 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        }
        else if constexpr (_firstRowIn == 2)
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
            tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
            tmp3 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        }
        else
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp1 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
            tmp2 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
            tmp3 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3;

    if constexpr (laneOffsetIn == 0)
    {
        __m256 tmp4, tmp5, tmp6, tmp7;
        if constexpr (laneIn == 0)
        {

            tmp4 = Permute2F128<0, 0, 1, 0>(in0, in4);
            tmp5 = Permute2F128<0, 0, 1, 0>(in1, in5);
            tmp6 = Permute2F128<0, 0, 1, 0>(in2, in6);
            tmp7 = Permute2F128<0, 0, 1, 0>(in3, in7);
        }
        else
        {

            tmp4 = Permute2F128<0, 1, 1, 1>(in0, in4);
            tmp5 = Permute2F128<0, 1, 1, 1>(in1, in5);
            tmp6 = Permute2F128<0, 1, 1, 1>(in2, in6);
            tmp7 = Permute2F128<0, 1, 1, 1>(in3, in7);
        }
        Transpose4x4(tmp4, tmp5, tmp6, tmp7, tmp0, tmp1, tmp2, tmp3);
    }
    else
    {
        __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;


        Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
        Transpose4x4<0, 0>(in4, in5, in6, in7, tmp10, tmp11, tmp12, tmp13);



        if constexpr (_firstRowIn == 1)
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
            tmp1 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
            tmp2 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp3 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        }
        else if constexpr (_firstRowIn == 2)
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
            tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
            tmp3 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        }
        else
        {
            tmp0 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
            tmp1 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
            tmp2 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
            tmp3 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
        }
    }


    // Write to output registers
    out0 = tmp0;
    out1 = tmp1;
    out2 = tmp2;
    out3 = tmp3;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;

    __m256 tmp0, tmp1, tmp2, tmp3, tmp4;
    __m256 tmp5 = Permute2F128<1, 0>(in0);

    if constexpr (_firstRowIn == _firstRowOut)
        tmp0 = in0;
    else if constexpr (_firstRowIn / numLaneVals == _firstRowOut / numLaneVals)
        tmp0 = Broadcast<_firstRowIn % numLaneVals>(in0);
    else
        tmp0 = Broadcast<_firstRowIn % numLaneVals>(tmp5);

    if constexpr (_firstRowIn + 1 == _firstRowOut)
        tmp1 = in0;
    else if constexpr ((_firstRowIn + 1) / numLaneVals == _firstRowOut / numLaneVals)
        tmp1 = Broadcast<(_firstRowIn + 1) % numLaneVals>(in0);
    else
        tmp1 = Broadcast<(_firstRowIn + 1) % numLaneVals>(tmp5);

    if constexpr (_firstRowIn + 2 == _firstRowOut)
        tmp2 = in0;
    else if constexpr ((_firstRowIn + 2) / numLaneVals == _firstRowOut / numLaneVals)
        tmp2 = Broadcast<(_firstRowIn + 2) % numLaneVals>(in0);
    else
        tmp2 = Broadcast<(_firstRowIn + 2) % numLaneVals>(tmp5);

    if constexpr (_firstRowIn + 3 == _firstRowOut)
        tmp3 = in0;
    else if constexpr ((_firstRowIn + 3) / numLaneVals == _firstRowOut / numLaneVals)
        tmp3 = Broadcast<(_firstRowIn + 3) % numLaneVals>(in0);
    else
        tmp3 = Broadcast<(_firstRowIn + 3) % numLaneVals>(tmp5);

    if constexpr (_firstRowIn + 4 == _firstRowOut)
        tmp4 = in0;
    else if constexpr ((_firstRowIn + 4) / numLaneVals == _firstRowOut / numLaneVals)
        tmp4 = Broadcast<(_firstRowIn + 4) % numLaneVals>(in0);
    else
        tmp4 = Broadcast<(_firstRowIn + 4) % numLaneVals>(tmp5);

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tmp0, tmp1,
                                                                          tmp2, tmp3, tmp4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4;

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    Transpose4x2<_firstRowIn, _firstRowOut>(in0, in1, tmp0, tmp1, tmp2, tmp3);

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneIn == laneOut)
            if constexpr (laneOffsetOut < 3)
                tmp4 = Permute2F128<1, 0>(tmp0);
            else
            {
                __m256 tmp5 = Permute2F128<0, 1, 1, 1>(in0, in1);

                tmp4 = Broadcast<0>(tmp5);
            }
        else
        {
            if constexpr (laneOffsetOut == 0)
                tmp4 = _mm_unpacklo(in0, in1);
            else if constexpr (laneOffsetOut == 1)
                tmp4 = Shuffle<0, 0, 0, 0>(in0, in1);
            else
            {
                __m256 tmp5 = _mm_unpacklo(in0, in1);
                tmp4 = Permute<0, 1, 0, 1>(tmp5);
            }
        }
    }

    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneOut == 0)
        {
            if constexpr (laneOffsetOut == 0)
                tmp4 = Permute<2, 3, 0, 1>(tmp3);
            else if constexpr (laneOffsetOut == 1)
                tmp4 = Permute2F128<1, 0>(tmp0);
            else if constexpr (laneOffsetOut == 2)
                tmp4 = Permute<2, 3, 0, 1>(tmp3);
            else
                tmp4 = Shuffle<1, 1, 1, 1>(in1, tmp0);
        }
        else
        {
            if constexpr (laneOffsetOut == 0)
                tmp4 = Permute<2, 3, 0, 1>(tmp3);
            else if constexpr (laneOffsetOut == 1)
                tmp4 = Shuffle<1, 1, 1, 1>(in0, in1);
            else
                tmp4 = _mm_unpacklo(in0, in1);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (laneOut == 0)
        {
            if constexpr (laneOffsetOut < 3)
                tmp4 = Permute2F128<1, 0>(tmp0);
            else
                tmp4 = Shuffle<2, 2, 1, 1>(in1, tmp0);
        }
        else
        {
            if constexpr (laneOffsetOut == 0)
                tmp4 = _mm_unpackhi(in0, in1);
            else if constexpr (laneOffsetOut == 1)
                tmp4 = Shuffle<2, 2, 2, 2>(in0, in1);
            else
                tmp4 = Permute2F128<1, 0>(tmp0);
        }
    }
    else
    {
        if constexpr (laneOffsetOut == 0)
            tmp4 = Permute<2, 3, 0, 1>(tmp3);
        else if constexpr (laneOffsetOut == 1)
        {
            if constexpr (laneOut == 0)
                tmp4 = Permute2F128<1, 0>(tmp0);
            else
                tmp4 = Shuffle<3, 3, 3, 3>(in0, in1);
        }
        else if constexpr (laneOffsetOut == 2)
            tmp4 = Permute<2, 3, 0, 1>(tmp3);
        else
            tmp4 = Shuffle<3, 3, 1, 1>(in1, tmp0);
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tmp0, tmp1,
                                                                          tmp2, tmp3, tmp4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4;

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;


    if constexpr (laneOffsetOut < 2)
    {
        Transpose4x3<_firstRowIn, _firstRowOut>(in0, in1, in2, tmp0, tmp1, tmp2, tmp3);

        tmp4 = Permute2F128<1, 0>(tmp0);
    }
    else if constexpr (_firstRowIn == 0)
    {
        Transpose4x3<_firstRowIn, _firstRowOut>(in0, in1, in2, tmp0, tmp1, tmp2, tmp3);
        if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp5 = _mm_unpacklo(in0, in1);
            __m256 tmp6 = Shuffle<0, 1, 0, 1>(in2, tmp5);

            tmp4 = Permute2F128<1, 1>(tmp6);
        }
        else
        {
            __m256 tmp5 = Broadcast<0>(in0);
            __m256 tmp6 = _mm_unpacklo(in1, in2);

            tmp4 = Permute2F128<0, 1, 1, 1>(tmp5, tmp6);
        }
    }
    else if constexpr (_firstRowIn == 1)
    {
        if constexpr (laneOffsetOut == 2)
        {
            Transpose4x3<_firstRowIn, _firstRowOut>(in0, in1, in2, tmp0, tmp1, tmp2, tmp3);

            tmp4 = Shuffle<1, 0, 0, 1>(in2, tmp1);
        }
        else
        {
            __m256 tmp5 = _mm_unpacklo(in1, in2);
            __m256 tmp6 = _mm_unpackhi(in1, in2);
            __m256 tmp7 = Permute2F128<1, 0>(tmp5);
            __m256 tmp8 = Permute2F128<0, 1, 1, 0>(in0, tmp6);

            tmp0 = Shuffle<2, 3, 0, 1>(tmp7, in0);
            tmp1 = Shuffle<0, 1, 3, 2>(tmp8, in0);
            tmp2 = _mm_movehl(in0, tmp8);
            tmp3 = Shuffle<0, 1, 1, 0>(tmp5, tmp8);
            tmp4 = Shuffle<2, 3, 0, 1>(tmp5, tmp8);
        }
    }
    else if constexpr (_firstRowIn == 2)
    {
        if constexpr (laneOffsetOut == 2)
        {
            Transpose4x3<_firstRowIn, _firstRowOut>(in0, in1, in2, tmp0, tmp1, tmp2, tmp3);

            __m256 tmp5 = Broadcast<2>(in2);

            tmp4 = Permute2F128<0, 1, 1, 1>(tmp0, tmp5);
        }
        else
        {
            __m256 tmp5 = _mm_unpacklo(in1, in2);
            __m256 tmp6 = _mm_unpackhi(in1, in2);
            __m256 tmp7 = Permute2F128<0, 1, 1, 0>(in0, tmp6);
            __m256 tmp8 = _mm_moveldup(tmp7);

            tmp0 = Shuffle<0, 1, 3, 2>(tmp7, in0);
            tmp1 = _mm_movehl(in0, tmp7);
            tmp2 = Shuffle<0, 1, 1, 0>(tmp5, tmp7);
            tmp3 = Shuffle<2, 3, 0, 1>(tmp5, tmp7);
            tmp4 = BlendBelowIndex<3>(tmp8, tmp6);
        }
    }
    else
    {
        if constexpr (laneOffsetOut == 2)
        {
            Transpose4x3<_firstRowIn, _firstRowOut>(in0, in1, in2, tmp0, tmp1, tmp2, tmp3);

            tmp4 = Shuffle<3, 2, 1, 0>(in2, tmp0);
        }
        else
        {
            __m256 tmp5 = _mm_unpacklo(in1, in2);
            __m256 tmp6 = _mm_unpackhi(in1, in2);
            __m256 tmp7 = Permute2F128<0, 1, 1, 0>(in0, tmp6);

            tmp0 = _mm_movehl(in0, tmp7);
            tmp1 = Shuffle<0, 1, 1, 0>(tmp5, tmp7);
            tmp2 = Shuffle<2, 3, 0, 1>(tmp5, tmp7);
            tmp3 = Shuffle<0, 1, 3, 2>(tmp6, tmp7);
            tmp4 = Shuffle<2, 3, 2, 3>(tmp6, tmp7);
        }
    }



    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tmp0, tmp1,
                                                                          tmp2, tmp3, tmp4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4)
{
    constexpr U32 idx_in_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_in_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_in_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_in_3 = (3 + _firstRowOut) % 4;
    constexpr U32 idx_tmp_0 = (0 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_1 = (1 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_2 = (2 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_3 = (3 - _firstRowIn) % 4;

    std::array<__m256, 4> tmp;
    std::array<__m256, 4> tin;
    tin[idx_in_0] = in0;
    tin[idx_in_1] = in1;
    tin[idx_in_2] = in2;
    tin[idx_in_3] = in3;


    Transpose4x4(tin[0], tin[1], tin[2], tin[3], tmp[idx_tmp_0], tmp[idx_tmp_1], tmp[idx_tmp_2], tmp[idx_tmp_3]);


    __m256 tout0, tout1, tout2, tout3, tout4;
    if constexpr (_firstRowOut == 0)
    {
        tout0 = tmp[0];
        tout1 = SwapLanesIf<(_firstRowIn > 2)>(tmp[1]);
        tout2 = SwapLanesIf<(_firstRowIn > 1)>(tmp[2]);
        tout3 = SwapLanesIf<(_firstRowIn > 0)>(tmp[3]);
        tout4 = Permute2F128<1, 0>(tmp[0]);
    }
    else if constexpr (_firstRowOut == 4)
    {
        tout0 = Permute2F128<1, 0>(tmp[0]);
        tout1 = SwapLanesIf<_firstRowIn <= 2>(tmp[1]);
        tout2 = SwapLanesIf<_firstRowIn <= 1>(tmp[2]);
        tout3 = SwapLanesIf<_firstRowIn <= 0>(tmp[3]);
        tout4 = tmp[0];
    }
    else
    {
        constexpr U32 sel_out1 = (_firstRowIn < 3) ? 0 : 1;
        constexpr U32 sel_out2 = (_firstRowIn < 2) ? 0 : 1;
        constexpr U32 sel_out3 = (_firstRowIn < 1) ? 0 : 1;

        tout0 = Permute2F128<0, 0>(tmp[0]);
        tout1 = Permute2F128<sel_out1, sel_out1>(tmp[1]);
        tout2 = Permute2F128<sel_out2, sel_out2>(tmp[2]);
        tout3 = Permute2F128<sel_out3, sel_out3>(tmp[3]);
        tout4 = Permute2F128<1, 1>(tmp[0]);
    }

    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tout0, tout1,
                                                                          tout2, tout3, tout4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4)
{

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3, tmp4;

    Transpose4x5<_firstRowIn, _firstRowOut>(in0, in1, in2, in3, in4, tmp0, tmp1, tmp2, tmp3);

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp6 = BlendIndex<4>(tmp0, in0);

            tmp4 = Permute2F128<0, 1, 1, 1>(tmp6, in4);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp6 = Shuffle<1, 0, 2, 3>(in0, tmp0);
            __m256 tmp7 = _mm_unpacklo(in3, in4);

            tmp4 = Permute2F128<0, 1, 1, 1>(tmp6, tmp7);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp6 = _mm_unpacklo(in0, in1);
            __m256 tmp7 = Permute2F128<0, 1, 1, 0>(tmp6, tmp0);
            __m256 tmp8 = BlendIndex<4>(tmp7, in4);

            tmp4 = _mm_movelh(tmp7, tmp8);
        }
        else
        {
            __m256 tmp6 = Permute2F128<0, 1, 1, 0>(in0, tmp0);
            __m256 tmp7 = BlendIndex<0>(in4, tmp6);
            __m256 tmp8 = Broadcast<0>(tmp7);

            tmp4 = BlendInRange<4, 6>(tmp8, tmp6);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp7 = _mm_movehdup(in0);
            __m256 tmp8 = _mm_movehdup(in4);
            __m256 tmp9 = BlendIndex<4>(tmp0, tmp7);

            tmp4 = Permute2F128<0, 1, 1, 1>(tmp9, tmp8);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp7 = BlendIndex<5>(tmp0, in0);
            __m256 tmp8 = Permute2F128<1, 0>(tmp7);

            tmp4 = BlendIndex<5>(tmp8, in4);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp7 = _mm_unpacklo(in0, in1);
            __m256 tmp8 = Permute2F128<0, 1, 1, 0>(tmp7, tmp0);
            __m256 tmp9 = Broadcast<1>(in4);

            tmp4 = BlendIndex<6>(tmp8, tmp9);
        }
        else
        {
            __m256 tmp7 = _mm_movelh(in0, in0);
            __m256 tmp8 = Permute2F128<0, 1, 1, 0>(tmp7, tmp0);
            __m256 tmp9 = _mm_movelh(in4, in4);

            tmp4 = BlendIndex<7>(tmp8, tmp9);
        }
    }
    else if constexpr (laneOffsetIn == 2)
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp7 = _mm_movehl(in0, in0);
            __m256 tmp8 = _mm_movehl(in4, in4);
            __m256 tmp9 = BlendIndex<4>(tmp0, tmp7);

            tmp4 = Permute2F128<0, 1, 1, 1>(tmp9, tmp8);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp7 = Shuffle<3, 2, 2, 3>(in0, tmp0);
            __m256 tmp8 = _mm_unpackhi(in3, in4);

            tmp4 = Permute2F128<0, 1, 1, 1>(tmp7, tmp8);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp7 = BlendIndex<6>(tmp0, in0);
            __m256 tmp8 = Permute2F128<1, 0>(tmp7);

            tmp4 = BlendIndex<6>(tmp8, in4);
        }
        else
        {
            __m256 tmp7 = _mm_moveldup(in0);
            __m256 tmp8 = _mm_moveldup(in4);
            __m256 tmp9 = Permute2F128<0, 1, 1, 0>(tmp7, tmp0);

            tmp4 = BlendIndex<7>(tmp9, tmp8);
        }
    }
    else
    {
        if constexpr (laneOffsetOut == 0)
        {
            __m256 tmp7 = Broadcast<3>(in0);
            __m256 tmp8 = Broadcast<3>(in4);
            __m256 tmp9 = BlendIndex<4>(tmp0, tmp7);

            tmp4 = Permute2F128<0, 1, 1, 1>(tmp9, tmp8);
        }
        else if constexpr (laneOffsetOut == 1)
        {
            __m256 tmp7 = _mm_movehl(tmp0, in0);
            __m256 tmp8 = Broadcast<3>(in4);
            __m256 tmp10 = Permute2F128<0, 1, 1, 0>(tmp7, tmp0);

            tmp4 = BlendIndex<5>(tmp10, tmp8);
        }
        else if constexpr (laneOffsetOut == 2)
        {
            __m256 tmp7 = _mm_unpackhi(in0, in1);
            __m256 tmp8 = _mm_movehdup(in4);
            __m256 tmp9 = Permute2F128<0, 1, 1, 0>(tmp7, tmp0);

            tmp4 = BlendIndex<6>(tmp9, tmp8);
        }
        else
        {
            __m256 tmp7 = Permute2F128<0, 1, 1, 0>(in0, tmp0);

            tmp4 = BlendIndex<7>(tmp7, in4);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tmp0, tmp1,
                                                                          tmp2, tmp3, tmp4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4;
    __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;


    if constexpr (_firstRowOut == 0)
    {
        Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
        Transpose4x2<0, 0>(in4, in5, tmp10, tmp11, tmp12, tmp13);
    }
    else if constexpr (_firstRowOut == 1)
    {
        Transpose4x3<0, 1>(in0, in1, in2, tmp6, tmp7, tmp8, tmp9);
        Transpose4x3<0, 0>(in3, in4, in5, tmp10, tmp11, tmp12, tmp13);
    }
    else
    {
        Transpose4x2<0, 2>(in0, in1, tmp6, tmp7, tmp8, tmp9);
        Transpose4x4<0, 0>(in2, in3, in4, in5, tmp10, tmp11, tmp12, tmp13);
    }


    if constexpr (_firstRowIn == 0)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp6, tmp10);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp3 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
    }
    else if constexpr (_firstRowIn == 1)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
    }
    else if constexpr (_firstRowIn == 2)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
    }
    else
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp1 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp9, tmp13);
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tmp0, tmp1,
                                                                          tmp2, tmp3, tmp4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4;

    __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;

    if constexpr (_firstRowOut == 0)
    {
        Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
        Transpose4x3<0, 0>(in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
    }
    else
    {
        Transpose4x3<0, 1>(in0, in1, in2, tmp6, tmp7, tmp8, tmp9);
        Transpose4x4<0, 0>(in3, in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
    }


    if constexpr (_firstRowIn == 0)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp6, tmp10);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp3 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
    }
    else if constexpr (_firstRowIn == 1)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
    }
    else if constexpr (_firstRowIn == 2)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
    }
    else
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp1 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp9, tmp13);
    }



    // Write to output registers
    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tmp0, tmp1,
                                                                          tmp2, tmp3, tmp4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4;

    __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;


    Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
    Transpose4x4<0, 0>(in4, in5, in6, in7, tmp10, tmp11, tmp12, tmp13);



    if constexpr (_firstRowIn == 0)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp6, tmp10);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp3 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
    }
    else if constexpr (_firstRowIn == 1)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
    }
    else if constexpr (_firstRowIn == 2)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
    }
    else
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp1 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp9, tmp13);
    }



    // Write to output registers
    out0 = tmp0;
    out1 = tmp1;
    out2 = tmp2;
    out3 = tmp3;
    out4 = tmp4;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    __m256 tmp6, tmp7;
    if constexpr (laneOut == 0)
    {
        tmp6 = in0;
        tmp7 = Permute2F128<1, 0>(in0);
    }
    else
    {
        tmp6 = Permute2F128<1, 0>(in0);
        tmp7 = in0;
    }


    if constexpr (laneOffsetIn == 0)
    {
        Transpose4x1<0, laneOffsetOut>(tmp6, tmp0, tmp1, tmp2, tmp3);
        Transpose2x1<0, laneOffsetOut>(tmp7, tmp4, tmp5);
    }
    else if constexpr (laneOffsetIn == 1)
    {
        Transpose3x1<1, laneOffsetOut>(tmp6, tmp0, tmp1, tmp2);
        Transpose3x1<0, laneOffsetOut>(tmp7, tmp3, tmp4, tmp5);
    }
    else
    {
        Transpose2x1<2, laneOffsetOut>(tmp6, tmp0, tmp1);
        Transpose4x1<0, laneOffsetOut>(tmp7, tmp2, tmp3, tmp4, tmp5);
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tmp0,
                                                                          tmp1, tmp2, tmp3, tmp4, tmp5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4,
                         __m256& out5)
{

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    [[maybe_unused]] constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    if constexpr (_firstRowOut == 0 || _firstRowOut == 1 || _firstRowOut == 2)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp0, tmp1, tmp2, tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
        }
        else if constexpr (_firstRowIn == 1)
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp3, tmp0, tmp1, tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
        }
        else
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp2, tmp3, tmp0, tmp1);
            tmp2 = Permute2F128<1, 0>(tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
        }
    }
    else if constexpr (_firstRowOut == 4 || _firstRowOut == 5 || _firstRowOut == 6)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp4, tmp5, tmp2, tmp3);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
        }
        else if constexpr (_firstRowIn == 1)
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp3, tmp4, tmp5, tmp2);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp2);
        }
        else
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp2, tmp3, tmp4, tmp5);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
        }
    }
    else
    {
        __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;



        tmp6 = Permute2F128<0, 1, 1, 0>(in0, in1);
        tmp7 = in0; // not relevant for result
        tmp8 = in0; // not relevant for result
        tmp9 = in0;

        tmp10 = in1;
        tmp11 = in0; // not relevant for result
        tmp12 = in0; // not relevant for result
        tmp13 = tmp6;


        if constexpr (_firstRowIn == 0)
        {
            Transpose4x4<0, 0>(tmp6, tmp7, tmp8, tmp9, tmp0, tmp1, tmp2, tmp3);
            Transpose2x4<0, 0>(tmp10, tmp11, tmp12, tmp13, tmp4, tmp5);
        }
        else if constexpr (_firstRowIn == 1)
        {
            Transpose3x4<1, 0>(tmp6, tmp7, tmp8, tmp9, tmp0, tmp1, tmp2);
            Transpose3x4<0, 0>(tmp10, tmp11, tmp12, tmp13, tmp3, tmp4, tmp5);
        }
        else
        {
            Transpose2x4<2, 0>(tmp6, tmp7, tmp8, tmp9, tmp0, tmp1);
            Transpose4x4<0, 0>(tmp10, tmp11, tmp12, tmp13, tmp2, tmp3, tmp4, tmp5);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tmp0,
                                                                          tmp1, tmp2, tmp3, tmp4, tmp5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4, __m256& out5)
{

    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    [[maybe_unused]] constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    if constexpr (_firstRowOut == 0 || _firstRowOut == 1)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp0, tmp1, tmp2, tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
        }
        else if constexpr (_firstRowIn == 1)
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp3, tmp0, tmp1, tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
        }
        else
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp2, tmp3, tmp0, tmp1);
            tmp2 = Permute2F128<1, 0>(tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
        }
    }
    else if constexpr (_firstRowOut == 4 || _firstRowOut == 5)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp4, tmp5, tmp2, tmp3);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
        }
        else if constexpr (_firstRowIn == 1)
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp3, tmp4, tmp5, tmp2);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp2);
        }
        else
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp2, tmp3, tmp4, tmp5);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
        }
    }
    else
    {
        __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;


        if constexpr (_firstRowOut == 2)
        {
            tmp6 = Permute2F128<0, 1, 1, 0>(in0, in2);
            tmp7 = in0; // not relevant for result
            tmp8 = in0;
            tmp9 = in1;

            tmp10 = in2;
            tmp11 = in0; // not relevant for result
            tmp12 = tmp6;
            tmp13 = Permute2F128<1, 0>(in1);
        }
        else
        {
            tmp6 = Permute2F128<1, 0>(in1);
            tmp7 = Permute2F128<0, 1, 1, 0>(in0, in2);
            tmp8 = in0; // not relevant for result
            tmp9 = in0;

            tmp10 = in1;
            tmp11 = in2;
            tmp12 = in0; // not relevant for result
            tmp13 = tmp7;
        }

        if constexpr (_firstRowIn == 0)
        {
            Transpose4x4<0, 0>(tmp6, tmp7, tmp8, tmp9, tmp0, tmp1, tmp2, tmp3);
            Transpose2x4<0, 0>(tmp10, tmp11, tmp12, tmp13, tmp4, tmp5);
        }
        else if constexpr (_firstRowIn == 1)
        {
            Transpose3x4<1, 0>(tmp6, tmp7, tmp8, tmp9, tmp0, tmp1, tmp2);
            Transpose3x4<0, 0>(tmp10, tmp11, tmp12, tmp13, tmp3, tmp4, tmp5);
        }
        else
        {
            Transpose2x4<2, 0>(tmp6, tmp7, tmp8, tmp9, tmp0, tmp1);
            Transpose4x4<0, 0>(tmp10, tmp11, tmp12, tmp13, tmp2, tmp3, tmp4, tmp5);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tmp0,
                                                                          tmp1, tmp2, tmp3, tmp4, tmp5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5)
{
    constexpr U32 idx_in_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_in_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_in_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_in_3 = (3 + _firstRowOut) % 4;
    constexpr U32 idx_tmp_0 = (0 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_1 = (1 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_2 = (2 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_3 = (3 - _firstRowIn) % 4;

    std::array<__m256, 4> tmp;
    std::array<__m256, 4> tin;
    tin[idx_in_0] = in0;
    tin[idx_in_1] = in1;
    tin[idx_in_2] = in2;
    tin[idx_in_3] = in3;


    Transpose4x4(tin[0], tin[1], tin[2], tin[3], tmp[idx_tmp_0], tmp[idx_tmp_1], tmp[idx_tmp_2], tmp[idx_tmp_3]);


    __m256 tout0, tout1, tout2, tout3, tout4, tout5;
    if constexpr (_firstRowOut == 0)
    {
        tout0 = tmp[0];
        tout1 = tmp[1];
        tout2 = SwapLanesIf<(_firstRowIn > 1)>(tmp[2]);
        tout3 = SwapLanesIf<(_firstRowIn > 0)>(tmp[3]);
        tout4 = Permute2F128<1, 0>(tmp[0]);
        tout5 = Permute2F128<1, 0>(tmp[1]);
    }
    else if constexpr (_firstRowOut == 4)
    {
        tout0 = Permute2F128<1, 0>(tmp[0]);
        tout1 = Permute2F128<1, 0>(tmp[1]);
        tout2 = SwapLanesIf<_firstRowIn <= 1>(tmp[2]);
        tout3 = SwapLanesIf<_firstRowIn == 0>(tmp[3]);
        tout4 = tmp[0];
        tout5 = tmp[1];
    }
    else
    {
        constexpr U32 sel_out2 = (_firstRowIn < 2) ? 0 : 1;
        constexpr U32 sel_out3 = (_firstRowIn < 1) ? 0 : 1;

        tout0 = Permute2F128<0, 0>(tmp[0]);
        tout1 = Permute2F128<0, 0>(tmp[1]);
        tout2 = Permute2F128<sel_out2, sel_out2>(tmp[2]);
        tout3 = Permute2F128<sel_out3, sel_out3>(tmp[3]);
        tout4 = Permute2F128<1, 1>(tmp[0]);
        tout5 = Permute2F128<1, 1>(tmp[1]);
    }

    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout0,
                                                                          tout1, tout2, tout3, tout4, tout5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    __m256 tout0, tout1, tout2, tout3, tout4, tout5;
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    PermuteBeforeIntraLaneTranspose<_firstRowOut>(tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, in0, in1, in2, in3,
                                                  in4);

    if constexpr (_firstRowIn == 0)
    {
        Transpose4x4<0, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2, tout3);
        Transpose2x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout4, tout5);
    }
    else if constexpr (_firstRowIn == 1)
    {
        Transpose3x4<1, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2);
        Transpose3x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout3, tout4, tout5);
    }
    else
    {
        Transpose2x4<2, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1);
        Transpose4x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout2, tout3, tout4, tout5);
    }

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout0,
                                                                          tout1, tout2, tout3, tout4, tout5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    __m256 tout0, tout1, tout2, tout3, tout4, tout5;
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    PermuteBeforeIntraLaneTranspose<_firstRowOut>(tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, in0, in1, in2, in3,
                                                  in4, in5);

    if constexpr (_firstRowIn == 0)
    {
        Transpose4x4<0, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2, tout3);
        Transpose2x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout4, tout5);
    }
    else if constexpr (_firstRowIn == 1)
    {
        Transpose3x4<1, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2);
        Transpose3x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout3, tout4, tout5);
    }
    else
    {
        Transpose2x4<2, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1);
        Transpose4x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout2, tout3, tout4, tout5);
    }

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout0,
                                                                          tout1, tout2, tout3, tout4, tout5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;

    if constexpr (_firstRowOut == 0)
    {
        Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
        Transpose4x3<0, 0>(in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
    }
    else
    {
        Transpose4x3<0, 1>(in0, in1, in2, tmp6, tmp7, tmp8, tmp9);
        Transpose4x4<0, 0>(in3, in4, in5, in6, tmp10, tmp11, tmp12, tmp13);
    }


    if constexpr (_firstRowIn == 0)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp6, tmp10);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp3 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp5 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
    }
    else if constexpr (_firstRowIn == 1)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp5 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
    }
    else
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
        tmp5 = Permute2F128<0, 1, 1, 1>(tmp9, tmp13);
    }



    // Write to output registers
    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tmp0,
                                                                          tmp1, tmp2, tmp3, tmp4, tmp5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;


    Transpose4x4<0, 0>(in0, in1, in2, in3, tmp6, tmp7, tmp8, tmp9);
    Transpose4x4<0, 0>(in4, in5, in6, in7, tmp10, tmp11, tmp12, tmp13);



    if constexpr (_firstRowIn == 0)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp6, tmp10);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp3 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp5 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
    }
    else if constexpr (_firstRowIn == 1)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp7, tmp11);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp2 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp5 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
    }
    else
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
        tmp5 = Permute2F128<0, 1, 1, 1>(tmp9, tmp13);
    }



    // Write to output registers
    out0 = tmp0;
    out1 = tmp1;
    out2 = tmp2;
    out3 = tmp3;
    out4 = tmp4;
    out5 = tmp5;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;

    __m256 tmp7, tmp8;
    if constexpr (laneOut == 0)
    {
        tmp7 = in0;
        tmp8 = Permute2F128<1, 0>(in0);
    }
    else
    {
        tmp7 = Permute2F128<1, 0>(in0);
        tmp8 = in0;
    }


    if constexpr (laneOffsetIn == 0)
    {
        Transpose4x1<0, laneOffsetOut>(tmp7, tmp0, tmp1, tmp2, tmp3);
        Transpose3x1<0, laneOffsetOut>(tmp8, tmp4, tmp5, tmp6);
    }
    else
    {
        Transpose3x1<1, laneOffsetOut>(tmp7, tmp0, tmp1, tmp2);
        Transpose4x1<0, laneOffsetOut>(tmp8, tmp3, tmp4, tmp5, tmp6);
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4,
                         __m256& out5, __m256& out6)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    [[maybe_unused]] constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;

    if constexpr (_firstRowOut == 0 || _firstRowOut == 1 || _firstRowOut == 2)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp0, tmp1, tmp2, tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
            tmp6 = Permute2F128<1, 0>(tmp2);
        }
        else
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp3, tmp0, tmp1, tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
            tmp6 = Permute2F128<1, 0>(tmp2);
        }
    }
    else if constexpr (_firstRowOut == 4 || _firstRowOut == 5 || _firstRowOut == 6)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp4, tmp5, tmp6, tmp3);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp6);
            tmp3 = Permute2F128<1, 0>(tmp3);
        }
        else
        {
            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp3, tmp4, tmp5, tmp6);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp6);
        }
    }
    else
    {
        __m256 tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14;



        tmp7 = Permute2F128<0, 1, 1, 0>(in0, in1);
        tmp8 = in0; // not relevant for result
        tmp9 = in0; // not relevant for result
        tmp10 = in0;

        tmp11 = in1;
        tmp12 = in0; // not relevant for result
        tmp13 = in0; // not relevant for result
        tmp14 = tmp7;

        if constexpr (_firstRowIn == 0)
        {
            Transpose4x4<0, 0>(tmp7, tmp8, tmp9, tmp10, tmp0, tmp1, tmp2, tmp3);
            Transpose3x4<0, 0>(tmp11, tmp12, tmp13, tmp14, tmp4, tmp5, tmp6);
        }
        else
        {
            Transpose3x4<1, 0>(tmp7, tmp8, tmp9, tmp10, tmp0, tmp1, tmp2);
            Transpose4x4<0, 0>(tmp11, tmp12, tmp13, tmp14, tmp3, tmp4, tmp5, tmp6);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4, __m256& out5, __m256& out6)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    [[maybe_unused]] constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;

    if constexpr (_firstRowOut == 0 || _firstRowOut == 1)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp0, tmp1, tmp2, tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
            tmp6 = Permute2F128<1, 0>(tmp2);
        }
        else
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp3, tmp0, tmp1, tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
            tmp6 = Permute2F128<1, 0>(tmp2);
        }
    }
    else if constexpr (_firstRowOut == 4 || _firstRowOut == 5)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp4, tmp5, tmp6, tmp3);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp6);
            tmp3 = Permute2F128<1, 0>(tmp3);
        }
        else
        {
            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp3, tmp4, tmp5, tmp6);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp6);
        }
    }
    else
    {
        __m256 tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14;


        if constexpr (_firstRowOut == 2)
        {
            tmp7 = Permute2F128<0, 1, 1, 0>(in0, in2);
            tmp8 = in0; // not relevant for result
            tmp9 = in0;
            tmp10 = in1;

            tmp11 = in2;
            tmp12 = in0; // not relevant for result
            tmp13 = tmp7;
            tmp14 = Permute2F128<1, 0>(in1);
        }
        else
        {
            tmp7 = Permute2F128<1, 0>(in1);
            tmp8 = Permute2F128<0, 1, 1, 0>(in0, in2);
            tmp9 = in0; // not relevant for result
            tmp10 = in0;

            tmp11 = in1;
            tmp12 = in2;
            tmp13 = in0; // not relevant for result
            tmp14 = tmp8;
        }

        if constexpr (_firstRowIn == 0)
        {
            Transpose4x4<0, 0>(tmp7, tmp8, tmp9, tmp10, tmp0, tmp1, tmp2, tmp3);
            Transpose3x4<0, 0>(tmp11, tmp12, tmp13, tmp14, tmp4, tmp5, tmp6);
        }
        else
        {
            Transpose3x4<1, 0>(tmp7, tmp8, tmp9, tmp10, tmp0, tmp1, tmp2);
            Transpose4x4<0, 0>(tmp11, tmp12, tmp13, tmp14, tmp3, tmp4, tmp5, tmp6);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    constexpr U32 idx_in_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_in_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_in_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_in_3 = (3 + _firstRowOut) % 4;
    constexpr U32 idx_tmp_0 = (0 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_1 = (1 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_2 = (2 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_3 = (3 - _firstRowIn) % 4;

    std::array<__m256, 4> tmp;
    std::array<__m256, 4> tin;
    tin[idx_in_0] = in0;
    tin[idx_in_1] = in1;
    tin[idx_in_2] = in2;
    tin[idx_in_3] = in3;


    Transpose4x4(tin[0], tin[1], tin[2], tin[3], tmp[idx_tmp_0], tmp[idx_tmp_1], tmp[idx_tmp_2], tmp[idx_tmp_3]);


    __m256 tout0, tout1, tout2, tout3, tout4, tout5, tout6;
    if constexpr (_firstRowOut == 0)
    {
        tout0 = tmp[0];
        tout1 = tmp[1];
        tout2 = tmp[2];
        tout3 = SwapLanesIf<_firstRowIn != 0>(tmp[3]);
        tout4 = Permute2F128<1, 0>(tmp[0]);
        tout5 = Permute2F128<1, 0>(tmp[1]);
        tout6 = Permute2F128<1, 0>(tmp[2]);
    }
    else if constexpr (_firstRowOut == 4)
    {
        tout0 = Permute2F128<1, 0>(tmp[0]);
        tout1 = Permute2F128<1, 0>(tmp[1]);
        tout2 = Permute2F128<1, 0>(tmp[2]);
        tout3 = SwapLanesIf<_firstRowIn == 0>(tmp[3]);
        tout4 = tmp[0];
        tout5 = tmp[1];
        tout6 = tmp[2];
    }
    else
    {
        tout0 = Permute2F128<0, 0>(tmp[0]);
        tout1 = Permute2F128<0, 0>(tmp[1]);
        tout2 = Permute2F128<0, 0>(tmp[2]);
        tout3 = Permute2F128<_firstRowIn, _firstRowIn>(tmp[3]);
        tout4 = Permute2F128<1, 1>(tmp[0]);
        tout5 = Permute2F128<1, 1>(tmp[1]);
        tout6 = Permute2F128<1, 1>(tmp[2]);
    }


    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout0, tout1, tout2, tout3, tout4, tout5, tout6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    __m256 tout0, tout1, tout2, tout3, tout4, tout5, tout6;
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    PermuteBeforeIntraLaneTranspose<_firstRowOut>(tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, in0, in1, in2, in3,
                                                  in4);

    if constexpr (_firstRowIn == 0)
    {
        Transpose4x4<0, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2, tout3);
        Transpose3x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout4, tout5, tout6);
    }
    else
    {
        Transpose3x4<1, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2);
        Transpose4x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout3, tout4, tout5, tout6);
    }

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout0, tout1, tout2, tout3, tout4, tout5, tout6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    __m256 tout0, tout1, tout2, tout3, tout4, tout5, tout6;
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    PermuteBeforeIntraLaneTranspose<_firstRowOut>(tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, in0, in1, in2, in3,
                                                  in4, in5);

    if constexpr (_firstRowIn == 0)
    {
        Transpose4x4<0, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2, tout3);
        Transpose3x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout4, tout5, tout6);
    }
    else
    {
        Transpose3x4<1, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2);
        Transpose4x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout3, tout4, tout5, tout6);
    }

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout0, tout1, tout2, tout3, tout4, tout5, tout6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    __m256 tout0, tout1, tout2, tout3, tout4, tout5, tout6;
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    PermuteBeforeIntraLaneTranspose<_firstRowOut>(tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, in0, in1, in2, in3,
                                                  in4, in5, in6);

    if constexpr (_firstRowIn == 0)
    {
        Transpose4x4<0, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2, tout3);
        Transpose3x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout4, tout5, tout6);
    }
    else
    {
        Transpose3x4<1, 0>(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2);
        Transpose4x4<0, 0>(tmp4, tmp5, tmp6, tmp7, tout3, tout4, tout5, tout6);
    }

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout0, tout1, tout2, tout3, tout4, tout5, tout6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;

    __m256 tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14;


    tmp7 = Permute2F128<0, 0, 1, 0>(in0, in4);
    tmp8 = Permute2F128<0, 0, 1, 0>(in1, in5);
    tmp9 = Permute2F128<0, 0, 1, 0>(in2, in6);
    tmp10 = Permute2F128<0, 0, 1, 0>(in3, in7);

    tmp11 = Permute2F128<0, 1, 1, 1>(in0, in4);
    tmp12 = Permute2F128<0, 1, 1, 1>(in1, in5);
    tmp13 = Permute2F128<0, 1, 1, 1>(in2, in6);
    tmp14 = Permute2F128<0, 1, 1, 1>(in3, in7);



    if constexpr (_firstRowIn == 0)
    {
        Transpose4x4<0, 0>(tmp7, tmp8, tmp9, tmp10, tmp0, tmp1, tmp2, tmp3);
        Transpose3x4<0, 0>(tmp11, tmp12, tmp13, tmp14, tmp4, tmp5, tmp6);
    }
    else
    {
        Transpose3x4<1, 0>(tmp7, tmp8, tmp9, tmp10, tmp0, tmp1, tmp2);
        Transpose4x4<0, 0>(tmp11, tmp12, tmp13, tmp14, tmp3, tmp4, tmp5, tmp6);
    }

    // Write to output registers
    out0 = tmp0;
    out1 = tmp1;
    out2 = tmp2;
    out3 = tmp3;
    out4 = tmp4;
    out5 = tmp5;
    out6 = tmp6;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6, __m256& out7)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    __m256 tmp8, tmp9;
    if constexpr (laneOut == 0)
    {
        tmp8 = in0;
        tmp9 = Permute2F128<1, 0>(in0);
    }
    else
    {
        tmp8 = Permute2F128<1, 0>(in0);
        tmp9 = in0;
    }


    Transpose4x1<0, laneOffsetOut>(tmp8, tmp0, tmp1, tmp2, tmp3);
    Transpose4x1<0, laneOffsetOut>(tmp9, tmp4, tmp5, tmp6, tmp7);


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, out7, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4,
                         __m256& out5, __m256& out6, __m256& out7)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    [[maybe_unused]] constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    if constexpr (laneOffsetOut <= 2)
    {
        if constexpr (_firstRowOut <= 2)
        {

            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp0, tmp1, tmp2, tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
            tmp6 = Permute2F128<1, 0>(tmp2);
            tmp7 = Permute2F128<1, 0>(tmp3);
        }
        else
        {

            Transpose4x2<0, laneOffsetOut>(in0, in1, tmp4, tmp5, tmp6, tmp7);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp6);
            tmp3 = Permute2F128<1, 0>(tmp7);
        }
    }
    else
    {
        __m256 tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14, tmp15;


        tmp8 = Permute2F128<1, 0>(in1);
        tmp9 = in0; // not relevant for result
        tmp10 = in0; // not relevant for result
        tmp11 = in0;

        tmp12 = in1;
        tmp13 = in0; // not relevant for result
        tmp14 = in0; // not relevant for result
        tmp15 = Permute2F128<1, 0>(in0);



        Transpose4x4(tmp8, tmp9, tmp10, tmp11, tmp0, tmp1, tmp2, tmp3);
        Transpose4x4(tmp12, tmp13, tmp14, tmp15, tmp4, tmp5, tmp6, tmp7);
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, out7, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4, __m256& out5, __m256& out6, __m256& out7)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    constexpr U32 numLaneVals = numValuesPerLane<__m256>;
    [[maybe_unused]] constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    if constexpr (laneOffsetOut <= 1)
    {
        if constexpr (_firstRowOut <= 1)
        {

            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp0, tmp1, tmp2, tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
            tmp6 = Permute2F128<1, 0>(tmp2);
            tmp7 = Permute2F128<1, 0>(tmp3);
        }
        else
        {

            Transpose4x3<0, laneOffsetOut>(in0, in1, in2, tmp4, tmp5, tmp6, tmp7);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp6);
            tmp3 = Permute2F128<1, 0>(tmp7);
        }
    }
    else
    {
        __m256 tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14, tmp15;

        if constexpr (_firstRowOut == 2)
        {
            tmp8 = Permute2F128<1, 0>(in2);
            tmp9 = in0; // not relevant for result
            tmp10 = in0;
            tmp11 = in1;

            tmp12 = in2;
            tmp13 = in0; // not relevant for result
            tmp14 = Permute2F128<1, 0>(in0);
            tmp15 = Permute2F128<1, 0>(in1);
        }
        else
        {
            tmp8 = Permute2F128<1, 0>(in1);
            tmp9 = Permute2F128<1, 0>(in2);
            tmp10 = in0; // not relevant for result
            tmp11 = in0;

            tmp12 = in1;
            tmp13 = in2;
            tmp14 = in0; // not relevant for result
            tmp15 = Permute2F128<1, 0>(in0);
        }


        Transpose4x4(tmp8, tmp9, tmp10, tmp11, tmp0, tmp1, tmp2, tmp3);
        Transpose4x4(tmp12, tmp13, tmp14, tmp15, tmp4, tmp5, tmp6, tmp7);
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, out7, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5, __m256& out6, __m256& out7)
{
    constexpr U32 idx_in_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_in_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_in_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_in_3 = (3 + _firstRowOut) % 4;

    std::array<__m256, 4> tmp;
    std::array<__m256, 4> tin;
    tin[idx_in_0] = in0;
    tin[idx_in_1] = in1;
    tin[idx_in_2] = in2;
    tin[idx_in_3] = in3;


    Transpose4x4(tin[0], tin[1], tin[2], tin[3], tmp[0], tmp[1], tmp[2], tmp[3]);


    __m256 tout0, tout1, tout2, tout3, tout4, tout5, tout6, tout7;
    if constexpr (_firstRowOut == 0)
    {
        tout0 = tmp[0];
        tout1 = tmp[1];
        tout2 = tmp[2];
        tout3 = tmp[3];
        tout4 = Permute2F128<1, 0>(tmp[0]);
        tout5 = Permute2F128<1, 0>(tmp[1]);
        tout6 = Permute2F128<1, 0>(tmp[2]);
        tout7 = Permute2F128<1, 0>(tmp[3]);
    }
    else if constexpr (_firstRowOut == 4)
    {
        tout0 = Permute2F128<1, 0>(tmp[0]);
        tout1 = Permute2F128<1, 0>(tmp[1]);
        tout2 = Permute2F128<1, 0>(tmp[2]);
        tout3 = Permute2F128<1, 0>(tmp[3]);
        tout4 = tmp[0];
        tout5 = tmp[1];
        tout6 = tmp[2];
        tout7 = tmp[3];
    }
    else
    {
        tout0 = Permute2F128<0, 0>(tmp[0]);
        tout1 = Permute2F128<0, 0>(tmp[1]);
        tout2 = Permute2F128<0, 0>(tmp[2]);
        tout3 = Permute2F128<0, 0>(tmp[3]);
        tout4 = Permute2F128<1, 1>(tmp[0]);
        tout5 = Permute2F128<1, 1>(tmp[1]);
        tout6 = Permute2F128<1, 1>(tmp[2]);
        tout7 = Permute2F128<1, 1>(tmp[3]);
    }

    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, out7, tout0, tout1, tout2, tout3, tout4, tout5, tout6, tout7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6, __m256& out7)
{
    __m256 tout0, tout1, tout2, tout3, tout4, tout5, tout6, tout7;
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    PermuteBeforeIntraLaneTranspose<_firstRowOut>(tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, in0, in1, in2, in3,
                                                  in4);

    Transpose4x4(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2, tout3);
    Transpose4x4(tmp4, tmp5, tmp6, tmp7, tout4, tout5, tout6, tout7);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, out7, tout0, tout1, tout2, tout3, tout4, tout5, tout6, tout7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6,
                         __m256& out7)
{
    __m256 tout0, tout1, tout2, tout3, tout4, tout5, tout6, tout7;
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    PermuteBeforeIntraLaneTranspose<_firstRowOut>(tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, in0, in1, in2, in3,
                                                  in4, in5);

    Transpose4x4(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2, tout3);
    Transpose4x4(tmp4, tmp5, tmp6, tmp7, tout4, tout5, tout6, tout7);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, out7, tout0, tout1, tout2, tout3, tout4, tout5, tout6, tout7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6, __m256& out7)
{
    __m256 tout0, tout1, tout2, tout3, tout4, tout5, tout6, tout7;
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    PermuteBeforeIntraLaneTranspose<_firstRowOut>(tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, in0, in1, in2, in3,
                                                  in4, in5, in6);

    Transpose4x4(tmp0, tmp1, tmp2, tmp3, tout0, tout1, tout2, tout3);
    Transpose4x4(tmp4, tmp5, tmp6, tmp7, tout4, tout5, tout6, tout7);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, out7, tout0, tout1, tout2, tout3, tout4, tout5, tout6, tout7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6, __m256& out7)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    out0 = _mm256_unpacklo_ps(in0, in1);
    out1 = _mm256_unpackhi_ps(in0, in1);
    out2 = _mm256_unpacklo_ps(in2, in3);
    out3 = _mm256_unpackhi_ps(in2, in3);
    out4 = _mm256_unpacklo_ps(in4, in5);
    out5 = _mm256_unpackhi_ps(in4, in5);
    out6 = _mm256_unpacklo_ps(in6, in7);
    out7 = _mm256_unpackhi_ps(in6, in7);

    // Faster than pure shuffle (around 6-8%) due to instruction parallelism of Blend.
    // source: https://stackoverflow.com/questions/25622745/transpose-an-8x8-float-using-avx-avx2

    __m256 tmpBlend0 = Shuffle<2, 3, 0, 1>(out0, out2);
    __m256 tmpBlend1 = Shuffle<2, 3, 0, 1>(out1, out3);
    __m256 tmpBlend2 = Shuffle<2, 3, 0, 1>(out4, out6);
    __m256 tmpBlend3 = Shuffle<2, 3, 0, 1>(out5, out7);

    tmp0 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out0, tmpBlend0);
    tmp1 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out2, tmpBlend0);
    tmp2 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out1, tmpBlend1);
    tmp3 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out3, tmpBlend1);
    tmp4 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out4, tmpBlend2);
    tmp5 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out6, tmpBlend2);
    tmp6 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out5, tmpBlend3);
    tmp7 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out7, tmpBlend3);

    out0 = Permute2F128<0, 0, 1, 0>(tmp0, tmp4);
    out1 = Permute2F128<0, 0, 1, 0>(tmp1, tmp5);
    out2 = Permute2F128<0, 0, 1, 0>(tmp2, tmp6);
    out3 = Permute2F128<0, 0, 1, 0>(tmp3, tmp7);
    out4 = Permute2F128<0, 1, 1, 1>(tmp0, tmp4);
    out5 = Permute2F128<0, 1, 1, 1>(tmp1, tmp5);
    out6 = Permute2F128<0, 1, 1, 1>(tmp2, tmp6);
    out7 = Permute2F128<0, 1, 1, 1>(tmp3, tmp7);
}



} // namespace GDL::simd


#endif // __AVX2__
