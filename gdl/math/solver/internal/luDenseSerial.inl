#pragma once

#include "gdl/math/solver/internal/luDenseSerial.h"

#include "gdl/base/approx.h"
#include "gdl/math/solver/internal/pivotDenseSerial.h"



namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline LUDenseSerial<_type, _size, _pivot>::Factorization::Factorization(const MatrixDataArray& matrixData)
    : mLU{matrixData}
    , mPermutationData()
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
[[nodiscard]] inline typename LUDenseSerial<_type, _size, _pivot>::VectorDataArray
LUDenseSerial<_type, _size, _pivot>::Solve(const Factorization& factorization, const VectorDataArray& rhsData)
{
    VectorDataArray vectorData = GetPermutedVectorData(rhsData, factorization);

    ForwardSubstitution(factorization.mLU, vectorData);
    BackwardSubstitution(factorization.mLU, vectorData);

    return vectorData;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline void LUDenseSerial<_type, _size, _pivot>::BackwardSubstitution(const MatrixDataArray& lu,
                                                                      VectorDataArray& rhsData)
{
    for (U32 i = _size; i-- > 0;)
    {
        const U32 pivIdx = (_size + 1) * i;

        rhsData[i] /= lu[pivIdx];

        for (U32 j = 0; j < i; ++j)
            rhsData[j] -= rhsData[i] * lu[j + i * _size];
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
[[nodiscard]] inline typename LUDenseSerial<_type, _size, _pivot>::Factorization
LUDenseSerial<_type, _size, _pivot>::Factorize(const MatrixDataArray& matrixData)
{
    Factorization factorization(matrixData);

    for (U32 i = 0; i < _size - 1; ++i)
    {
        if constexpr (_pivot != Pivot::NONE)
            PivotDenseSerial<_type, _size>::template PivotingStep<_pivot, true>(i, factorization.mLU,
                                                                                factorization.mPermutationData);
        FactorizationStep(i, factorization);
    }

    DEV_EXCEPTION(factorization.mLU[_size * _size - 1] == ApproxZero<_type>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");

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
inline void LUDenseSerial<_type, _size, _pivot>::ForwardSubstitution(const MatrixDataArray& lu,
                                                                     VectorDataArray& rhsData)
{

    for (U32 i = 0; i < _size - 1; ++i)
        for (U32 j = i + 1; j < _size; ++j)
            rhsData[j] -= lu[j + i * _size] * rhsData[i];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline typename LUDenseSerial<_type, _size, _pivot>::VectorDataArray
LUDenseSerial<_type, _size, _pivot>::GetPermutedVectorData(const VectorDataArray& rhsData,
                                                           const Factorization& factorization)
{
    if constexpr (_pivot != Pivot::NONE)
        return PivotDenseSerial<_type, _size>::PermuteVector(rhsData, factorization.mPermutationData);
    else
        return rhsData;
}



} // namespace GDL::Solver
