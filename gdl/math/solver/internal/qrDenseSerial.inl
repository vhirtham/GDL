#pragma once

#include "gdl/math/solver/internal/qrDenseSerial.h"

#include "gdl/base/approx.h"
#include "gdl/base/functions/pow.h"
#include "gdl/math/solver/internal/backwardSubstitutionDenseSerial.h"
//#include "gdl/math/solver/internal/forwardSubstitutionDenseSerial.h"
#include "gdl/math/solver/internal/pivotDenseSerial.h"

#include <cmath>



#include <iostream>


namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline QRDenseSerial<_type, _rows, _cols, _pivot>::Factorization::Factorization(const MatrixDataArray& matrixData)
    : mQR{matrixData}
    , mPermutationData()
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline typename QRDenseSerial<_type, _rows, _cols, _pivot>::MatrixDataArray&
QRDenseSerial<_type, _rows, _cols, _pivot>::Factorization::GetQ()
{
    return mQR.mQData.Q;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline const typename QRDenseSerial<_type, _rows, _cols, _pivot>::MatrixDataArray&
QRDenseSerial<_type, _rows, _cols, _pivot>::Factorization::GetQ() const
{
    return mQR.mQData.Q;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline typename QRDenseSerial<_type, _rows, _cols, _pivot>::MatrixDataArray&
QRDenseSerial<_type, _rows, _cols, _pivot>::Factorization::GetR()
{
    return mQR.mRData.R;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline const typename QRDenseSerial<_type, _rows, _cols, _pivot>::MatrixDataArray&
QRDenseSerial<_type, _rows, _cols, _pivot>::Factorization::GetR() const
{
    return mQR.mRData.R;
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
            PivotDenseSerial<_type, _rows>::template PivotingStep<_pivot, true>(i, factorization.GetR(),
                                                                                factorization.mPermutationData);
        FactorizationStep(i, factorization);
    }

    DEV_EXCEPTION(factorization.GetR()[_rows * _cols - 1] == ApproxZero<_type>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
[[nodiscard]] inline typename QRDenseSerial<_type, _rows, _cols, _pivot>::VectorDataArray
QRDenseSerial<_type, _rows, _cols, _pivot>::Solve(const Factorization& factorization, const VectorDataArray& rhsData)
{
    VectorDataArray vectorData = GetPermutedVectorData(rhsData, factorization);

    const MatrixDataArray& R = factorization.GetR();
    const MatrixDataArray& Q = factorization.GetQ();

    for (U32 i = 0; i < _cols - 1; ++i)
    {
        const U32 colStartIdx = _rows * i;
        _type dot = 0;
        for (U32 r = i; r < _rows; ++r)
            dot += vectorData[r] * Q[colStartIdx + r];
        dot *= 2;

        for (U32 r = i; r < _rows; ++r)
            vectorData[r] -= dot * Q[colStartIdx + r];
    }

    BackwardSubstitutionDenseSerial<_type, _rows, false>::SolveInPlace(R, vectorData);

    return vectorData;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline void QRDenseSerial<_type, _rows, _cols, _pivot>::FactorizationStep(U32 iteration, Factorization& factorization)
{
    const U32 colStartIdx = _rows * iteration;
    const U32 pivIdx = colStartIdx + iteration;

    MatrixDataArray& R = factorization.GetR();
    MatrixDataArray& Q = factorization.GetQ();

    DEV_EXCEPTION(R[pivIdx] == ApproxZero<_type>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");


    // Calculate reflector
    _type sqSum = 0;
    for (U32 i = pivIdx + 1; i < (iteration + 1) * _rows; ++i)
        sqSum += Square(R[i]);

    _type colSignedNorm = std::copysign(sqrt(sqSum + Square(R[pivIdx])), R[pivIdx]);

    _type wPiv = R[pivIdx] + colSignedNorm;

    _type wNorm = 1. / sqrt(sqSum + Square(wPiv));

    Q[pivIdx] = wPiv * wNorm;
    for (U32 i = pivIdx + 1; i < (iteration + 1) * _rows; ++i)
        Q[i] = R[i] * wNorm;



    // calculate new R
    R[pivIdx] = -colSignedNorm;

    std::array<_type, _rows> tmp{0.};

    for (U32 c = iteration + 1; c < _cols; ++c)
    {
        for (U32 r = iteration; r < _rows; ++r)
        {
            U32 idx = c * _rows + r;
            tmp[c] += Q[colStartIdx + r] * R[idx];
        }
        tmp[c] *= 2;
    }

    for (U32 c = iteration + 1; c < _cols; ++c)
        for (U32 r = iteration; r < _rows; ++r)
        {
            U32 idx = c * _rows + r;
            R[idx] -= Q[colStartIdx + r] * tmp[c];
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
