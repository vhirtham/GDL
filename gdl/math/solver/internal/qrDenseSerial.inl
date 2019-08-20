#pragma once

#include "gdl/math/solver/internal/qrDenseSerial.h"

#include "gdl/base/approx.h"
#include "gdl/base/functions/pow.h"
#include "gdl/math/solver/internal/backwardSubstitutionDenseSerial.h"
//#include "gdl/math/solver/internal/forwardSubstitutionDenseSerial.h"
#include "gdl/math/solver/internal/pivotDenseSerial.h"



#include <iostream>


namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline QRDenseSerial<_type, _rows, _cols, _pivot>::Factorization::Factorization(const MatrixDataArray& matrixData)
    : mQ{0}
    , mR{matrixData}
    , mPermutationData()
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
[[nodiscard]] inline typename QRDenseSerial<_type, _rows, _cols, _pivot>::Factorization
QRDenseSerial<_type, _rows, _cols, _pivot>::Factorize(const MatrixDataArray& matrixData)
{
    Factorization factorization(matrixData);

    for (U32 i = 0; i < _rows - 1; ++i)
    {
        if constexpr (_pivot != Pivot::NONE)
            PivotDenseSerial<_type, _rows>::template PivotingStep<_pivot, true>(i, factorization.mQR,
                                                                                factorization.mPermutationData);
        FactorizationStep(i, factorization);
    }

    DEV_EXCEPTION(factorization.mR[_rows * _cols - 1] == ApproxZero<_type>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
[[nodiscard]] inline typename QRDenseSerial<_type, _rows, _cols, _pivot>::VectorDataArray
QRDenseSerial<_type, _rows, _cols, _pivot>::Solve(const Factorization& factorization, const VectorDataArray& rhsData)
{
    VectorDataArray vectorData = GetPermutedVectorData(rhsData, factorization);

    for (U32 i = 0; i < _cols - 1; ++i)
    {
        const U32 colStartIdx = _rows * i;
        _type dot = 0;
        for (U32 r = i; r < _rows; ++r)
            dot += vectorData[r] * factorization.mQ[colStartIdx + r];
        dot *= 2;

        for (U32 r = i; r < _rows; ++r)
            vectorData[r] -= dot * factorization.mQ[colStartIdx + r];
    }

    BackwardSubstitutionDenseSerial<_type, _rows, false>::SolveInPlace(factorization.mR, vectorData);

    return vectorData;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline void QRDenseSerial<_type, _rows, _cols, _pivot>::FactorizationStep(U32 iteration, Factorization& factorization)
{
    const U32 colStartIdx = _rows * iteration;
    const U32 pivIdx = colStartIdx + iteration;

    DEV_EXCEPTION(factorization.mR[pivIdx] == ApproxZero<_type>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");

    // Calculate reflector
    _type s = 0;
    for (U32 i = pivIdx; i < (iteration + 1) * _rows; ++i)
        s += Square(factorization.mR[i]);
    s = sqrt(s);

    _type diff = factorization.mR[pivIdx] - s;

    _type my = 1. / sqrt(-2. * s * diff);

    factorization.mQ[pivIdx] = diff * my;
    for (U32 i = pivIdx + 1; i < (iteration + 1) * _rows; ++i)
        factorization.mQ[i] = factorization.mR[i] * my;


    // calculate new R
    factorization.mR[pivIdx] = s;

    std::array<_type, _rows> tmp{0.};

    for (U32 c = iteration + 1; c < _cols; ++c)
    {
        for (U32 r = iteration; r < _rows; ++r)
        {
            U32 idx = c * _rows + r;
            tmp[c] += factorization.mQ[colStartIdx + r] * factorization.mR[idx];
        }
        tmp[c] *= 2;
    }

    for (U32 c = iteration + 1; c < _cols; ++c)
        for (U32 r = iteration; r < _rows; ++r)
        {
            U32 idx = c * _rows + r;
            factorization.mR[idx] -= factorization.mQ[colStartIdx + r] * tmp[c];
        }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline typename QRDenseSerial<_type, _rows, _cols, _pivot>::VectorDataArray
QRDenseSerial<_type, _rows, _cols, _pivot>::GetPermutedVectorData(const VectorDataArray& rhsData,
                                                                  const Factorization& factorization)
{
    if constexpr (_pivot != Pivot::NONE)
        return PivotDenseSerial<_type, _rows>::PermuteVector(rhsData, factorization.mPermutationData);
    else
        return rhsData;
}



} // namespace GDL::Solver
