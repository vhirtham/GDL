#pragma once

#include "gdl/math/solver/internal/pivotDenseSerial.h"

#include "gdl/math/serial/matSerial.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/base/simd/swizzle.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/simd/vecSIMD.h"

#include <cmath>

namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline U32 PivotDenseSerial<_type, _size>::FindPivotPartial(U32 iteration, const MatrixDataArray& matData)
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
template <bool _swapAllRows>
inline void PivotDenseSerial<_type, _size>::PartialPivotingStep(U32 iteration, MatrixDataArray& matData,
                                                                VectorDataArray& vecData)
{
    U32 pivotRowIdx = FindPivotPartial(iteration, matData);

    if (pivotRowIdx != iteration)
        SwapRows<_swapAllRows>(pivotRowIdx, iteration, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline std::array<_type, _size> PivotDenseSerial<_type, _size>::PermuteVector(const VectorDataArray& vec,
                                                                              const VectorDataArray& permutation)
{
    std::array<_type, _size> rPermute;
    for (U32 i = 0; i < _size; ++i)
        rPermute[i] = vec[permutation[i]];

    return rPermute;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <Pivot _pivot, bool _swapAllRows>
inline void PivotDenseSerial<_type, _size>::PivotingStep(U32 iteration, MatrixDataArray& matData,
                                                         VectorDataArray& vecData)
{
    static_assert(_pivot != Pivot::NONE, "Unneccessary function call");
    static_assert(_pivot == Pivot::PARTIAL, "Unsupported pivoting strategy");

    if constexpr (_pivot == Pivot::PARTIAL)
        PartialPivotingStep<_swapAllRows>(iteration, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <bool _swapAllRows>
inline void PivotDenseSerial<_type, _size>::SwapRows(U32 pivotRowIdx, U32 iteration, MatrixDataArray& matData,
                                                     VectorDataArray& vecData)
{
    DEV_EXCEPTION(pivotRowIdx < iteration,
                  "Internal error. Row of the pivot element must be higher or equal to the current iteration number");

    const U32 rowDiff = pivotRowIdx - iteration;

    if constexpr (_swapAllRows)
        for (U32 i = iteration; i < matData.size(); i += _size)
            std::swap(matData[i], matData[i + rowDiff]);
    else
        for (U32 i = iteration * _size + iteration; i < matData.size(); i += _size)
            std::swap(matData[i], matData[i + rowDiff]);

    std::swap(vecData[iteration], vecData[pivotRowIdx]);
}



} // namespace GDL::Solver
