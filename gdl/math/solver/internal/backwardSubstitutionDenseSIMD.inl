#pragma once

#include "gdl/math/solver/internal/backwardSubstitutionDenseSIMD.h"

#include "gdl/base/approx.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/swizzle.h"


namespace GDL::Solver
{



template <typename _registerType, U32 _size, bool _isUnit>
inline void
BackwardSubstitutionDenseSIMD<_registerType, _size, _isUnit>::SolveInPlace(const MatrixDataArray& matrixData,
                                                                           VectorDataArray& rhsData)
{
    constexpr U32 numIterations = _size;
    constexpr U32 numFullRegisterIterations = numIterations / numRegisterValues;
    constexpr U32 numNonFullRegIterations = numIterations % numRegisterValues;

    if constexpr (numNonFullRegIterations != 0)
        SubstitutionSteps<numNonFullRegIterations - 1>(numRegistersPerCol - 1, matrixData, rhsData);

    for (U32 i = numFullRegisterIterations; 0 < i--;)
        SubstitutionSteps(i, matrixData, rhsData);
}



// --------------------------------------------------------------------------------------------------------------------


template <typename _registerType, U32 _size, bool _isUnit>
template <U32 _regValueIdx>
inline void BackwardSubstitutionDenseSIMD<_registerType, _size, _isUnit>::SubstitutionStep(
        U32 regRowIdx, const MatrixDataArray& matrixData, VectorDataArray& rhsData)
{
    using namespace GDL::simd;

    const U32 iteration = regRowIdx * numRegisterValues + _regValueIdx;
    const U32 colStartIdx = iteration * numRegistersPerCol;


    DEV_EXCEPTION(GetValue<_regValueIdx>(matrixData[colStartIdx + regRowIdx]) == ApproxZero<ValueType>(1, 100),
                  "Can't solve system - Singular matrix.");


    if constexpr (not _isUnit)
        rhsData[regRowIdx] = BlendIndex<_regValueIdx>(rhsData[regRowIdx],
                                                      _mm_div(rhsData[regRowIdx], matrixData[colStartIdx + regRowIdx]));

    _registerType mult = BroadcastAcrossLanes<_regValueIdx>(rhsData[regRowIdx]);

    if constexpr (_regValueIdx > 0)
        rhsData[regRowIdx] = BlendAboveIndex<_regValueIdx>(
                rhsData[regRowIdx], _mm_fnmadd(matrixData[colStartIdx + regRowIdx], mult, rhsData[regRowIdx]));

    for (U32 i = regRowIdx; 0 < i--;)
        rhsData[i] = _mm_fnmadd(matrixData[colStartIdx + i], mult, rhsData[i]);
}



// --------------------------------------------------------------------------------------------------------------------


template <typename _registerType, U32 _size, bool _isUnit>
template <U32 _regValueIdx>
inline void BackwardSubstitutionDenseSIMD<_registerType, _size, _isUnit>::SubstitutionSteps(
        U32 regRowIdx, const MatrixDataArray& matrixData, VectorDataArray& rhsData)
{
    using namespace GDL::simd;

    static_assert(_regValueIdx < numRegisterValues, "_regValueIdx must be smaller than the number of register values");

    SubstitutionStep<_regValueIdx>(regRowIdx, matrixData, rhsData);

    if constexpr (_regValueIdx > 0)
        SubstitutionSteps<_regValueIdx - 1>(regRowIdx, matrixData, rhsData);
}


} // namespace GDL::Solver
