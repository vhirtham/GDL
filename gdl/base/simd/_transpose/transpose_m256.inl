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

template <U32 _firstRowIn, UST _arraySizeIn>
inline std::array<__m256, 2> IntraLaneTransposeAfterPermute2xN(std::array<__m256, _arraySizeIn> in)
{
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 2> out;
    if constexpr (_arraySizeIn == 4)
    {
        constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
        Transpose2x4<laneOffsetIn, 0>(in[0], in[1], in[2], in[3], out[0], out[1]);
    }
    else
    {
        Transpose1x4<3, 0>(in[0], in[1], in[2], in[3], out[0]);
        Transpose1x4(in[4], in[5], in[6], in[7], out[1]);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, UST _arraySizeIn>
inline std::array<__m256, 3> IntraLaneTransposeAfterPermute3xN(std::array<__m256, _arraySizeIn> in)
{
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;

    std::array<__m256, 3> out;
    if constexpr (_arraySizeIn == 4)
    {
        Transpose3x4<laneOffsetIn, 0>(in[0], in[1], in[2], in[3], out[0], out[1], out[2]);
    }
    else if constexpr (laneOffsetIn == 2)
    {
        Transpose2x4<2, 0>(in[0], in[1], in[2], in[3], out[0], out[1]);
        Transpose1x4(in[4], in[5], in[6], in[7], out[2]);
    }
    else
    {
        Transpose1x4<3, 0>(in[0], in[1], in[2], in[3], out[0]);
        Transpose2x4(in[4], in[5], in[6], in[7], out[1], out[2]);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, UST _arraySizeIn>
inline std::array<__m256, 4> IntraLaneTransposeAfterPermute4xN(std::array<__m256, _arraySizeIn> in)
{
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;

    std::array<__m256, 4> out;
    if constexpr (_arraySizeIn == 4)
    {
        Transpose4x4(in[0], in[1], in[2], in[3], out[0], out[1], out[2], out[3]);
    }
    else if constexpr (laneOffsetIn == 1)
    {
        Transpose3x4<_firstRowIn>(in[0], in[1], in[2], in[3], out[0], out[1], out[2]);
        Transpose1x4(in[4], in[5], in[6], in[7], out[3]);
    }
    else if constexpr (laneOffsetIn == 2)
    {
        Transpose2x4<_firstRowIn>(in[0], in[1], in[2], in[3], out[0], out[1]);
        Transpose2x4(in[4], in[5], in[6], in[7], out[2], out[3]);
    }
    else
    {
        Transpose1x4<_firstRowIn>(in[0], in[1], in[2], in[3], out[0]);
        Transpose3x4(in[4], in[5], in[6], in[7], out[1], out[2], out[3]);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn>
inline std::array<__m256, 5> IntraLaneTransposeAfterPermute5xN(std::array<__m256, 8> in)
{
    std::array<__m256, 5> out;
    if constexpr (_firstRowIn == 0)
    {
        Transpose4x4<0, 0>(in[0], in[1], in[2], in[3], out[0], out[1], out[2], out[3]);
        Transpose1x4<0, 0>(in[4], in[5], in[6], in[7], out[4]);
    }
    else if constexpr (_firstRowIn == 1)
    {
        Transpose3x4<1, 0>(in[0], in[1], in[2], in[3], out[0], out[1], out[2]);
        Transpose2x4<0, 0>(in[4], in[5], in[6], in[7], out[3], out[4]);
    }
    else if constexpr (_firstRowIn == 2)
    {
        Transpose2x4<2, 0>(in[0], in[1], in[2], in[3], out[0], out[1]);
        Transpose3x4<0, 0>(in[4], in[5], in[6], in[7], out[2], out[3], out[4]);
    }
    else
    {
        Transpose1x4<3, 0>(in[0], in[1], in[2], in[3], out[0]);
        Transpose4x4<0, 0>(in[4], in[5], in[6], in[7], out[1], out[2], out[3], out[4]);
    }
    return out;
}

// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn>
inline std::array<__m256, 6> IntraLaneTransposeAfterPermute6xN(std::array<__m256, 8> in)
{
    std::array<__m256, 6> out;
    if constexpr (_firstRowIn == 0)
    {
        Transpose4x4<0, 0>(in[0], in[1], in[2], in[3], out[0], out[1], out[2], out[3]);
        Transpose2x4<0, 0>(in[4], in[5], in[6], in[7], out[4], out[5]);
    }
    else if constexpr (_firstRowIn == 1)
    {
        Transpose3x4<1, 0>(in[0], in[1], in[2], in[3], out[0], out[1], out[2]);
        Transpose3x4<0, 0>(in[4], in[5], in[6], in[7], out[3], out[4], out[5]);
    }
    else
    {
        Transpose2x4<2, 0>(in[0], in[1], in[2], in[3], out[0], out[1]);
        Transpose4x4<0, 0>(in[4], in[5], in[6], in[7], out[2], out[3], out[4], out[5]);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn>
inline std::array<__m256, 7> IntraLaneTransposeAfterPermute7xN(std::array<__m256, 8> in)
{
    std::array<__m256, 7> out;
    if constexpr (_firstRowIn == 0)
    {
        Transpose4x4<0, 0>(in[0], in[1], in[2], in[3], out[0], out[1], out[2], out[3]);
        Transpose3x4<0, 0>(in[4], in[5], in[6], in[7], out[4], out[5], out[6]);
    }
    else
    {
        Transpose3x4<1, 0>(in[0], in[1], in[2], in[3], out[0], out[1], out[2]);
        Transpose4x4<0, 0>(in[4], in[5], in[6], in[7], out[3], out[4], out[5], out[6]);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut>
inline std::array<__m256, 4> IntraLaneTransposeBeforePermuteNx1(__m256 in0)
{
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;

    std::array<__m256, 4> out;
    Transpose4x1<0, laneOffsetOut>(in0, out[0], out[1], out[2], out[3]);

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut>
inline auto IntraLaneTransposeBeforePermuteNx2(__m256 in0, __m256 in1)
{
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;
    constexpr std::size_t _array_size = (laneOffsetOut <= 2) ? 4 : 8;

    std::array<__m256, _array_size> out;
    if constexpr (laneOffsetOut <= 2)
    {
        Transpose4x2<0, laneOffsetOut>(in0, in1, out[0], out[1], out[2], out[3]);
    }
    else
    {
        Transpose4x1<0, 3>(in0, out[0], out[1], out[2], out[3]);
        Transpose4x1(in1, out[4], out[5], out[6], out[7]);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut>
inline auto IntraLaneTransposeBeforePermuteNx3(__m256 in0, __m256 in1, __m256 in2)
{
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;
    constexpr std::size_t _array_size = (laneOffsetOut <= 1) ? 4 : 8;

    std::array<__m256, _array_size> out;
    if constexpr (laneOffsetOut <= 1)
    {
        Transpose4x3<0, laneOffsetOut>(in0, in1, in2, out[0], out[1], out[2], out[3]);
    }
    else if constexpr (laneOffsetOut == 2)
    {
        Transpose4x2<0, 2>(in0, in1, out[0], out[1], out[2], out[3]);
        Transpose4x1(in2, out[4], out[5], out[6], out[7]);
    }
    else
    {
        Transpose4x1<0, 3>(in0, out[0], out[1], out[2], out[3]);
        Transpose4x2(in1, in2, out[4], out[5], out[6], out[7]);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut>
inline std::array<__m256, 4> IntraLaneTransposeBeforePermuteNx4(__m256 in0, __m256 in1, __m256 in2, __m256 in3)
{
    constexpr U32 idx_in_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_in_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_in_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_in_3 = (3 + _firstRowOut) % 4;
    constexpr U32 idx_tmp_0 = (0 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_1 = (1 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_2 = (2 - _firstRowIn) % 4;
    constexpr U32 idx_tmp_3 = (3 - _firstRowIn) % 4;

    std::array<__m256, 4> tin;
    tin[idx_in_0] = in0;
    tin[idx_in_1] = in1;
    tin[idx_in_2] = in2;
    tin[idx_in_3] = in3;

    std::array<__m256, 4> tmp;
    Transpose4x4(tin[0], tin[1], tin[2], tin[3], tmp[idx_tmp_0], tmp[idx_tmp_1], tmp[idx_tmp_2], tmp[idx_tmp_3]);
    return tmp;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut>
inline std::array<__m256, 8> IntraLaneTransposeBeforePermuteNx5(__m256 in0, __m256 in1, __m256 in2, __m256 in3,
                                                                __m256 in4)
{
    std::array<__m256, 8> out;
    if constexpr (_firstRowOut == 0)
    {
        Transpose4x4(in0, in1, in2, in3, out[0], out[1], out[2], out[3]);
        Transpose4x1(in4, out[4], out[5], out[6], out[7]);
    }
    else if constexpr (_firstRowOut == 1)
    {
        Transpose4x3<0, 1>(in0, in1, in2, out[0], out[1], out[2], out[3]);
        Transpose4x2(in3, in4, out[4], out[5], out[6], out[7]);
    }
    else if constexpr (_firstRowOut == 2)
    {
        Transpose4x2<0, 2>(in0, in1, out[0], out[1], out[2], out[3]);
        Transpose4x3(in2, in3, in4, out[4], out[5], out[6], out[7]);
    }
    else
    {
        Transpose4x1<0, 3>(in0, out[0], out[1], out[2], out[3]);
        Transpose4x4(in1, in2, in3, in4, out[4], out[5], out[6], out[7]);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut>
inline std::array<__m256, 8> IntraLaneTransposeBeforePermuteNx6(__m256 in0, __m256 in1, __m256 in2, __m256 in3,
                                                                __m256 in4, __m256 in5)
{
    std::array<__m256, 8> out;
    if constexpr (_firstRowOut == 0)
    {
        Transpose4x4(in0, in1, in2, in3, out[0], out[1], out[2], out[3]);
        Transpose4x2(in4, in5, out[4], out[5], out[6], out[7]);
    }
    else if constexpr (_firstRowOut == 1)
    {
        Transpose4x3<0, 1>(in0, in1, in2, out[0], out[1], out[2], out[3]);
        Transpose4x3(in3, in4, in5, out[4], out[5], out[6], out[7]);
    }
    else
    {
        Transpose4x2<0, 2>(in0, in1, out[0], out[1], out[2], out[3]);
        Transpose4x4(in2, in3, in4, in5, out[4], out[5], out[6], out[7]);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut>
inline std::array<__m256, 8> IntraLaneTransposeBeforePermuteNx7(__m256 in0, __m256 in1, __m256 in2, __m256 in3,
                                                                __m256 in4, __m256 in5, __m256 in6)
{
    std::array<__m256, 8> out;
    if constexpr (_firstRowOut == 0)
    {
        Transpose4x4(in0, in1, in2, in3, out[0], out[1], out[2], out[3]);
        Transpose4x3(in4, in5, in6, out[4], out[5], out[6], out[7]);
    }
    else
    {
        Transpose4x3<0, 1>(in0, in1, in2, out[0], out[1], out[2], out[3]);
        Transpose4x4(in3, in4, in5, in6, out[4], out[5], out[6], out[7]);
    }


    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut>
inline std::array<__m256, 8> IntraLaneTransposeBeforePermuteNx8(__m256 in0, __m256 in1, __m256 in2, __m256 in3,
                                                                __m256 in4, __m256 in5, __m256 in6, __m256 in7)
{
    std::array<__m256, 8> out;

    Transpose4x4(in0, in1, in2, in3, out[0], out[1], out[2], out[3]);
    Transpose4x4(in4, in5, in6, in7, out[4], out[5], out[6], out[7]);

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, U32 _rows>
inline std::array<__m256, _rows> PermuteAfterIntraLaneTransposeNx4(std::array<__m256, 4> in)
{
    std::array<__m256, _rows> out;
    if constexpr (_firstRowOut == 0)
    {
        out[0] = SwapLanesIf<(_firstRowIn > 3)>(in[0]);
        out[1] = SwapLanesIf<(_firstRowIn > 2)>(in[1]);
        out[2] = SwapLanesIf<(_firstRowIn > 1)>(in[2]);
        out[3] = SwapLanesIf<(_firstRowIn > 0)>(in[3]);
        if constexpr (_rows > 4)
            out[4] = Permute2F128<1, 0>(in[0]);
        if constexpr (_rows > 5)
            out[5] = Permute2F128<1, 0>(in[1]);
        if constexpr (_rows > 6)
            out[6] = Permute2F128<1, 0>(in[2]);
        if constexpr (_rows > 7)
            out[7] = Permute2F128<1, 0>(in[3]);
    }
    else if constexpr (_firstRowOut == 4)
    {
        out[0] = SwapLanesIf<_firstRowIn <= 3>(in[0]);
        out[1] = SwapLanesIf<_firstRowIn <= 2>(in[1]);
        out[2] = SwapLanesIf<_firstRowIn <= 1>(in[2]);
        out[3] = SwapLanesIf<_firstRowIn <= 0>(in[3]);
        if constexpr (_rows > 4)
            out[4] = in[0];
        if constexpr (_rows > 5)
            out[5] = in[1];
        if constexpr (_rows > 6)
            out[6] = in[2];
        if constexpr (_rows > 7)
            out[7] = in[3];
    }
    else
    {
        constexpr U32 sel_out0 = (_firstRowIn < 4) ? 0 : 1;
        constexpr U32 sel_out1 = (_firstRowIn < 3) ? 0 : 1;
        constexpr U32 sel_out2 = (_firstRowIn < 2) ? 0 : 1;
        constexpr U32 sel_out3 = (_firstRowIn < 1) ? 0 : 1;

        out[0] = Permute2F128<sel_out0, sel_out0>(in[0]);
        out[1] = Permute2F128<sel_out1, sel_out1>(in[1]);
        out[2] = Permute2F128<sel_out2, sel_out2>(in[2]);
        out[3] = Permute2F128<sel_out3, sel_out3>(in[3]);
        if constexpr (_rows > 4)
            out[4] = Permute2F128<1, 1>(in[0]);
        if constexpr (_rows > 5)
            out[5] = Permute2F128<1, 1>(in[1]);
        if constexpr (_rows > 6)
            out[6] = Permute2F128<1, 1>(in[2]);
        if constexpr (_rows > 7)
            out[7] = Permute2F128<1, 1>(in[3]);
    }
    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeIn>
inline std::array<__m256, 5> PermuteAfterIntraLaneTranspose5xN(std::array<__m256, _arraySizeIn> in)
{
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 5> out;
    if constexpr (_arraySizeIn == 4)
    {
        constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;

        constexpr U32 idx_in0 = (0 + _firstRowIn);
        constexpr U32 idx_in1 = (1 + _firstRowIn) % numValuesPerLane<__m256>;
        constexpr U32 idx_in2 = (2 + _firstRowIn) % numValuesPerLane<__m256>;
        constexpr U32 idx_in3 = (3 + _firstRowIn) % numValuesPerLane<__m256>;

        constexpr U32 cmp_1 = (_firstRowIn > 2) ? 0 : 1;
        constexpr U32 cmp_2 = (_firstRowIn > 1) ? 0 : 1;
        constexpr U32 cmp_3 = (_firstRowIn > 0) ? 0 : 1;

        out[0] = SwapLanesIf<laneOut == 1>(in[idx_in0]);
        out[1] = SwapLanesIf<laneOut == cmp_1>(in[idx_in1]);
        out[2] = SwapLanesIf<laneOut == cmp_2>(in[idx_in2]);
        out[3] = SwapLanesIf<laneOut == cmp_3>(in[idx_in3]);
        out[4] = SwapLanesIf<laneOut == 0>(in[idx_in0]);
    }
    else
    {
        if constexpr (_firstRowIn == 0)
        {
            out[0] = Permute2F128<0, 0, 1, 0>(in[0], in[4]);
            out[1] = Permute2F128<0, 0, 1, 0>(in[1], in[5]);
            out[2] = Permute2F128<0, 0, 1, 0>(in[2], in[6]);
            out[3] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

            out[4] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
        }
        else if constexpr (_firstRowIn == 1)
        {
            out[0] = Permute2F128<0, 0, 1, 0>(in[1], in[5]);
            out[1] = Permute2F128<0, 0, 1, 0>(in[2], in[6]);
            out[2] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

            out[3] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
            out[4] = Permute2F128<0, 1, 1, 1>(in[1], in[5]);
        }
        else if constexpr (_firstRowIn == 2)
        {
            out[0] = Permute2F128<0, 0, 1, 0>(in[2], in[6]);
            out[1] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

            out[2] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
            out[3] = Permute2F128<0, 1, 1, 1>(in[1], in[5]);
            out[4] = Permute2F128<0, 1, 1, 1>(in[2], in[6]);
        }
        else
        {
            out[0] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

            out[1] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
            out[2] = Permute2F128<0, 1, 1, 1>(in[1], in[5]);
            out[3] = Permute2F128<0, 1, 1, 1>(in[2], in[6]);
            out[4] = Permute2F128<0, 1, 1, 1>(in[3], in[7]);
        }
    }
    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeIn>
inline std::array<__m256, 6> PermuteAfterIntraLaneTranspose6xN(std::array<__m256, _arraySizeIn> in)
{
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 6> out;
    if constexpr (_arraySizeIn == 4)
    {
        constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;

        constexpr U32 idx_in0 = (0 + _firstRowIn);
        constexpr U32 idx_in1 = (1 + _firstRowIn);
        constexpr U32 idx_in2 = (2 + _firstRowIn) % numValuesPerLane<__m256>;
        constexpr U32 idx_in3 = (3 + _firstRowIn) % numValuesPerLane<__m256>;

        constexpr U32 cmp_2 = (_firstRowIn > 1) ? 0 : 1;
        constexpr U32 cmp_3 = (_firstRowIn > 0) ? 0 : 1;

        out[0] = SwapLanesIf<laneOut == 1>(in[idx_in0]);
        out[1] = SwapLanesIf<laneOut == 1>(in[idx_in1]);
        out[2] = SwapLanesIf<laneOut == cmp_2>(in[idx_in2]);
        out[3] = SwapLanesIf<laneOut == cmp_3>(in[idx_in3]);
        out[4] = SwapLanesIf<laneOut == 0>(in[idx_in0]);
        out[5] = SwapLanesIf<laneOut == 0>(in[idx_in1]);
    }
    else
    {
        if constexpr (_firstRowIn == 0)
        {
            out[0] = Permute2F128<0, 0, 1, 0>(in[0], in[4]);
            out[1] = Permute2F128<0, 0, 1, 0>(in[1], in[5]);
            out[2] = Permute2F128<0, 0, 1, 0>(in[2], in[6]);
            out[3] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

            out[4] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
            out[5] = Permute2F128<0, 1, 1, 1>(in[1], in[5]);
        }
        else if constexpr (_firstRowIn == 1)
        {
            out[0] = Permute2F128<0, 0, 1, 0>(in[1], in[5]);
            out[1] = Permute2F128<0, 0, 1, 0>(in[2], in[6]);
            out[2] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

            out[3] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
            out[4] = Permute2F128<0, 1, 1, 1>(in[1], in[5]);
            out[5] = Permute2F128<0, 1, 1, 1>(in[2], in[6]);
        }
        else
        {
            out[0] = Permute2F128<0, 0, 1, 0>(in[2], in[6]);
            out[1] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

            out[2] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
            out[3] = Permute2F128<0, 1, 1, 1>(in[1], in[5]);
            out[4] = Permute2F128<0, 1, 1, 1>(in[2], in[6]);
            out[5] = Permute2F128<0, 1, 1, 1>(in[3], in[7]);
        }
    }
    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeIn>
inline std::array<__m256, 7> PermuteAfterIntraLaneTranspose7xN(std::array<__m256, _arraySizeIn> in)
{
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 7> out;
    if constexpr (_arraySizeIn == 4)
    {
        constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;

        constexpr U32 idx_in0 = (0 + _firstRowIn);
        constexpr U32 idx_in1 = (1 + _firstRowIn);
        constexpr U32 idx_in2 = (2 + _firstRowIn);
        constexpr U32 idx_in3 = (3 + _firstRowIn) % numValuesPerLane<__m256>;

        out[0] = SwapLanesIf<laneOut == 1>(in[idx_in0]);
        out[1] = SwapLanesIf<laneOut == 1>(in[idx_in1]);
        out[2] = SwapLanesIf<laneOut == 1>(in[idx_in2]);
        out[3] = SwapLanesIf<laneOut != _firstRowIn>(in[idx_in3]);
        out[4] = SwapLanesIf<laneOut == 0>(in[idx_in0]);
        out[5] = SwapLanesIf<laneOut == 0>(in[idx_in1]);
        out[6] = SwapLanesIf<laneOut == 0>(in[idx_in2]);
    }
    else
    {
        if constexpr (_firstRowIn == 0)
        {
            out[0] = Permute2F128<0, 0, 1, 0>(in[0], in[4]);
            out[1] = Permute2F128<0, 0, 1, 0>(in[1], in[5]);
            out[2] = Permute2F128<0, 0, 1, 0>(in[2], in[6]);
            out[3] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

            out[4] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
            out[5] = Permute2F128<0, 1, 1, 1>(in[1], in[5]);
            out[6] = Permute2F128<0, 1, 1, 1>(in[2], in[6]);
        }
        else
        {

            out[0] = Permute2F128<0, 0, 1, 0>(in[1], in[5]);
            out[1] = Permute2F128<0, 0, 1, 0>(in[2], in[6]);
            out[2] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

            out[3] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
            out[4] = Permute2F128<0, 1, 1, 1>(in[1], in[5]);
            out[5] = Permute2F128<0, 1, 1, 1>(in[2], in[6]);
            out[6] = Permute2F128<0, 1, 1, 1>(in[3], in[7]);
        }
    }
    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeIn>
inline std::array<__m256, 8> PermuteAfterIntraLaneTranspose8xN(std::array<__m256, _arraySizeIn> in)
{
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 8> out;
    if constexpr (_arraySizeIn == 4)
    {
        constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;

        out[0] = SwapLanesIf<laneOut == 1>(in[0]);
        out[1] = SwapLanesIf<laneOut == 1>(in[1]);
        out[2] = SwapLanesIf<laneOut == 1>(in[2]);
        out[3] = SwapLanesIf<laneOut == 1>(in[3]);
        out[4] = SwapLanesIf<laneOut == 0>(in[0]);
        out[5] = SwapLanesIf<laneOut == 0>(in[1]);
        out[6] = SwapLanesIf<laneOut == 0>(in[2]);
        out[7] = SwapLanesIf<laneOut == 0>(in[3]);
    }
    else
    {
        out[0] = Permute2F128<0, 0, 1, 0>(in[0], in[4]);
        out[1] = Permute2F128<0, 0, 1, 0>(in[1], in[5]);
        out[2] = Permute2F128<0, 0, 1, 0>(in[2], in[6]);
        out[3] = Permute2F128<0, 0, 1, 0>(in[3], in[7]);

        out[4] = Permute2F128<0, 1, 1, 1>(in[0], in[4]);
        out[5] = Permute2F128<0, 1, 1, 1>(in[1], in[5]);
        out[6] = Permute2F128<0, 1, 1, 1>(in[2], in[6]);
        out[7] = Permute2F128<0, 1, 1, 1>(in[3], in[7]);
    }
    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeOut>
inline std::array<__m256, _arraySizeOut> PermuteBeforeIntraLaneTransposeNx5(__m256 in0, __m256 in1, __m256 in2,
                                                                            __m256 in3, __m256 in4)
{
    static_assert(_arraySizeOut == 4 || _arraySizeOut == 8, "Array size must be 4 or 8");

    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;

    constexpr U32 idx_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_3 = (3 + _firstRowOut) % 4;

    [[maybe_unused]] constexpr U32 trgt_lane_0 = (laneIn == 0) ? 1 : 0;
    constexpr U32 comp_lane_1 = (_firstRowOut + 1) / 4;
    constexpr U32 comp_lane_2 = (_firstRowOut + 2) / 4;
    constexpr U32 comp_lane_3 = (_firstRowOut + 3) / 4;


    std::array<__m256, _arraySizeOut> out;
    out[idx_0] = Permute2F128<0, laneIn, 1, laneIn>(in0, in4);
    out[idx_1] = SwapLanesIf<laneIn != comp_lane_1>(in1);
    out[idx_2] = SwapLanesIf<laneIn != comp_lane_2>(in2);
    if constexpr (_firstRowOut == 0 || _firstRowOut == 3)
        out[idx_3] = SwapLanesIf<laneIn != comp_lane_3>(in3);
    else
        out[idx_3] = Permute2F128<0, trgt_lane_0, 1, laneIn>(in1, in3);


    if constexpr (_arraySizeOut == 8)
    {
        out[idx_0 + 4] = Permute2F128<0, trgt_lane_0, 1, trgt_lane_0>(in0, in4);
        out[idx_2 + 4] = SwapLanesIf<laneIn == comp_lane_2>(in2);
        out[idx_3 + 4] = SwapLanesIf<laneIn == comp_lane_3>(in3);
        if constexpr (_firstRowOut == 0 || _firstRowOut == 3)
            out[idx_1 + 4] = SwapLanesIf<laneIn == comp_lane_1>(in1);
        else
            out[idx_1 + 4] = out[idx_3];
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeOut>
inline std::array<__m256, _arraySizeOut> PermuteBeforeIntraLaneTransposeNx6(__m256 in0, __m256 in1, __m256 in2,
                                                                            __m256 in3, __m256 in4, __m256 in5)
{
    static_assert(_arraySizeOut == 4 || _arraySizeOut == 8, "Array size must be 4 or 8");

    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;

    constexpr U32 idx_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_3 = (3 + _firstRowOut) % 4;

    [[maybe_unused]] constexpr U32 notLaneIn = (laneIn == 0) ? 1 : 0;
    constexpr U32 comp_lane_2 = (_firstRowOut + 2) / 4;
    constexpr U32 comp_lane_3 = (_firstRowOut + 3) / 4;


    std::array<__m256, _arraySizeOut> out;
    out[idx_0] = Permute2F128<0, laneIn, 1, laneIn>(in0, in4);
    out[idx_1] = Permute2F128<0, laneIn, 1, laneIn>(in1, in5);
    out[idx_2] = SwapLanesIf<laneIn != comp_lane_2>(in2);
    if constexpr (_firstRowOut == 0 || _firstRowOut == 2)
        out[idx_3] = SwapLanesIf<laneIn != comp_lane_3>(in3);
    else
        out[idx_3] = Permute2F128<0, notLaneIn, 1, laneIn>(in2, in3);


    if constexpr (_arraySizeOut == 8)
    {
        out[idx_0 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in0, in4);
        out[idx_1 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in1, in5);
        out[idx_3 + 4] = SwapLanesIf<laneIn == comp_lane_3>(in3);
        if constexpr (_firstRowOut == 0 || _firstRowOut == 2)
            out[idx_2 + 4] = SwapLanesIf<laneIn == comp_lane_2>(in2);
        else
            out[idx_2 + 4] = out[idx_3];
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeOut>
inline std::array<__m256, _arraySizeOut>
PermuteBeforeIntraLaneTransposeNx7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6)
{
    static_assert(_arraySizeOut == 4 || _arraySizeOut == 8, "Array size must be 4 or 8");

    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;

    constexpr U32 idx_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_3 = (3 + _firstRowOut) % 4;

    constexpr U32 comp_lane_3 = (_firstRowOut + 3) / 4;


    std::array<__m256, _arraySizeOut> out;
    out[idx_0] = Permute2F128<0, laneIn, 1, laneIn>(in0, in4);
    out[idx_1] = Permute2F128<0, laneIn, 1, laneIn>(in1, in5);
    out[idx_2] = Permute2F128<0, laneIn, 1, laneIn>(in2, in6);
    out[idx_3] = SwapLanesIf<laneIn != comp_lane_3>(in3);


    if constexpr (_arraySizeOut == 8)
    {
        constexpr U32 notLaneIn = (laneIn == 0) ? 1 : 0;

        out[idx_0 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in0, in4);
        out[idx_1 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in1, in5);
        out[idx_2 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in2, in6);
        out[idx_3 + 4] = SwapLanesIf<laneIn == comp_lane_3>(in3);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeOut>
inline std::array<__m256, _arraySizeOut> PermuteBeforeIntraLaneTransposeNx8(__m256 in0, __m256 in1, __m256 in2,
                                                                            __m256 in3, __m256 in4, __m256 in5,
                                                                            __m256 in6, __m256 in7)
{
    static_assert(_arraySizeOut == 4 || _arraySizeOut == 8, "Array size must be 4 or 8");

    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;


    std::array<__m256, _arraySizeOut> out;
    out[0] = Permute2F128<0, laneIn, 1, laneIn>(in0, in4);
    out[1] = Permute2F128<0, laneIn, 1, laneIn>(in1, in5);
    out[2] = Permute2F128<0, laneIn, 1, laneIn>(in2, in6);
    out[3] = Permute2F128<0, laneIn, 1, laneIn>(in3, in7);


    if constexpr (_arraySizeOut == 8)
    {
        constexpr U32 notLaneIn = (laneIn == 0) ? 1 : 0;

        out[4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in0, in4);
        out[5] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in1, in5);
        out[6] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in2, in6);
        out[7] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in3, in7);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x1(__m256 in, __m256& out)
{
    __m256 tout;

    if constexpr (_firstRowIn == _firstRowOut)
        tout = in;
    else
    {
        constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;
        constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;

        if constexpr (laneIn == laneOut)
        {
            constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
            tout = Broadcast<laneOffsetIn>(in);
        }
        else
        {
            tout = BroadcastAcrossLanes<_firstRowIn>(in);
        }
    }

    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m256 in0, __m256 in1, __m256& out0)
{
    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;
    constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;

    __m256 tout;

    if constexpr (laneOffsetOut == 1)
    {
        __m256 tmp = Shuffle<laneOffsetIn, laneOffsetIn, laneOffsetIn, laneOffsetIn>(in0, in1);
        tout = SwapLanesIf<laneIn != laneOut>(tmp);
    }
    else if constexpr (laneOffsetOut == 3)
    {
        __m256 tmp = Shuffle<laneOffsetIn, laneOffsetIn, laneOffsetIn, laneOffsetIn>(in1, in0);
        tout = Permute2F128<laneIn, laneIn>(tmp);
    }
    else
    {
        __m256 tmp0;
        if constexpr (laneOffsetIn < 2)
            tmp0 = _mm_unpacklo(in0, in1);
        else
            tmp0 = _mm_unpackhi(in0, in1);

        if constexpr (laneOffsetIn % 2 != laneOffsetOut / 2)
            tmp0 = Permute<2, 3, 0, 1>(tmp0);


        tout = SwapLanesIf<laneIn != laneOut>(tmp0);
    }

    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;

    std::array<__m256, 3> tin = {{in0, in1, in2}};

    constexpr U32 offset = laneOffsetOut % 2;
    constexpr U32 idx_in_0 = offset;
    constexpr U32 idx_in_1 = 1 + offset;
    constexpr U32 idx_in_2 = (2 + offset) % 3;


    __m256 tmp0;
    if constexpr (laneOffsetIn < 2)
        tmp0 = _mm_unpacklo(tin[idx_in_0], tin[idx_in_1]);
    else
        tmp0 = _mm_unpackhi(tin[idx_in_0], tin[idx_in_1]);


    constexpr U32 idx_0 = (laneOffsetIn % 2) * 2;
    constexpr U32 idx_1 = idx_0 + 1;
    constexpr U32 idx_2 = laneOffsetIn;

    __m256 tmp1;
    if constexpr (laneOffsetOut == 0)
        tmp1 = Shuffle<idx_0, idx_1, idx_2, 0>(tmp0, tin[idx_in_2]);
    else if constexpr (laneOffsetOut == 1)
        tmp1 = Shuffle<0, idx_2, idx_0, idx_1>(tin[idx_in_2], tmp0);
    else if constexpr (laneOffsetOut == 2)
        tmp1 = Shuffle<idx_2, 0, idx_0, idx_1>(tin[idx_in_2], tmp0);
    else
        tmp1 = Shuffle<idx_0, idx_1, 0, idx_2>(tmp0, tin[idx_in_2]);


    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;
    constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;
    __m256 tout;

    if constexpr (laneOffsetOut < 2)
        tout = SwapLanesIf<laneIn != laneOut>(tmp1);
    else
        tout = Permute2F128<laneIn, laneIn>(tmp1);


    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0)
{
    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;
    constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;

    std::array<__m256, 4> tin = {{in0, in1, in2, in3}};

    constexpr U32 idx_in_0 = (4 - laneOffsetOut) % 4;
    constexpr U32 idx_in_1 = (5 - laneOffsetOut) % 4;
    constexpr U32 idx_in_2 = (6 - laneOffsetOut) % 4;
    constexpr U32 idx_in_3 = (7 - laneOffsetOut) % 4;

    __m256 tmp0, tmp1;
    if constexpr (laneOffsetIn < 2)
    {
        tmp0 = _mm_unpacklo(tin[idx_in_0], tin[idx_in_1]);
        tmp1 = _mm_unpacklo(tin[idx_in_2], tin[idx_in_3]);
    }
    else
    {
        tmp0 = _mm_unpackhi(tin[idx_in_0], tin[idx_in_1]);
        tmp1 = _mm_unpackhi(tin[idx_in_2], tin[idx_in_3]);
    }

    __m256 tout;
    if constexpr (laneOffsetIn % 2 == 0)
        tout = _mm_movelh(tmp0, tmp1);
    else
        tout = _mm_movehl(tmp1, tmp0);


    if constexpr (laneIn != laneOut || laneOffsetOut > 0)
        tout = Permute2F128<laneIn, laneIn>(tout);


    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;


    __m256 tmp0, tmp1;
    if constexpr (laneOffsetOut == 0)
    {
        Transpose1x4<laneOffsetIn, 0>(in0, in1, in2, in3, tmp0);
        Transpose1x1<laneOffsetIn, 0>(in4, tmp1);
    }
    else if constexpr (laneOffsetOut == 1)
    {
        Transpose1x3<laneOffsetIn, 1>(in0, in1, in2, tmp0);
        Transpose1x2<laneOffsetIn, 0>(in3, in4, tmp1);
    }
    else if constexpr (laneOffsetOut == 2)
    {
        Transpose1x2<laneOffsetIn, 2>(in0, in1, tmp0);
        Transpose1x3<laneOffsetIn, 0>(in2, in3, in4, tmp1);
    }
    else
    {
        Transpose1x1<laneOffsetIn, 3>(in0, tmp0);
        Transpose1x4<laneOffsetIn, 0>(in1, in2, in3, in4, tmp1);
    }

    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;
    __m256 tout = Permute2F128<0, laneIn, 1, laneIn>(tmp0, tmp1);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;


    __m256 tmp0, tmp1;
    if constexpr (laneOffsetOut == 0)
    {
        Transpose1x4<laneOffsetIn, 0>(in0, in1, in2, in3, tmp0);
        Transpose1x2<laneOffsetIn, 0>(in4, in5, tmp1);
    }
    else if constexpr (laneOffsetOut == 1)
    {
        Transpose1x3<laneOffsetIn, 1>(in0, in1, in2, tmp0);
        Transpose1x3<laneOffsetIn, 0>(in3, in4, in5, tmp1);
    }
    else
    {
        Transpose1x2<laneOffsetIn, 2>(in0, in1, tmp0);
        Transpose1x4<laneOffsetIn, 0>(in2, in3, in4, in5, tmp1);
    }

    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;
    __m256 tout = Permute2F128<0, laneIn, 1, laneIn>(tmp0, tmp1);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;


    __m256 tmp0, tmp1;
    if constexpr (laneOffsetOut == 0)
    {
        Transpose1x4<laneOffsetIn, 0>(in0, in1, in2, in3, tmp0);
        Transpose1x3<laneOffsetIn, 0>(in4, in5, in6, tmp1);
    }
    else
    {
        Transpose1x3<laneOffsetIn, 1>(in0, in1, in2, tmp0);
        Transpose1x4<laneOffsetIn, 0>(in3, in4, in5, in6, tmp1);
    }

    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;
    __m256 tout = Permute2F128<0, laneIn, 1, laneIn>(tmp0, tmp1);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;

    __m256 tmp0, tmp1;

    Transpose1x4<laneOffsetIn, 0>(in0, in1, in2, in3, tmp0);
    Transpose1x4<laneOffsetIn, 0>(in4, in5, in6, in7, tmp1);

    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;
    __m256 tout = Permute2F128<0, laneIn, 1, laneIn>(tmp0, tmp1);

    out0 = tout;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m256 in0, __m256& out0, __m256& out1)
{
    constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;

    std::array<__m256, 2> tout;

    if constexpr (laneOffsetIn < 3)
    {
        constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;

        __m256 tmp0 = SwapLanesIf<laneIn != laneOut>(in0);
        Transpose1x1<laneOffsetIn, laneOffsetOut>(tmp0, tout[0]);
        Transpose1x1<laneOffsetIn + 1, laneOffsetOut>(tmp0, tout[1]);
    }
    else
    {
        __m256 tmp0 = SwapLanesIf<laneOut == 1>(in0);
        __m256 tmp1 = SwapLanesIf<laneOut == 0>(in0);

        Transpose1x1<laneOffsetIn, laneOffsetOut>(tmp0, tout[0]);
        Transpose1x1<0, laneOffsetOut>(tmp1, tout[1]);
    }

    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, tout[0], tout[1]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1)
{
    constexpr U32 laneIn = _firstRowIn / numValuesPerLane<__m256>;
    constexpr U32 laneOut = _firstRowOut / numValuesPerLane<__m256>;
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr U32 laneOffsetOut = _firstRowOut % numValuesPerLane<__m256>;

    std::array<__m256, 2> tout;

    if constexpr (laneOffsetOut % 2 == 0)
    {
        constexpr U32 idx_out_0 = (laneOffsetOut == 0) ? 0 : 1;
        constexpr U32 idx_out_1 = (laneOffsetOut == 0) ? 1 : 0;

        if constexpr (laneOffsetIn % 2 == 0)
        {
            __m256 tmp0;
            if constexpr (laneOffsetIn == 0)
                tmp0 = _mm_unpacklo(in0, in1);
            else
                tmp0 = _mm_unpackhi(in0, in1);

            tout[idx_out_0] = SwapLanesIf<laneIn != laneOut>(tmp0);
            tout[idx_out_1] = Permute<2, 3, 0, 1>(tout[idx_out_0]);
        }
        else if constexpr (laneOffsetIn == 1)
        {
            __m256 tmp0 = Shuffle<1, 2, 1, 2>(in0, in1);
            __m256 tmp1 = SwapLanesIf<laneIn != laneOut>(tmp0);
            tout[idx_out_0] = Permute<0, 2, 1, 3>(tmp1);
            tout[idx_out_1] = Permute<1, 3, 0, 2>(tmp1);
        }
        else
        {
            __m256 tmp0, tmp1;
            if constexpr (laneOffsetOut == 0)
            {
                tmp0 = _mm_unpackhi(in0, in1);
                tmp1 = _mm_unpacklo(in0, in1);
            }
            else
            {
                tmp0 = _mm_unpacklo(in0, in1);
                tmp1 = _mm_unpackhi(in0, in1);
            }

            __m256 tmp2 = Permute<2, 3, 0, 1>(tmp0);
            tout[idx_out_0] = SwapLanesIf<laneOut != idx_out_0>(tmp2);
            tout[idx_out_1] = SwapLanesIf<laneOut != idx_out_1>(tmp1);
        }
    }
    else if constexpr (laneOffsetOut == 1)
    {
        if constexpr (laneOffsetIn < 3)
        {
            if constexpr (laneIn == laneOut)
            {
                tout[0] = Shuffle<laneOffsetIn, laneOffsetIn, laneOffsetIn, laneOffsetIn>(in0, in1);
                tout[1] = Shuffle<laneOffsetIn + 1, laneOffsetIn + 1, laneOffsetIn + 1, laneOffsetIn + 1>(in0, in1);
            }
            else
            {
                __m256 tmp2 = Shuffle<laneOffsetIn + 1, laneOffsetIn, laneOffsetIn, laneOffsetIn + 1>(in0, in1);
                tout[0] = SwapLanes(tmp2);
                tout[1] = Permute<0, 0, 3, 3>(tout[0]);
            }
        }
        else
        {
            __m256 tmp2 = Shuffle<0, 3, 3, 0>(in0, in1);
            __m256 tmp3 = Shuffle<3, 0, 0, 3>(in0, in1);
            tout[0] = SwapLanesIf<laneOut == 1>(tmp2);
            tout[1] = SwapLanesIf<laneOut == 0>(tmp3);
        }
    }
    else
    {
        if constexpr (laneOffsetIn < 3)
        {
            __m256 tmp0 = SwapLanesIf<laneIn != laneOut>(in0);
            __m256 tmp1 = SwapLanesIf<laneIn == laneOut>(in1);
            tout[0] = Shuffle<laneOffsetIn, laneOffsetIn, laneOffsetIn, laneOffsetIn>(tmp1, tmp0);
            tout[1] = Shuffle<laneOffsetIn + 1, laneOffsetIn + 1, laneOffsetIn + 1, laneOffsetIn + 1>(tmp1, tmp0);
        }
        else
        {
            __m256 tmp0 = Permute2F128<0, 1, 1, 0>(in0, in1);
            tout[0] = Shuffle<3, 3, 3, 3>(tmp0, in0);
            tout[1] = Shuffle<0, 0, 0, 0>(in1, tmp0);
        }
    }

    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, tout[0], tout[1]);
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
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 3) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx5<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4);

    std::array<__m256, 2> tout = IntraLaneTransposeAfterPermute2xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, tout[0], tout[1]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 3) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 2> tout = IntraLaneTransposeAfterPermute2xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, tout[0], tout[1]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 3) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 2> tout = IntraLaneTransposeAfterPermute2xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, tout[0], tout[1]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 3) ? 4 : 8;

    std::array<__m256, arraySize> tmp = PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, arraySize>(
            in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 2> tout = IntraLaneTransposeAfterPermute2xN<_firstRowIn>(tmp);


    out0 = tout[0];
    out1 = tout[1];
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
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 2) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx5<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4);

    std::array<__m256, 3> tout = IntraLaneTransposeAfterPermute3xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tout[0], tout[1], tout[2]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 2) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 3> tout = IntraLaneTransposeAfterPermute3xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tout[0], tout[1], tout[2]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 2) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 3> tout = IntraLaneTransposeAfterPermute3xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, out2, tout[0], tout[1], tout[2]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 2) ? 4 : 8;

    std::array<__m256, arraySize> tmp = PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, arraySize>(
            in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 3> tout = IntraLaneTransposeAfterPermute3xN<_firstRowIn>(tmp);


    out0 = tout[0];
    out1 = tout[1];
    out2 = tout[2];
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
    constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
    constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

    std::array<__m256, 4> tmp;
    if constexpr (laneOffsetIn == 0 && laneOffsetOut == 0)
    {
        __m256 tmp8 = _mm_unpacklo(in0, in1);
        __m256 tmp9 = _mm_unpackhi(in0, in1);
        __m256 tmp10 = _mm_unpacklo(in2, in3);
        __m256 tmp11 = _mm_unpackhi(in2, in3);

        tmp[0] = _mm_movelh(tmp8, tmp10);
        tmp[1] = _mm_movehl(tmp10, tmp8);
        tmp[2] = _mm_movelh(tmp9, tmp11);
        tmp[3] = _mm_movehl(tmp11, tmp9);
    }
    else
        tmp = IntraLaneTransposeBeforePermuteNx4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3);

    std::array<__m256, 4> tout = PermuteAfterIntraLaneTransposeNx4<_firstRowIn, _firstRowOut, 4>(tmp);

    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tout[0], tout[1],
                                                                          tout[2], tout[3]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 1) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx5<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4);

    std::array<__m256, 4> tout = IntraLaneTransposeAfterPermute4xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tout[0], tout[1],
                                                                          tout[2], tout[3]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 1) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 4> tout = IntraLaneTransposeAfterPermute4xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tout[0], tout[1],
                                                                          tout[2], tout[3]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 1) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 4> tout = IntraLaneTransposeAfterPermute4xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, tout[0], tout[1],
                                                                          tout[2], tout[3]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    constexpr U32 laneOffsetIn = _firstRowIn % numValuesPerLane<__m256>;
    constexpr UST arraySize = (laneOffsetIn < 1) ? 4 : 8;

    std::array<__m256, arraySize> tmp = PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, arraySize>(
            in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 4> tout = IntraLaneTransposeAfterPermute4xN<_firstRowIn>(tmp);

    out0 = tout[0];
    out1 = tout[1];
    out2 = tout[2];
    out3 = tout[3];
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx1<_firstRowOut>(in0);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTranspose5xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx2<_firstRowOut>(in0, in1);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTranspose5xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx3<_firstRowOut>(in0, in1, in2);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTranspose5xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTransposeNx4<_firstRowIn, _firstRowOut, 5>(tmp);

    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx5<_firstRowOut>(in0, in1, in2, in3, in4);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTranspose5xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 5> tout = IntraLaneTransposeAfterPermute5xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 5> tout = IntraLaneTransposeAfterPermute5xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 5> tout = IntraLaneTransposeAfterPermute5xN<_firstRowIn>(tmp);

    out0 = tout[0];
    out1 = tout[1];
    out2 = tout[2];
    out3 = tout[3];
    out4 = tout[4];
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx1<_firstRowOut>(in0);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTranspose6xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4], tout[5]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4,
                         __m256& out5)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx2<_firstRowOut>(in0, in1);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTranspose6xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4], tout[5]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4, __m256& out5)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx3<_firstRowOut>(in0, in1, in2);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTranspose6xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4], tout[5]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTransposeNx4<_firstRowIn, _firstRowOut, 6>(tmp);

    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4], tout[5]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx5<_firstRowOut>(in0, in1, in2, in3, in4);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTranspose6xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4], tout[5]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 6> tout = IntraLaneTransposeAfterPermute6xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4], tout[5]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 6> tout = IntraLaneTransposeAfterPermute6xN<_firstRowIn>(tmp);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, tout[0],
                                                                          tout[1], tout[2], tout[3], tout[4], tout[5]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 6> tout = IntraLaneTransposeAfterPermute6xN<_firstRowIn>(tmp);

    out0 = tout[0];
    out1 = tout[1];
    out2 = tout[2];
    out3 = tout[3];
    out4 = tout[4];
    out5 = tout[5];
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx1<_firstRowOut>(in0);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout[0], tout[1], tout[2], tout[3], tout[4], tout[5], tout[6]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4,
                         __m256& out5, __m256& out6)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx2<_firstRowOut>(in0, in1);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout[0], tout[1], tout[2], tout[3], tout[4], tout[5], tout[6]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4, __m256& out5, __m256& out6)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx3<_firstRowOut>(in0, in1, in2);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout[0], tout[1], tout[2], tout[3], tout[4], tout[5], tout[6]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTransposeNx4<_firstRowIn, _firstRowOut, 7>(tmp);

    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout[0], tout[1], tout[2], tout[3], tout[4], tout[5], tout[6]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx5<_firstRowOut>(in0, in1, in2, in3, in4);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout[0], tout[1], tout[2], tout[3], tout[4], tout[5], tout[6]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx6<_firstRowOut>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout[0], tout[1], tout[2], tout[3], tout[4], tout[5], tout[6]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx7<_firstRowOut>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(
            out0, out1, out2, out3, out4, out5, out6, tout[0], tout[1], tout[2], tout[3], tout[4], tout[5], tout[6]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 7> tout = IntraLaneTransposeAfterPermute7xN<_firstRowIn>(tmp);

    out0 = tout[0];
    out1 = tout[1];
    out2 = tout[2];
    out3 = tout[3];
    out4 = tout[4];
    out5 = tout[5];
    out6 = tout[6];
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6, __m256& out7)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx1<_firstRowOut>(in0);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          out7, tout[0], tout[1], tout[2], tout[3],
                                                                          tout[4], tout[5], tout[6], tout[7]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4,
                         __m256& out5, __m256& out6, __m256& out7)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx2<_firstRowOut>(in0, in1);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          out7, tout[0], tout[1], tout[2], tout[3],
                                                                          tout[4], tout[5], tout[6], tout[7]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4, __m256& out5, __m256& out6, __m256& out7)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx3<_firstRowOut>(in0, in1, in2);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          out7, tout[0], tout[1], tout[2], tout[3],
                                                                          tout[4], tout[5], tout[6], tout[7]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5, __m256& out6, __m256& out7)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTransposeNx4<_firstRowIn, _firstRowOut, 8>(tmp);

    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          out7, tout[0], tout[1], tout[2], tout[3],
                                                                          tout[4], tout[5], tout[6], tout[7]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6, __m256& out7)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx5<_firstRowOut>(in0, in1, in2, in3, in4);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          out7, tout[0], tout[1], tout[2], tout[3],
                                                                          tout[4], tout[5], tout[6], tout[7]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6,
                         __m256& out7)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx6<_firstRowOut>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          out7, tout[0], tout[1], tout[2], tout[3],
                                                                          tout[4], tout[5], tout[6], tout[7]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6, __m256& out7)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx7<_firstRowOut>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, out1, out2, out3, out4, out5, out6,
                                                                          out7, tout[0], tout[1], tout[2], tout[3],
                                                                          tout[4], tout[5], tout[6], tout[7]);
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
