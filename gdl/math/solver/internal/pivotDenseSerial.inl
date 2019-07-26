#pragma once

#include "gdl/math/solver/internal/pivotDenseSerial.h"

#include "gdl/base/exception.h"

#include <cmath>

namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline U32 PivotDenseSerial<_type, _size>::FindMaxAbsValueCol(U32 iteration, const MatrixDataArray& matData)
{
    const U32 colStartIdx = iteration * _size;
    F32 maxAbs = std::abs(matData[colStartIdx + iteration]);
    U32 maxValIdx = iteration;
    for (U32 i = iteration + 1; i < _size; ++i)
    {
        F32 cmpAbs = std::abs(matData[i + colStartIdx]);
        if (maxAbs < cmpAbs)
        {
            maxAbs = cmpAbs;
            maxValIdx = i;
        }
    }

    return maxValIdx;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <bool _swapAllCols>
inline void PivotDenseSerial<_type, _size>::PartialPivotingStep(U32 iteration, MatrixDataArray& matData,
                                                                VectorDataArray& vecData)
{
    U32 rowIdxSwp = FindMaxAbsValueCol(iteration, matData);

    if (rowIdxSwp != iteration)
        SwapRowPivot<_swapAllCols>(rowIdxSwp, iteration, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <Pivot _pivot>
inline std::array<_type, _size>
PivotDenseSerial<_type, _size>::PermuteVector(const VectorDataArray& vectorData,
                                              const PermutationData<_type, _size, _pivot>& permutationData)
{
    std::array<_type, _size> rPermute;
    for (U32 i = 0; i < _size; ++i)
        rPermute[i] = vectorData[permutationData.mRowPermutation[i]];

    return rPermute;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <Pivot _pivot, bool _swapAllCols>
inline void PivotDenseSerial<_type, _size>::PivotingStep(U32 iteration, MatrixDataArray& matData,
                                                         VectorDataArray& vecData)
{
    static_assert(_pivot != Pivot::NONE, "Unneccessary function call");
    static_assert(_pivot == Pivot::PARTIAL, "Unsupported pivoting strategy");

    if constexpr (_pivot == Pivot::PARTIAL)
        PartialPivotingStep<_swapAllCols>(iteration, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <Pivot _pivot, bool _swapAllCols>
inline void PivotDenseSerial<_type, _size>::PivotingStep(U32 iteration, MatrixDataArray& matData,
                                                         PermutationData<_type, _size, _pivot>& permutatationData)
{
    PivotingStep<_pivot, _swapAllCols>(iteration, matData, permutatationData.mRowPermutation);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <bool _swapAllCols>
inline void PivotDenseSerial<_type, _size>::SwapRowPivot(U32 rowIdxSwp, U32 iteration, MatrixDataArray& matData,
                                                         VectorDataArray& vecData)
{
    DEV_EXCEPTION(rowIdxSwp < iteration,
                  "Internal error. Row of the pivot element must be higher or equal to the current iteration number");

    const U32 rowDiff = rowIdxSwp - iteration;

    if constexpr (_swapAllCols)
        for (U32 i = iteration; i < matData.size(); i += _size)
            std::swap(matData[i], matData[i + rowDiff]);
    else
        for (U32 i = iteration * _size + iteration; i < matData.size(); i += _size)
            std::swap(matData[i], matData[i + rowDiff]);

    std::swap(vecData[iteration], vecData[rowIdxSwp]);
}



} // namespace GDL::Solver
