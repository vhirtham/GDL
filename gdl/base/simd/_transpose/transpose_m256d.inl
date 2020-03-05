#pragma once

#include "gdl/base/simd/_transpose/transpose_m256d.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/_transpose/transpose_utility.h"


#ifdef __AVX2__

namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x1(__m256d in, __m256d& out)
{
    static_assert(not(_overwriteUnused == false && _unusedSetZero == true), "Option _unusedSetZero has no effect.");

    __m256d tmp;

    if constexpr (_firstRowIn == _firstRowOut)
    {
        tmp = in;
    }
    else
    {
        constexpr U32 numLaneVals = numValuesPerLane<__m256d>;
        constexpr U32 laneIn = _firstRowIn / numLaneVals;
        constexpr U32 laneOut = _firstRowOut / numLaneVals;
        constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
        constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;


        if constexpr (laneIn == laneOut)
        {
            if constexpr (laneOffsetIn == 0)
                tmp = _mm_moveldup(in);
            else
                tmp = _mm_unpackhi(in, in);
        }
        else
        {
            __m256d perm = Permute2F128<1, 0>(in);
            if constexpr (laneOffsetIn == laneOffsetOut)
                tmp = perm;
            else if constexpr (laneOffsetIn == 0)
                tmp = _mm_moveldup(perm);
            else
                tmp = _mm_unpackhi(perm, perm);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out, tmp);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m256d in0, __m256d in1, __m256d& out0)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256d>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256d tmp0;

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = _mm_unpacklo(in0, in1);
            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else
        {
            __m256d tmp1 = _mm_unpacklo(in1, in0);
            tmp0 = Permute2F128<laneIn, laneIn>(tmp1);
        }
    }
    else
    {
        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = _mm_unpackhi(in0, in1);
            if constexpr (laneIn != laneOut)
                tmp0 = Permute2F128<1, 0>(tmp0);
        }
        else
        {
            __m256d tmp1 = _mm_unpackhi(in1, in0);
            tmp0 = Permute2F128<laneIn, laneIn>(tmp1);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x3(__m256d in0, __m256d in1, __m256d in2, __m256d& out0)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256d>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;

    __m256d tmp0;

    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256d tmp1 = _mm_unpacklo(in0, in1);

            tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp1, in2);
        }
        else
        {
            __m256d tmp1 = _mm_unpacklo(in1, in2);
            __m256d tmp2 = _mm_moveldup(in0);

            tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp2, tmp1);
        }
    }
    else if constexpr (laneOffsetIn == 1)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256d tmp1 = _mm_unpackhi(in0, in1);
            __m256d tmp2 = _mm_unpackhi(in2, in2);

            tmp0 = Permute2F128<0, laneIn, 1, laneIn>(tmp1, tmp2);
        }
        else
        {
            __m256d tmp1 = _mm_unpackhi(in1, in2);

            tmp0 = Permute2F128<0, laneIn, 1, laneIn>(in0, tmp1);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x4(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0)
{

    if constexpr (_firstRowIn == 0)
    {
        __m256d tmp0 = _mm_unpacklo(in0, in1);
        __m256d tmp1 = _mm_unpacklo(in2, in3);

        out0 = Permute2F128<0, 0, 1, 0>(tmp0, tmp1);
    }
    else if constexpr (_firstRowIn == 1)
    {
        __m256d tmp0 = _mm_unpackhi(in0, in1);
        __m256d tmp1 = _mm_unpackhi(in2, in3);

        out0 = Permute2F128<0, 0, 1, 0>(tmp0, tmp1);
    }
    else if constexpr (_firstRowIn == 2)
    {
        __m256d tmp0 = _mm_unpacklo(in0, in1);
        __m256d tmp1 = _mm_unpacklo(in2, in3);

        out0 = Permute2F128<0, 1, 1, 1>(tmp0, tmp1);
    }
    else
    {
        __m256d tmp0 = _mm_unpackhi(in0, in1);
        __m256d tmp1 = _mm_unpackhi(in2, in3);

        out0 = Permute2F128<0, 1, 1, 1>(tmp0, tmp1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m256d in0, __m256d& out0, __m256d& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256d>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256d tmp0, tmp1;

    if constexpr (laneOffsetIn == 0)
    {
        __m256d tmp2;
        if constexpr (laneIn == laneOut)
            tmp2 = in0;
        else
            tmp2 = Permute2F128<1, 0>(in0);

        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = tmp2;
            tmp1 = _mm_unpackhi(tmp2, tmp2);
        }
        else
        {
            tmp0 = _mm_moveldup(tmp2);
            tmp1 = tmp2;
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = _mm_unpackhi(in0, in0);
            tmp1 = Permute2F128<1, 0>(in0);
        }
        else if constexpr (_firstRowOut == 1)
        {
            __m256d tmp2 = Permute2F128<1, 0>(in0);
            tmp0 = in0;
            tmp1 = _mm_moveldup(tmp2);
        }
        else if constexpr (_firstRowOut == 2)
        {
            __m256d tmp2 = Permute2F128<1, 0>(in0);
            tmp0 = _mm_unpackhi(tmp2, tmp2);
            tmp1 = in0;
        }
        else
        {
            tmp0 = Permute2F128<1, 0>(in0);
            tmp1 = _mm_moveldup(in0);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m256d in0, __m256d in1, __m256d& out0, __m256d& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<__m256d>;
    constexpr U32 laneIn = _firstRowIn / numLaneVals;
    constexpr U32 laneOut = _firstRowOut / numLaneVals;
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    __m256d tmp0, tmp1;


    if constexpr (laneOffsetIn == 0)
    {
        if constexpr (laneOffsetOut == 0)
        {
            tmp0 = _mm_unpacklo(in0, in1);
            tmp1 = _mm_unpackhi(in0, in1);
            if constexpr (laneIn != laneOut)
            {
                tmp0 = Permute2F128<1, 0>(tmp0);
                tmp1 = Permute2F128<1, 0>(tmp1);
            }
        }
        else
        {
            if constexpr (_firstRowIn == 0)
            {
                __m256d perm = Permute2F128<1, 0>(in1);
                tmp0 = _mm_unpacklo(perm, in0);
                tmp1 = _mm_unpackhi(perm, in0);
            }
            else
            {
                __m256d perm = Permute2F128<1, 0>(in0);
                tmp0 = _mm_unpacklo(in1, perm);
                tmp1 = _mm_unpackhi(in1, perm);
            }
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = Shuffle<1, 1, 0, 0>(in0, in1);
            tmp1 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (_firstRowOut == 1)
        {
            __m256d tmp2 = Blend<1, 1, 0, 0>(in0, in1);
            __m256d perm = Permute4F64<2, 2, 1, 1>(tmp2);
            tmp0 = Blend<0, 0, 1, 1>(in0, perm);
            tmp1 = Blend<0, 0, 1, 1>(perm, in1);
        }
        else
        {
            tmp1 = Shuffle<1, 1, 0, 0>(in0, in1);
            tmp0 = Permute2F128<1, 0>(tmp1);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x3(__m256d in0, __m256d in1, __m256d in2, __m256d& out0, __m256d& out1)
{
    __m256d tmp0, tmp1;

    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256d tmp2 = Permute2F128<0, 0, 1, 0>(in0, in2);

            tmp0 = _mm_unpacklo(tmp2, in1);
            tmp1 = _mm_unpackhi(tmp2, in1);
        }
        else
        {
            __m256d tmp2 = Permute2F128<0, 0, 1, 0>(in0, in1);
            __m256d tmp3 = Permute2F128<0, 0, 1, 0>(in0, in2);

            tmp0 = _mm_unpacklo(tmp2, tmp3);
            tmp1 = _mm_unpackhi(tmp2, tmp3);
        }
    }
    else if constexpr (_firstRowIn == 1)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256d tmp2 = Shuffle<1, 1, 0, 0>(in0, in1);
            __m256d tmp3 = Broadcast<1, 0>(in2);

            tmp0 = Permute2F128<0, 0, 1, 0>(tmp2, tmp3);
            tmp1 = Permute2F128<0, 1, 1, 1>(tmp2, tmp3);
        }
        else
        {
            __m256d tmp2 = Shuffle<1, 1, 0, 0>(in1, in2);
            __m256d tmp3 = Broadcast<1, 0>(in0);

            tmp0 = Permute2F128<1, 0, 0, 0>(tmp2, tmp3);
            tmp1 = Permute2F128<1, 1, 0, 1>(tmp2, tmp3);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256d tmp2 = Permute2F128<0, 1, 1, 1>(in0, in2);
            __m256d tmp3 = Permute2F128<0, 1, 1, 1>(in1, in2);

            tmp0 = _mm_unpacklo(tmp2, tmp3);
            tmp1 = _mm_unpackhi(tmp2, tmp3);
        }
        else
        {
            __m256d tmp2 = Permute2F128<0, 1, 1, 1>(in0, in2);

            tmp0 = _mm_unpacklo(in1, tmp2);
            tmp1 = _mm_unpackhi(in1, tmp2);
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x4(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1)
{
    if constexpr (_firstRowIn == 0)
    {
        __m256d tmp0 = Permute2F128<0, 0, 1, 0>(in0, in2);
        __m256d tmp1 = Permute2F128<0, 0, 1, 0>(in1, in3);

        out0 = _mm_unpacklo(tmp0, tmp1);
        out1 = _mm_unpackhi(tmp0, tmp1);
    }
    else if constexpr (_firstRowIn == 1)
    {
        __m256d tmp0 = Shuffle<1, 1, 0, 0>(in0, in1);
        __m256d tmp1 = Shuffle<1, 1, 0, 0>(in2, in3);

        out0 = Permute2F128<0, 0, 1, 0>(tmp0, tmp1);
        out1 = Permute2F128<0, 1, 1, 1>(tmp0, tmp1);
    }
    else
    {
        __m256d tmp0 = Permute2F128<0, 1, 1, 1>(in0, in2);
        __m256d tmp1 = Permute2F128<0, 1, 1, 1>(in1, in3);

        out0 = _mm_unpacklo(tmp0, tmp1);
        out1 = _mm_unpackhi(tmp0, tmp1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x1(__m256d in0, __m256d& out0, __m256d& out1, __m256d& out2)
{

    __m256d tmp0, tmp1, tmp2;

    __m256d tmp3 = Permute2F128<1, 0>(in0);

    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = in0;
            tmp1 = _mm_unpackhi(in0, in0);
            tmp2 = tmp3;
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = _mm_moveldup(in0);
            tmp1 = in0;
            tmp2 = _mm_moveldup(tmp3);
        }
        else if constexpr (_firstRowOut == 2)
        {
            tmp0 = tmp3;
            tmp1 = _mm_unpackhi(tmp3, tmp3);
            tmp2 = in0;
        }
        else
        {
            tmp0 = _mm_moveldup(tmp3);
            tmp1 = tmp3;
            tmp2 = _mm_moveldup(in0);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = _mm_unpackhi(in0, in0);
            tmp1 = tmp3;
            tmp2 = _mm_unpackhi(tmp3, tmp3);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = in0;
            tmp1 = _mm_moveldup(tmp3);
            tmp2 = tmp3;
        }
        else if constexpr (_firstRowOut == 2)
        {
            tmp0 = _mm_unpackhi(tmp3, tmp3);
            tmp1 = in0;
            tmp2 = _mm_unpackhi(in0, in0);
        }
        else
        {
            tmp0 = tmp3;
            tmp1 = _mm_moveldup(in0);
            tmp2 = in0;
        }
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------


template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x2(__m256d in0, __m256d in1, __m256d& out0, __m256d& out1, __m256d& out2)
{
    __m256d tmp0, tmp1, tmp2;


    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = _mm_unpacklo(in0, in1);
            tmp1 = _mm_unpackhi(in0, in1);
            tmp2 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (_firstRowOut == 1)
        {
            __m256d tmp3 = Permute2F128<0, 1, 1, 0>(in0, in1);

            tmp0 = _mm_unpacklo(tmp3, in0);
            tmp1 = _mm_unpackhi(tmp3, in0);
            tmp2 = _mm_unpacklo(in1, tmp3);
        }
        else
        {
            __m256d tmp3 = tmp1 = _mm_unpackhi(in0, in1);

            tmp2 = _mm_unpacklo(in0, in1);
            tmp1 = Permute2F128<1, 0>(tmp3);
            tmp0 = Permute2F128<1, 0>(tmp2);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256d tmp3 = _mm_unpacklo(in0, in1);

            tmp0 = _mm_unpackhi(in0, in1);
            tmp1 = Permute2F128<1, 0>(tmp3);
            tmp2 = Permute2F128<1, 0>(tmp0);
        }
        else if constexpr (_firstRowOut == 1)
        {
            __m256d tmp3 = Permute2F128<0, 1, 1, 0>(in0, in1);

            tmp0 = _mm_unpackhi(tmp3, in0);
            tmp1 = _mm_unpacklo(in1, tmp3);
            tmp2 = _mm_unpackhi(in1, tmp3);
        }
        else
        {
            tmp2 = _mm_unpackhi(in0, in1);
            tmp1 = _mm_unpacklo(in0, in1);
            tmp0 = Permute2F128<1, 0>(tmp2);
        }
    }

    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x3(__m256d in0, __m256d in1, __m256d in2, __m256d& out0, __m256d& out1, __m256d& out2)
{
    __m256d tmp0, tmp1, tmp2;


    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256d tmp3 = _mm_unpacklo(in0, in1);
            __m256d tmp4 = Permute2F128<0, 1, 1, 0>(tmp3, in2);
            __m256d tmp5 = BlendIndex<3>(in0, tmp4);

            tmp0 = BlendIndex<2>(tmp3, tmp4);
            tmp2 = BlendIndex<2>(tmp4, in2);
            tmp1 = _mm_unpackhi(tmp5, in1);
        }
        else
        {
            __m256d tmp3 = Permute2F128<0, 1, 1, 0>(in0, in1);
            __m256d tmp4 = Permute2F128<0, 0, 1, 0>(in0, in2);
            __m256d tmp5 = BlendIndex<0>(in2, tmp3);

            tmp0 = _mm_unpacklo(tmp3, tmp4);
            tmp1 = _mm_unpackhi(tmp3, tmp4);
            tmp2 = _mm_unpacklo(in1, tmp5);
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            __m256d tmp3 = Permute2F128<0, 1, 1, 1>(in0, in2);
            __m256d tmp4 = Permute2F128<0, 1, 1, 0>(in1, in2);
            __m256d tmp5 = BlendIndex<3>(in0, tmp4);

            tmp0 = _mm_unpackhi(tmp5, in1);
            tmp1 = _mm_unpacklo(tmp3, tmp4);
            tmp2 = _mm_unpackhi(tmp3, tmp4);
        }
        else
        {
            __m256d tmp3 = _mm_unpackhi(in1, in2);
            __m256d tmp4 = Permute2F128<0, 1, 1, 0>(in0, tmp3);
            __m256d tmp5 = BlendIndex<0>(in2, tmp4);

            tmp0 = BlendIndex<1>(tmp4, in0);
            tmp2 = BlendIndex<1>(tmp3, tmp4);
            tmp1 = _mm_unpacklo(in1, tmp5);
        }
    }



    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tmp0, tmp1, tmp2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x4(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1,
                         __m256d& out2)
{
    __m256d perm0 = Permute2F128<0, 1, 1, 0>(in0, in2);
    __m256d perm1 = Permute2F128<0, 1, 1, 0>(in1, in3);

    __m256d tmp0 = BlendBelowIndex<1>(in0, perm0);
    __m256d tmp1 = BlendBelowIndex<1>(in1, perm1);
    __m256d tmp2 = BlendBelowIndex<1>(perm0, in2);
    __m256d tmp3 = BlendBelowIndex<1>(perm1, in3);

    if constexpr (_firstRowIn == 0)
    {
        out0 = _mm_unpacklo(tmp0, tmp1);
        out1 = _mm_unpackhi(tmp0, tmp1);
        out2 = _mm_unpacklo(tmp2, tmp3);
    }
    else
    {
        out0 = _mm_unpackhi(tmp0, tmp1);
        out1 = _mm_unpacklo(tmp2, tmp3);
        out2 = _mm_unpackhi(tmp2, tmp3);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x1(__m256d in0, __m256d& out0, __m256d& out1, __m256d& out2, __m256d& out3)
{
    __m256d tmp0, tmp1, tmp2, tmp3;

    __m256d tmp4 = Permute2F128<1, 0>(in0);

    if constexpr (_firstRowOut == 0)
    {
        tmp0 = in0;
        tmp1 = _mm_unpackhi(in0, in0);
        tmp2 = tmp4;
        tmp3 = _mm_unpackhi(tmp4, tmp4);
    }
    else if constexpr (_firstRowOut == 1)
    {
        tmp0 = _mm_moveldup(in0);
        tmp1 = in0;
        tmp2 = _mm_moveldup(tmp4);
        tmp3 = tmp4;
    }
    else if constexpr (_firstRowOut == 2)
    {
        tmp0 = tmp4;
        tmp1 = _mm_unpackhi(tmp4, tmp4);
        tmp2 = in0;
        tmp3 = _mm_unpackhi(in0, in0);
    }
    else
    {
        tmp0 = _mm_moveldup(tmp4);
        tmp1 = tmp4;
        tmp2 = _mm_moveldup(in0);
        tmp3 = in0;
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x2(__m256d in0, __m256d in1, __m256d& out0, __m256d& out1, __m256d& out2, __m256d& out3)
{

    __m256d tmp0, tmp1, tmp2, tmp3;


    if constexpr (_firstRowOut == 0)
    {
        tmp0 = _mm_unpacklo(in0, in1);
        tmp1 = _mm_unpackhi(in0, in1);
        tmp2 = Permute2F128<1, 0>(tmp0);
        tmp3 = Permute2F128<1, 0>(tmp1);
    }
    else if constexpr (_firstRowOut == 1)
    {
        __m256d tmp4 = Permute2F128<0, 1, 1, 0>(in0, in1);

        tmp0 = _mm_unpacklo(tmp4, in0);
        tmp1 = _mm_unpackhi(tmp4, in0);
        tmp2 = _mm_unpacklo(in1, tmp4);
        tmp3 = _mm_unpackhi(in1, tmp4);
    }
    else
    {
        tmp3 = _mm_unpackhi(in0, in1);
        tmp2 = _mm_unpacklo(in0, in1);
        tmp1 = Permute2F128<1, 0>(tmp3);
        tmp0 = Permute2F128<1, 0>(tmp2);
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x3(__m256d in0, __m256d in1, __m256d in2, __m256d& out0, __m256d& out1, __m256d& out2,
                         __m256d& out3)
{
    __m256d tmp0, tmp1, tmp2, tmp3;


    if constexpr (_firstRowOut == 0)
    {
        __m256d tmp4 = _mm_unpacklo(in0, in1);
        __m256d tmp5 = _mm_unpackhi(in0, in1);
        __m256d tmp6 = _mm_unpackhi(in2, in2);

        tmp0 = Permute2F128<0, 0, 1, 0>(tmp4, in2);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp5, tmp6);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp4, in2);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp5, tmp6);
    }
    else
    {
        __m256d tmp4 = _mm_unpacklo(in1, in2);
        __m256d tmp5 = _mm_unpackhi(in1, in2);
        __m256d tmp6 = _mm_unpacklo(in0, in0);

        tmp0 = Permute2F128<0, 0, 1, 0>(tmp6, tmp4);
        tmp1 = Permute2F128<0, 0, 1, 0>(in0, tmp5);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp4);
        tmp3 = Permute2F128<0, 1, 1, 1>(in0, tmp5);
    }


    // Write to output registers
    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tmp0, tmp1, tmp2,
                                                                          tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x4(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1,
                         __m256d& out2, __m256d& out3)
{
    __m256d perm0 = Permute2F128<0, 1, 1, 0>(in0, in2);
    __m256d perm1 = Permute2F128<0, 1, 1, 0>(in1, in3);

    __m256d tmp0 = BlendBelowIndex<1>(in0, perm0);
    __m256d tmp1 = BlendBelowIndex<1>(in1, perm1);
    __m256d tmp2 = BlendBelowIndex<1>(perm0, in2);
    __m256d tmp3 = BlendBelowIndex<1>(perm1, in3);

    out0 = _mm_unpacklo(tmp0, tmp1);
    out1 = _mm_unpackhi(tmp0, tmp1);
    out2 = _mm_unpacklo(tmp2, tmp3);
    out3 = _mm_unpackhi(tmp2, tmp3);
}



} // namespace GDL::simd


#endif // __AVX2__
