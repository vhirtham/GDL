#pragma once

#include "gdl/base/simd/transpose.h"

#include "gdl/base/simd/_transpose/transpose_m128.h"
#include "gdl/base/simd/_transpose/transpose_m128d.h"
#include "gdl/base/simd/_transpose/transpose_m256.h"
#include "gdl/base/simd/_transpose/transpose_m256d.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"



#include <iostream>

namespace GDL::simd
{



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, U32 _firstColIn, U32 _firstColOut, U32 _colStrideIn,
          U32 _colStrideOut, bool _overwriteUnused, bool _unusedSetZero, typename _registerType, UST _arrSizeIn,
          UST _arrSizeOut>
inline void Transpose(const std::array<_registerType, _arrSizeIn>& matDataI,
                      std::array<_registerType, _arrSizeOut>& matDataO) noexcept
{
    static_assert(_firstColIn + _colStrideIn * (_cols - 1) + 1 <= _arrSizeIn, "Input submatrix exceeds array size.");
    static_assert(_firstColOut + _colStrideOut * (_rows - 1) + 1 <= _arrSizeOut,
                  "Output submatrix exceeds array size.");
    static_assert(_colStrideIn > 0 && _colStrideOut > 0, "Column stride template parameters must be larger than 0.");
    static_assert(_rows > 0 && _cols > 0, "Columns and rows must be larger than 0.");


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
        else if constexpr (_cols == 7)
            Transpose1x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataO[idxO[0]]);
        else if constexpr (_cols == 8)
            Transpose1x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataI[idxI[7]], matDataO[idxO[0]]);
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
        else if constexpr (_cols == 7)
            Transpose2x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataO[idxO[0]], matDataO[idxO[1]]);
        else if constexpr (_cols == 8)
            Transpose2x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataI[idxI[7]], matDataO[idxO[0]], matDataO[idxO[1]]);
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
        else if constexpr (_cols == 7)
            Transpose3x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]]);
        else if constexpr (_cols == 8)
            Transpose3x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataI[idxI[7]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]]);
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
        else if constexpr (_cols == 7)
            Transpose4x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]]);
        else if constexpr (_cols == 8)
            Transpose4x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataI[idxI[7]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]]);
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
        else if constexpr (_cols == 7)
            Transpose5x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]], matDataO[idxO[4]]);
        else if constexpr (_cols == 8)
            Transpose5x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataI[idxI[7]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]]);
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
        else if constexpr (_cols == 8)
            Transpose6x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataI[idxI[7]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]]);
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
        else if constexpr (_cols == 8)
            Transpose7x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataI[idxI[7]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]]);
    }
    else if constexpr (_rows == 8)
    {
        if constexpr (_cols == 1)
            Transpose8x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]],
                    matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]], matDataO[idxO[7]]);
        else if constexpr (_cols == 2)
            Transpose8x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]], matDataO[idxO[7]]);
        else if constexpr (_cols == 3)
            Transpose8x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]],
                    matDataO[idxO[7]]);
        else if constexpr (_cols == 4)
            Transpose8x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataO[idxO[0]],
                    matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]],
                    matDataO[idxO[6]], matDataO[idxO[7]]);
        else if constexpr (_cols == 5)
            Transpose8x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]],
                    matDataO[idxO[5]], matDataO[idxO[6]], matDataO[idxO[7]]);
        else if constexpr (_cols == 6)
            Transpose8x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]], matDataO[idxO[3]],
                    matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]], matDataO[idxO[7]]);
        else if constexpr (_cols == 7)
            Transpose8x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataO[idxO[0]], matDataO[idxO[1]], matDataO[idxO[2]],
                    matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]], matDataO[idxO[7]]);
        else if constexpr (_cols == 8)
            Transpose8x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    matDataI[idxI[0]], matDataI[idxI[1]], matDataI[idxI[2]], matDataI[idxI[3]], matDataI[idxI[4]],
                    matDataI[idxI[5]], matDataI[idxI[6]], matDataI[idxI[7]], matDataO[idxO[0]], matDataO[idxO[1]],
                    matDataO[idxO[2]], matDataO[idxO[3]], matDataO[idxO[4]], matDataO[idxO[5]], matDataO[idxO[6]],
                    matDataO[idxO[7]]);
    }
}



} // namespace GDL::simd
