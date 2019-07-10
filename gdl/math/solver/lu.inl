#pragma once

#include "gdl/math/solver/lu.h"

#include "gdl/base/approx.h"
#include "gdl/base/simd/directAccess.h"
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

template <Pivot _pivot, typename _type, U32 _size>
VecSerial<_type, _size, true> LU(const MatSerial<_type, _size, _size>& A, const VecSerial<_type, _size, true>& r)
{
    return LUDenseSerial<_type, _size, _pivot>::Solve(A, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _size>
VecSIMD<_type, _size, true> LU(const MatSIMD<_type, _size, _size>& A, const VecSIMD<_type, _size, true>& r)
{
    using RegisterType = typename MatSIMD<_type, _size, _size>::RegisterType;
    return LUDenseSSE<RegisterType, _size, _pivot>::Solve(A, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline LUDenseSerial<_type, _size, _pivot>::Factorization::Factorization(const MatrixDataArray& matrixData)
    : mLU{matrixData}
    , mPermutation{}
{
    if constexpr (_pivot != Pivot::NONE)
        for (U32 i = 0; i < _size; ++i)
            mPermutation[i] = i;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
[[nodiscard]] inline typename LUDenseSerial<_type, _size, _pivot>::VectorType
LUDenseSerial<_type, _size, _pivot>::Solve(const MatrixType& A, const VectorType& r)
{
    Factorization factorization = Factorize(A.Data());

    return Solve(factorization, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
[[nodiscard]] inline typename LUDenseSerial<_type, _size, _pivot>::VectorType
LUDenseSerial<_type, _size, _pivot>::Solve(const Factorization& factorization, const VectorType& r)
{
    VectorDataArray vectorData = GetPermutedVectorData(r, factorization);

    ForwardSubstitution(factorization.mLU, vectorData);
    BackwardSubstitution(factorization.mLU, vectorData);

    return VectorType(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline void LUDenseSerial<_type, _size, _pivot>::BackwardSubstitution(const MatrixDataArray& lu, VectorDataArray& r)
{

    for (U32 i = _size; i-- > 0;)
    {
        const U32 pivIdx = (_size + 1) * i;

        r[i] /= lu[pivIdx];

        for (U32 j = 0; j < i; ++j)
            r[j] -= r[i] * lu[j + i * _size];
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
[[nodiscard]] inline typename LUDenseSerial<_type, _size, _pivot>::Factorization
LUDenseSerial<_type, _size, _pivot>::Factorize(const MatrixDataArray& matrixData)
{
    Factorization factorization(matrixData);

    for (U32 i = 0; i < _size; ++i)
    {
        if constexpr (_pivot != Pivot::NONE)
            PivotDenseSerial<_type, _size>::template PivotingStep<_pivot, true>(i, factorization.mLU,
                                                                                factorization.mPermutation);
        FactorizationStep(i, factorization);
    }

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline void LUDenseSerial<_type, _size, _pivot>::FactorizationStep(U32 iteration, Factorization& factorization)
{
    const U32 pivIdx = iteration * _size + iteration;

    DEV_EXCEPTION(factorization.mLU[pivIdx] == ApproxZero<_type>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");


    for (U32 i = pivIdx + 1; i < _size * (iteration + 1); ++i)
    {
        factorization.mLU[i] /= factorization.mLU[pivIdx];
        for (U32 j = _size; j < (_size - iteration) * _size; j += _size)
            factorization.mLU[i + j] -= factorization.mLU[i] * factorization.mLU[pivIdx + j];
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline void LUDenseSerial<_type, _size, _pivot>::ForwardSubstitution(const MatrixDataArray& lu, VectorDataArray& r)
{

    for (U32 i = 0; i < _size - 1; ++i)
        for (U32 j = i + 1; j < _size; ++j)
            r[j] -= lu[j + i * _size] * r[i];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline typename LUDenseSerial<_type, _size, _pivot>::VectorDataArray
LUDenseSerial<_type, _size, _pivot>::GetPermutedVectorData(const VectorType& r, const Factorization& factorization)
{
    if constexpr (_pivot != Pivot::NONE)
        return PivotDenseSerial<_type, _size>::PermuteVector(r.Data(), factorization.mPermutation);
    else
        return r.Data();
}



// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// SSE Version
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
inline LUDenseSSE<_registerType, _size, _pivot>::Factorization::Factorization(const MatrixDataArray& matrixData)
    : mLU{matrixData}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
[[nodiscard]] inline typename LUDenseSSE<_registerType, _size, _pivot>::Factorization
LUDenseSSE<_registerType, _size, _pivot>::Factorize(const MatrixType& matrix)
{
    using namespace GDL::simd;

    constexpr U32 numNonFullRegValues = _size % numRegisterValues;


    Factorization factorization(matrix.DataSSE());

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
inline typename LUDenseSSE<_registerType, _size, _pivot>::VectorType
LUDenseSSE<_registerType, _size, _pivot>::Solve(const MatrixType& A, const VectorType& r)
{
    using namespace GDL::simd;

    Factorization factorization = Factorize(A);

    //    std::cout << "SSE:" << std::endl;
    //    std::cout << MatrixType(factorization.mLU) << std::endl;

    //    alignas(alignment) MatrixDataArray matData = A.DataSSE();
    alignas(alignment) VectorDataArray vecData = r.DataSSE();

    //    constexpr U32 numRowsFullRegisters = _size / numRegisterValues;
    //    constexpr U32 numNonFullRegValues = _size % numRegisterValues;

    //    // Set memory of unused register values to zero -> Important for pivoting step
    //    if constexpr (numNonFullRegValues != 0)
    //        for (U32 i = numColRegisters - 1; i < _size * numColRegisters; i += numColRegisters)
    //            matData[i] = BlendBelowIndex<numNonFullRegValues - 1>(matData[i], _mm_set1<_registerType>(0));

    //    // Perform Gauss steps for all registers that do not contain unused values
    //    for (U32 i = 0; i < numRowsFullRegisters; ++i)
    //        GaussStepsRegister(i, matData, vecData);

    //    // Perform Gauss steps for remaining rows that are stored in registers with unused values
    //    if constexpr (numNonFullRegValues != 0)
    //        GaussStepsRegister<0, numNonFullRegValues>(numColRegisters - 1, matData, vecData);


    return VectorType(vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
inline void LUDenseSSE<_registerType, _size, _pivot>::FactorizationLoop(Factorization& factorization)
{
    constexpr U32 numRowsFullRegisters = _size / numRegisterValues;
    constexpr U32 numNonFullRegValues = _size % numRegisterValues;

    for (U32 i = 0; i < numRowsFullRegisters; ++i)
        FactorizationStepsRegister(i, factorization);

    if constexpr (numNonFullRegValues != 0)
        FactorizationStepsRegister<0, numNonFullRegValues>(numColRegisters - 1, factorization);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _size, Pivot _pivot>
template <U32 _regValueIdx>
inline void LUDenseSSE<_registerType, _size, _pivot>::FactorizationStep(U32 iteration, U32 regRowIdx,
                                                                        MatrixDataArray& lu)
{
    using namespace GDL::simd;

    const U32 colStartIdx = iteration * numColRegisters;
    const U32 actRowRegIdx = colStartIdx + regRowIdx;

    DEV_EXCEPTION(GetValue<_regValueIdx>(lu[actRowRegIdx]) == ApproxZero<F32>(1, 10),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");



    const _registerType zero = _mm_setzero<_registerType>();
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
inline void LUDenseSSE<_registerType, _size, _pivot>::FactorizationStepsRegister(U32 regRowIdx,
                                                                                 Factorization& factorization)
{
    static_assert(_maxRecursionDepth <= numRegisterValues,
                  "_maxRecursionDepth must be equal or smaller than the number of register values.");

    const U32 iteration = regRowIdx * numRegisterValues + _regValueIdx;

    FactorizationStep<_regValueIdx>(iteration, regRowIdx, factorization.mLU);

    if constexpr (_regValueIdx + 1 < _maxRecursionDepth)
        FactorizationStepsRegister<_regValueIdx + 1, _maxRecursionDepth>(regRowIdx, factorization);
}

} // namespace GDL::Solver
