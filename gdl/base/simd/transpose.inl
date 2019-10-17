#pragma once

#include "gdl/base/simd/transpose.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"



#include <iostream>

namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, U32 _firstRowOut,
          bool _unusedSetZero, U32 _colStrideIn, U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<_registerType, _arrSizeIn>& matDataI)
{
    std::array<_registerType, _colStrideOut*(_rows - 1) + 1> matDataO;
    Transpose<_rows, _cols, _firstRowIn, _firstColIn, true, _unusedSetZero, _colStrideIn, _firstRowOut, 0,
              _colStrideOut>(matDataI, matDataO);
    return matDataO;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, U32 _firstRowOut, bool _unusedSetZero,
          U32 _colStrideIn, U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m128d, _arrSizeIn>& matDataI)
{
    return Transpose<__m128d, _rows, _cols, _firstRowIn, _firstColIn, _firstRowOut, _unusedSetZero, _colStrideIn,
                     _colStrideOut>(matDataI);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, U32 _firstRowOut, bool _unusedSetZero,
          U32 _colStrideIn, U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m128, _arrSizeIn>& matDataI)
{
    return Transpose<__m128, _rows, _cols, _firstRowIn, _firstColIn, _firstRowOut, _unusedSetZero, _colStrideIn,
                     _colStrideOut>(matDataI);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, U32 _firstRowOut, bool _unusedSetZero,
          U32 _colStrideIn, U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m256d, _arrSizeIn>& matDataI)
{
    return Transpose<__m256d, _rows, _cols, _firstRowIn, _firstColIn, _firstRowOut, _unusedSetZero, _colStrideIn,
                     _colStrideOut>(matDataI);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, U32 _firstRowOut, bool _unusedSetZero,
          U32 _colStrideIn, U32 _colStrideOut, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m256, _arrSizeIn>& matDataI)
{
    return Transpose<__m256, _rows, _cols, _firstRowIn, _firstColIn, _firstRowOut, _unusedSetZero, _colStrideIn,
                     _colStrideOut>(matDataI);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, bool _overwriteUnused,
          bool _unusedSetZero, U32 _colStrideIn, U32 _firstRowOut, U32 _firstColOut, U32 _colStrideOut,
          std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<_registerType, _arrSizeIn>& matDataI,
                      std::array<_registerType, _arrSizeOut>& matDataO)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    static_assert(_firstRowIn + _rows <= numRegVals, "Input submatrix exceeds register size.");
    static_assert(_firstRowOut + _cols <= numRegVals, "Output submatrix exceeds register size.");
    static_assert(_firstColIn + _colStrideIn * (_cols - 1) + 1 <= _arrSizeIn, "Input submatrix exceeds array size.");
    static_assert(_firstColOut + _colStrideOut * (_rows - 1) + 1 <= _arrSizeOut,
                  "Output submatrix exceeds array size.");
    static_assert(_colStrideIn > 0 && _colStrideOut > 0, "Column strides must be larger than 0.");


    // Create index arrays
    constexpr auto funcInitIdxIn = []() {
        std::array<U32, _cols> idxIn{};
        for (U32 i = 0; i < idxIn.size(); ++i)
            idxIn[i] = i * _colStrideIn + _firstColIn;
        return idxIn;
    };
    constexpr auto funcInitIdxOut = []() {
        std::array<U32, _rows> idxOut{};
        for (U32 i = 0; i < idxOut.size(); ++i)
            idxOut[i] = i * _colStrideOut + _firstColOut;
        return idxOut;
    };

    constexpr std::array<U32, _cols> idxI = funcInitIdxIn();
    constexpr std::array<U32, _rows> idxO = funcInitIdxOut();


    // Select and execute correct transpose function
    if constexpr (_rows == 1)
    {
        if constexpr (_cols == 1)
            Transpose1x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(matDataI[idxI[0]],
                                                                                      matDataO[idxO[0]]);
        else if constexpr (_cols == 2)
            Transpose1x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataO[idxO[0]]);
    }
    else if constexpr (_rows == 2)
    {
        if constexpr (_cols == 1)
            Transpose2x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataO[idxO[0]], matDataO[idxO[1]]);
        else if constexpr (_cols == 2)
            Transpose2x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataO[idxO[0]], matDataO[idxO[1]]);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, bool _overwriteUnused, bool _unusedSetZero,
          U32 _colStrideIn, U32 _firstRowOut, U32 _firstColOut, U32 _colStrideOut, std::size_t _arrSizeIn,
          std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m128d, _arrSizeIn>& matDataI, std::array<__m128d, _arrSizeOut>& matDataO)
{
    Transpose<__m128d, _rows, _cols, _firstRowIn, _firstColIn, _overwriteUnused, _unusedSetZero, _colStrideIn,
              _firstRowOut, _firstColOut, _colStrideOut>(matDataI, matDataO);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, bool _overwriteUnused, bool _unusedSetZero,
          U32 _colStrideIn, U32 _firstRowOut, U32 _firstColOut, U32 _colStrideOut, std::size_t _arrSizeIn,
          std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m128, _arrSizeIn>& matDataI, std::array<__m128, _arrSizeOut>& matDataO)
{
    Transpose<__m128, _rows, _cols, _firstRowIn, _firstColIn, _overwriteUnused, _unusedSetZero, _colStrideIn,
              _firstRowOut, _firstColOut, _colStrideOut>(matDataI, matDataO);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, bool _overwriteUnused, bool _unusedSetZero,
          U32 _colStrideIn, U32 _firstRowOut, U32 _firstColOut, U32 _colStrideOut, std::size_t _arrSizeIn,
          std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m256d, _arrSizeIn>& matDataI, std::array<__m256d, _arrSizeOut>& matDataO)
{
    Transpose<__m256d, _rows, _cols, _firstRowIn, _firstColIn, _overwriteUnused, _unusedSetZero, _colStrideIn,
              _firstRowOut, _firstColOut, _colStrideOut>(matDataI, matDataO);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstColIn, bool _overwriteUnused, bool _unusedSetZero,
          U32 _colStrideIn, U32 _firstRowOut, U32 _firstColOut, U32 _colStrideOut, std::size_t _arrSizeIn,
          std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m256, _arrSizeIn>& matDataI, std::array<__m256, _arrSizeOut>& matDataO)
{
    Transpose<__m256, _rows, _cols, _firstRowIn, _firstColIn, _overwriteUnused, _unusedSetZero, _colStrideIn,
              _firstRowOut, _firstColOut, _colStrideOut>(matDataI, matDataO);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void Transpose1x1(_registerType in, _registerType& out)
{
    static_assert(not(_overwriteUnused == false && _unusedSetZero == true), "Option _unusedSetZero has no effect.");

    _registerType tmp;

    if constexpr (_firstRowIn == _firstRowOut)
    {
        tmp = in;
    }
    else
    {
        constexpr U32 numLaneVals = numValuesPerLane<_registerType>;
        constexpr U32 laneIn = _firstRowIn / numLaneVals;
        constexpr U32 laneOut = _firstRowOut / numLaneVals;
        constexpr U32 laneOffsetIn = _firstRowIn % numLaneVals;
        constexpr U32 laneOffsetOut = _firstRowOut % numLaneVals;

        if constexpr (numLaneVals == 2)
        {
            if constexpr (laneIn == laneOut)
            {
                if constexpr (laneOffsetIn == 0)
                    tmp = _mm_moveldup(in);
                else
                    tmp = _mm_unpackhi(in, in);
            }
            else
            {
                _registerType perm = Permute2F128<1, 0>(in);
                if constexpr (laneOffsetIn == laneOffsetOut)
                    tmp = perm;
                else if constexpr (laneOffsetIn == 0)
                    tmp = _mm_moveldup(perm);
                else
                    tmp = _mm_unpackhi(perm, perm);
            }
        }
        else
        {
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
                _registerType perm = Permute2F128<1, 0>(in);
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
    }


    // Write to output registers
    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
            out = BlendIndex<_firstRowOut>(_mm_setzero<_registerType>(), tmp);
        else
            out = tmp;
    else
        out = BlendIndex<_firstRowOut>(out, tmp);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void Transpose1x2(_registerType in0, _registerType in1, _registerType& out0)
{
    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;
    constexpr U32 numRegVals = numRegisterValues<_registerType>;
    constexpr U32 laneOffset = _firstRowIn % numLaneVals;

    _registerType tmp0;


    // Determine transposed without crossing lane boundaries
    if constexpr (laneOffset == 0)
        tmp0 = _mm_unpacklo(in0, in1);
    else if constexpr (laneOffset < numLaneVals - 1)
    {
        if constexpr (laneOffset == 1)
            tmp0 = Shuffle<0, 1, 1, 0>(in0, in1);
        else
        {
            _registerType tmp1 = _mm_unpackhi(in0, in1);
            tmp0 = Permute<2, 3, 0, 1>(tmp1);
        }
    }

    // Determine transposed accross lane boundaries
#ifdef __AVX2__
    else
    {
        _registerType perm = Permute2F128<0, 1, 1, 0>(in0, in1);
        tmp0 = Shuffle<laneOffset, laneOffset, laneOffset, laneOffset>(perm, in0);
    }
#endif //__AVX2__


    // Write to output registers
    if constexpr (numRegVals == 2 || _overwriteUnused)
    {
        if constexpr (_unusedSetZero && numRegVals > 2)
            out0 = BlendInRange<_firstRowIn, _firstRowIn + 1>(_mm_setzero<_registerType>(), tmp0);
        else
            out0 = tmp0;
    }
    else
        out0 = BlendInRange<_firstRowIn, _firstRowIn + 1>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void Transpose2x1(_registerType in0, _registerType& out0, _registerType& out1)
{
    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;
    constexpr U32 numRegVals = numRegisterValues<_registerType>;
    constexpr U32 laneOffset = _firstRowIn % numLaneVals;

    _registerType tmp0;

    // Determine transposed without crossing lane boundaries
    if constexpr (laneOffset < numLaneVals - 1)
    {
        if constexpr (numLaneVals == 2)
            tmp0 = _mm_unpackhi(in0, _mm_setzero<_registerType>());
        else if constexpr (laneOffset == 1)
            tmp0 = _mm_unpackhi(in0, in0);
        else
            tmp0 = _mm_movehdup(in0);
    }

    // Determine transposed accross lane boundaries
#ifdef __AVX2__
    else
    {
        _registerType perm = Permute2F128<1, 0>(in0);
        tmp0 = Broadcast<0>(perm);
    }
#endif //__AVX2__


    // Write to output registers
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const _registerType zero = _mm_setzero<_registerType>();
            out0 = BlendIndex<_firstRowIn>(zero, in0);
            if constexpr (numRegVals > 2)
                out1 = BlendIndex<_firstRowIn>(zero, tmp0);
            else
                out1 = tmp0;
        }
        else
        {
            out0 = in0;
            out1 = tmp0;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowIn>(out0, in0);
        out1 = BlendIndex<_firstRowIn>(out1, tmp0);
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(const __m128& in0, const __m128& in1, __m128& out0, __m128& out1)
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
            __m128 tmp2 = _mm_unpacklo(in0, in1);
            tmp0 = _mm_movehl_ps(tmp2, tmp2);
            tmp1 = _mm_unpackhi(in0, in1);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Shuffle<1, 1, 1, 1>(in0, in1);
            tmp1 = Shuffle<2, 2, 2, 2>(in0, in1);
        }
        else
        {
            __m128 tmp2 = _mm_unpackhi(in0, in1);
            tmp0 = _mm_unpacklo(in0, in1);
            tmp1 = _mm_movelh_ps(tmp2, tmp2);
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m128 zero = _mm_setzero<__m128>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(const __m128d& in0, const __m128d& in1, __m128d& out0, __m128d& out1)
{
    out0 = _mm_unpacklo(in0, in1);
    out1 = _mm_unpackhi(in0, in1);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(const __m256& in0, const __m256& in1, __m256& out0, __m256& out1)
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose2x2(const __m256d& in0, const __m256d& in1, __m256d& out0, __m256d& out1)
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256d zero = _mm_setzero<__m256d>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
    }
}

#endif // __AVX2__



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
