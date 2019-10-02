#pragma once

#include "gdl/base/simd/transpose.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"



#include <iostream>

namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _unusedSetZero,
          U32 _colStrideIn, U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<_registerType, _arrSizeIn>& matDataIn)
{
    std::array<_registerType, _colStrideOut*(_rows - 1) + 1> matDataOut;
    Transpose<_rows, _cols, _rowStart, _colStartIn, true, _unusedSetZero, _colStrideIn, 0, _colStrideOut>(matDataIn,
                                                                                                          matDataOut);
    return matDataOut;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _unusedSetZero, U32 _colStrideIn,
          U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m128d, _arrSizeIn>& matDataIn)
{
    return Transpose<__m128d, _rows, _cols, _rowStart, _colStartIn, _unusedSetZero, _colStrideIn, _colStrideOut>(
            matDataIn);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _unusedSetZero, U32 _colStrideIn,
          U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m128, _arrSizeIn>& matDataIn)
{
    return Transpose<__m128, _rows, _cols, _rowStart, _colStartIn, _unusedSetZero, _colStrideIn, _colStrideOut>(
            matDataIn);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _unusedSetZero, U32 _colStrideIn,
          U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m256d, _arrSizeIn>& matDataIn)
{
    return Transpose<__m256d, _rows, _cols, _rowStart, _colStartIn, _unusedSetZero, _colStrideIn, _colStrideOut>(
            matDataIn);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _unusedSetZero, U32 _colStrideIn,
          U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m256, _arrSizeIn>& matDataIn)
{
    return Transpose<__m256, _rows, _cols, _rowStart, _colStartIn, _unusedSetZero, _colStrideIn, _colStrideOut>(
            matDataIn);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _overwriteUnused,
          bool _unusedSetZero, U32 _colStrideIn, U32 _colStartOut, U32 _colStrideOut, std::size_t _arrSizeIn,
          std::size_t _arrSizeOut>
inline void Transpose(const std::array<_registerType, _arrSizeIn>& matDataIn,
                      std::array<_registerType, _arrSizeOut>& matDataOut)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    static_assert(_rowStart + _rows <= numRegVals, "Submatrix exceeds register size.");
    static_assert(_colStartIn + _colStrideIn * (_cols - 1) + 1 <= _arrSizeIn, "Input submatrix exceeds array size.");
    static_assert(_colStartOut + _colStrideOut * (_rows - 1) + 1 <= _arrSizeOut,
                  "Output submatrix exceeds array size.");

    if constexpr (_rows == 2 && _cols == 2)
        Transpose2x2<_rowStart, _overwriteUnused, _unusedSetZero>(
                matDataIn[0 + _colStartIn], matDataIn[1 * _colStrideIn + _colStartIn], matDataOut[0 + _colStartOut],
                matDataOut[1 * _colStrideOut + _colStartOut]);
    else if constexpr (_rows == 2 && _cols == 1)
        Transpose2x1(matDataIn[0 + _colStartIn], matDataOut[0], matDataOut[1]);
    else if constexpr (_rows == 1 && _cols == 2)
        Transpose1x2(matDataIn[0], matDataIn[1], matDataOut[0 + _colStartOut]);
    else
        Transpose1x1<_rowStart, _overwriteUnused, _unusedSetZero>(matDataIn[0 + _colStartIn],
                                                                  matDataOut[0 + _colStartOut]);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _overwriteUnused, bool _unusedSetZero,
          U32 _colStrideIn, U32 _colStartOut, U32 _colStrideOut, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m128d, _arrSizeIn>& matDataIn, std::array<__m128d, _arrSizeOut>& matDataOut)
{
    Transpose<__m128d, _rows, _cols, _rowStart, _colStartIn, _overwriteUnused, _unusedSetZero, _colStrideIn,
              _colStartOut, _colStrideOut>(matDataIn, matDataOut);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _overwriteUnused, bool _unusedSetZero,
          U32 _colStrideIn, U32 _colStartOut, U32 _colStrideOut, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m128, _arrSizeIn>& matDataIn, std::array<__m128, _arrSizeOut>& matDataOut)
{
    Transpose<__m128, _rows, _cols, _rowStart, _colStartIn, _overwriteUnused, _unusedSetZero, _colStrideIn,
              _colStartOut, _colStrideOut>(matDataIn, matDataOut);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _overwriteUnused, bool _unusedSetZero,
          U32 _colStrideIn, U32 _colStartOut, U32 _colStrideOut, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m256d, _arrSizeIn>& matDataIn, std::array<__m256d, _arrSizeOut>& matDataOut)
{
    Transpose<__m256d, _rows, _cols, _rowStart, _colStartIn, _overwriteUnused, _unusedSetZero, _colStrideIn,
              _colStartOut, _colStrideOut>(matDataIn, matDataOut);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _rowStart, U32 _colStartIn, bool _overwriteUnused, bool _unusedSetZero,
          U32 _colStrideIn, U32 _colStartOut, U32 _colStrideOut, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m256, _arrSizeIn>& matDataIn, std::array<__m256, _arrSizeOut>& matDataOut)
{
    Transpose<__m256, _rows, _cols, _rowStart, _colStartIn, _overwriteUnused, _unusedSetZero, _colStrideIn,
              _colStartOut, _colStrideOut>(matDataIn, matDataOut);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rowStart, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void Transpose1x1(_registerType in, _registerType& out)
{
    static_assert(not(_overwriteUnused == false && _unusedSetZero == true), "Option _unusedSetZero has no effect.");

    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
            out = BlendIndex<_rowStart>(_mm_setzero<_registerType>(), in);
        else
            out = in;
    else
        out = BlendIndex<_rowStart>(out, in);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rowStart, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void Transpose2x2(_registerType in0, _registerType in1, _registerType& out0, _registerType& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;
    constexpr U32 numRegVals = numRegisterValues<_registerType>;
    constexpr U32 laneOffset = _rowStart % numLaneVals;

    _registerType tmp0, tmp1;


    // Determine transposed for doupble precision registers
    if constexpr (numLaneVals == 2)
    {
        if constexpr (laneOffset == 0)
        {
            tmp0 = _mm_unpacklo(in0, in1);
            tmp1 = _mm_unpackhi(in0, in1);
        }
        else
        {
            _registerType perm = Permute2F128<0, 1, 1, 0>(in0, in1);
            tmp0 = Shuffle<1, 1, 1, 1>(perm, in0);
            tmp1 = Shuffle<0, 0, 0, 0>(in1, perm);
        }
    }

    // Determine transposed for single precision registers
    else if constexpr (numLaneVals == 4)
    {
        if constexpr (laneOffset == 0)
        {
            tmp0 = _mm_unpacklo(in0, in1);
            tmp1 = Permute<2, 3, 0, 1>(tmp0);
        }
        else if constexpr (laneOffset == 1)
        {
            tmp0 = Shuffle<0, 1, 1, 0>(in0, in1);
            tmp1 = Shuffle<0, 2, 2, 0>(in0, in1);
        }
        else if constexpr (laneOffset == 2)
        {
            tmp1 = _mm_unpackhi(in0, in1);
            tmp0 = Permute<2, 3, 0, 1>(tmp1);
        }
        else
        {
            _registerType perm = Permute2F128<0, 1, 1, 0>(in0, in1);
            tmp0 = Shuffle<3, 3, 3, 3>(perm, in0);
            tmp1 = Shuffle<0, 0, 0, 0>(in1, perm);
        }
    }


    // Write to output registers
    if constexpr (numRegVals == 2 || _overwriteUnused)
    {
        if constexpr (_unusedSetZero && numRegVals > 2)
        {
            const _registerType zero = _mm_setzero<_registerType>();
            out0 = BlendInRange<_rowStart, _rowStart + 1>(zero, tmp0);
            out1 = BlendInRange<_rowStart, _rowStart + 1>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_rowStart, _rowStart + 1>(out0, tmp0);
        out1 = BlendInRange<_rowStart, _rowStart + 1>(out1, tmp1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

inline void Transpose2x1(__m128d in0, __m128d& out0, __m128d& out1)
{
    Transpose2x2(in0, _mm_setzero_pd(), out0, out1);
}



// --------------------------------------------------------------------------------------------------------------------

inline void Transpose1x2(__m128d in0, __m128d in1, __m128d& out0)
{
    out0 = _mm_unpacklo_pd(in0, in1);
}



// --------------------------------------------------------------------------------------------------------------------

inline void Transpose4x4(__m128 in0, __m128 in1, __m128 in2, __m128 in3, __m128& out0, __m128& out1, __m128& out2,
                         __m128& out3)
{
    alignas(alignmentBytes<__m128>) __m128 tmp0, tmp1, tmp2, tmp3;

    tmp0 = _mm_unpacklo_ps(in0, in1);
    tmp1 = _mm_unpackhi_ps(in0, in1);
    tmp2 = _mm_unpacklo_ps(in2, in3);
    tmp3 = _mm_unpackhi_ps(in2, in3);

    out0 = _mm_movelh_ps(tmp0, tmp2);
    out1 = _mm_movehl_ps(tmp2, tmp0);
    out2 = _mm_movelh_ps(tmp1, tmp3);
    out3 = _mm_movehl_ps(tmp3, tmp1);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

inline void Transpose4x4(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1,
                         __m256d& out2, __m256d& out3)
{
    alignas(alignmentBytes<__m256d>) __m256d tmp0, tmp1, tmp2, tmp3;

    tmp0 = _mm256_unpacklo_pd(in0, in1);
    tmp1 = _mm256_unpackhi_pd(in0, in1);
    tmp2 = _mm256_unpacklo_pd(in2, in3);
    tmp3 = _mm256_unpackhi_pd(in2, in3);

    out0 = Permute2F128<0, 0, 1, 0>(tmp0, tmp2);
    out1 = Permute2F128<0, 0, 1, 0>(tmp1, tmp3);
    out2 = Permute2F128<0, 1, 1, 1>(tmp0, tmp2);
    out3 = Permute2F128<0, 1, 1, 1>(tmp1, tmp3);
}



// --------------------------------------------------------------------------------------------------------------------

inline void Transpose8x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6, __m256& out7)
{
    alignas(alignmentBytes<__m256>) __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmpBlend;

    out0 = _mm256_unpacklo_ps(in0, in1);
    out1 = _mm256_unpackhi_ps(in0, in1);
    out2 = _mm256_unpacklo_ps(in2, in3);
    out3 = _mm256_unpackhi_ps(in2, in3);
    out4 = _mm256_unpacklo_ps(in4, in5);
    out5 = _mm256_unpackhi_ps(in4, in5);
    out6 = _mm256_unpacklo_ps(in6, in7);
    out7 = _mm256_unpackhi_ps(in6, in7);

    //    tmp0 = Shuffle<0, 1, 0, 1>(out0, out2);
    //    tmp1 = Shuffle<2, 3, 2, 3>(out0, out2);
    //    tmp2 = Shuffle<0, 1, 0, 1>(out1, out3);
    //    tmp3 = Shuffle<2, 3, 2, 3>(out1, out3);
    //    tmp4 = Shuffle<0, 1, 0, 1>(out4, out6);
    //    tmp5 = Shuffle<2, 3, 2, 3>(out4, out6);
    //    tmp6 = Shuffle<0, 1, 0, 1>(out5, out7);
    //    tmp7 = Shuffle<2, 3, 2, 3>(out5, out7);

    // Faster than pure shuffle (Around 6-8%). Have a look at the link in the function description
    tmpBlend = Shuffle<2, 3, 0, 1>(out0, out2);
    tmp0 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out0, tmpBlend);
    tmp1 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out2, tmpBlend);
    tmpBlend = Shuffle<2, 3, 0, 1>(out1, out3);
    tmp2 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out1, tmpBlend);
    tmp3 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out3, tmpBlend);
    tmpBlend = Shuffle<2, 3, 0, 1>(out4, out6);
    tmp4 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out4, tmpBlend);
    tmp5 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out6, tmpBlend);
    tmpBlend = Shuffle<2, 3, 0, 1>(out5, out7);
    tmp6 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out5, tmpBlend);
    tmp7 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out7, tmpBlend);

    out0 = Permute2F128<0, 0, 1, 0>(tmp0, tmp4);
    out1 = Permute2F128<0, 0, 1, 0>(tmp1, tmp5);
    out2 = Permute2F128<0, 0, 1, 0>(tmp2, tmp6);
    out3 = Permute2F128<0, 0, 1, 0>(tmp3, tmp7);
    out4 = Permute2F128<0, 1, 1, 1>(tmp0, tmp4);
    out5 = Permute2F128<0, 1, 1, 1>(tmp1, tmp5);
    out6 = Permute2F128<0, 1, 1, 1>(tmp2, tmp6);
    out7 = Permute2F128<0, 1, 1, 1>(tmp3, tmp7);
}

#endif // __AVX2__

} // namespace GDL::simd
