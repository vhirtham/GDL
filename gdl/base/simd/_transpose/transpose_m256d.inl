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
    using Lane = intern::TranspositionLaneData<__m256d, _firstRowIn, _firstRowOut>;
    __m256d tout;

    if constexpr (_firstRowIn == _firstRowOut)
        tout = in;
    else
    {
        if constexpr (Lane::In == Lane::Out)
            tout = Broadcast<Lane::OffsetIn>(in);
        else
            tout = BroadcastAcrossLanes<_firstRowIn>(in);
    }

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m256d in0, __m256d in1, __m256d& out0)
{
    using Lane = intern::TranspositionLaneData<__m256d, _firstRowIn, _firstRowOut>;

    __m256d tout;

    if constexpr (Lane::OffsetOut == 0)
    {
        if constexpr (Lane::OffsetIn == 0)
            tout = _mm_unpacklo(in0, in1);
        else
            tout = _mm_unpackhi(in0, in1);

        if constexpr (Lane::In != Lane::Out)
            tout = Permute2F128<1, 0>(tout);
    }
    else
    {
        if constexpr (Lane::OffsetIn == 0)
            tout = _mm_unpacklo(in1, in0);
        else
            tout = _mm_unpackhi(in1, in0);

        tout = Permute2F128<Lane::In, Lane::In>(tout);
    }

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x3(__m256d in0, __m256d in1, __m256d in2, __m256d& out0)
{
    using Lane = intern::TranspositionLaneData<__m256d, _firstRowIn, _firstRowOut>;

    __m256d tmp0, tmp1;
    if constexpr (_firstRowOut == 0)
    {
        Transpose1x2<Lane::OffsetIn, 0>(in0, in1, tmp0);
        Transpose1x1<Lane::OffsetIn, 0>(in2, tmp1);
    }
    else
    {
        Transpose1x1<Lane::OffsetIn, 1>(in0, tmp0);
        Transpose1x2<Lane::OffsetIn, 0>(in1, in2, tmp1);
    }

    __m256d tout = Permute2F128<0, Lane::In, 1, Lane::In>(tmp0, tmp1);

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x4(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0)
{
    using Lane = intern::TranspositionLaneData<__m256d, _firstRowIn, _firstRowOut>;

    __m256d tmp0, tmp1;
    Transpose1x2<Lane::OffsetIn, 0>(in0, in1, tmp0);
    Transpose1x2<Lane::OffsetIn, 0>(in2, in3, tmp1);

    out0 = Permute2F128<0, Lane::In, 1, Lane::In>(tmp0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m256d in0, __m256d& out0, __m256d& out1)
{
    using Lane = intern::TranspositionLaneData<__m256d, _firstRowIn, _firstRowOut>;

    std::array<__m256d, 2> tout;

    if constexpr (Lane::OffsetIn == 0)
    {
        __m256d tmp0 = SwapLanesIf<Lane::In != Lane::Out>(in0);
        Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp0, tout[0]);
        Transpose1x1<Lane::OffsetIn + 1, Lane::OffsetOut>(tmp0, tout[1]);
    }
    else
    {
        __m256d tmp0 = SwapLanesIf<Lane::Out == 1>(in0);
        __m256d tmp1 = SwapLanesIf<Lane::Out == 0>(in0);

        Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp0, tout[0]);
        Transpose1x1<0, Lane::OffsetOut>(tmp1, tout[1]);
    }

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m256d in0, __m256d in1, __m256d& out0, __m256d& out1)
{
    using Lane = intern::TranspositionLaneData<__m256d, _firstRowIn, _firstRowOut>;

    std::array<__m256d, 2> tout;
    if constexpr (Lane::OffsetOut == 0)
    {
        if constexpr (Lane::OffsetIn == 0)
        {
            __m256d tmp_0 = _mm_unpacklo(in0, in1);
            __m256d tmp_1 = _mm_unpackhi(in0, in1);

            tout[0] = SwapLanesIf<Lane::In != Lane::Out>(tmp_0);
            tout[1] = SwapLanesIf<Lane::In != Lane::Out>(tmp_1);
        }
        else
        {
            tout[Lane::Out] = Shuffle<1, 1, 0, 0>(in0, in1);
            tout[1 - Lane::Out] = SwapLanes(tout[Lane::Out]);
        }
    }
    else
    {
        if constexpr (Lane::OffsetIn == 0)
        {
            __m256d tmp_0 = SwapLanesIf<Lane::In == 0>(in1);
            __m256d tmp_1 = SwapLanesIf<Lane::In == 1>(in0);

            tout[0] = _mm_unpacklo(tmp_0, tmp_1);
            tout[1] = _mm_unpackhi(tmp_0, tmp_1);
        }
        else
        {
            __m256d tmp2 = Blend<1, 1, 0, 0>(in0, in1);
            __m256d perm = Permute4F64<2, 2, 1, 1>(tmp2);
            tout[0] = Blend<0, 0, 1, 1>(in0, perm);
            tout[1] = Blend<0, 0, 1, 1>(perm, in1);
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x3(__m256d in0, __m256d in1, __m256d in2, __m256d& out0, __m256d& out1)
{
    using Lane = intern::TranspositionLaneData<__m256d, _firstRowIn, _firstRowOut>;
    std::array<__m256d, 3> tin = {{in0, in1, in2}};
    std::array<__m256d, 2> tout;

    if constexpr (Lane::OffsetIn == 0)
    {
        __m256d tmp_0, tmp_1;
        if constexpr (_firstRowOut == 1 && Lane::In == 1)
            tmp_0 = in1;
        else
            tmp_0 = Permute2F128<0, Lane::In, 1, Lane::In>(tin[0], tin[2 - _firstRowOut]);

        if constexpr (_firstRowOut == 0 && Lane::In == 0)
            tmp_1 = in1;
        else
            tmp_1 = Permute2F128<0, Lane::In, 1, Lane::In>(tin[1 - _firstRowOut], tin[2]);

        tout[0] = _mm_unpacklo(tmp_0, tmp_1);
        tout[1] = _mm_unpackhi(tmp_0, tmp_1);
    }
    else
    {
        constexpr U32 idx_0 = _firstRowOut;
        constexpr U32 idx_1 = idx_0 + 1;
        constexpr U32 idx_2 = (idx_0 + 2) % 3;

        __m256d tmp_0 = Shuffle<1, 1, 0, 0>(tin[idx_0], tin[idx_1]);
        __m256d tmp_1 = Broadcast<1, 0>(tin[idx_2]);

        tout[0] = Permute2F128<idx_0, 0, 1 - idx_0, 0>(tmp_0, tmp_1);
        tout[1] = Permute2F128<idx_0, 1, 1 - idx_0, 1>(tmp_0, tmp_1);
    }

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x4(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1)
{
    using Lane = intern::TranspositionLaneData<__m256d, _firstRowIn, _firstRowOut>;

    if constexpr (Lane::OffsetIn == 0)
    {
        __m256d tmp_0 = Permute2F128<0, Lane::In, 1, Lane::In>(in0, in2);
        __m256d tmp_1 = Permute2F128<0, Lane::In, 1, Lane::In>(in1, in3);

        out0 = _mm_unpacklo(tmp_0, tmp_1);
        out1 = _mm_unpackhi(tmp_0, tmp_1);
    }
    else
    {
        __m256d tmp_0 = Shuffle<1, 1, 0, 0>(in0, in1);
        __m256d tmp_1 = Shuffle<1, 1, 0, 0>(in2, in3);

        out0 = Permute2F128<0, 0, 1, 0>(tmp_0, tmp_1);
        out1 = Permute2F128<0, 1, 1, 1>(tmp_0, tmp_1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x1(__m256d in0, __m256d& out0, __m256d& out1, __m256d& out2)
{
    using Lane = intern::TranspositionLaneData<__m256d, _firstRowIn, _firstRowOut>;
    std::array<__m256d, 3> tout;

    std::array<__m256d, 2> tmp;
    tmp[0] = SwapLanesIf<Lane::Out == 1>(in0);
    tmp[1] = SwapLanesIf<Lane::Out == 0>(in0);

    Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp[0], tout[0]);
    Transpose1x1<1 - Lane::OffsetIn, Lane::OffsetOut>(tmp[Lane::OffsetIn], tout[1]);
    Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp[1], tout[2]);

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------


template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x2(__m256d in0, __m256d in1, __m256d& out0, __m256d& out1, __m256d& out2)
{
    std::array<__m256d, 3> tout;

    if constexpr (_firstRowOut == 0)
    {
        __m256d tmp_0 = _mm_unpacklo(in0, in1);

        tout[_firstRowIn] = SwapLanesIf<_firstRowIn == 1>(tmp_0);
        tout[1 - _firstRowIn] = _mm_unpackhi(in0, in1);
        tout[2] = Permute2F128<1, 0>(tout[0]);
    }
    else if constexpr (_firstRowOut == 1)
    {
        __m256d tmp_0 = Permute2F128<0, 1, 1, 0>(in0, in1);

        if constexpr (_firstRowIn == 0)
            tout[0] = _mm_unpacklo(tmp_0, in0);
        else
            tout[2] = _mm_unpackhi(in1, tmp_0);
        tout[1 - _firstRowIn] = _mm_unpackhi(tmp_0, in0);
        tout[2 - _firstRowIn] = _mm_unpacklo(in1, tmp_0);
    }
    else
    {
        __m256d tmp_0 = _mm_unpackhi(in0, in1);

        tout[1 + _firstRowIn] = SwapLanesIf<_firstRowIn == 0>(tmp_0);
        tout[2 - _firstRowIn] = _mm_unpacklo(in0, in1);
        tout[0] = Permute2F128<1, 0>(tout[2]);
    }

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x3(__m256d in0, __m256d in1, __m256d in2, __m256d& out0, __m256d& out1, __m256d& out2)
{
    std::array<__m256d, 3> tout;

    if constexpr (_firstRowOut == 0)
    {
        if constexpr (_firstRowIn == 0)
        {
            __m256d tmp_0 = _mm_unpacklo(in0, in1);
            __m256d tmp_1 = Permute2F128<0, 1, 1, 0>(tmp_0, in2);
            __m256d tmp_2 = BlendIndex<3>(in0, tmp_1);

            tout[0] = BlendIndex<2>(tmp_0, tmp_1);
            tout[2] = BlendIndex<2>(tmp_1, in2);
            tout[1] = _mm_unpackhi(tmp_2, in1);
        }
        else
        {
            __m256d tmp_0 = Permute2F128<0, 1, 1, 1>(in0, in2);
            __m256d tmp_1 = Permute2F128<0, 1, 1, 0>(in1, in2);
            __m256d tmp_2 = BlendIndex<3>(in0, tmp_1);

            tout[0] = _mm_unpackhi(tmp_2, in1);
            tout[1] = _mm_unpacklo(tmp_0, tmp_1);
            tout[2] = _mm_unpackhi(tmp_0, tmp_1);
        }
    }
    else
    {
        if constexpr (_firstRowIn == 0)
        {
            __m256d tmp_0 = Permute2F128<0, 1, 1, 0>(in0, in1);
            __m256d tmp_1 = Permute2F128<0, 0, 1, 0>(in0, in2);
            __m256d tmp_2 = BlendIndex<0>(in2, tmp_0);

            tout[0] = _mm_unpacklo(tmp_0, tmp_1);
            tout[1] = _mm_unpackhi(tmp_0, tmp_1);
            tout[2] = _mm_unpacklo(in1, tmp_2);
        }
        else
        {
            __m256d tmp_0 = _mm_unpackhi(in1, in2);
            __m256d tmp_1 = Permute2F128<0, 1, 1, 0>(in0, tmp_0);
            __m256d tmp_2 = BlendIndex<0>(in2, tmp_1);

            tout[0] = BlendIndex<1>(tmp_1, in0);
            tout[2] = BlendIndex<1>(tmp_0, tmp_1);
            tout[1] = _mm_unpacklo(in1, tmp_2);
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x4(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1,
                         __m256d& out2)
{
    __m256d tmp_0 = Permute2F128<0, 1, 1, 0>(in0, in2);
    __m256d tmp_1 = Permute2F128<0, 1, 1, 0>(in1, in3);

    __m256d tmp_2 = BlendBelowIndex<1>(in0, tmp_0);
    __m256d tmp_3 = BlendBelowIndex<1>(in1, tmp_1);
    __m256d tmp_4 = BlendBelowIndex<1>(tmp_0, in2);
    __m256d tmp_5 = BlendBelowIndex<1>(tmp_1, in3);

    if constexpr (_firstRowIn == 0)
    {
        out0 = _mm_unpacklo(tmp_2, tmp_3);
        out1 = _mm_unpackhi(tmp_2, tmp_3);
        out2 = _mm_unpacklo(tmp_4, tmp_5);
    }
    else
    {
        out0 = _mm_unpackhi(tmp_2, tmp_3);
        out1 = _mm_unpacklo(tmp_4, tmp_5);
        out2 = _mm_unpackhi(tmp_4, tmp_5);
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
