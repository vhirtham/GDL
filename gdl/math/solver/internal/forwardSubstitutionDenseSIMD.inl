#pragma once

#include "gdl/math/solver/internal/forwardSubstitutionDenseSIMD.h"

#include "gdl/base/approx.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/swizzle.h"


namespace GDL::Solver
{



template <typename _registerType, U32 _size, bool _isUnit>
inline void ForwardSubstitutionDenseSIMD<_registerType, _size, _isUnit>::SolveInPlace(const MatrixDataArray& matrixData,
                                                                                      VectorDataArray& rhsData)
{
    constexpr U32 numIterations = _size - 1;
    constexpr U32 numFullRegisterIterations = numIterations / numRegisterValues;
    constexpr U32 numNonFullRegIterations = numIterations % numRegisterValues;

    for (U32 i = 0; i < numFullRegisterIterations; ++i)
        SubstitutionSteps(i, matrixData, rhsData);

    if constexpr (numNonFullRegIterations != 0)
        SubstitutionSteps<0, numNonFullRegIterations>(numRegistersPerCol - 1, matrixData, rhsData);
}



// --------------------------------------------------------------------------------------------------------------------


template <typename _registerType, U32 _size, bool _isUnit>
template <U32 _regValueIdx>
inline void ForwardSubstitutionDenseSIMD<_registerType, _size, _isUnit>::SubstitutionStep(
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

    if constexpr (_regValueIdx < numRegisterValues - 1)
        rhsData[regRowIdx] = BlendBelowIndex<_regValueIdx>(
                rhsData[regRowIdx], _mm_fnmadd(matrixData[colStartIdx + regRowIdx], mult, rhsData[regRowIdx]));

    for (U32 i = regRowIdx + 1; i < numRegistersPerCol; ++i)
        rhsData[i] = _mm_fnmadd(matrixData[colStartIdx + i], mult, rhsData[i]);
}



// --------------------------------------------------------------------------------------------------------------------


template <typename _registerType, U32 _size, bool _isUnit>
template <U32 _regValueIdx, U32 _maxRecursionDepth>
inline void ForwardSubstitutionDenseSIMD<_registerType, _size, _isUnit>::SubstitutionSteps(
        U32 regRowIdx, const MatrixDataArray& matrixData, VectorDataArray& rhsData)
{
    using namespace GDL::simd;

    static_assert(_maxRecursionDepth <= numRegisterValues,
                  "_maxRecursionDepth must be equal or smaller than the number of register values.");

    SubstitutionStep<_regValueIdx>(regRowIdx, matrixData, rhsData);

    if constexpr (_regValueIdx + 1 < _maxRecursionDepth)
        SubstitutionSteps<_regValueIdx + 1, _maxRecursionDepth>(regRowIdx, matrixData, rhsData);
}


} // namespace GDL::Solver
