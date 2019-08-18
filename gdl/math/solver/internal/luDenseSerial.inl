#pragma once

#include "gdl/math/solver/internal/luDenseSerial.h"

#include "gdl/base/approx.h"
#include "gdl/math/solver/internal/backwardSubstitutionDenseSerial.h"
#include "gdl/math/solver/internal/forwardSubstitutionDenseSerial.h"
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

    ForwardSubstitutionDenseSerial<_type, _size, true>::SolveInPlace(factorization.mLU, vectorData);
    BackwardSubstitutionDenseSerial<_type, _size, false>::SolveInPlace(factorization.mLU, vectorData);

    return vectorData;
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
    const U32 pivIdx = (_size + 1) * iteration;

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
