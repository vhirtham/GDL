#pragma once

#include "gdl/base/simd/_transpose/transpose_m128.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/_transpose/transpose_utility.h"

namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x1(__m128 in, __m128& out) noexcept
{
    __m128 tout;

    if constexpr (_firstRowIn == _firstRowOut && not(_overwriteUnused && _unusedSetZero))
        tout = in;
    else
    {
        constexpr bool z0 = (_firstRowOut == 0) ? false : true;
        constexpr bool z1 = (_firstRowOut == 1) ? false : true;
        constexpr bool z2 = (_firstRowOut == 2) ? false : true;
        constexpr bool z3 = (_firstRowOut == 3) ? false : true;

        tout = Insert<_firstRowIn, _firstRowOut, z0, z1, z2, z3>(in, in);
    }

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, false>(tout, out);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m128 in_0, __m128 in_1, __m128& out_0) noexcept
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
                out_0 = Insert<_firstRowIn, _firstRowIn + 1, b0, b0, b1, b1>(in_1, in_0);
            else
                out_0 = Insert<_firstRowIn, _firstRowIn - 1, b0, b0, b1, b1>(in_0, in_1);

            constexpr bool perm_con = (_firstRowOut == 0) ? _firstRowIn > 1 : _firstRowIn < 2;
            if constexpr (perm_con)
                out_0 = Permute<2, 3, 0, 1>(out_0);
        }
        else
        {
            if constexpr (_firstRowIn == 1)
                out_0 = Insert<1, 2, true, false, false, true>(in_1, in_0);
            else
                out_0 = Insert<2, 1, true, false, false, true>(in_0, in_1);
        }
        return;
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {

            if constexpr (_firstRowIn == 0)
                tout = _mm_unpacklo(in_0, in_1);
            else if constexpr (_firstRowIn == 1)
                tout = Insert<1, 0>(in_0, in_1);
            else if constexpr (_firstRowIn == 2)
                tout = _mm_unpackhi(in_0, in_1);
            else
            {
                __m128 tmp_0 = _mm_unpackhi(in_0, in_1);
                tout = _mm_movehl(tmp_0, tmp_0);
            }
        }
        else if constexpr (_firstRowOut == 1)
        {
            tout = Shuffle<_firstRowIn, _firstRowIn, _firstRowIn, _firstRowIn>(in_0, in_1);
        }
        else
        {
            if constexpr (_firstRowIn == 0)
            {
                __m128 tmp_0 = _mm_unpacklo(in_0, in_1);
                tout = _mm_movelh(tmp_0, tmp_0);
            }
            else if constexpr (_firstRowIn == 1)
                tout = _mm_unpacklo(in_0, in_1);
            else if constexpr (_firstRowIn == 2)
                tout = Insert<2, 3>(in_1, in_0);
            else
                tout = _mm_unpackhi(in_0, in_1);
        }
    }


    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out_0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x3(__m128 in_0, __m128 in_1, __m128 in_2, __m128& out_0) noexcept
{
    __m128 tout;

    if constexpr (_firstRowOut == 0)
    {
        __m128 tmp_0 = Shuffle<_firstRowIn, _firstRowIn, _firstRowIn, _firstRowIn>(in_1, in_2);
        tout = Insert<_firstRowIn, 0, false, false, false, true>(in_0, tmp_0);
    }

    else
    {
        __m128 tmp_0 = Shuffle<_firstRowIn, _firstRowIn, _firstRowIn, _firstRowIn>(in_0, in_1);
        tout = Insert<_firstRowIn, 3, true, false, false, false>(in_2, tmp_0);
    }

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, false>(tout, out_0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x4(__m128 in_0, __m128 in_1, __m128 in_2, __m128 in_3, __m128& out_0) noexcept
{
    __m128 tmp_0 = Shuffle<_firstRowIn, _firstRowIn, _firstRowIn, _firstRowIn>(in_0, in_1);
    __m128 tmp1 = Shuffle<_firstRowIn, _firstRowIn, _firstRowIn, _firstRowIn>(in_2, in_3);

    out_0 = Shuffle<0, 2, 0, 2>(tmp_0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m128 in_0, __m128& out_0, __m128& out_1) noexcept
{
    std::array<__m128, 2> tout;

    Transpose1x1<_firstRowIn, _firstRowOut, true, _unusedSetZero>(in_0, tout[0]);
    Transpose1x1<_firstRowIn + 1, _firstRowOut, true, _unusedSetZero>(in_0, tout[1]);

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, false>(tout, out_0, out_1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m128 in_0, __m128 in_1, __m128& out_0, __m128& out_1) noexcept
{
    std::array<__m128, 2> tout;

    if constexpr (_firstRowIn == _firstRowOut)
    {
        constexpr bool b0 = _firstRowIn > 0;
        constexpr bool b1 = _firstRowIn > 1;
        constexpr bool b2 = _firstRowIn < 1;
        constexpr bool b3 = _firstRowIn < 2;

        tout[0] = Insert<_firstRowIn, _firstRowIn + 1, b0, b1, b2, b3>(in_1, in_0);
        tout[1] = Insert<_firstRowIn + 1, _firstRowIn, b0, b1, b2, b3>(in_0, in_1);

        if constexpr (_overwriteUnused && _unusedSetZero)
        {
            out_0 = tout[0];
            out_1 = tout[1];
            return;
        }
    }
    else if constexpr (_firstRowOut == 0)
    {
        if constexpr (_firstRowIn == 1)
        {
            tout[0] = Insert<1, 0, false, false, true, true>(in_0, in_1);
            tout[1] = _mm_unpackhi(in_0, in_1);
        }
        else
        {
            tout[0] = _mm_unpackhi(in_0, in_1);
            tout[1] = _mm_movehl(tout[0], tout[0]);
        }
    }
    else if constexpr (_firstRowOut == 1)
    {
        constexpr U32 s0 = _firstRowIn;
        constexpr U32 s1 = _firstRowIn + 1;

        tout[0] = Shuffle<s0, s0, s0, s0>(in_0, in_1);
        tout[1] = Shuffle<s1, s1, s1, s1>(in_0, in_1);
    }
    else
    {
        if constexpr (_firstRowIn == 0)
        {
            tout[1] = _mm_unpacklo(in_0, in_1);
            tout[0] = _mm_movelh(tout[1], tout[1]);
        }
        else
        {
            tout[0] = _mm_unpacklo(in_0, in_1);
            tout[1] = Insert<2, 3, true, true, false, false>(in_1, in_0);
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x3(__m128 in_0, __m128 in_1, __m128 in_2, __m128& out_0, __m128& out_1) noexcept
{
    constexpr U32 idx_0 = _firstRowOut;
    constexpr U32 idx_1 = _firstRowOut + 1;
    std::array<__m128, 3> tin = {{in_0, in_1, in_2}};

    __m128 tmp_0;
    if constexpr (_firstRowIn == 0)
        tmp_0 = _mm_unpacklo(tin[idx_0], tin[idx_1]);
    else if constexpr (_firstRowIn == 1)
        tmp_0 = Shuffle<1, 2, 1, 2>(tin[idx_0], tin[idx_1]);
    else
        tmp_0 = _mm_unpackhi(tin[idx_0], tin[idx_1]);


    constexpr U32 s0 = (_firstRowIn % 2 == 0) ? 1 : 2;
    constexpr U32 s1 = (_firstRowIn % 2 == 0) ? 2 : 1;
    constexpr U32 s2 = _firstRowIn;
    constexpr U32 s3 = _firstRowIn + 1;

    std::array<__m128, 2> tout;
    if constexpr (_firstRowOut == 0)
    {
        tout[0] = Shuffle<0, s0, s2, 0>(tmp_0, in_2);
        tout[1] = Shuffle<s1, 3, s3, 0>(tmp_0, in_2);
    }
    else
    {
        tout[0] = Shuffle<0, s2, 0, s0>(in_0, tmp_0);
        tout[1] = Shuffle<0, s3, s1, 3>(in_0, tmp_0);
    }

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x4(__m128 in_0, __m128 in_1, __m128 in_2, __m128 in_3, __m128& out_0, __m128& out_1) noexcept
{
    __m128 tmp_0 = Shuffle<_firstRowIn, _firstRowIn + 1, _firstRowIn, _firstRowIn + 1>(in_0, in_1);
    __m128 tmp1 = Shuffle<_firstRowIn, _firstRowIn + 1, _firstRowIn, _firstRowIn + 1>(in_2, in_3);

    out_0 = Shuffle<0, 2, 0, 2>(tmp_0, tmp1);
    out_1 = Shuffle<1, 3, 1, 3>(tmp_0, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x1(__m128 in_0, __m128& out_0, __m128& out_1, __m128& out_2) noexcept
{
    std::array<__m128, 3> tout;

    Transpose1x1<_firstRowIn, _firstRowOut, true, _unusedSetZero>(in_0, tout[0]);
    Transpose1x1<_firstRowIn + 1, _firstRowOut, true, _unusedSetZero>(in_0, tout[1]);
    Transpose1x1<_firstRowIn + 2, _firstRowOut, true, _unusedSetZero>(in_0, tout[2]);

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, false>(tout, out_0, out_1, out_2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x2(__m128 in_0, __m128 in_1, __m128& out_0, __m128& out_1, __m128& out_2) noexcept
{
    std::array<__m128, 3> tout;
    if constexpr (_firstRowOut == 0)
    {
        if constexpr (_firstRowIn == 0)
        {
            tout[0] = _mm_unpacklo(in_0, in_1);
            tout[1] = _mm_movehl(tout[0], tout[0]);
            tout[2] = _mm_unpackhi(in_0, in_1);
        }
        else
        {
            tout[0] = Insert<1, 0, false, false, true, true>(in_0, in_1);
            tout[1] = _mm_unpackhi(in_0, in_1);
            tout[2] = _mm_movehl(tout[1], tout[1]);
        }
    }
    else if constexpr (_firstRowOut == 1)
    {
        constexpr U32 s0 = _firstRowIn;
        constexpr U32 s1 = _firstRowIn + 1;
        constexpr U32 s2 = _firstRowIn + 2;

        tout[0] = Shuffle<s0, s0, s0, s0>(in_0, in_1);
        tout[1] = Shuffle<s1, s1, s1, s1>(in_0, in_1);
        tout[2] = Shuffle<s2, s2, s2, s2>(in_0, in_1);
    }

    else if constexpr (_firstRowIn == 0)
    {

        tout[1] = _mm_unpacklo(in_0, in_1);
        tout[0] = _mm_movelh(tout[1], tout[1]);
        tout[2] = Insert<2, 3, true, true, false, false>(in_1, in_0);
    }
    else
    {
        tout[0] = _mm_unpacklo(in_0, in_1);
        tout[1] = Insert<2, 3, true, true, false, false>(in_1, in_0);
        tout[2] = _mm_unpackhi(in_0, in_1);
    }

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1, out_2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x3(__m128 in_0, __m128 in_1, __m128 in_2, __m128& out_0, __m128& out_1, __m128& out_2) noexcept
{
    std::array<__m128, 3> tout;

    constexpr U32 idx_0 = (_firstRowIn == 0) ? 0 : 2;
    constexpr U32 idx_1 = 1 - _firstRowIn;
    constexpr U32 idx_2 = 2 - _firstRowIn;

    if constexpr (_firstRowOut == 0)
    {
        __m128 tmp_0 = _mm_unpackhi(in_0, in_1);
        __m128 tmp1 = Shuffle<1, 1, 1, 1>(in_0, in_2);

        if constexpr (_firstRowIn == 0)
        {
            __m128 tmp2 = Shuffle<0, 0, 0, 0>(in_1, in_2);
            tout[idx_0] = BlendIndex<0>(tmp2, in_0);
        }
        else
            tout[idx_0] = Shuffle<2, 3, 3, 2>(tmp_0, in_2);

        tout[idx_1] = BlendIndex<1>(tmp1, in_1);
        tout[idx_2] = BlendIndex<2>(tmp_0, in_2);
    }
    else
    {
        __m128 tmp_0 = _mm_unpacklo(in_1, in_2);
        __m128 tmp1 = Shuffle<2, 2, 2, 2>(in_0, in_2);

        if constexpr (_firstRowIn == 0)
            tout[idx_0] = Shuffle<1, 0, 0, 1>(in_0, tmp_0);
        else
        {
            __m128 tmp2 = Shuffle<3, 3, 3, 3>(in_0, in_1);
            tout[idx_0] = BlendIndex<3>(tmp2, in_2);
        }

        tout[idx_1] = BlendIndex<1>(tmp_0, in_0);
        tout[idx_2] = BlendIndex<2>(tmp1, in_1);
    }

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1, out_2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x4(__m128 in_0, __m128 in_1, __m128 in_2, __m128 in_3, __m128& out_0, __m128& out_1,
                         __m128& out_2) noexcept
{
    __m128 tmp_0 = _mm_unpacklo(in_0, in_1);
    __m128 tmp1 = _mm_unpackhi(in_0, in_1);
    __m128 tmp2 = _mm_unpacklo(in_2, in_3);
    __m128 tmp3 = _mm_unpackhi(in_2, in_3);

    if constexpr (_firstRowIn == 0)
    {
        out_0 = _mm_movelh(tmp_0, tmp2);
        out_1 = _mm_movehl(tmp2, tmp_0);
        out_2 = _mm_movelh(tmp1, tmp3);
    }
    else
    {
        out_0 = _mm_movehl(tmp2, tmp_0);
        out_1 = _mm_movelh(tmp1, tmp3);
        out_2 = _mm_movehl(tmp3, tmp1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x1(__m128 in_0, __m128& out_0, __m128& out_1, __m128& out_2, __m128& out_3) noexcept
{
    std::array<__m128, 4> tout;

    Transpose1x1<_firstRowIn, _firstRowOut, true, _unusedSetZero>(in_0, tout[0]);
    Transpose1x1<_firstRowIn + 1, _firstRowOut, true, _unusedSetZero>(in_0, tout[1]);
    Transpose1x1<_firstRowIn + 2, _firstRowOut, true, _unusedSetZero>(in_0, tout[2]);
    Transpose1x1<_firstRowIn + 3, _firstRowOut, true, _unusedSetZero>(in_0, tout[3]);

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, false>(tout, out_0, out_1, out_2, out_3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x2(__m128 in_0, __m128 in_1, __m128& out_0, __m128& out_1, __m128& out_2, __m128& out_3) noexcept
{
    std::array<__m128, 4> tout;

    if constexpr (_firstRowOut == 0)
    {
        tout[0] = Insert<0, 1, false, false, true, true>(in_1, in_0);
        tout[1] = Insert<1, 0, false, false, true, true>(in_0, in_1);
        tout[2] = _mm_unpackhi(in_0, in_1);
        tout[3] = Permute<2, 3, 0, 1>(tout[2]);
    }
    else if constexpr (_firstRowOut == 1)
    {
        tout[0] = Shuffle<0, 0, 0, 0>(in_0, in_1);
        tout[1] = Insert<1, 2, true, false, false, true>(in_1, in_0);
        tout[2] = Insert<2, 1, true, false, false, true>(in_0, in_1);
        tout[3] = Shuffle<3, 3, 3, 3>(in_0, in_1);
    }
    else
    {
        tout[3] = Insert<3, 2, true, true, false, false>(in_0, in_1);
        tout[2] = Insert<2, 3, true, true, false, false>(in_1, in_0);
        tout[1] = _mm_unpacklo(in_0, in_1);
        tout[0] = Permute<2, 3, 0, 1>(tout[1]);
    }

    if constexpr (_overwriteUnused && _unusedSetZero)
    {
        constexpr U32 idx_0 = (_firstRowOut < 1) ? 2 : 0;
        constexpr U32 idx_1 = (_firstRowOut > 1) ? 1 : 3;

        const __m128 zero = _mm_setzero<__m128>();
        tout[idx_0] = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tout[idx_0]);
        tout[idx_1] = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tout[idx_1]);
    }

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, false>(tout, out_0, out_1, out_2, out_3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x3(__m128 in_0, __m128 in_1, __m128 in_2, __m128& out_0, __m128& out_1, __m128& out_2,
                         __m128& out_3) noexcept
{
    std::array<__m128, 4> tout;

    if constexpr (_firstRowOut == 0)
    {
        __m128 tmp_0 = _mm_unpacklo(in_0, in_1);
        __m128 tmp1 = _mm_unpackhi(in_0, in_1);

        tout[0] = _mm_movelh(tmp_0, in_2);
        tout[1] = Shuffle<2, 3, 1, 0>(tmp_0, in_2);
        tout[2] = BlendIndex<2>(tmp1, in_2);
        tout[3] = Shuffle<2, 3, 3, 2>(tmp1, in_2);
    }
    else
    {
        __m128 tmp_0 = _mm_unpacklo(in_1, in_2);
        __m128 tmp1 = _mm_unpackhi(in_1, in_2);

        tout[0] = Shuffle<1, 0, 0, 1>(in_0, tmp_0);
        tout[1] = BlendIndex<1>(tmp_0, in_0);
        tout[2] = Shuffle<3, 2, 0, 1>(in_0, tmp1);
        tout[3] = _mm_movehl(tmp1, in_0);
    }

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out_0, out_1, out_2, out_3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x4(__m128 in_0, __m128 in_1, __m128 in_2, __m128 in_3, __m128& out_0, __m128& out_1,
                         __m128& out_2, __m128& out_3) noexcept
{
    __m128 tmp_0 = _mm_unpacklo(in_0, in_1);
    __m128 tmp1 = _mm_unpackhi(in_0, in_1);
    __m128 tmp2 = _mm_unpacklo(in_2, in_3);
    __m128 tmp3 = _mm_unpackhi(in_2, in_3);

    out_0 = _mm_movelh(tmp_0, tmp2);
    out_1 = _mm_movehl(tmp2, tmp_0);
    out_2 = _mm_movelh(tmp1, tmp3);
    out_3 = _mm_movehl(tmp3, tmp1);
}

} // namespace GDL::simd
