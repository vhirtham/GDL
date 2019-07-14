#pragma once

#include "gdl/math/solver/internal/luDenseSIMD.h"

#include "gdl/base/approx.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/swizzle.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/solver/pivot.h"



namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
inline LUDenseSIMD<_registerType, _size, _pivot>::Factorization::Factorization(const MatrixDataArray& matrixData)
    : mLU{matrixData}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
[[nodiscard]] inline typename LUDenseSIMD<_registerType, _size, _pivot>::Factorization
LUDenseSIMD<_registerType, _size, _pivot>::Factorize(const MatrixDataArray& matrixData)
{
    using namespace GDL::simd;

    constexpr U32 numNonFullRegValues = _size % numRegisterValues;


    Factorization factorization(matrixData);

    // Set memory of unused register values to zero -> Important for pivoting step
    if constexpr (numNonFullRegValues != 0)
        for (U32 i = numColRegisters - 1; i < _size * numColRegisters; i += numColRegisters)
            factorization.mLU[i] =
                    BlendBelowIndex<numNonFullRegValues - 1>(factorization.mLU[i], _mm_set1<_registerType>(0));

    FactorizationLoop(factorization);

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
inline typename LUDenseSIMD<_registerType, _size, _pivot>::VectorDataArray
LUDenseSIMD<_registerType, _size, _pivot>::Solve(const Factorization& factorization, const VectorDataArray& rData)
{
    alignas(alignment) VectorDataArray vectorData = rData;

    ForwardSubstitution(factorization.mLU, vectorData);
    BackwardSubstitution(factorization.mLU, vectorData);

    return vectorData;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
inline void LUDenseSIMD<_registerType, _size, _pivot>::BackwardSubstitution(const MatrixDataArray& lu,
                                                                            VectorDataArray& rhsData)
{
    constexpr U32 numRowsFullRegisters = _size / numRegisterValues;
    constexpr U32 numNonFullRegValues = _size % numRegisterValues;


    if constexpr (numNonFullRegValues != 0)
        BackwardSubstitutionSteps<numNonFullRegValues - 1>(numColRegisters - 1, lu, rhsData);

    for (U32 i = numRowsFullRegisters; 0 < i--;)
        BackwardSubstitutionSteps(i, lu, rhsData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
template <U32 _regValueIdx>
inline void LUDenseSIMD<_registerType, _size, _pivot>::BackwardSubstitutionSteps(U32 regRowIdx,
                                                                                 const MatrixDataArray& lu,
                                                                                 VectorDataArray& rhsData)
{
    using namespace GDL::simd;



    //    // Also remove from small matrix version!!!!!!!!!!
    //    const __m128 zero = _mm_setzero<__m128>();

    const U32 iteration = regRowIdx * numRegisterValues + _regValueIdx;
    const U32 colStartIdx = iteration * numColRegisters;


    rhsData[regRowIdx] =
            BlendIndex<_regValueIdx>(rhsData[regRowIdx], _mm_div(rhsData[regRowIdx], lu[colStartIdx + regRowIdx]));

    _registerType mult = BroadcastAcrossLanes<_regValueIdx>(rhsData[regRowIdx]);

    if constexpr (_regValueIdx > 0)
        rhsData[regRowIdx] = BlendAboveIndex<_regValueIdx>(
                rhsData[regRowIdx], _mm_fnmadd(lu[colStartIdx + regRowIdx], mult, rhsData[regRowIdx]));

    for (U32 i = regRowIdx; 0 < i--;)
        rhsData[i] = _mm_fnmadd(lu[colStartIdx + i], mult, rhsData[i]);


    if constexpr (_regValueIdx > 0)
        BackwardSubstitutionSteps<_regValueIdx - 1>(regRowIdx, lu, rhsData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
inline void LUDenseSIMD<_registerType, _size, _pivot>::FactorizationLoop(Factorization& factorization)
{
    constexpr U32 numRowsFullRegisters = (_size - 1) / numRegisterValues;
    constexpr U32 numNonFullRegValues = (_size - 1) % numRegisterValues;

    for (U32 i = 0; i < numRowsFullRegisters; ++i)
        FactorizationStepsRegister(i, factorization);

    if constexpr (numNonFullRegValues != 0)
        FactorizationStepsRegister<0, numNonFullRegValues>(numColRegisters - 1, factorization);


    DEV_EXCEPTION(simd::GetValue<(_size - 1) % numRegisterValues>(factorization.mLU[factorization.mLU.size() - 1]) ==
                          ApproxZero<F32>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
template <U32 _regValueIdx>
inline void LUDenseSIMD<_registerType, _size, _pivot>::FactorizationStep(U32 iteration, U32 regRowIdx,
                                                                         MatrixDataArray& lu)
{
    using namespace GDL::simd;

    const U32 colStartIdx = iteration * numColRegisters;
    const U32 actRowRegIdx = colStartIdx + regRowIdx;

    DEV_EXCEPTION(GetValue<_regValueIdx>(lu[actRowRegIdx]) == ApproxZero<F32>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");


    const _registerType one = _mm_set1<_registerType>(1);
    const _registerType div = _mm_div(one, BroadcastAcrossLanes<_regValueIdx>(lu[actRowRegIdx]));



    lu[actRowRegIdx] = BlendBelowIndex<_regValueIdx>(lu[actRowRegIdx], _mm_mul(div, lu[actRowRegIdx]));
    for (U32 i = regRowIdx + 1; i < numColRegisters; ++i)
        lu[colStartIdx + i] = _mm_mul(div, lu[colStartIdx + i]);

    for (U32 i = actRowRegIdx + numColRegisters; i < _size * numColRegisters; i += numColRegisters)
    {
        _registerType pivValue = BroadcastAcrossLanes<_regValueIdx>(lu[i]);
        lu[i] = BlendBelowIndex<_regValueIdx>(lu[i], _mm_fnmadd(lu[actRowRegIdx], pivValue, lu[i]));
        for (U32 j = 1; j < numColRegisters - regRowIdx; ++j)
            lu[i + j] = _mm_fnmadd(lu[actRowRegIdx + j], pivValue, lu[i + j]);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
template <U32 _regValueIdx, U32 _maxRecursionDepth>
inline void LUDenseSIMD<_registerType, _size, _pivot>::FactorizationStepsRegister(U32 regRowIdx,
                                                                                  Factorization& factorization)
{
    static_assert(_maxRecursionDepth <= numRegisterValues,
                  "_maxRecursionDepth must be equal or smaller than the number of register values.");

    const U32 iteration = regRowIdx * numRegisterValues + _regValueIdx;

    FactorizationStep<_regValueIdx>(iteration, regRowIdx, factorization.mLU);

    if constexpr (_regValueIdx + 1 < _maxRecursionDepth)
        FactorizationStepsRegister<_regValueIdx + 1, _maxRecursionDepth>(regRowIdx, factorization);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
inline void LUDenseSIMD<_registerType, _size, _pivot>::ForwardSubstitution(const MatrixDataArray& lu,
                                                                           VectorDataArray& rhsData)
{
    constexpr U32 numRowsFullRegisters = (_size - 1) / numRegisterValues;
    constexpr U32 numNonFullRegValues = (_size - 1) % numRegisterValues;

    for (U32 i = 0; i < numRowsFullRegisters; ++i)
        ForwardSubstitutionSteps(i, lu, rhsData);

    if constexpr (numNonFullRegValues != 0)
        ForwardSubstitutionSteps<0, numNonFullRegValues>(numColRegisters - 1, lu, rhsData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
template <U32 _regValueIdx, U32 _maxRecursionDepth>
inline void LUDenseSIMD<_registerType, _size, _pivot>::ForwardSubstitutionSteps(U32 regRowIdx,
                                                                                const MatrixDataArray& lu,
                                                                                VectorDataArray& rhsData)
{
    using namespace GDL::simd;


    // Also remove from small matrix version!!!!!!!!!!
    // const __m128 zero = _mm_setzero<__m128>();

    const U32 iteration = regRowIdx * numRegisterValues + _regValueIdx;
    const U32 colStartIdx = iteration * numColRegisters;


    _registerType mult = BroadcastAcrossLanes<_regValueIdx>(rhsData[regRowIdx]);

    if constexpr (_regValueIdx < numRegisterValues - 1)
        rhsData[regRowIdx] = BlendBelowIndex<_regValueIdx>(
                rhsData[regRowIdx], _mm_fnmadd(lu[colStartIdx + regRowIdx], mult, rhsData[regRowIdx]));

    for (U32 i = regRowIdx + 1; i < numColRegisters; ++i)
        rhsData[i] = _mm_fnmadd(lu[colStartIdx + i], mult, rhsData[i]);


    if constexpr (_regValueIdx + 1 < _maxRecursionDepth)
        ForwardSubstitutionSteps<_regValueIdx + 1, _maxRecursionDepth>(regRowIdx, lu, rhsData);
}



} // namespace GDL::Solver
