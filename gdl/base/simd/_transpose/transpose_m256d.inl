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
inline void Transpose1x1(__m256d in, __m256d& out) noexcept
{
    intern::transpose::StaticAssertions<1, 1, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;
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

    intern::transpose::SetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m256d in_0, __m256d in_1, __m256d& out_0) noexcept
{
    intern::transpose::StaticAssertions<1, 2, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;

    __m256d tout;

    if constexpr (Lane::OffsetOut == 0)
    {
        if constexpr (Lane::OffsetIn == 0)
            tout = _mm_unpacklo(in_0, in_1);
        else
            tout = _mm_unpackhi(in_0, in_1);

        if constexpr (Lane::In != Lane::Out)
            tout = SwapLanes(tout);
    }
    else
    {
        if constexpr (Lane::OffsetIn == 0)
            tout = _mm_unpacklo(in_1, in_0);
        else
            tout = _mm_unpackhi(in_1, in_0);

        tout = Permute2F128<Lane::In, Lane::In>(tout);
    }

    intern::transpose::SetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out_0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x3(__m256d in_0, __m256d in_1, __m256d in_2, __m256d& out_0) noexcept
{
    intern::transpose::StaticAssertions<1, 3, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;

    __m256d tmp_0, tmp_1;
    if constexpr (_firstRowOut == 0)
    {
        Transpose1x2<Lane::OffsetIn, 0>(in_0, in_1, tmp_0);
        Transpose1x1<Lane::OffsetIn, 0>(in_2, tmp_1);
    }
    else
    {
        Transpose1x1<Lane::OffsetIn, 1>(in_0, tmp_0);
        Transpose1x2<Lane::OffsetIn, 0>(in_1, in_2, tmp_1);
    }

    __m256d tout = Permute2F128<0, Lane::In, 1, Lane::In>(tmp_0, tmp_1);

    intern::transpose::SetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out_0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x4(__m256d in_0, __m256d in_1, __m256d in_2, __m256d in_3, __m256d& out_0) noexcept
{
    intern::transpose::StaticAssertions<1, 4, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;

    __m256d tmp_0, tmp_1;
    Transpose1x2<Lane::OffsetIn, 0>(in_0, in_1, tmp_0);
    Transpose1x2<Lane::OffsetIn, 0>(in_2, in_3, tmp_1);

    out_0 = Permute2F128<0, Lane::In, 1, Lane::In>(tmp_0, tmp_1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m256d in_0, __m256d& out_0, __m256d& out_1) noexcept
{
    intern::transpose::StaticAssertions<2, 1, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;

    std::array<__m256d, 2> tout;

    if constexpr (Lane::OffsetIn == 0)
    {
        __m256d tmp_0 = SwapLanesIf<Lane::In != Lane::Out>(in_0);
        Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp_0, tout[0]);
        Transpose1x1<Lane::OffsetIn + 1, Lane::OffsetOut>(tmp_0, tout[1]);
    }
    else
    {
        __m256d tmp_0 = SwapLanesIf<Lane::Out == 1>(in_0);
        __m256d tmp_1 = SwapLanesIf<Lane::Out == 0>(in_0);

        Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp_0, tout[0]);
        Transpose1x1<0, Lane::OffsetOut>(tmp_1, tout[1]);
    }

    intern::transpose::SetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m256d in_0, __m256d in_1, __m256d& out_0, __m256d& out_1) noexcept
{
    intern::transpose::StaticAssertions<2, 2, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;

    std::array<__m256d, 2> tout;
    if constexpr (Lane::OffsetOut == 0)
    {
        if constexpr (Lane::OffsetIn == 0)
        {
            __m256d tmp_0 = _mm_unpacklo(in_0, in_1);
            __m256d tmp_1 = _mm_unpackhi(in_0, in_1);

            tout[0] = SwapLanesIf<Lane::In != Lane::Out>(tmp_0);
            tout[1] = SwapLanesIf<Lane::In != Lane::Out>(tmp_1);
        }
        else
        {
            tout[Lane::Out] = Shuffle<1, 1, 0, 0>(in_0, in_1);
            tout[1 - Lane::Out] = SwapLanes(tout[Lane::Out]);
        }
    }
    else
    {
        if constexpr (Lane::OffsetIn == 0)
        {
            __m256d tmp_0 = SwapLanesIf<Lane::In == 0>(in_1);
            __m256d tmp_1 = SwapLanesIf<Lane::In == 1>(in_0);

            tout[0] = _mm_unpacklo(tmp_0, tmp_1);
            tout[1] = _mm_unpackhi(tmp_0, tmp_1);
        }
        else
        {
            __m256d tmp_2 = Blend<1, 1, 0, 0>(in_0, in_1);
            __m256d perm = Permute4F64<2, 2, 1, 1>(tmp_2);
            tout[0] = Blend<0, 0, 1, 1>(in_0, perm);
            tout[1] = Blend<0, 0, 1, 1>(perm, in_1);
        }
    }

    intern::transpose::SetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x3(__m256d in_0, __m256d in_1, __m256d in_2, __m256d& out_0, __m256d& out_1) noexcept
{
    intern::transpose::StaticAssertions<2, 3, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;
    std::array<__m256d, 3> tin = {{in_0, in_1, in_2}};
    std::array<__m256d, 2> tout;

    if constexpr (Lane::OffsetIn == 0)
    {
        __m256d tmp_0, tmp_1;
        if constexpr (_firstRowOut == 1 && Lane::In == 1)
            tmp_0 = in_1;
        else
            tmp_0 = Permute2F128<0, Lane::In, 1, Lane::In>(tin[0], tin[2 - _firstRowOut]);

        if constexpr (_firstRowOut == 0 && Lane::In == 0)
            tmp_1 = in_1;
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

    intern::transpose::SetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x4(__m256d in_0, __m256d in_1, __m256d in_2, __m256d in_3, __m256d& out_0,
                         __m256d& out_1) noexcept
{
    intern::transpose::StaticAssertions<2, 4, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;

    if constexpr (Lane::OffsetIn == 0)
    {
        __m256d tmp_0 = Permute2F128<0, Lane::In, 1, Lane::In>(in_0, in_2);
        __m256d tmp_1 = Permute2F128<0, Lane::In, 1, Lane::In>(in_1, in_3);

        out_0 = _mm_unpacklo(tmp_0, tmp_1);
        out_1 = _mm_unpackhi(tmp_0, tmp_1);
    }
    else
    {
        __m256d tmp_0 = Shuffle<1, 1, 0, 0>(in_0, in_1);
        __m256d tmp_1 = Shuffle<1, 1, 0, 0>(in_2, in_3);

        out_0 = Permute2F128<0, 0, 1, 0>(tmp_0, tmp_1);
        out_1 = Permute2F128<0, 1, 1, 1>(tmp_0, tmp_1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x1(__m256d in_0, __m256d& out_0, __m256d& out_1, __m256d& out_2) noexcept
{
    intern::transpose::StaticAssertions<3, 1, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;
    std::array<__m256d, 3> tout;

    std::array<__m256d, 2> tmp;
    tmp[0] = SwapLanesIf<Lane::Out == 1>(in_0);
    tmp[1] = SwapLanesIf<Lane::Out == 0>(in_0);

    Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp[0], tout[0]);
    Transpose1x1<1 - Lane::OffsetIn, Lane::OffsetOut>(tmp[Lane::OffsetIn], tout[1]);
    Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp[1], tout[2]);

    intern::transpose::SetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1, out_2);
}



// --------------------------------------------------------------------------------------------------------------------


template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x2(__m256d in_0, __m256d in_1, __m256d& out_0, __m256d& out_1, __m256d& out_2) noexcept
{
    intern::transpose::StaticAssertions<3, 2, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    std::array<__m256d, 3> tout;

    if constexpr (_firstRowOut == 0)
    {
        __m256d tmp_0 = _mm_unpacklo(in_0, in_1);

        tout[_firstRowIn] = SwapLanesIf<_firstRowIn == 1>(tmp_0);
        tout[1 - _firstRowIn] = _mm_unpackhi(in_0, in_1);
        tout[2] = SwapLanes(tout[0]);
    }
    else if constexpr (_firstRowOut == 1)
    {
        __m256d tmp_0 = Permute2F128<0, 1, 1, 0>(in_0, in_1);

        if constexpr (_firstRowIn == 0)
            tout[0] = _mm_unpacklo(tmp_0, in_0);
        else
            tout[2] = _mm_unpackhi(in_1, tmp_0);
        tout[1 - _firstRowIn] = _mm_unpackhi(tmp_0, in_0);
        tout[2 - _firstRowIn] = _mm_unpacklo(in_1, tmp_0);
    }
    else
    {
        __m256d tmp_0 = _mm_unpackhi(in_0, in_1);

        tout[1 + _firstRowIn] = SwapLanesIf<_firstRowIn == 0>(tmp_0);
        tout[2 - _firstRowIn] = _mm_unpacklo(in_0, in_1);
        tout[0] = SwapLanes(tout[2]);
    }

    intern::transpose::SetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1, out_2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x3(__m256d in_0, __m256d in_1, __m256d in_2, __m256d& out_0, __m256d& out_1,
                         __m256d& out_2) noexcept
{
    intern::transpose::StaticAssertions<3, 3, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    std::array<__m256d, 3> tout;

    if constexpr (_firstRowOut == 0)
    {
        if constexpr (_firstRowIn == 0)
        {
            __m256d tmp_0 = _mm_unpacklo(in_0, in_1);
            __m256d tmp_1 = Permute2F128<0, 1, 1, 0>(tmp_0, in_2);
            __m256d tmp_2 = BlendIndex<3>(in_0, tmp_1);

            tout[0] = BlendIndex<2>(tmp_0, tmp_1);
            tout[2] = BlendIndex<2>(tmp_1, in_2);
            tout[1] = _mm_unpackhi(tmp_2, in_1);
        }
        else
        {
            __m256d tmp_0 = Permute2F128<0, 1, 1, 1>(in_0, in_2);
            __m256d tmp_1 = Permute2F128<0, 1, 1, 0>(in_1, in_2);
            __m256d tmp_2 = BlendIndex<3>(in_0, tmp_1);

            tout[0] = _mm_unpackhi(tmp_2, in_1);
            tout[1] = _mm_unpacklo(tmp_0, tmp_1);
            tout[2] = _mm_unpackhi(tmp_0, tmp_1);
        }
    }
    else
    {
        if constexpr (_firstRowIn == 0)
        {
            __m256d tmp_0 = Permute2F128<0, 1, 1, 0>(in_0, in_1);
            __m256d tmp_1 = Permute2F128<0, 0, 1, 0>(in_0, in_2);
            __m256d tmp_2 = BlendIndex<0>(in_2, tmp_0);

            tout[0] = _mm_unpacklo(tmp_0, tmp_1);
            tout[1] = _mm_unpackhi(tmp_0, tmp_1);
            tout[2] = _mm_unpacklo(in_1, tmp_2);
        }
        else
        {
            __m256d tmp_0 = _mm_unpackhi(in_1, in_2);
            __m256d tmp_1 = Permute2F128<0, 1, 1, 0>(in_0, tmp_0);
            __m256d tmp_2 = BlendIndex<0>(in_2, tmp_1);

            tout[0] = BlendIndex<1>(tmp_1, in_0);
            tout[2] = BlendIndex<1>(tmp_0, tmp_1);
            tout[1] = _mm_unpacklo(in_1, tmp_2);
        }
    }

    intern::transpose::SetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1, out_2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x4(__m256d in_0, __m256d in_1, __m256d in_2, __m256d in_3, __m256d& out_0, __m256d& out_1,
                         __m256d& out_2) noexcept
{
    intern::transpose::StaticAssertions<3, 4, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();

    __m256d tmp_0 = Permute2F128<0, 1, 1, 0>(in_0, in_2);
    __m256d tmp_1 = Permute2F128<0, 1, 1, 0>(in_1, in_3);

    __m256d tmp_2 = BlendBelowIndex<1>(in_0, tmp_0);
    __m256d tmp_3 = BlendBelowIndex<1>(in_1, tmp_1);
    __m256d tmp_4 = BlendBelowIndex<1>(tmp_0, in_2);
    __m256d tmp_5 = BlendBelowIndex<1>(tmp_1, in_3);

    if constexpr (_firstRowIn == 0)
    {
        out_0 = _mm_unpacklo(tmp_2, tmp_3);
        out_1 = _mm_unpackhi(tmp_2, tmp_3);
        out_2 = _mm_unpacklo(tmp_4, tmp_5);
    }
    else
    {
        out_0 = _mm_unpackhi(tmp_2, tmp_3);
        out_1 = _mm_unpacklo(tmp_4, tmp_5);
        out_2 = _mm_unpackhi(tmp_4, tmp_5);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x1(__m256d in_0, __m256d& out_0, __m256d& out_1, __m256d& out_2, __m256d& out_3) noexcept
{
    intern::transpose::StaticAssertions<4, 1, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;
    std::array<__m256d, 4> tout;

    std::array<__m256d, 2> tmp;
    tmp[0] = SwapLanesIf<Lane::Out == 1>(in_0);
    tmp[1] = SwapLanesIf<Lane::Out == 0>(in_0);

    Transpose1x1<0, Lane::OffsetOut>(tmp[0], tout[0]);
    Transpose1x1<1, Lane::OffsetOut>(tmp[0], tout[1]);
    Transpose1x1<0, Lane::OffsetOut>(tmp[1], tout[2]);
    Transpose1x1<1, Lane::OffsetOut>(tmp[1], tout[3]);

    intern::transpose::SetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1, out_2, out_3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x2(__m256d in_0, __m256d in_1, __m256d& out_0, __m256d& out_1, __m256d& out_2,
                         __m256d& out_3) noexcept
{
    intern::transpose::StaticAssertions<4, 2, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    using Lane = intern::transpose::LaneData<__m256d, _firstRowIn, _firstRowOut>;
    std::array<__m256d, 4> tout;

    if constexpr (Lane::OffsetOut == 0)
    {
        __m256d tmp_0 = _mm_unpacklo(in_0, in_1);
        __m256d tmp_1 = _mm_unpackhi(in_0, in_1);

        tout[0] = SwapLanesIf<Lane::Out == 1>(tmp_0);
        tout[1] = SwapLanesIf<Lane::Out == 1>(tmp_1);
        tout[2] = SwapLanesIf<Lane::Out == 0>(tmp_0);
        tout[3] = SwapLanesIf<Lane::Out == 0>(tmp_1);
    }
    else
    {
        __m256d tmp_0 = Permute2F128<0, 1, 1, 0>(in_0, in_1);

        tout[0] = _mm_unpacklo(tmp_0, in_0);
        tout[1] = _mm_unpackhi(tmp_0, in_0);
        tout[2] = _mm_unpacklo(in_1, tmp_0);
        tout[3] = _mm_unpackhi(in_1, tmp_0);
    }

    intern::transpose::SetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1, out_2, out_3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x3(__m256d in_0, __m256d in_1, __m256d in_2, __m256d& out_0, __m256d& out_1, __m256d& out_2,
                         __m256d& out_3) noexcept
{
    intern::transpose::StaticAssertions<4, 3, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();
    std::array<__m256d, 4> tout;

    if constexpr (_firstRowOut == 0)
    {
        __m256d tmp_0 = _mm_unpacklo(in_0, in_1);
        __m256d tmp_1 = _mm_unpackhi(in_0, in_1);
        __m256d tmp_2 = _mm_unpackhi(in_2, in_2);

        tout[0] = Permute2F128<0, 0, 1, 0>(tmp_0, in_2);
        tout[1] = Permute2F128<0, 0, 1, 0>(tmp_1, tmp_2);
        tout[2] = Permute2F128<0, 1, 1, 1>(tmp_0, in_2);
        tout[3] = Permute2F128<0, 1, 1, 1>(tmp_1, tmp_2);
    }
    else
    {
        __m256d tmp_0 = _mm_unpacklo(in_1, in_2);
        __m256d tmp_1 = _mm_unpackhi(in_1, in_2);
        __m256d tmp_2 = _mm_unpacklo(in_0, in_0);

        tout[0] = Permute2F128<0, 0, 1, 0>(tmp_2, tmp_0);
        tout[1] = Permute2F128<0, 0, 1, 0>(in_0, tmp_1);
        tout[2] = Permute2F128<0, 1, 1, 1>(tmp_2, tmp_0);
        tout[3] = Permute2F128<0, 1, 1, 1>(in_0, tmp_1);
    }

    intern::transpose::SetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1, out_2, out_3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x4(__m256d in_0, __m256d in_1, __m256d in_2, __m256d in_3, __m256d& out_0, __m256d& out_1,
                         __m256d& out_2, __m256d& out_3) noexcept
{
    intern::transpose::StaticAssertions<4, 4, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero, __m256d>();

    __m256d tmp_0 = Permute2F128<0, 1, 1, 0>(in_0, in_2);
    __m256d tmp_1 = Permute2F128<0, 1, 1, 0>(in_1, in_3);

    __m256d tmp_2 = BlendBelowIndex<1>(in_0, tmp_0);
    __m256d tmp_3 = BlendBelowIndex<1>(in_1, tmp_1);
    __m256d tmp_4 = BlendBelowIndex<1>(tmp_0, in_2);
    __m256d tmp_5 = BlendBelowIndex<1>(tmp_1, in_3);

    out_0 = _mm_unpacklo(tmp_2, tmp_3);
    out_1 = _mm_unpackhi(tmp_2, tmp_3);
    out_2 = _mm_unpacklo(tmp_4, tmp_5);
    out_3 = _mm_unpackhi(tmp_4, tmp_5);
}



} // namespace GDL::simd


#endif // __AVX2__
