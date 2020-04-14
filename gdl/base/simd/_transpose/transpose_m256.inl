#pragma once

#include "gdl/base/simd/_transpose/transpose_m256.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/_transpose/transpose_utility.h"



#ifdef __AVX2__

namespace GDL::simd
{



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut>
inline std::array<__m256, 4> IntraLaneTransposeBeforePermuteNx1(__m256 in0)
{
    using Lane = TranspositionLaneData<__m256, 0, _firstRowOut>;

    std::array<__m256, 4> out;
    Transpose4x1<0, Lane::OffsetOut>(in0, out[0], out[1], out[2], out[3]);

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut>
inline auto IntraLaneTransposeBeforePermuteNx2(__m256 in0, __m256 in1)
{
    using Lane = TranspositionLaneData<__m256, 0, _firstRowOut>;

    constexpr std::size_t _array_size = (Lane::OffsetOut <= 2) ? 4 : 8;

    std::array<__m256, _array_size> out;
    if constexpr (Lane::OffsetOut <= 2)
    {
        Transpose4x2<0, Lane::OffsetOut>(in0, in1, out[0], out[1], out[2], out[3]);
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
    using Lane = TranspositionLaneData<__m256, 0, _firstRowOut>;

    constexpr std::size_t _array_size = (Lane::OffsetOut <= 1) ? 4 : 8;

    std::array<__m256, _array_size> out;
    if constexpr (Lane::OffsetOut <= 1)
    {
        Transpose4x3<0, Lane::OffsetOut>(in0, in1, in2, out[0], out[1], out[2], out[3]);
    }
    else if constexpr (Lane::OffsetOut == 2)
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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 5> out;
    if constexpr (_arraySizeIn == 4)
    {
        constexpr U32 idx_in0 = (0 + _firstRowIn);
        constexpr U32 idx_in1 = (1 + _firstRowIn) % numValuesPerLane<__m256>;
        constexpr U32 idx_in2 = (2 + _firstRowIn) % numValuesPerLane<__m256>;
        constexpr U32 idx_in3 = (3 + _firstRowIn) % numValuesPerLane<__m256>;

        constexpr U32 cmp_1 = (_firstRowIn > 2) ? 0 : 1;
        constexpr U32 cmp_2 = (_firstRowIn > 1) ? 0 : 1;
        constexpr U32 cmp_3 = (_firstRowIn > 0) ? 0 : 1;

        out[0] = SwapLanesIf<Lane::Out == 1>(in[idx_in0]);
        out[1] = SwapLanesIf<Lane::Out == cmp_1>(in[idx_in1]);
        out[2] = SwapLanesIf<Lane::Out == cmp_2>(in[idx_in2]);
        out[3] = SwapLanesIf<Lane::Out == cmp_3>(in[idx_in3]);
        out[4] = SwapLanesIf<Lane::Out == 0>(in[idx_in0]);
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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 6> out;
    if constexpr (_arraySizeIn == 4)
    {
        constexpr U32 idx_in0 = (0 + _firstRowIn);
        constexpr U32 idx_in1 = (1 + _firstRowIn);
        constexpr U32 idx_in2 = (2 + _firstRowIn) % numValuesPerLane<__m256>;
        constexpr U32 idx_in3 = (3 + _firstRowIn) % numValuesPerLane<__m256>;

        constexpr U32 cmp_2 = (_firstRowIn > 1) ? 0 : 1;
        constexpr U32 cmp_3 = (_firstRowIn > 0) ? 0 : 1;

        out[0] = SwapLanesIf<Lane::Out == 1>(in[idx_in0]);
        out[1] = SwapLanesIf<Lane::Out == 1>(in[idx_in1]);
        out[2] = SwapLanesIf<Lane::Out == cmp_2>(in[idx_in2]);
        out[3] = SwapLanesIf<Lane::Out == cmp_3>(in[idx_in3]);
        out[4] = SwapLanesIf<Lane::Out == 0>(in[idx_in0]);
        out[5] = SwapLanesIf<Lane::Out == 0>(in[idx_in1]);
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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 7> out;
    if constexpr (_arraySizeIn == 4)
    {
        constexpr U32 idx_in0 = (0 + _firstRowIn);
        constexpr U32 idx_in1 = (1 + _firstRowIn);
        constexpr U32 idx_in2 = (2 + _firstRowIn);
        constexpr U32 idx_in3 = (3 + _firstRowIn) % numValuesPerLane<__m256>;

        out[0] = SwapLanesIf<Lane::Out == 1>(in[idx_in0]);
        out[1] = SwapLanesIf<Lane::Out == 1>(in[idx_in1]);
        out[2] = SwapLanesIf<Lane::Out == 1>(in[idx_in2]);
        out[3] = SwapLanesIf<Lane::Out != _firstRowIn>(in[idx_in3]);
        out[4] = SwapLanesIf<Lane::Out == 0>(in[idx_in0]);
        out[5] = SwapLanesIf<Lane::Out == 0>(in[idx_in1]);
        out[6] = SwapLanesIf<Lane::Out == 0>(in[idx_in2]);
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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 8> out;
    if constexpr (_arraySizeIn == 4)
    {
        out[0] = SwapLanesIf<Lane::Out == 1>(in[0]);
        out[1] = SwapLanesIf<Lane::Out == 1>(in[1]);
        out[2] = SwapLanesIf<Lane::Out == 1>(in[2]);
        out[3] = SwapLanesIf<Lane::Out == 1>(in[3]);
        out[4] = SwapLanesIf<Lane::Out == 0>(in[0]);
        out[5] = SwapLanesIf<Lane::Out == 0>(in[1]);
        out[6] = SwapLanesIf<Lane::Out == 0>(in[2]);
        out[7] = SwapLanesIf<Lane::Out == 0>(in[3]);
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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    static_assert(_arraySizeOut == 4 || _arraySizeOut == 8, "Array size must be 4 or 8");

    constexpr U32 idx_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_3 = (3 + _firstRowOut) % 4;

    [[maybe_unused]] constexpr U32 trgt_lane_0 = (Lane::In == 0) ? 1 : 0;
    constexpr U32 comp_lane_1 = (_firstRowOut + 1) / 4;
    constexpr U32 comp_lane_2 = (_firstRowOut + 2) / 4;
    [[maybe_unused]] constexpr U32 comp_lane_3 = (_firstRowOut + 3) / 4;


    std::array<__m256, _arraySizeOut> out;
    out[idx_0] = Permute2F128<0, Lane::In, 1, Lane::In>(in0, in4);
    out[idx_1] = SwapLanesIf<Lane::In != comp_lane_1>(in1);
    out[idx_2] = SwapLanesIf<Lane::In != comp_lane_2>(in2);
    if constexpr (_firstRowOut == 0 || _firstRowOut == 3)
        out[idx_3] = SwapLanesIf<Lane::In != comp_lane_3>(in3);
    else
        out[idx_3] = Permute2F128<0, trgt_lane_0, 1, Lane::In>(in1, in3);


    if constexpr (_arraySizeOut == 8)
    {
        out[idx_0 + 4] = Permute2F128<0, trgt_lane_0, 1, trgt_lane_0>(in0, in4);
        out[idx_2 + 4] = SwapLanesIf<Lane::In == comp_lane_2>(in2);
        out[idx_3 + 4] = SwapLanesIf<Lane::In == comp_lane_3>(in3);
        if constexpr (_firstRowOut == 0 || _firstRowOut == 3)
            out[idx_1 + 4] = SwapLanesIf<Lane::In == comp_lane_1>(in1);
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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    static_assert(_arraySizeOut == 4 || _arraySizeOut == 8, "Array size must be 4 or 8");

    constexpr U32 idx_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_3 = (3 + _firstRowOut) % 4;

    [[maybe_unused]] constexpr U32 notLaneIn = (Lane::In == 0) ? 1 : 0;
    constexpr U32 comp_lane_2 = (_firstRowOut + 2) / 4;
    [[maybe_unused]] constexpr U32 comp_lane_3 = (_firstRowOut + 3) / 4;


    std::array<__m256, _arraySizeOut> out;
    out[idx_0] = Permute2F128<0, Lane::In, 1, Lane::In>(in0, in4);
    out[idx_1] = Permute2F128<0, Lane::In, 1, Lane::In>(in1, in5);
    out[idx_2] = SwapLanesIf<Lane::In != comp_lane_2>(in2);
    if constexpr (_firstRowOut == 0 || _firstRowOut == 2)
        out[idx_3] = SwapLanesIf<Lane::In != comp_lane_3>(in3);
    else
        out[idx_3] = Permute2F128<0, notLaneIn, 1, Lane::In>(in2, in3);


    if constexpr (_arraySizeOut == 8)
    {
        out[idx_0 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in0, in4);
        out[idx_1 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in1, in5);
        out[idx_3 + 4] = SwapLanesIf<Lane::In == comp_lane_3>(in3);
        if constexpr (_firstRowOut == 0 || _firstRowOut == 2)
            out[idx_2 + 4] = SwapLanesIf<Lane::In == comp_lane_2>(in2);
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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    static_assert(_arraySizeOut == 4 || _arraySizeOut == 8, "Array size must be 4 or 8");

    constexpr U32 idx_0 = (0 + _firstRowOut) % 4;
    constexpr U32 idx_1 = (1 + _firstRowOut) % 4;
    constexpr U32 idx_2 = (2 + _firstRowOut) % 4;
    constexpr U32 idx_3 = (3 + _firstRowOut) % 4;

    constexpr U32 comp_lane_3 = (_firstRowOut + 3) / 4;


    std::array<__m256, _arraySizeOut> out;
    out[idx_0] = Permute2F128<0, Lane::In, 1, Lane::In>(in0, in4);
    out[idx_1] = Permute2F128<0, Lane::In, 1, Lane::In>(in1, in5);
    out[idx_2] = Permute2F128<0, Lane::In, 1, Lane::In>(in2, in6);
    out[idx_3] = SwapLanesIf<Lane::In != comp_lane_3>(in3);


    if constexpr (_arraySizeOut == 8)
    {
        constexpr U32 notLaneIn = (Lane::In == 0) ? 1 : 0;

        out[idx_0 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in0, in4);
        out[idx_1 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in1, in5);
        out[idx_2 + 4] = Permute2F128<0, notLaneIn, 1, notLaneIn>(in2, in6);
        out[idx_3 + 4] = SwapLanesIf<Lane::In == comp_lane_3>(in3);
    }

    return out;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeOut>
inline std::array<__m256, _arraySizeOut> PermuteBeforeIntraLaneTransposeNx8(__m256 in0, __m256 in1, __m256 in2,
                                                                            __m256 in3, __m256 in4, __m256 in5,
                                                                            __m256 in6, __m256 in7)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    static_assert(_arraySizeOut == 4 || _arraySizeOut == 8, "Array size must be 4 or 8");

    std::array<__m256, _arraySizeOut> out;
    out[0] = Permute2F128<0, Lane::In, 1, Lane::In>(in0, in4);
    out[1] = Permute2F128<0, Lane::In, 1, Lane::In>(in1, in5);
    out[2] = Permute2F128<0, Lane::In, 1, Lane::In>(in2, in6);
    out[3] = Permute2F128<0, Lane::In, 1, Lane::In>(in3, in7);


    if constexpr (_arraySizeOut == 8)
    {
        constexpr U32 notLaneIn = (Lane::In == 0) ? 1 : 0;

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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    __m256 tout;

    if constexpr (_firstRowIn == _firstRowOut)
        tout = in;
    else
    {
        if constexpr (Lane::In == Lane::Out)
            tout = Broadcast<Lane::OffsetIn>(in);
        else
            tout = BroadcastAcrossLanes<_firstRowIn>(in);
    }

    TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(out, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m256 in0, __m256 in1, __m256& out0)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    __m256 tout;

    if constexpr (Lane::OffsetOut == 1)
    {
        __m256 tmp = Shuffle<Lane::OffsetIn, Lane::OffsetIn, Lane::OffsetIn, Lane::OffsetIn>(in0, in1);
        tout = SwapLanesIf<Lane::In != Lane::Out>(tmp);
    }
    else if constexpr (Lane::OffsetOut == 3)
    {
        __m256 tmp = Shuffle<Lane::OffsetIn, Lane::OffsetIn, Lane::OffsetIn, Lane::OffsetIn>(in1, in0);
        tout = Permute2F128<Lane::In, Lane::In>(tmp);
    }
    else
    {
        __m256 tmp0;
        if constexpr (Lane::OffsetIn < 2)
            tmp0 = _mm_unpacklo(in0, in1);
        else
            tmp0 = _mm_unpackhi(in0, in1);

        if constexpr (Lane::OffsetIn % 2 != Lane::OffsetOut / 2)
            tmp0 = Permute<2, 3, 0, 1>(tmp0);


        tout = SwapLanesIf<Lane::In != Lane::Out>(tmp0);
    }

    TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    std::array<__m256, 3> tin = {{in0, in1, in2}};

    constexpr U32 offset = Lane::OffsetOut % 2;
    constexpr U32 idx_in_0 = offset;
    constexpr U32 idx_in_1 = 1 + offset;
    constexpr U32 idx_in_2 = (2 + offset) % 3;


    __m256 tmp0;
    if constexpr (Lane::OffsetIn < 2)
        tmp0 = _mm_unpacklo(tin[idx_in_0], tin[idx_in_1]);
    else
        tmp0 = _mm_unpackhi(tin[idx_in_0], tin[idx_in_1]);


    constexpr U32 idx_0 = (Lane::OffsetIn % 2) * 2;
    constexpr U32 idx_1 = idx_0 + 1;
    constexpr U32 idx_2 = Lane::OffsetIn;

    __m256 tmp1;
    if constexpr (Lane::OffsetOut == 0)
        tmp1 = Shuffle<idx_0, idx_1, idx_2, 0>(tmp0, tin[idx_in_2]);
    else if constexpr (Lane::OffsetOut == 1)
        tmp1 = Shuffle<0, idx_2, idx_0, idx_1>(tin[idx_in_2], tmp0);
    else if constexpr (Lane::OffsetOut == 2)
        tmp1 = Shuffle<idx_2, 0, idx_0, idx_1>(tin[idx_in_2], tmp0);
    else
        tmp1 = Shuffle<idx_0, idx_1, 0, idx_2>(tmp0, tin[idx_in_2]);



    __m256 tout;

    if constexpr (Lane::OffsetOut < 2)
        tout = SwapLanesIf<Lane::In != Lane::Out>(tmp1);
    else
        tout = Permute2F128<Lane::In, Lane::In>(tmp1);


    TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    std::array<__m256, 4> tin = {{in0, in1, in2, in3}};

    constexpr U32 idx_in_0 = (4 - Lane::OffsetOut) % 4;
    constexpr U32 idx_in_1 = (5 - Lane::OffsetOut) % 4;
    constexpr U32 idx_in_2 = (6 - Lane::OffsetOut) % 4;
    constexpr U32 idx_in_3 = (7 - Lane::OffsetOut) % 4;

    __m256 tmp0, tmp1;
    if constexpr (Lane::OffsetIn < 2)
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
    if constexpr (Lane::OffsetIn % 2 == 0)
        tout = _mm_movelh(tmp0, tmp1);
    else
        tout = _mm_movehl(tmp1, tmp0);


    if constexpr (Lane::In != Lane::Out || Lane::OffsetOut > 0)
        tout = Permute2F128<Lane::In, Lane::In>(tout);


    TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;


    __m256 tmp0, tmp1;
    if constexpr (Lane::OffsetOut == 0)
    {
        Transpose1x4<Lane::OffsetIn, 0>(in0, in1, in2, in3, tmp0);
        Transpose1x1<Lane::OffsetIn, 0>(in4, tmp1);
    }
    else if constexpr (Lane::OffsetOut == 1)
    {
        Transpose1x3<Lane::OffsetIn, 1>(in0, in1, in2, tmp0);
        Transpose1x2<Lane::OffsetIn, 0>(in3, in4, tmp1);
    }
    else if constexpr (Lane::OffsetOut == 2)
    {
        Transpose1x2<Lane::OffsetIn, 2>(in0, in1, tmp0);
        Transpose1x3<Lane::OffsetIn, 0>(in2, in3, in4, tmp1);
    }
    else
    {
        Transpose1x1<Lane::OffsetIn, 3>(in0, tmp0);
        Transpose1x4<Lane::OffsetIn, 0>(in1, in2, in3, in4, tmp1);
    }

    __m256 tout = Permute2F128<0, Lane::In, 1, Lane::In>(tmp0, tmp1);

    TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;


    __m256 tmp0, tmp1;
    if constexpr (Lane::OffsetOut == 0)
    {
        Transpose1x4<Lane::OffsetIn, 0>(in0, in1, in2, in3, tmp0);
        Transpose1x2<Lane::OffsetIn, 0>(in4, in5, tmp1);
    }
    else if constexpr (Lane::OffsetOut == 1)
    {
        Transpose1x3<Lane::OffsetIn, 1>(in0, in1, in2, tmp0);
        Transpose1x3<Lane::OffsetIn, 0>(in3, in4, in5, tmp1);
    }
    else
    {
        Transpose1x2<Lane::OffsetIn, 2>(in0, in1, tmp0);
        Transpose1x4<Lane::OffsetIn, 0>(in2, in3, in4, in5, tmp1);
    }

    __m256 tout = Permute2F128<0, Lane::In, 1, Lane::In>(tmp0, tmp1);

    TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;


    __m256 tmp0, tmp1;
    if constexpr (Lane::OffsetOut == 0)
    {
        Transpose1x4<Lane::OffsetIn, 0>(in0, in1, in2, in3, tmp0);
        Transpose1x3<Lane::OffsetIn, 0>(in4, in5, in6, tmp1);
    }
    else
    {
        Transpose1x3<Lane::OffsetIn, 1>(in0, in1, in2, tmp0);
        Transpose1x4<Lane::OffsetIn, 0>(in3, in4, in5, in6, tmp1);
    }

    __m256 tout = Permute2F128<0, Lane::In, 1, Lane::In>(tmp0, tmp1);

    TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(out0, tout);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    __m256 tmp0, tmp1;

    Transpose1x4<Lane::OffsetIn, 0>(in0, in1, in2, in3, tmp0);
    Transpose1x4<Lane::OffsetIn, 0>(in4, in5, in6, in7, tmp1);

    __m256 tout = Permute2F128<0, Lane::In, 1, Lane::In>(tmp0, tmp1);

    out0 = tout;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x1(__m256 in0, __m256& out0, __m256& out1)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    std::array<__m256, 2> tout;

    if constexpr (Lane::OffsetIn < 3)
    {
        __m256 tmp0 = SwapLanesIf<Lane::In != Lane::Out>(in0);
        Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp0, tout[0]);
        Transpose1x1<Lane::OffsetIn + 1, Lane::OffsetOut>(tmp0, tout[1]);
    }
    else
    {
        __m256 tmp0 = SwapLanesIf<Lane::Out == 1>(in0);
        __m256 tmp1 = SwapLanesIf<Lane::Out == 0>(in0);

        Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp0, tout[0]);
        Transpose1x1<0, Lane::OffsetOut>(tmp1, tout[1]);
    }

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    std::array<__m256, 2> tout;

    if constexpr (Lane::OffsetOut == 0 || Lane::OffsetOut == 2)
    {
        constexpr U32 idx_out_0 = (Lane::OffsetOut == 0) ? 0 : 1;
        constexpr U32 idx_out_1 = (Lane::OffsetOut == 0) ? 1 : 0;

        if constexpr (Lane::OffsetIn == 0 || Lane::OffsetIn == 2)
        {
            __m256 tmp0;
            if constexpr (Lane::OffsetIn == 0)
                tmp0 = _mm_unpacklo(in0, in1);
            else
                tmp0 = _mm_unpackhi(in0, in1);

            tout[idx_out_0] = SwapLanesIf<Lane::In != Lane::Out>(tmp0);
            tout[idx_out_1] = Permute<2, 3, 0, 1>(tout[idx_out_0]);
        }
        else if constexpr (Lane::OffsetIn == 1)
        {
            __m256 tmp0 = Shuffle<1, 2, 1, 2>(in0, in1);
            __m256 tmp1 = SwapLanesIf<Lane::In != Lane::Out>(tmp0);
            tout[idx_out_0] = Permute<0, 2, 1, 3>(tmp1);
            tout[idx_out_1] = Permute<1, 3, 0, 2>(tmp1);
        }
        else
        {
            __m256 tmp0, tmp1;
            if constexpr (Lane::OffsetOut == 0)
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
            tout[idx_out_0] = SwapLanesIf<Lane::Out != idx_out_0>(tmp2);
            tout[idx_out_1] = SwapLanesIf<Lane::Out != idx_out_1>(tmp1);
        }
    }
    else if constexpr (Lane::OffsetOut == 1)
    {
        if constexpr (Lane::OffsetIn < 3)
        {
            if constexpr (Lane::In == Lane::Out)
            {
                tout[0] = Shuffle<Lane::OffsetIn, Lane::OffsetIn, Lane::OffsetIn, Lane::OffsetIn>(in0, in1);
                tout[1] = Shuffle<Lane::OffsetIn + 1, Lane::OffsetIn + 1, Lane::OffsetIn + 1, Lane::OffsetIn + 1>(in0,
                                                                                                                  in1);
            }
            else
            {
                __m256 tmp2 = Shuffle<Lane::OffsetIn + 1, Lane::OffsetIn, Lane::OffsetIn, Lane::OffsetIn + 1>(in0, in1);
                tout[0] = SwapLanes(tmp2);
                tout[1] = Permute<0, 0, 3, 3>(tout[0]);
            }
        }
        else
        {
            __m256 tmp2 = Shuffle<0, 3, 3, 0>(in0, in1);
            __m256 tmp3 = Shuffle<3, 0, 0, 3>(in0, in1);
            tout[0] = SwapLanesIf<Lane::Out == 1>(tmp2);
            tout[1] = SwapLanesIf<Lane::Out == 0>(tmp3);
        }
    }
    else
    {
        if constexpr (Lane::OffsetIn < 3)
        {
            __m256 tmp0 = SwapLanesIf<Lane::In != Lane::Out>(in0);
            __m256 tmp1 = SwapLanesIf<Lane::In == Lane::Out>(in1);
            tout[0] = Shuffle<Lane::OffsetIn, Lane::OffsetIn, Lane::OffsetIn, Lane::OffsetIn>(tmp1, tmp0);
            tout[1] =
                    Shuffle<Lane::OffsetIn + 1, Lane::OffsetIn + 1, Lane::OffsetIn + 1, Lane::OffsetIn + 1>(tmp1, tmp0);
        }
        else
        {
            __m256 tmp0 = Permute2F128<0, 1, 1, 0>(in0, in1);
            tout[0] = Shuffle<3, 3, 3, 3>(tmp0, in0);
            tout[1] = Shuffle<0, 0, 0, 0>(in1, tmp0);
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    constexpr U32 s0 = Lane::OffsetIn;
    constexpr U32 s1 = (Lane::OffsetIn + 1) % numValuesPerLane<__m256>;
    constexpr U32 idx0 = Lane::OffsetOut % 2;
    constexpr U32 idx1 = idx0 + 1;
    constexpr U32 idx2 = (idx0 + 2) % 3;
    std::array<__m256, 3> tin = {{in0, in1, in2}};


    __m256 tmp0;
    if constexpr (Lane::OffsetIn == 0)
        tmp0 = _mm_unpacklo(tin[idx0], tin[idx1]);
    else if constexpr (Lane::OffsetIn == 1)
        tmp0 = Shuffle<1, 2, 1, 2>(tin[idx0], tin[idx1]);
    else if constexpr (Lane::OffsetIn == 2)
        tmp0 = _mm_unpackhi(tin[idx0], tin[idx1]);
    else
        tmp0 = Shuffle<0, 3, 0, 3>(tin[idx0], tin[idx1]);


    std::array<__m256, 2> tout;

    if constexpr (Lane::OffsetOut < 2)
    {
        constexpr U32 m0 = Lane::OffsetIn % 2;
        constexpr U32 m1 = (Lane::OffsetIn == 3) ? 1 : 0;

        constexpr U32 s2 = 0 + m1;
        constexpr U32 s3 = 1 + m0 + m1;
        constexpr U32 s4 = 2 - m0 - m1;
        constexpr U32 s5 = 3 - m1;

        if constexpr (Lane::OffsetOut == 0)
        {
            tout[0] = Shuffle<s2, s3, s0, 0>(tmp0, tin[idx2]);
            tout[1] = Shuffle<s4, s5, s1, 0>(tmp0, tin[idx2]);
        }
        else
        {
            tout[0] = Shuffle<0, s0, s2, s3>(tin[idx2], tmp0);
            tout[1] = Shuffle<0, s1, s4, s5>(tin[idx2], tmp0);
        }

        constexpr bool swp0 = (Lane::In != Lane::Out);
        constexpr bool swp1 = (Lane::OffsetIn < 3) ? swp0 : !swp0;

        tout[0] = SwapLanesIf<swp0>(tout[0]);
        tout[1] = SwapLanesIf<swp1>(tout[1]);
    }
    else
    {
        constexpr U32 pr0 = (Lane::OffsetOut == 2) ? 0 : 1;
        constexpr U32 pr1 = 1 - pr0;
        constexpr U32 pl0 = (Lane::OffsetIn == 3) ? 1 : (Lane::In == Lane::Out) ? 0 : 1;
        constexpr U32 pl1 = (Lane::OffsetIn == 3) ? 0 : pl0;

        __m256 tmp1 = Permute2F128<pr0, pl0, pr1, pl1>(tmp0, tin[idx2]);

        if constexpr (Lane::OffsetIn < 3)
        {
            constexpr U32 s2 = (Lane::OffsetIn == 1) ? 2 : 1;
            constexpr U32 s3 = (Lane::OffsetIn == 1) ? 1 : 2;

            if constexpr (Lane::OffsetOut == 2)
            {
                tout[0] = Permute<s0, 0, 0, s2>(tmp1);
                tout[1] = Permute<s1, 0, s3, 3>(tmp1);
            }
            else
            {
                tout[0] = Permute<0, s2, 0, s0>(tmp1);
                tout[1] = Permute<s3, 3, 0, s1>(tmp1);
            }
        }
        else
        {
            if constexpr (Lane::OffsetOut == 2)
            {
                tout[0] = Shuffle<s0, 0, 1, 3>(tmp1, tmp0);
                tout[1] = Shuffle<s1, 0, 0, 2>(tin[idx2], tmp1);
            }
            else
            {
                tout[0] = Shuffle<1, 3, 0, s0>(tmp1, tin[idx2]);
                tout[1] = Shuffle<0, 2, 0, s1>(tmp0, tmp1);
            }
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    std::array<__m256, 2> tout;


    if constexpr (Lane::OffsetOut % 2 == 1 && Lane::In == Lane::OffsetOut / 2 && Lane::OffsetIn != 3)
    {
        constexpr U32 s0 = Lane::OffsetIn;
        constexpr U32 s1 = s0 + 1;
        constexpr U32 p0 = Lane::OffsetOut / 2;

        __m256 tmp0 = Permute2F128<0, p0, 1, p0>(in0, in3);
        __m256 tmp1 = Shuffle<s0, s1, s0, s1>(in1, in2);

        if constexpr (Lane::OffsetOut == 1)
        {
            tout[0] = Shuffle<s0, s0, 0, 2>(tmp0, tmp1);
            tout[1] = Shuffle<s1, s1, 1, 3>(tmp0, tmp1);
        }
        else
        {
            tout[0] = Shuffle<0, 2, s0, s0>(tmp1, tmp0);
            tout[1] = Shuffle<1, 3, s1, s1>(tmp1, tmp0);
        }
    }
    else
    {
        constexpr U32 idx0 = Lane::OffsetOut;
        constexpr U32 idx1 = (idx0 + 1) % 4;
        constexpr U32 idx2 = (idx0 + 2) % 4;
        constexpr U32 idx3 = (idx0 + 3) % 4;
        std::array<__m256, 4> tin = {{in0, in1, in2, in3}};

        __m256 tmp0, tmp1;
        if constexpr (Lane::OffsetIn == 0)
        {
            tmp0 = _mm_unpacklo(tin[idx0], tin[idx1]);
            tmp1 = _mm_unpacklo(tin[idx2], tin[idx3]);
        }
        else if constexpr (Lane::OffsetIn == 1)
        {
            tmp0 = Shuffle<1, 2, 1, 2>(tin[idx0], tin[idx1]);
            tmp1 = Shuffle<1, 2, 1, 2>(tin[idx2], tin[idx3]);
        }
        else if constexpr (Lane::OffsetIn == 2)
        {
            tmp0 = _mm_unpackhi(tin[idx0], tin[idx1]);
            tmp1 = _mm_unpackhi(tin[idx2], tin[idx3]);
        }
        else
        {
            tmp0 = Shuffle<3, 0, 3, 0>(tin[idx0], tin[idx1]);
            tmp1 = Shuffle<3, 0, 3, 0>(tin[idx2], tin[idx3]);
        }


        if constexpr (Lane::OffsetOut == 2 && Lane::OffsetIn == 3)
        {
            __m256 tmp2 = Permute2F128<0, 1, 1, 0>(tmp1, tmp0);

            tout[0] = Shuffle<0, 2, 0, 2>(tmp2, tmp1);
            tout[1] = Shuffle<1, 3, 1, 3>(tmp0, tmp2);
        }
        else if constexpr (Lane::OffsetOut == 0 || Lane::OffsetOut == 2)
        {
            if constexpr (Lane::OffsetOut == 2)
            {
                tmp0 = SwapLanesIf<Lane::In == 0>(tmp0);
                tmp1 = SwapLanesIf<Lane::In == 1>(tmp1);
            }

            if constexpr (Lane::OffsetIn == 0 || Lane::OffsetIn == 2)
            {
                tout[0] = _mm_movelh(tmp0, tmp1);
                tout[1] = _mm_movehl(tmp1, tmp0);
            }
            else
            {
                tout[0] = Shuffle<0, 2, 0, 2>(tmp0, tmp1);
                tout[1] = Shuffle<1, 3, 1, 3>(tmp0, tmp1);
            }

            constexpr bool swp0 = (Lane::In != Lane::Out && Lane::OffsetOut == 0);
            constexpr bool swp1 = (Lane::OffsetOut == 2) ? false : (Lane::OffsetIn < 3) ? swp0 : !swp0;

            tout[0] = SwapLanesIf<swp0>(tout[0]);
            tout[1] = SwapLanesIf<swp1>(tout[1]);
        }
        else if constexpr (Lane::OffsetOut == 1 || Lane::OffsetOut == 3)
        {
            __m256 tmp2, tmp3;
            if constexpr (Lane::OffsetIn == 0 || Lane::OffsetIn == 2)
            {
                tmp2 = _mm_movelh(tmp1, tmp0);
                tmp3 = _mm_movehl(tmp0, tmp1);
            }
            else
            {
                tmp2 = Shuffle<0, 2, 0, 2>(tmp1, tmp0);
                tmp3 = Shuffle<1, 3, 1, 3>(tmp1, tmp0);
            }

            constexpr U32 l0 = (Lane::OffsetIn == 3 || Lane::OffsetOut == 3) ? 0 : 1;
            constexpr U32 l1 = (Lane::OffsetIn == 3 || Lane::OffsetOut == 1) ? 1 : 0;
            tout[0] = Permute2F128<l0, l0>(tmp2);
            tout[1] = Permute2F128<l1, l1>(tmp3);
        }
    }


    intern::TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 3) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx5<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4);

    std::array<__m256, 2> tout = intern::IntraLaneTransposeAfterPermute2xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 3) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 2> tout = intern::IntraLaneTransposeAfterPermute2xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 3) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 2> tout = intern::IntraLaneTransposeAfterPermute2xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 3) ? 4 : 8;

    std::array<__m256, arraySize> tmp = PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, arraySize>(
            in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 2> tout = intern::IntraLaneTransposeAfterPermute2xN<_firstRowIn>(tmp);


    intern::TransposeSetOutput<_firstRowOut, 8, _overwriteUnused, _unusedSetZero>(tout, out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    std::array<__m256, 3> tout;

    if constexpr (Lane::OffsetIn < 2)
    {
        __m256 tmp0 = SwapLanesIf<Lane::In != Lane::Out>(in0);
        Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp0, tout[0]);
        Transpose1x1<Lane::OffsetIn + 1, Lane::OffsetOut>(tmp0, tout[1]);
        Transpose1x1<Lane::OffsetIn + 2, Lane::OffsetOut>(tmp0, tout[2]);
    }
    else
    {
        std::array<__m256, 2> tmp;
        tmp[0] = SwapLanesIf<Lane::Out == 1>(in0);
        tmp[1] = SwapLanesIf<Lane::Out == 0>(in0);

        constexpr U32 idx_1 = (Lane::OffsetIn < 3) ? 0 : 1;

        constexpr U32 row_0 = Lane::OffsetIn;
        constexpr U32 row_1 = (Lane::OffsetIn + 1) % 4;
        constexpr U32 row_2 = (Lane::OffsetIn + 2) % 4;

        Transpose1x1<row_0, Lane::OffsetOut>(tmp[0], tout[0]);
        Transpose1x1<row_1, Lane::OffsetOut>(tmp[idx_1], tout[1]);
        Transpose1x1<row_2, Lane::OffsetOut>(tmp[1], tout[2]);
    }

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    std::array<__m256, 3> tout;

    if constexpr (Lane::OffsetOut == 0 || Lane::OffsetOut == 2)
    {
        std::array<__m256, 2> tmp;

        if constexpr (Lane::OffsetIn < 2)
        {
            __m256 tmp0 = SwapLanesIf<Lane::In != Lane::Out>(in0);
            __m256 tmp1 = SwapLanesIf<Lane::In != Lane::Out>(in1);
            tmp[0] = _mm_unpacklo(tmp0, tmp1);
            tmp[1] = _mm_unpackhi(tmp0, tmp1);
        }
        else
        {
            __m256 tmp0 = _mm_unpackhi(in0, in1);
            __m256 tmp1 = _mm_unpacklo(in0, in1);
            tmp[0] = SwapLanesIf<Lane::Out == 1>(tmp0);
            tmp[1] = SwapLanesIf<Lane::Out == 0>(tmp1);
        }

        constexpr U32 mod = Lane::OffsetOut / 2;
        if constexpr (Lane::OffsetIn % 2 == mod)
        {
            tout[0] = tmp[0];
            tout[1] = Permute<2, 3, 0, 1>(tmp[0 + mod]);
            tout[2] = tmp[1];
        }
        else
        {
            tout[0] = Permute<2, 3, 0, 1>(tmp[0]);
            tout[1] = tmp[1 - mod];
            tout[2] = Permute<2, 3, 0, 1>(tmp[1]);
        }
    }
    else
    {
        if constexpr (Lane::OffsetIn == 2 && Lane::OffsetOut == 1 && Lane::Out == 1)
        {
            __m256 tmp0 = Shuffle<3, 2, 2, 3>(in0, in1);

            tout[0] = SwapLanes(tmp0);
            tout[1] = Permute<0, 0, 3, 0>(tout[0]);
            tout[2] = Shuffle<0, 0, 0, 0>(in0, in1);
        }
        else if constexpr (Lane::OffsetIn == 3 && Lane::OffsetOut == 1 && Lane::Out == 0)
        {
            __m256 tmp0 = Shuffle<1, 0, 0, 1>(in0, in1);

            tout[1] = SwapLanes(tmp0);
            tout[2] = Permute<0, 0, 3, 0>(tout[1]);
            tout[0] = Shuffle<0, 3, 3, 0>(in0, in1);
        }
        else
        {
            constexpr U32 s0 = Lane::OffsetIn;
            constexpr U32 s1 = (s0 + 1) % 4;
            constexpr U32 s2 = (s0 + 2) % 4;

            if constexpr (Lane::OffsetIn > 1 && Lane::OffsetOut == 3)
            {
                __m256 tmp0 = Permute2F128<0, 1, 1, 0>(in0, in1);

                tout[0] = Shuffle<s0, 0, 0, s0>(tmp0, in0);
                tout[2] = Shuffle<s2, 0, 0, s2>(in1, tmp0);

                if constexpr (Lane::OffsetIn == 2)
                    tout[1] = Shuffle<s1, 0, 0, s1>(tmp0, in0);
                else
                    tout[1] = Shuffle<s1, 0, 0, s1>(in1, tmp0);
            }
            else
            {
                __m256 tmp0, tmp1;

                if constexpr (Lane::OffsetOut == 3)
                {
                    tmp0 = SwapLanesIf<Lane::In == 0>(in1);
                    tmp1 = SwapLanesIf<Lane::In == 1>(in0);
                }
                else
                {
                    tmp0 = SwapLanesIf<(Lane::In != Lane::Out && Lane::OffsetIn < 2)>(in0);
                    tmp1 = SwapLanesIf<(Lane::In != Lane::Out && Lane::OffsetIn < 2)>(in1);
                }

                tout[0] = Shuffle<s0, s0, s0, s0>(tmp0, tmp1);
                tout[1] = Shuffle<s1, s1, s1, s1>(tmp0, tmp1);
                tout[2] = Shuffle<s2, s2, s2, s2>(tmp0, tmp1);

                tout[0] = SwapLanesIf<Lane::OffsetIn == 3>(tout[0]);
                tout[2] = SwapLanesIf<Lane::OffsetIn == 2>(tout[2]);
            }
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    std::array<__m256, 3> tin = {{in0, in1, in2}};
    std::array<__m256, 3> tout;

    if constexpr (Lane::OffsetOut < 2)
    {
        constexpr U32 mod = Lane::OffsetOut;

        __m256 tmp0;
        if constexpr (Lane::OffsetOut == 0)
            tmp0 = _mm_unpackhi(tin[0], tin[1]);
        else
            tmp0 = _mm_unpacklo(tin[1], tin[2]);

        if constexpr (Lane::OffsetIn != 0 + mod)
        {
            constexpr U32 idx = (7 + mod - Lane::OffsetIn) % 4;
            if constexpr (Lane::OffsetOut == 0)
                tout[idx] = Shuffle<2, 3, 3, 2>(tmp0, tin[2]);
            else
                tout[idx] = Shuffle<1, 0, 0, 1>(tin[0], tmp0);
        }

        if constexpr (Lane::OffsetIn != 1 - mod)
        {
            __m256 tmp1;
            if constexpr (Lane::OffsetOut == 0)
                tmp1 = Shuffle<0, 0, 0, 0>(tin[1], tin[2]);
            else
                tmp1 = Shuffle<3, 3, 3, 3>(tin[0], tin[1]);

            constexpr U32 idx = (8 - mod - Lane::OffsetIn) % 4;
            tout[idx] = BlendIndex<_firstRowIn + idx>(tmp1, tin[Lane::OffsetOut * 2]);
        }

        if constexpr (Lane::OffsetIn != 2 + mod)
        {
            constexpr U32 s = mod + 1;
            __m256 tmp1 = Shuffle<s, s, s, s>(tin[0], tin[2]);

            constexpr U32 idx = (5 + mod - Lane::OffsetIn) % 4;
            tout[idx] = BlendIndex<_firstRowIn + idx>(tmp1, tin[1]);
        }

        if constexpr (Lane::OffsetIn != 3 - mod)
        {
            constexpr U32 idx = (6 - mod - Lane::OffsetIn) % 4;
            tout[idx] = BlendIndex<_firstRowIn + idx>(tmp0, tin[(1 - mod) * 2]);
        }

        constexpr bool swp_con_0 = Lane::In != Lane::Out;
        constexpr bool swp_con_1 = (Lane::OffsetIn < 2) ? swp_con_0 : Lane::Out != Lane::OffsetIn % 2;
        constexpr bool swp_con_2 = (Lane::OffsetIn < 2) ? swp_con_0 : Lane::Out == 0;

        tout[0] = SwapLanesIf<swp_con_0>(tout[0]);
        tout[1] = SwapLanesIf<swp_con_1>(tout[1]);
        tout[2] = SwapLanesIf<swp_con_2>(tout[2]);
    }
    else
    {
        if constexpr (Lane::OffsetIn < 2)
        {
            std::array<__m256, 2> tmp;
            constexpr U32 idx0 = Lane::OffsetOut % 2;
            constexpr U32 idx1 = 1 - idx0;

            if constexpr (Lane::In == idx1)
                tmp[idx1] = Permute2F128<0, idx1, 1, idx1>(tin[idx1], tin[idx1 + 1]);
            else
                tmp[idx1] = tin[1];

            tmp[idx0] = Permute2F128<0, Lane::In, 1, Lane::In>(tin[0], tin[2]);

            __m256 tmp2 = _mm_unpacklo(tmp[0], tmp[1]);
            __m256 tmp3 = _mm_unpackhi(tmp[0], tmp[1]);

            if constexpr (Lane::OffsetIn == 0)
                tout[0] = Permute<0, 1, 0, 1>(tmp2);
            else
                tout[2] = Permute<2, 3, 2, 3>(tmp3);

            tout[1 - Lane::OffsetIn] = Permute<2, 3, 2, 3>(tmp2);
            tout[2 - Lane::OffsetIn] = Permute<0, 1, 0, 1>(tmp3);
        }
        else
        {
            constexpr U32 mod = Lane::OffsetOut % 2;
            std::array<__m256, 4> tmp;

            constexpr U32 idx_tmp2 = (Lane::OffsetOut == 3) ? 0 : 2;
            tmp[0] = _mm_unpacklo(tin[mod], tin[mod + 1]);
            tmp[1] = _mm_unpackhi(tin[mod], tin[mod + 1]);
            tmp[2] = tin[idx_tmp2];
            if constexpr (Lane::OffsetOut == 2)
                tmp[3] = Permute2F128<0, 1, 1, 0>(tmp[0], tin[2]);
            else
                tmp[3] = Permute2F128<0, 1, 1, 0>(tin[0], tmp[1]);

            constexpr U32 idx0 = mod + 1;
            constexpr U32 idx1 = 2 - mod * 2;

            if constexpr (Lane::OffsetIn == 2)
                tout[0] = Shuffle<idx1, 1, 0, idx0>(tmp[3], tmp[idx0]);
            else
                tout[2] = Shuffle<idx0, 3, 2, idx1 + 1>(tmp[idx1], tmp[3]);

            tout[3 - Lane::OffsetIn] = Shuffle<3 - mod, 3, 2, 3>(tmp[3], tmp[idx0]);
            tout[4 - Lane::OffsetIn] = Shuffle<0, 1, 0, 1 - mod>(tmp[idx1], tmp[3]);
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    std::array<__m256, 3> tout;
    std::array<__m256, 4> tin = {{in0, in1, in2, in3}};


    constexpr U32 idx_0 = (4 - Lane::OffsetOut) % 4;
    constexpr U32 idx_1 = (5 - Lane::OffsetOut) % 4;
    constexpr U32 idx_2 = (6 - Lane::OffsetOut) % 4;
    constexpr U32 idx_3 = (7 - Lane::OffsetOut) % 4;


    if constexpr (Lane::OffsetIn < 2 && Lane::OffsetOut > 0)
    {
        // This branch is not necessary, since the else-case also produces the correct results. However, it saves 1-2
        // Permute2F128.
        constexpr bool swp_con_0 = Lane::In == Lane::Out;
        constexpr bool swp_con_1 = (Lane::OffsetOut < 2) ? Lane::In != Lane::Out : Lane::In == Lane::Out;
        constexpr bool swp_con_2 = (Lane::OffsetOut < 3) ? Lane::In != Lane::Out : Lane::In == Lane::Out;
        constexpr bool swp_con_3 = Lane::In != Lane::Out;

        __m256 tmp0 = SwapLanesIf<swp_con_0>(tin[idx_0]);
        __m256 tmp1 = SwapLanesIf<swp_con_1>(tin[idx_1]);
        __m256 tmp2 = SwapLanesIf<swp_con_2>(tin[idx_2]);
        __m256 tmp3 = SwapLanesIf<swp_con_3>(tin[idx_3]);

        __m256 tmp4 = _mm_unpacklo(tmp0, tmp1);
        __m256 tmp5 = _mm_unpackhi(tmp0, tmp1);
        __m256 tmp6 = _mm_unpacklo(tmp2, tmp3);
        __m256 tmp7 = _mm_unpackhi(tmp2, tmp3);

        if constexpr (Lane::OffsetIn == 0)
        {
            tout[0] = _mm_movelh(tmp4, tmp6);
            tout[1] = _mm_movehl(tmp6, tmp4);
            tout[2] = _mm_movelh(tmp5, tmp7);
        }
        else
        {
            tout[0] = _mm_movehl(tmp6, tmp4);
            tout[1] = _mm_movelh(tmp5, tmp7);
            tout[2] = _mm_movehl(tmp7, tmp5);
        }
    }
    else
    {
        std::array<__m256, 4> tmp;
        tmp[0] = _mm_unpacklo(tin[idx_0], tin[idx_1]);
        tmp[1] = _mm_unpacklo(tin[idx_2], tin[idx_3]);
        tmp[2] = _mm_unpackhi(tin[idx_0], tin[idx_1]);
        tmp[3] = _mm_unpackhi(tin[idx_2], tin[idx_3]);


        constexpr U32 mod = (Lane::OffsetIn / 2) * 2;
        if constexpr (Lane::OffsetIn == 0 || Lane::OffsetIn == 2)
        {
            tout[0] = _mm_movelh(tmp[0 + mod], tmp[1 + mod]);
            tout[1] = _mm_movehl(tmp[1 + mod], tmp[0 + mod]);
            tout[2] = _mm_movelh(tmp[2 - mod], tmp[3 - mod]);
        }
        else
        {
            tout[0] = _mm_movehl(tmp[1 + mod], tmp[0 + mod]);
            tout[1] = _mm_movelh(tmp[2 - mod], tmp[3 - mod]);
            tout[2] = _mm_movehl(tmp[3 - mod], tmp[2 - mod]);
        }


        if constexpr (Lane::OffsetOut == 0)
        {
            constexpr bool swp_con_0 = Lane::In != Lane::Out;
            constexpr bool swp_con_1 = (Lane::OffsetIn < 3) ? Lane::In != Lane::Out : Lane::Out == 0;
            constexpr bool swp_con_2 = (Lane::OffsetIn < 2) ? Lane::In != Lane::Out : Lane::Out == 0;

            tout[0] = SwapLanesIf<swp_con_0>(tout[0]);
            tout[1] = SwapLanesIf<swp_con_1>(tout[1]);
            tout[2] = SwapLanesIf<swp_con_2>(tout[2]);
        }
        else
        {
            constexpr U32 lane_0 = Lane::In;
            constexpr U32 lane_1 = (_firstRowIn + 1) / 4;
            constexpr U32 lane_2 = (_firstRowIn + 2) / 4;

            tout[0] = Permute2F128<lane_0, lane_0>(tout[0]);
            tout[1] = Permute2F128<lane_1, lane_1>(tout[1]);
            tout[2] = Permute2F128<lane_2, lane_2>(tout[2]);
        }
    }


    intern::TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 2) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx5<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4);

    std::array<__m256, 3> tout = intern::IntraLaneTransposeAfterPermute3xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 2) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 3> tout = intern::IntraLaneTransposeAfterPermute3xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 2) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 3> tout = intern::IntraLaneTransposeAfterPermute3xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose3x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 2) ? 4 : 8;

    std::array<__m256, arraySize> tmp = PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, arraySize>(
            in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 3> tout = intern::IntraLaneTransposeAfterPermute3xN<_firstRowIn>(tmp);


    intern::TransposeSetOutput<_firstRowOut, 8, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    std::array<__m256, 4> tout;

    if constexpr (Lane::OffsetIn == 0)
    {
        __m256 tmp0 = SwapLanesIf<Lane::In != Lane::Out>(in0);
        Transpose1x1<Lane::OffsetIn, Lane::OffsetOut>(tmp0, tout[0]);
        Transpose1x1<Lane::OffsetIn + 1, Lane::OffsetOut>(tmp0, tout[1]);
        Transpose1x1<Lane::OffsetIn + 2, Lane::OffsetOut>(tmp0, tout[2]);
        Transpose1x1<Lane::OffsetIn + 3, Lane::OffsetOut>(tmp0, tout[3]);
    }
    else
    {
        std::array<__m256, 2> tmp;
        tmp[0] = SwapLanesIf<Lane::Out == 1>(in0);
        tmp[1] = SwapLanesIf<Lane::Out == 0>(in0);

        constexpr U32 idx_1 = (Lane::OffsetIn < 3) ? 0 : 1;
        constexpr U32 idx_2 = (Lane::OffsetIn < 2) ? 0 : 1;

        constexpr U32 row_0 = Lane::OffsetIn;
        constexpr U32 row_1 = (Lane::OffsetIn + 1) % 4;
        constexpr U32 row_2 = (Lane::OffsetIn + 2) % 4;
        constexpr U32 row_3 = (Lane::OffsetIn + 3) % 4;

        Transpose1x1<row_0, Lane::OffsetOut>(tmp[0], tout[0]);
        Transpose1x1<row_1, Lane::OffsetOut>(tmp[idx_1], tout[1]);
        Transpose1x1<row_2, Lane::OffsetOut>(tmp[idx_2], tout[2]);
        Transpose1x1<row_3, Lane::OffsetOut>(tmp[1], tout[3]);
    }

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    std::array<__m256, 4> tout;

    constexpr U32 idx_0 = (4 - Lane::OffsetIn) % 4;
    constexpr U32 idx_1 = (5 - Lane::OffsetIn) % 4;
    constexpr U32 idx_2 = (6 - Lane::OffsetIn) % 4;
    constexpr U32 idx_3 = (7 - Lane::OffsetIn) % 4;

    if constexpr (Lane::OffsetOut < 3)
    {
        __m256 tmp0 = SwapLanesIf<(Lane::OffsetIn == 0 && Lane::In != Lane::Out)>(in0);
        __m256 tmp1 = SwapLanesIf<(Lane::OffsetIn == 0 && Lane::In != Lane::Out)>(in1);


        if constexpr (Lane::OffsetOut == 0)
        {
            __m256 tmp2 = _mm_unpacklo(tmp0, tmp1);
            __m256 tmp3 = _mm_unpackhi(tmp0, tmp1);

            tout[idx_0] = tmp2;
            tout[idx_1] = Permute<2, 3, 0, 1>(tmp2);
            tout[idx_2] = tmp3;
            tout[idx_3] = Permute<2, 3, 0, 1>(tmp3);
        }
        else if constexpr (Lane::OffsetOut == 1)
        {
            tout[idx_0] = Shuffle<0, 0, 0, 0>(tmp0, tmp1);
            tout[idx_1] = Shuffle<1, 1, 1, 1>(tmp0, tmp1);
            tout[idx_2] = Shuffle<2, 2, 2, 2>(tmp0, tmp1);
            tout[idx_3] = Shuffle<3, 3, 3, 3>(tmp0, tmp1);
        }
        else
        {
            __m256 tmp2 = _mm_unpacklo(tmp0, tmp1);
            __m256 tmp3 = _mm_unpackhi(tmp0, tmp1);

            tout[idx_0] = Permute<2, 3, 0, 1>(tmp2);
            tout[idx_1] = tmp2;
            tout[idx_2] = Permute<2, 3, 0, 1>(tmp3);
            tout[idx_3] = tmp3;
        }


        if constexpr (Lane::OffsetIn > 0)
        {
            constexpr bool swp_con0 = Lane::Out == 1;
            constexpr bool swp_con1 = (Lane::OffsetIn < 3) ? Lane::Out == 1 : Lane::Out == 0;
            constexpr bool swp_con2 = (Lane::OffsetIn < 2) ? Lane::Out == 1 : Lane::Out == 0;
            constexpr bool swp_con3 = Lane::Out == 0;

            tout[0] = SwapLanesIf<swp_con0>(tout[0]);
            tout[1] = SwapLanesIf<swp_con1>(tout[1]);
            tout[2] = SwapLanesIf<swp_con2>(tout[2]);
            tout[3] = SwapLanesIf<swp_con3>(tout[3]);
        }
    }
    else
    {
        constexpr U32 lane_0 = (Lane::OffsetIn == 0 && Lane::In == 0) ? 0 : 1;
        constexpr U32 lane_1 = (Lane::OffsetIn == 0 && Lane::In == 1) ? 1 : 0;

        __m256 tmp0 = Permute2F128<0, lane_0, 1, lane_1>(in0, in1);

        if constexpr (Lane::OffsetIn == 0)
        {
            tout[idx_0] = Broadcast<0>(tmp0);
            tout[idx_1] = Broadcast<1>(tmp0);
            tout[idx_2] = Broadcast<2>(tmp0);
            tout[idx_3] = Broadcast<3>(tmp0);
        }
        else
        {
            tout[idx_0] = Shuffle<0, 0, 0, 0>(in1, tmp0);

            if constexpr (Lane::OffsetIn < 2)
                tout[idx_1] = Shuffle<1, 1, 1, 1>(tmp0, in0);
            else
                tout[idx_1] = Shuffle<1, 1, 1, 1>(in1, tmp0);

            if constexpr (Lane::OffsetIn < 3)
                tout[idx_2] = Shuffle<2, 2, 2, 2>(tmp0, in0);
            else
                tout[idx_2] = Shuffle<2, 2, 2, 2>(in1, tmp0);

            tout[idx_3] = Shuffle<3, 3, 3, 3>(tmp0, in0);
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3);
} // namespace GDL::simd



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    std::array<__m256, 4> tout;

    constexpr U32 idx_0 = (4 - Lane::OffsetIn) % 4;
    constexpr U32 idx_1 = (5 - Lane::OffsetIn) % 4;
    constexpr U32 idx_2 = (6 - Lane::OffsetIn) % 4;
    constexpr U32 idx_3 = (7 - Lane::OffsetIn) % 4;

    if constexpr (Lane::OffsetOut < 2)
    {
        __m256 tmp0 = SwapLanesIf<(Lane::OffsetIn == 0 && Lane::In != Lane::Out)>(in0);
        __m256 tmp1 = SwapLanesIf<(Lane::OffsetIn == 0 && Lane::In != Lane::Out)>(in1);
        __m256 tmp2 = SwapLanesIf<(Lane::OffsetIn == 0 && Lane::In != Lane::Out)>(in2);


        if constexpr (Lane::OffsetOut == 0)
        {
            __m256 tmp3 = _mm_unpacklo(tmp0, tmp1);
            __m256 tmp4 = _mm_unpackhi(tmp0, tmp1);

            tout[idx_0] = _mm_movelh(tmp3, tmp2);
            tout[idx_1] = Shuffle<2, 3, 1, 0>(tmp3, tmp2);
            tout[idx_2] = Blend<0, 0, 1, 0, 0, 0, 1, 0>(tmp4, tmp2);
            tout[idx_3] = Shuffle<2, 3, 3, 2>(tmp4, tmp2);
        }
        else
        {
            __m256 tmp3 = _mm_unpacklo(tmp1, tmp2);
            __m256 tmp4 = _mm_unpackhi(tmp1, tmp2);

            tout[idx_0] = Shuffle<1, 0, 0, 1>(tmp0, tmp3);
            tout[idx_1] = Blend<0, 1, 0, 0, 0, 1, 0, 0>(tmp3, tmp0);
            tout[idx_2] = Shuffle<3, 2, 0, 1>(tmp0, tmp4);
            tout[idx_3] = _mm_movehl(tmp4, tmp0);
        }


        if constexpr (Lane::OffsetIn > 0)
        {
            constexpr bool swp_con0 = Lane::Out == 1;
            constexpr bool swp_con1 = (Lane::OffsetIn < 3) ? Lane::Out == 1 : Lane::Out == 0;
            constexpr bool swp_con2 = (Lane::OffsetIn < 2) ? Lane::Out == 1 : Lane::Out == 0;
            constexpr bool swp_con3 = Lane::Out == 0;

            tout[0] = SwapLanesIf<swp_con0>(tout[0]);
            tout[1] = SwapLanesIf<swp_con1>(tout[1]);
            tout[2] = SwapLanesIf<swp_con2>(tout[2]);
            tout[3] = SwapLanesIf<swp_con3>(tout[3]);
        }
    }
    else
    {
        constexpr U32 mod = Lane::OffsetOut % 2;

        if constexpr (Lane::OffsetIn == 0)
        {
            std::array<__m256, 2> tmp;
            tmp[0 + mod] = Permute2F128<0, Lane::In, 1, Lane::In>(in0, in2);
            tmp[1 - mod] = SwapLanesIf<Lane::In != mod>(in1);

            __m256 tmp2 = _mm_unpacklo(tmp[0], tmp[1]);
            __m256 tmp3 = _mm_unpackhi(tmp[0], tmp[1]);

            tout[idx_0] = Permute<0, 1, 0, 1>(tmp2);
            tout[idx_1] = Permute<2, 3, 2, 3>(tmp2);
            tout[idx_2] = Permute<0, 1, 0, 1>(tmp3);
            tout[idx_3] = Permute<2, 3, 2, 3>(tmp3);
        }
        else
        {
            std::array<__m256, 3> tin = {{in0, in1, in2}};

            __m256 tmp2 = _mm_unpacklo(tin[0 + mod], tin[1 + mod]);
            __m256 tmp3 = _mm_unpackhi(tin[0 + mod], tin[1 + mod]);


            if constexpr (Lane::OffsetOut == 2)
            {
                __m256 tmp4 = Permute2F128<0, 1, 1, 0>(tmp2, in2);

                tout[idx_0] = _mm_movelh(in2, tmp4);

                if constexpr (Lane::OffsetIn < 2)
                    tout[idx_1] = Shuffle<1, 0, 2, 3>(tmp4, tmp2);
                else
                    tout[idx_1] = Shuffle<1, 0, 2, 3>(in2, tmp4);

                if constexpr (Lane::OffsetIn < 3)
                    tout[idx_2] = Shuffle<2, 0, 0, 1>(tmp4, tmp3);
                else
                {
                    __m256 tmp5 = SwapLanes(tmp3);
                    tout[idx_2] = Shuffle<2, 0, 0, 1>(in2, tmp5);
                }

                tout[idx_3] = Shuffle<3, 0, 2, 3>(tmp4, tmp3);
            }
            else
            {
                __m256 tmp4 = Permute2F128<0, 1, 1, 0>(in0, tmp3);

                tout[idx_0] = Shuffle<0, 1, 0, 0>(tmp2, tmp4);

                if constexpr (Lane::OffsetIn < 2)
                {
                    __m256 tmp5 = SwapLanes(tmp2);
                    tout[idx_1] = Shuffle<2, 3, 0, 1>(tmp5, in0);
                }
                else
                    tout[idx_1] = Shuffle<2, 3, 0, 1>(tmp2, tmp4);

                if constexpr (Lane::OffsetIn < 3)
                    tout[idx_2] = Shuffle<0, 1, 0, 2>(tmp4, in0);
                else
                    tout[idx_2] = Shuffle<0, 1, 3, 2>(tmp3, tmp4);

                tout[idx_3] = _mm_movehl(in0, tmp4);
            }
        }
    }

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;

    std::array<__m256, 4> tmp;
    if constexpr (Lane::OffsetIn == 0 && Lane::OffsetOut == 0)
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

    intern::TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 1) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx5<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4);

    std::array<__m256, 4> tout = intern::IntraLaneTransposeAfterPermute4xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 1) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 4> tout = intern::IntraLaneTransposeAfterPermute4xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 1) ? 4 : 8;

    std::array<__m256, arraySize> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, arraySize>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 4> tout = intern::IntraLaneTransposeAfterPermute4xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose4x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, _firstRowOut>;
    constexpr UST arraySize = (Lane::OffsetIn < 1) ? 4 : 8;

    std::array<__m256, arraySize> tmp = PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, arraySize>(
            in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 4> tout = intern::IntraLaneTransposeAfterPermute4xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 8, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx1<_firstRowOut>(in0);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTranspose5xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx2<_firstRowOut>(in0, in1);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTranspose5xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx3<_firstRowOut>(in0, in1, in2);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTranspose5xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTransposeNx4<_firstRowIn, _firstRowOut, 5>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx5<_firstRowOut>(in0, in1, in2, in3, in4);

    std::array<__m256, 5> tout = PermuteAfterIntraLaneTranspose5xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 5> tout = intern::IntraLaneTransposeAfterPermute5xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 5> tout = intern::IntraLaneTransposeAfterPermute5xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 5> tout = intern::IntraLaneTransposeAfterPermute5xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 8, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx1<_firstRowOut>(in0);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTranspose6xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4,
                         __m256& out5)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx2<_firstRowOut>(in0, in1);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTranspose6xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4, __m256& out5)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx3<_firstRowOut>(in0, in1, in2);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTranspose6xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTransposeNx4<_firstRowIn, _firstRowOut, 6>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx5<_firstRowOut>(in0, in1, in2, in3, in4);

    std::array<__m256, 6> tout = PermuteAfterIntraLaneTranspose6xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx6<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 6> tout = intern::IntraLaneTransposeAfterPermute6xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx7<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 6> tout = intern::IntraLaneTransposeAfterPermute6xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 6> tout = intern::IntraLaneTransposeAfterPermute6xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 8, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx1<_firstRowOut>(in0);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4,
                         __m256& out5, __m256& out6)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx2<_firstRowOut>(in0, in1);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4, __m256& out5, __m256& out6)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx3<_firstRowOut>(in0, in1, in2);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTransposeNx4<_firstRowIn, _firstRowOut, 7>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx5<_firstRowOut>(in0, in1, in2, in3, in4);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx6<_firstRowOut>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx7<_firstRowOut>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 7> tout = PermuteAfterIntraLaneTranspose7xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    std::array<__m256, 8> tmp =
            PermuteBeforeIntraLaneTransposeNx8<_firstRowIn, _firstRowOut, 8>(in0, in1, in2, in3, in4, in5, in6, in7);

    std::array<__m256, 7> tout = intern::IntraLaneTransposeAfterPermute7xN<_firstRowIn>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 8, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x1(__m256 in0, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6, __m256& out7)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx1<_firstRowOut>(in0);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 1, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6, out7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x2(__m256 in0, __m256 in1, __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4,
                         __m256& out5, __m256& out6, __m256& out7)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx2<_firstRowOut>(in0, in1);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 2, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6, out7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x3(__m256 in0, __m256 in1, __m256 in2, __m256& out0, __m256& out1, __m256& out2, __m256& out3,
                         __m256& out4, __m256& out5, __m256& out6, __m256& out7)
{
    auto tmp = IntraLaneTransposeBeforePermuteNx3<_firstRowOut>(in0, in1, in2);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 3, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6, out7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5, __m256& out6, __m256& out7)
{
    std::array<__m256, 4> tmp = IntraLaneTransposeBeforePermuteNx4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTransposeNx4<_firstRowIn, _firstRowOut, 8>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 4, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6, out7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6, __m256& out7)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx5<_firstRowOut>(in0, in1, in2, in3, in4);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 5, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6, out7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6,
                         __m256& out7)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx6<_firstRowOut>(in0, in1, in2, in3, in4, in5);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 6, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6, out7);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose8x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6, __m256& out7)
{
    std::array<__m256, 8> tmp = IntraLaneTransposeBeforePermuteNx7<_firstRowOut>(in0, in1, in2, in3, in4, in5, in6);

    std::array<__m256, 8> tout = PermuteAfterIntraLaneTranspose8xN<_firstRowIn, _firstRowOut>(tmp);

    intern::TransposeSetOutput<_firstRowOut, 7, _overwriteUnused, _unusedSetZero>(tout, out0, out1, out2, out3, out4,
                                                                                  out5, out6, out7);
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



// --------------------------------------------------------------------------------------------------------------------

namespace intern
{

template <U32 _firstRowIn, UST _arraySizeIn>
inline std::array<__m256, 2> IntraLaneTransposeAfterPermute2xN(std::array<__m256, _arraySizeIn> in)
{
    using Lane = TranspositionLaneData<__m256, _firstRowIn, 0>;
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");

    std::array<__m256, 2> out;
    if constexpr (_arraySizeIn == 4)
    {
        Transpose2x4<Lane::OffsetIn, 0>(in[0], in[1], in[2], in[3], out[0], out[1]);
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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, 0>;
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");


    std::array<__m256, 3> out;
    if constexpr (_arraySizeIn == 4)
    {
        Transpose3x4<Lane::OffsetIn, 0>(in[0], in[1], in[2], in[3], out[0], out[1], out[2]);
    }
    else if constexpr (Lane::OffsetIn == 2)
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
    using Lane = TranspositionLaneData<__m256, _firstRowIn, 0>;
    static_assert(_arraySizeIn == 4 || _arraySizeIn == 8, "Only arrays of size 4 and 8 accepted");


    std::array<__m256, 4> out;
    if constexpr (_arraySizeIn == 4)
    {
        Transpose4x4(in[0], in[1], in[2], in[3], out[0], out[1], out[2], out[3]);
    }
    else if constexpr (Lane::OffsetIn == 1)
    {
        Transpose3x4<_firstRowIn>(in[0], in[1], in[2], in[3], out[0], out[1], out[2]);
        Transpose1x4(in[4], in[5], in[6], in[7], out[3]);
    }
    else if constexpr (Lane::OffsetIn == 2)
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


} // namespace intern


} // namespace GDL::simd

#endif // __AVX2__
