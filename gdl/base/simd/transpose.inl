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
        else if constexpr (_cols == 3)
            Transpose1x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataO[idxO[0]]);
        else if constexpr (_cols == 4)
            Transpose1x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataO[idxO[0]]);
        else if constexpr (_cols == 5)
            Transpose1x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataO[idxO[0]]);
        else if constexpr (_cols == 6)
            Transpose1x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataO[idxO[0]]);
    }
    else if constexpr (_rows == 2)
    {
        if constexpr (_cols == 1)
            Transpose2x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataO[idxO[0]], matDataO[idxO[1]]);
        else if constexpr (_cols == 2)
            Transpose2x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataO[idxO[0]], matDataO[idxO[1]]);
        else if constexpr (_cols == 3)
            Transpose2x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataO[idxO[0]], matDataO[idxO[1]]);
        else if constexpr (_cols == 4)
            Transpose2x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataO[idxO[0]],
                    matDataO[idxO[1]]);
        else if constexpr (_cols == 5)
            Transpose2x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataO[idxO[0]], matDataO[idxO[1]]);
        else if constexpr (_cols == 6)
            Transpose2x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataO[idxO[0]], matDataO[idxO[1]]);
    }
    else if constexpr (_rows == 3)
    {
        if constexpr (_cols == 1)
            Transpose3x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]]);
        else if constexpr (_cols == 2)
            Transpose3x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]]);
        else if constexpr (_cols == 3)
            Transpose3x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]]);
        else if constexpr (_cols == 4)
            Transpose3x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataO[idxO[0]],
                    matDataO[idxO[1]], matDataO[idxO[2]]);
        else if constexpr (_cols == 5)
            Transpose3x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]]);
        else if constexpr (_cols == 6)
            Transpose3x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]]);
    }
    else if constexpr (_rows == 4)
    {
        if constexpr (_cols == 1)
            Transpose4x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]]);
        if constexpr (_cols == 2)
            Transpose4x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]]);
        else if constexpr (_cols == 3)
            Transpose4x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]]);
        else if constexpr (_cols == 4)
            Transpose4x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataO[idxO[0]],
                    matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]]);
        else if constexpr (_cols == 5)
            Transpose4x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]]);
        else if constexpr (_cols == 6)
            Transpose4x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]]);
    }
    else if constexpr (_rows == 5)
    {
        if constexpr (_cols == 1)
            Transpose5x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]],
                    matDataO[idxO[4]]);
        else if constexpr (_cols == 2)
            Transpose5x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]], matDataO[idxO[4]]);
        else if constexpr (_cols == 3)
            Transpose5x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]]);
        else if constexpr (_cols == 4)
            Transpose5x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataO[idxO[0]],
                    matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]]);
        else if constexpr (_cols == 5)
            Transpose5x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]]);
        else if constexpr (_cols == 6)
            Transpose5x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]],
                    matDataO[idxO[4]]);
    }
    else if constexpr (_rows == 6)
    {

        if constexpr (_cols == 1)
            Transpose6x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]],
                    matDataO[idxO[4]], matDataO[idxO[5]]);
        else if constexpr (_cols == 2)
            Transpose6x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]]);
        else if constexpr (_cols == 3)
            Transpose6x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]]);
        else if constexpr (_cols == 4)
            Transpose6x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataO[idxO[0]],
                    matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]]);
        else if constexpr (_cols == 5)
            Transpose6x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]],
                    matDataO[idxO[5]]);
        else if constexpr (_cols == 6)
            Transpose6x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]],
                    matDataO[idxO[4]], matDataO[idxO[5]]);
        else if constexpr (_cols == 7)
            Transpose6x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]]);
    }
    else if constexpr (_rows == 7)
    {

        if constexpr (_cols == 1)
            Transpose7x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]],
                    matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]]);
        else if constexpr (_cols == 2)
            Transpose7x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]]);
        else if constexpr (_cols == 3)
            Transpose7x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]]);
        else if constexpr (_cols == 4)
            Transpose7x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataO[idxO[0]],
                    matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]],
                    matDataO[idxO[6]]);
        else if constexpr (_cols == 5)
            Transpose7x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]],
                    matDataO[idxO[5]], matDataO[idxO[6]]);
        else if constexpr (_cols == 6)
            Transpose7x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]],
                    matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]]);
        else if constexpr (_cols == 7)
            Transpose7x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]]);
    }
} // namespace GDL::simd



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
// 1x1
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
#ifdef __AVX2__
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
#endif //__AVX2__
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
#ifdef __AVX2__
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
#endif //__AVX2__
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
// 1x2
// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m128 in0, __m128 in1, __m128& out0)
{
    [[maybe_unused]] constexpr bool setZero = _overwriteUnused && _unusedSetZero;

    __m128 tmp0;


    if constexpr (_firstRowIn == 0)
    {
        if constexpr (_firstRowOut == 0)
        {
            if constexpr (setZero)
            {
                out0 = Insert<0, 1, false, false, true, true>(in1, in0);
                return;
            }
            else
                tmp0 = _mm_unpacklo(in0, in1);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Shuffle<0, 0, 0, 0>(in0, in1);
        }
        else
        {
            if constexpr (setZero)
            {
                __m128 tmp1 = Insert<0, 1, false, false, true, true>(in1, in0);
                out0 = Permute<2, 3, 0, 1>(tmp1);
                return;
            }
            else
            {
                __m128 tmp1 = _mm_unpacklo(in0, in1);
                tmp0 = _mm_movelh_ps(tmp1, tmp1);
            }
        }
    }

    else if constexpr (_firstRowIn == 1)
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = Insert<1, 0, false, false, true, true>(in0, in1);
            if constexpr (setZero)
            {
                out0 = tmp0;
                return;
            }
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Insert<1, 2, true, false, false, true>(in1, in0);
            if constexpr (setZero)
            {
                out0 = tmp0;
                return;
            }
        }
        else
        {
            tmp0 = _mm_unpacklo(in0, in1);
        }
    }
    else if constexpr (_firstRowIn == 2)
    {
        if constexpr (_firstRowOut == 0)
        {
            tmp0 = _mm_unpackhi(in0, in1);
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Insert<2, 1, true, false, false, true>(in0, in1);
            if constexpr (setZero)
            {
                out0 = tmp0;
                return;
            }
        }
        else
        {
            tmp0 = Insert<2, 3, true, true, false, false>(in1, in0);
            if constexpr (setZero)
            {
                out0 = tmp0;
                return;
            }
        }
    }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            if constexpr (setZero)
            {
                __m128 tmp1 = Insert<3, 2, true, true, false, false>(in0, in1);
                out0 = Permute<2, 3, 0, 1>(tmp1);
                return;
            }
            else
            {
                __m128 tmp1 = _mm_unpackhi(in0, in1);
                tmp0 = _mm_movehl_ps(tmp1, tmp1);
            }
        }
        else if constexpr (_firstRowOut == 1)
        {
            tmp0 = Shuffle<3, 3, 3, 3>(in0, in1);
        }
        else
        {
            if constexpr (setZero)
            {
                out0 = Insert<3, 2, true, true, false, false>(in0, in1);
                return;
            }
            else
                tmp0 = _mm_unpackhi(in0, in1);
        }
    }

    // Write to output registers
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(_mm_setzero<__m128>(), tmp0);
        else
            out0 = tmp0;
    }
    else
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose1x2(__m128d in0, __m128d in1, __m128d& out0)
{
    if constexpr (_firstRowIn == 0)
        out0 = _mm_unpacklo(in0, in1);
    else
        out0 = _mm_unpackhi(in0, in1);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(_mm_setzero<__m256>(), tmp0);
        else
            out0 = tmp0;
    }
    else
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
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
    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(_mm_setzero<__m256d>(), tmp0);
        else
            out0 = tmp0;
    else
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
}

#endif //__AVX2__



// --------------------------------------------------------------------------------------------------------------------
// 1x3
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

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(_mm_setzero<__m256>(), tmp0);
        else
            out0 = tmp0;
    else
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
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
    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(_mm_setzero<__m256d>(), tmp0);
        else
            out0 = tmp0;
    else
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
}

#endif //__AVX2__



// --------------------------------------------------------------------------------------------------------------------
// 1x4
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

#ifdef __AVX2__


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
    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(_mm_setzero<__m256>(), tmp0);
        else
            out0 = tmp0;
    else
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out0, tmp0);
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
// 1x5
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
    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(_mm_setzero<__m256>(), tmp0);
        else
            out0 = tmp0;
    else
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out0, tmp0);
}



// --------------------------------------------------------------------------------------------------------------------
// 1x6
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
    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(_mm_setzero<__m256>(), tmp0);
        else
            out0 = tmp0;
    else
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out0, tmp0);
}



#endif //__AVX2__



// --------------------------------------------------------------------------------------------------------------------
// 2x1
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
inline void Transpose2x1(__m128d in0, __m128d& out0, __m128d& out1)
{

    if constexpr (_overwriteUnused)
        if constexpr (_unusedSetZero)
        {
            __m128d zero = _mm_setzero<__m128d>();
            if constexpr (_firstRowOut == 0)
            {
                out0 = BlendIndex<_firstRowOut>(zero, in0);
                out1 = _mm_unpackhi(in0, zero);
            }
            else
            {
                out0 = _mm_unpacklo(zero, in0);
                out1 = BlendIndex<_firstRowOut>(zero, in0);
            }
        }
        else
        {
            if constexpr (_firstRowOut == 0)
            {
                out0 = in0;
                out1 = _mm_unpackhi(in0, in0);
            }
            else
            {
                out0 = _mm_unpacklo(in0, in0);
                out1 = in0;
            }
        }
    else
    {
        if constexpr (_firstRowOut == 0)
        {
            out0 = BlendIndex<_firstRowOut>(out0, in0);
            out1 = _mm_unpackhi(in0, out1);
        }
        else
        {
            out0 = _mm_unpacklo(out0, in0);
            out1 = BlendIndex<_firstRowOut>(out1, in0);
        }
    }
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendIndex<_firstRowOut>(zero, tmp0);
            out1 = BlendIndex<_firstRowOut>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256d zero = _mm_setzero<__m256d>();
            out0 = BlendIndex<_firstRowOut>(zero, tmp0);
            out1 = BlendIndex<_firstRowOut>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
    }
}

#endif // __AVX__



// --------------------------------------------------------------------------------------------------------------------
// 2x2
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
inline void Transpose2x2(__m128d in0, __m128d in1, __m128d& out0, __m128d& out1)
{
    out0 = _mm_unpacklo(in0, in1);
    out1 = _mm_unpackhi(in0, in1);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

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
// 2x3
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m128 zero = _mm_setzero<__m128>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
    }
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256d zero = _mm_setzero<__m256d>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
    }
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------
// 2x4
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

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out1, tmp1);
    }
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
// 2x5
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out1, tmp1);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 2x6
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp1);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out1, tmp1);
    }
}



#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------
// 3x1
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendIndex<_firstRowOut>(zero, tmp0);
            out1 = BlendIndex<_firstRowOut>(zero, tmp1);
            out2 = BlendIndex<_firstRowOut>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
        out2 = BlendIndex<_firstRowOut>(out2, tmp2);
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256d zero = _mm_setzero<__m256d>();
            out0 = BlendIndex<_firstRowOut>(zero, tmp0);
            out1 = BlendIndex<_firstRowOut>(zero, tmp1);
            out2 = BlendIndex<_firstRowOut>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
        out2 = BlendIndex<_firstRowOut>(out2, tmp2);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 3x2
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m128 zero = _mm_setzero<__m128>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out2, tmp2);
    }
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out2, tmp2);
    }
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256d zero = _mm_setzero<__m256d>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out2, tmp2);
    }
}

#endif //__AVX2__



// --------------------------------------------------------------------------------------------------------------------
// 3x3
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m128 zero = _mm_setzero<__m128>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out2, tmp2);
    }
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out2, tmp2);
    }
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256d zero = _mm_setzero<__m256d>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out2, tmp2);
    }
}



#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------
// 3x4
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

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out2, tmp2);
    }
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
// 3x5
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out2, tmp2);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 3x6
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp2);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out2, tmp2);
    }
}

#endif // __AVX2__


// --------------------------------------------------------------------------------------------------------------------
// 4x1
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

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendIndex<_firstRowOut>(zero, tmp0);
            out1 = BlendIndex<_firstRowOut>(zero, tmp1);
            out2 = BlendIndex<_firstRowOut>(zero, tmp2);
            out3 = BlendIndex<_firstRowOut>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
        out2 = BlendIndex<_firstRowOut>(out2, tmp2);
        out3 = BlendIndex<_firstRowOut>(out3, tmp3);
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256d zero = _mm_setzero<__m256d>();
            out0 = BlendIndex<_firstRowOut>(zero, tmp0);
            out1 = BlendIndex<_firstRowOut>(zero, tmp1);
            out2 = BlendIndex<_firstRowOut>(zero, tmp2);
            out3 = BlendIndex<_firstRowOut>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
        out2 = BlendIndex<_firstRowOut>(out2, tmp2);
        out3 = BlendIndex<_firstRowOut>(out3, tmp3);
    }
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------
// 4x2
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
    if constexpr (_overwriteUnused)
    {

        out0 = tmp0;
        out1 = tmp1;
        out2 = tmp2;
        out3 = tmp3;
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out3, tmp3);
    }
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out3, tmp3);
    }
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256d zero = _mm_setzero<__m256d>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out3, tmp3);
    }
}

#endif // __AVX2__


// --------------------------------------------------------------------------------------------------------------------
// 4x3
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m128 zero = _mm_setzero<__m128>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out3, tmp3);
    }
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out3, tmp3);
    }
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256d zero = _mm_setzero<__m256d>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out3, tmp3);
    }
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------
// 4x4
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



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out3, tmp3);
    }
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



// --------------------------------------------------------------------------------------------------------------------
// 4x5
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out3, tmp3);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 4x6
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp3);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out3, tmp3);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 5x1
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendIndex<_firstRowOut>(zero, tmp0);
            out1 = BlendIndex<_firstRowOut>(zero, tmp1);
            out2 = BlendIndex<_firstRowOut>(zero, tmp2);
            out3 = BlendIndex<_firstRowOut>(zero, tmp3);
            out4 = BlendIndex<_firstRowOut>(zero, tmp4);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
        out2 = BlendIndex<_firstRowOut>(out2, tmp2);
        out3 = BlendIndex<_firstRowOut>(out3, tmp3);
        out4 = BlendIndex<_firstRowOut>(out4, tmp4);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 5x2
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp4);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out4, tmp4);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 5x3
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp4);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out4, tmp4);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 5x4
// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose5x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4;

    Transpose4x4<_firstRowIn, _firstRowOut>(in0, in1, in2, in3, tmp0, tmp1, tmp2, tmp3);

    tmp4 = Permute2F128<1, 0>(tmp0);

    // Write to output registers
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp4);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out4, tmp4);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 5x5
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp4);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out4, tmp4);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 5x6
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp4);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out4, tmp4);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 6x1
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendIndex<_firstRowOut>(zero, tmp0);
            out1 = BlendIndex<_firstRowOut>(zero, tmp1);
            out2 = BlendIndex<_firstRowOut>(zero, tmp2);
            out3 = BlendIndex<_firstRowOut>(zero, tmp3);
            out4 = BlendIndex<_firstRowOut>(zero, tmp4);
            out5 = BlendIndex<_firstRowOut>(zero, tmp5);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
        out2 = BlendIndex<_firstRowOut>(out2, tmp2);
        out3 = BlendIndex<_firstRowOut>(out3, tmp3);
        out4 = BlendIndex<_firstRowOut>(out4, tmp4);
        out5 = BlendIndex<_firstRowOut>(out5, tmp5);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 6x2
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp5);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out5, tmp5);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 6x3
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp5);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out5, tmp5);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 6x4
// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    if constexpr (_firstRowOut == 0)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x4(in0, in1, in2, in3, tmp0, tmp1, tmp2, tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
        }
        else if constexpr (_firstRowIn == 1)
        {
            Transpose4x4(in0, in1, in2, in3, tmp3, tmp0, tmp1, tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
        }
        else
        {
            Transpose4x4(in0, in1, in2, in3, tmp2, tmp3, tmp0, tmp1);
            tmp2 = Permute2F128<1, 0>(tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
        }
    }
    else if constexpr (_firstRowOut == 4)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x4(in0, in1, in2, in3, tmp4, tmp5, tmp2, tmp3);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
        }
        else if constexpr (_firstRowIn == 1)
        {
            Transpose4x4(in0, in1, in2, in3, tmp3, tmp4, tmp5, tmp2);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp2);
        }
        else
        {
            Transpose4x4(in0, in1, in2, in3, tmp2, tmp3, tmp4, tmp5);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
        }
    }
    else
    {
        __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;

        if constexpr (_firstRowOut == 1)
        {
            tmp6 = Permute2F128<0, 1, 1, 0>(in0, in3);
            tmp7 = in0;
            tmp8 = in1;
            tmp9 = in2;

            tmp10 = in3;
            tmp11 = tmp6;
            tmp12 = Permute2F128<1, 0>(in1);
            tmp13 = Permute2F128<1, 0>(in2);
        }
        else if constexpr (_firstRowOut == 2)
        {
            tmp6 = Permute2F128<1, 0>(in2);
            tmp7 = Permute2F128<0, 1, 1, 0>(in0, in3);
            tmp8 = in0;
            tmp9 = in1;

            tmp10 = in2;
            tmp11 = in3;
            tmp12 = tmp7;
            tmp13 = Permute2F128<1, 0>(in1);
        }
        else
        {
            tmp6 = Permute2F128<1, 0>(in1);
            tmp7 = Permute2F128<1, 0>(in2);
            tmp8 = Permute2F128<0, 1, 1, 0>(in0, in3);
            tmp9 = in0;

            tmp10 = in1;
            tmp11 = in2;
            tmp12 = in3;
            tmp13 = tmp8;
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp5);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out5, tmp5);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 6x5
// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;

    if constexpr (_firstRowOut == 0)
    {
        tmp6 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp7 = in1;
        tmp8 = in2;
        tmp9 = in3;

        tmp10 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp11 = Permute2F128<1, 0>(in1);
        tmp12 = Permute2F128<1, 0>(in2);
        tmp13 = Permute2F128<1, 0>(in3);
    }
    else if constexpr (_firstRowOut == 1)
    {
        tmp6 = Permute2F128<1, 0>(in3);
        tmp7 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp8 = in1;
        tmp9 = in2;

        tmp10 = in3;
        tmp11 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp12 = Permute2F128<1, 0>(in1);
        tmp13 = Permute2F128<1, 0>(in2);
    }
    else if constexpr (_firstRowOut == 2)
    {
        tmp6 = Permute2F128<1, 0>(in2);
        tmp7 = Permute2F128<1, 0>(in3);
        tmp8 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp9 = in1;

        tmp10 = in2;
        tmp11 = in3;
        tmp12 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp13 = Permute2F128<1, 0>(in1);
    }
    else
    {
        tmp6 = Permute2F128<1, 0>(in1);
        tmp7 = Permute2F128<1, 0>(in2);
        tmp8 = Permute2F128<1, 0>(in3);
        tmp9 = Permute2F128<0, 0, 1, 0>(in0, in4);

        tmp10 = in1;
        tmp11 = in2;
        tmp12 = in3;
        tmp13 = Permute2F128<0, 1, 1, 1>(in0, in4);
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



    // Write to output registers
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp5);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out5, tmp5);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 6x6
// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose6x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    __m256 tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13;

    if constexpr (_firstRowOut == 0)
    {
        tmp6 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp7 = Permute2F128<0, 0, 1, 0>(in1, in5);
        tmp8 = in2;
        tmp9 = in3;

        tmp10 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp11 = Permute2F128<0, 1, 1, 1>(in1, in5);
        tmp12 = Permute2F128<1, 0>(in2);
        tmp13 = Permute2F128<1, 0>(in3);
    }
    else if constexpr (_firstRowOut == 1)
    {
        tmp6 = Permute2F128<1, 0>(in3);
        tmp7 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp8 = Permute2F128<0, 0, 1, 0>(in1, in5);
        tmp9 = in2;

        tmp10 = in3;
        tmp11 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp12 = Permute2F128<0, 1, 1, 1>(in1, in5);
        tmp13 = Permute2F128<1, 0>(in2);
    }
    else
    {
        tmp6 = Permute2F128<1, 0>(in2);
        tmp7 = Permute2F128<1, 0>(in3);
        tmp8 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp9 = Permute2F128<0, 0, 1, 0>(in1, in5);

        tmp10 = in2;
        tmp11 = in3;
        tmp12 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp13 = Permute2F128<0, 1, 1, 1>(in1, in5);
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



    // Write to output registers
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp5);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out5, tmp5);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 6x7
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
    else if constexpr (_firstRowIn == 2)
    {
        tmp0 = Permute2F128<0, 0, 1, 0>(tmp8, tmp12);
        tmp1 = Permute2F128<0, 0, 1, 0>(tmp9, tmp13);
        tmp2 = Permute2F128<0, 1, 1, 1>(tmp6, tmp10);
        tmp3 = Permute2F128<0, 1, 1, 1>(tmp7, tmp11);
        tmp4 = Permute2F128<0, 1, 1, 1>(tmp8, tmp12);
        tmp5 = Permute2F128<0, 1, 1, 1>(tmp9, tmp13);
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp5);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out5, tmp5);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 7x1
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendIndex<_firstRowOut>(zero, tmp0);
            out1 = BlendIndex<_firstRowOut>(zero, tmp1);
            out2 = BlendIndex<_firstRowOut>(zero, tmp2);
            out3 = BlendIndex<_firstRowOut>(zero, tmp3);
            out4 = BlendIndex<_firstRowOut>(zero, tmp4);
            out5 = BlendIndex<_firstRowOut>(zero, tmp5);
            out6 = BlendIndex<_firstRowOut>(zero, tmp6);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
            out6 = tmp6;
        }
    }
    else
    {
        out0 = BlendIndex<_firstRowOut>(out0, tmp0);
        out1 = BlendIndex<_firstRowOut>(out1, tmp1);
        out2 = BlendIndex<_firstRowOut>(out2, tmp2);
        out3 = BlendIndex<_firstRowOut>(out3, tmp3);
        out4 = BlendIndex<_firstRowOut>(out4, tmp4);
        out5 = BlendIndex<_firstRowOut>(out5, tmp5);
        out6 = BlendIndex<_firstRowOut>(out6, tmp6);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 7x2
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp5);
            out6 = BlendInRange<_firstRowOut, _firstRowOut + 1>(zero, tmp6);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
            out6 = tmp6;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out5, tmp5);
        out6 = BlendInRange<_firstRowOut, _firstRowOut + 1>(out6, tmp6);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 7x3
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp5);
            out6 = BlendInRange<_firstRowOut, _firstRowOut + 2>(zero, tmp6);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
            out6 = tmp6;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out5, tmp5);
        out6 = BlendInRange<_firstRowOut, _firstRowOut + 2>(out6, tmp6);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 7x4
// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x4(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256& out0, __m256& out1, __m256& out2,
                         __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;


    if constexpr (_firstRowOut == 0)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x4(in0, in1, in2, in3, tmp0, tmp1, tmp2, tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
            tmp6 = Permute2F128<1, 0>(tmp2);
        }
        else
        {
            Transpose4x4(in0, in1, in2, in3, tmp3, tmp0, tmp1, tmp2);
            tmp3 = Permute2F128<1, 0>(tmp3);
            tmp4 = Permute2F128<1, 0>(tmp0);
            tmp5 = Permute2F128<1, 0>(tmp1);
            tmp6 = Permute2F128<1, 0>(tmp2);
        }
    }
    else if constexpr (_firstRowOut == 4)
    {
        if constexpr (_firstRowIn == 0)
        {
            Transpose4x4(in0, in1, in2, in3, tmp4, tmp5, tmp6, tmp3);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp6);
            tmp3 = Permute2F128<1, 0>(tmp3);
        }
        else
        {
            Transpose4x4(in0, in1, in2, in3, tmp3, tmp4, tmp5, tmp6);
            tmp0 = Permute2F128<1, 0>(tmp4);
            tmp1 = Permute2F128<1, 0>(tmp5);
            tmp2 = Permute2F128<1, 0>(tmp6);
        }
    }
    else
    {
        __m256 tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14;

        if constexpr (_firstRowOut == 1)
        {
            tmp7 = Permute2F128<0, 1, 1, 0>(in0, in3);
            tmp8 = in0;
            tmp9 = in1;
            tmp10 = in2;

            tmp11 = in3;
            tmp12 = tmp7;
            tmp13 = Permute2F128<1, 0>(in1);
            tmp14 = Permute2F128<1, 0>(in2);
        }
        else if constexpr (_firstRowOut == 2)
        {
            tmp7 = Permute2F128<1, 0>(in2);
            tmp8 = Permute2F128<0, 1, 1, 0>(in0, in3);
            tmp9 = in0;
            tmp10 = in1;

            tmp11 = in2;
            tmp12 = in3;
            tmp13 = tmp8;
            tmp14 = Permute2F128<1, 0>(in1);
        }
        else
        {
            tmp7 = Permute2F128<1, 0>(in1);
            tmp8 = Permute2F128<1, 0>(in2);
            tmp9 = Permute2F128<0, 1, 1, 0>(in0, in3);
            tmp10 = in0;

            tmp11 = in1;
            tmp12 = in2;
            tmp13 = in3;
            tmp14 = tmp9;
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
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp5);
            out6 = BlendInRange<_firstRowOut, _firstRowOut + 3>(zero, tmp6);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
            out6 = tmp6;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out5, tmp5);
        out6 = BlendInRange<_firstRowOut, _firstRowOut + 3>(out6, tmp6);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 7x5
// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x5(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256& out0, __m256& out1,
                         __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;

    __m256 tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14;

    if constexpr (_firstRowOut == 0)
    {
        tmp7 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp8 = in1;
        tmp9 = in2;
        tmp10 = in3;

        tmp11 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp12 = Permute2F128<1, 0>(in1);
        tmp13 = Permute2F128<1, 0>(in2);
        tmp14 = Permute2F128<1, 0>(in3);
    }
    else if constexpr (_firstRowOut == 1)
    {
        tmp7 = Permute2F128<1, 0>(in3);
        tmp8 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp9 = in1;
        tmp10 = in2;

        tmp11 = in3;
        tmp12 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp13 = Permute2F128<1, 0>(in1);
        tmp14 = Permute2F128<1, 0>(in2);
    }
    else if constexpr (_firstRowOut == 2)
    {
        tmp7 = Permute2F128<1, 0>(in2);
        tmp8 = Permute2F128<1, 0>(in3);
        tmp9 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp10 = in1;

        tmp11 = in2;
        tmp12 = in3;
        tmp13 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp14 = Permute2F128<1, 0>(in1);
    }
    else
    {
        tmp7 = Permute2F128<1, 0>(in1);
        tmp8 = Permute2F128<1, 0>(in2);
        tmp9 = Permute2F128<1, 0>(in3);
        tmp10 = Permute2F128<0, 0, 1, 0>(in0, in4);

        tmp11 = in1;
        tmp12 = in2;
        tmp13 = in3;
        tmp14 = Permute2F128<0, 1, 1, 1>(in0, in4);
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

    // Write to output registers
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp5);
            out6 = BlendInRange<_firstRowOut, _firstRowOut + 4>(zero, tmp6);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
            out6 = tmp6;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out5, tmp5);
        out6 = BlendInRange<_firstRowOut, _firstRowOut + 4>(out6, tmp6);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 7x6
// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x6(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256& out0,
                         __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;

    __m256 tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14;

    if constexpr (_firstRowOut == 0)
    {
        tmp7 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp8 = Permute2F128<0, 0, 1, 0>(in1, in5);
        tmp9 = in2;
        tmp10 = in3;

        tmp11 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp12 = Permute2F128<0, 1, 1, 1>(in1, in5);
        tmp13 = Permute2F128<1, 0>(in2);
        tmp14 = Permute2F128<1, 0>(in3);
    }
    else if constexpr (_firstRowOut == 1)
    {
        tmp7 = Permute2F128<1, 0>(in3);
        tmp8 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp9 = Permute2F128<0, 0, 1, 0>(in1, in5);
        tmp10 = in2;

        tmp11 = in3;
        tmp12 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp13 = Permute2F128<0, 1, 1, 1>(in1, in5);
        tmp14 = Permute2F128<1, 0>(in2);
    }
    else
    {
        tmp7 = Permute2F128<1, 0>(in2);
        tmp8 = Permute2F128<1, 0>(in3);
        tmp9 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp10 = Permute2F128<0, 0, 1, 0>(in1, in5);

        tmp11 = in2;
        tmp12 = in3;
        tmp13 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp14 = Permute2F128<0, 1, 1, 1>(in1, in5);
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

    // Write to output registers
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp5);
            out6 = BlendInRange<_firstRowOut, _firstRowOut + 5>(zero, tmp6);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
            out6 = tmp6;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out5, tmp5);
        out6 = BlendInRange<_firstRowOut, _firstRowOut + 5>(out6, tmp6);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 7x7
// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero>
inline void Transpose7x7(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6)
{
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;

    __m256 tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14;

    if constexpr (_firstRowOut == 0)
    {
        tmp7 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp8 = Permute2F128<0, 0, 1, 0>(in1, in5);
        tmp9 = Permute2F128<0, 0, 1, 0>(in2, in6);
        tmp10 = in3;

        tmp11 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp12 = Permute2F128<0, 1, 1, 1>(in1, in5);
        tmp13 = Permute2F128<0, 1, 1, 1>(in2, in6);
        tmp14 = Permute2F128<1, 0>(in3);
    }
    else
    {
        tmp7 = Permute2F128<1, 0>(in3);
        tmp8 = Permute2F128<0, 0, 1, 0>(in0, in4);
        tmp9 = Permute2F128<0, 0, 1, 0>(in1, in5);
        tmp10 = Permute2F128<0, 0, 1, 0>(in2, in6);

        tmp11 = in3;
        tmp12 = Permute2F128<0, 1, 1, 1>(in0, in4);
        tmp13 = Permute2F128<0, 1, 1, 1>(in1, in5);
        tmp14 = Permute2F128<0, 1, 1, 1>(in2, in6);
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



    // Write to output registers
    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            const __m256 zero = _mm_setzero<__m256>();
            out0 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp0);
            out1 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp1);
            out2 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp2);
            out3 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp3);
            out4 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp4);
            out5 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp5);
            out6 = BlendInRange<_firstRowOut, _firstRowOut + 6>(zero, tmp6);
        }
        else
        {
            out0 = tmp0;
            out1 = tmp1;
            out2 = tmp2;
            out3 = tmp3;
            out4 = tmp4;
            out5 = tmp5;
            out6 = tmp6;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out0, tmp0);
        out1 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out1, tmp1);
        out2 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out2, tmp2);
        out3 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out3, tmp3);
        out4 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out4, tmp4);
        out5 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out5, tmp5);
        out6 = BlendInRange<_firstRowOut, _firstRowOut + 6>(out6, tmp6);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// 8x8
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
