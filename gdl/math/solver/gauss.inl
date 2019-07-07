#pragma once

#include "gdl/math/solver/gauss.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/math/serial/matSerial.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/solver/pivot.h"


#include <cmath>

namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, I32 _size>
VecSerial<_type, _size, true> Gauss(const MatSerial<_type, _size, _size>& A, const VecSerial<_type, _size, true>& b)
{
    return GaussDenseSerial<_type, _size, _pivot>::Solve(A, b);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, I32 _size>
VecSIMD<_type, _size, true> Gauss(const MatSIMD<_type, _size, _size>& A, const VecSIMD<_type, _size, true>& b)
{
    using RegisterType = typename MatSIMD<_type, _size, _size>::RegisterType;
    return GaussDenseSSE<RegisterType, _size, _pivot>::Solve(A, b);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, I32 _size, Pivot _pivot>
inline typename GaussDenseSerial<_type, _size, _pivot>::VectorType
GaussDenseSerial<_type, _size, _pivot>::Solve(const MatrixType& A, const VectorType& b)
{

    MatrixDataArray matData = A.Data();
    VectorDataArray vecData = b.Data();

    for (U32 i = 0; i < _size; ++i)
        GaussStep(i, matData, vecData);

    return VectorType(vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, I32 _size, Pivot _pivot>
inline void GaussDenseSerial<_type, _size, _pivot>::EliminationStep(U32 iteration, MatrixDataArray& matData,
                                                                    VectorDataArray& vecData)
{
    const U32 colStartIdx = iteration * _size;
    const U32 pivotIdx = colStartIdx + iteration;

    DEV_EXCEPTION(matData[pivotIdx] == ApproxZero<_type>(1, 100), "Singular matrix - system not solveable");

    std::array<_type, _size> rowMult;

    // Calculate row multipliers
    _type div = 1 / matData[pivotIdx];

    matData[colStartIdx + iteration] -= 1;
    for (U32 i = 0; i < _size; ++i)
        rowMult[i] = div * matData[colStartIdx + i];

    // Perform elimination for all relevant columns
    for (U32 i = colStartIdx + _size; i < matData.size(); i += _size)
    {
        _type pivValue = matData[iteration + i];
        for (U32 j = 0; j < _size; ++j)
            matData[i + j] -= rowMult[j] * pivValue;
    }

    _type pivValue = vecData[iteration];
    for (U32 i = 0; i < _size; ++i)
        vecData[i] -= rowMult[i] * pivValue;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, I32 _size, Pivot _pivot>
inline void GaussDenseSerial<_type, _size, _pivot>::GaussStep(U32 iteration, MatrixDataArray& matData,
                                                              VectorDataArray& vecData)
{
    if constexpr (_pivot != Pivot::NONE)
        PivotDenseSerial<_type, _size>::template PivotingStep<_pivot>(iteration, matData, vecData);

    EliminationStep(iteration, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// SSE Version
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size, Pivot _pivot>
inline typename GaussDenseSSE<_registerType, _size, _pivot>::VectorType
GaussDenseSSE<_registerType, _size, _pivot>::Solve(const MatrixType& A, const VectorType& b)
{
    using namespace GDL::simd;

    alignas(alignment) MatrixDataArray matData = A.DataSSE();
    alignas(alignment) VectorDataArray vecData = b.DataSSE();

    constexpr U32 numRowsFullRegisters = _size / numRegisterValues;
    constexpr U32 numNonFullRegValues = _size % numRegisterValues;

    // Set memory of unused register values to zero -> Important for pivoting step
    if constexpr (numNonFullRegValues != 0)
        for (U32 i = numColRegisters - 1; i < _size * numColRegisters; i += numColRegisters)
            matData[i] = BlendBelowIndex<numNonFullRegValues - 1>(matData[i], _mm_set1<_registerType>(0));

    // Perform Gauss steps for all registers that do not contain unused values
    for (U32 i = 0; i < numRowsFullRegisters; ++i)
        GaussStepsRegister(i, matData, vecData);

    // Perform Gauss steps for remaining rows that are stored in registers with unused values
    if constexpr (numNonFullRegValues != 0)
        GaussStepsRegister<0, numNonFullRegValues>(numColRegisters - 1, matData, vecData);


    return VectorType(vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size, Pivot _pivot>
template <U32 _regValueIdx>
inline void GaussDenseSSE<_registerType, _size, _pivot>::EliminationStepRegister(U32 iteration, U32 regRowIdx,
                                                                                 MatrixDataArray& matData,
                                                                                 VectorDataArray& vecData)
{
    using namespace GDL::simd;

    const U32 colStartIdx = iteration * numColRegisters;
    const U32 actRowRegIdx = colStartIdx + regRowIdx;

    DEV_EXCEPTION(GetValue<_regValueIdx>(matData[actRowRegIdx]) == ApproxZero<ValueType>(1, 100),
                  "Singular matrix - system not solveable");


    // Calculate row multipliers
    alignas(alignment) std::array<_registerType, numColRegisters> rowMult;
    const _registerType zero = _mm_setzero<_registerType>();
    const _registerType one = _mm_set1<_registerType>(1);
    const _registerType div = _mm_div(one, BroadcastAcrossLanes<_regValueIdx>(matData[actRowRegIdx]));

    matData[actRowRegIdx] = _mm_sub(matData[actRowRegIdx], BlendIndex<_regValueIdx>(zero, one));
    for (U32 i = 0; i < numColRegisters; ++i)
        rowMult[i] = _mm_mul(div, matData[colStartIdx + i]);


    // Perform elimination for all relevant columns
    for (U32 i = colStartIdx + numColRegisters; i < _size * numColRegisters; i += numColRegisters)
    {
        _registerType pivValue = BroadcastAcrossLanes<_regValueIdx>(matData[regRowIdx + i]);
        for (U32 j = 0; j < numColRegisters; ++j)
            matData[i + j] = _mm_fnmadd(rowMult[j], pivValue, matData[i + j]);
    }

    _registerType pivValue = BroadcastAcrossLanes<_regValueIdx>(vecData[regRowIdx]);
    for (U32 i = 0; i < numColRegisters; ++i)
        vecData[i] = _mm_fnmadd(rowMult[i], pivValue, vecData[i]);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size, Pivot _pivot>
template <U32 _regValueIdx, U32 _maxRecursionDepth>
inline void GaussDenseSSE<_registerType, _size, _pivot>::GaussStepsRegister(U32 regRowIdx, MatrixDataArray& matData,
                                                                            VectorDataArray& vecData)
{

    static_assert(_maxRecursionDepth <= numRegisterValues,
                  "_maxRecursionDepth must be equal or smaller than the number of register values.");

    const U32 iteration = regRowIdx * numRegisterValues + _regValueIdx;

    if constexpr (_pivot != Pivot::NONE)
        PivotDenseSSE<_registerType, _size>::template PivotingStepRegister<_regValueIdx, _pivot>(iteration, regRowIdx,
                                                                                                 matData, vecData);
    EliminationStepRegister<_regValueIdx>(iteration, regRowIdx, matData, vecData);

    if constexpr (_regValueIdx + 1 < _maxRecursionDepth)
        GaussStepsRegister<_regValueIdx + 1, _maxRecursionDepth>(regRowIdx, matData, vecData);
}



} // namespace GDL::Solver
