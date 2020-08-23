#pragma once

#include "gdl/math/solver/internal/qrDenseSIMD.h"

#include "gdl/base/approx.h"
#include "gdl/base/simd/copySign.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/negate.h"
#include "gdl/base/simd/registerSum.h"
#include "gdl/base/simd/swizzle.h"
#include "gdl/math/solver/internal/backwardSubstitutionDenseSIMD.h"
#include "gdl/math/solver/internal/forwardSubstitutionDenseSIMD.h"



namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, Pivot _pivot>
inline QRDenseSIMD<_registerType, _rows, _cols, _pivot>::Factorization::Factorization(const MatrixDataArray& matrixData)
    : mVR{matrixData}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, Pivot _pivot>
inline QRDenseSIMD<_registerType, _rows, _cols, _pivot>::Factorization::VRData::VRData(const MatrixDataArray& dataQ)
    : mRData{{{{0}}}, {dataQ}}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline typename QRDenseSIMD<_type, _rows, _cols, _pivot>::MatrixDataArray&
QRDenseSIMD<_type, _rows, _cols, _pivot>::Factorization::GetV()
{
    return mVR.mVData.V;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline typename QRDenseSIMD<_type, _rows, _cols, _pivot>::Factorization::VRDataArray&
QRDenseSIMD<_type, _rows, _cols, _pivot>::Factorization::GetVR()
{
    return mVR.mVR;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline const typename QRDenseSIMD<_type, _rows, _cols, _pivot>::MatrixDataArray&
QRDenseSIMD<_type, _rows, _cols, _pivot>::Factorization::GetV() const
{
    return mVR.mVData.V;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline typename QRDenseSIMD<_type, _rows, _cols, _pivot>::MatrixDataArray&
QRDenseSIMD<_type, _rows, _cols, _pivot>::Factorization::GetR()
{
    return mVR.mRData.R;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
inline const typename QRDenseSIMD<_type, _rows, _cols, _pivot>::MatrixDataArray&
QRDenseSIMD<_type, _rows, _cols, _pivot>::Factorization::GetR() const
{
    return mVR.mRData.R;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, Pivot _pivot>
[[nodiscard]] inline typename QRDenseSIMD<_registerType, _rows, _cols, _pivot>::Factorization
QRDenseSIMD<_registerType, _rows, _cols, _pivot>::Factorize(const MatrixDataArray& matrixData)
{
    constexpr U32 numFullRegistersPerCol = (_rows - 1) / numRegisterValues;
    constexpr U32 numNonFullRegValues = (_rows - 1) % numRegisterValues;

    Factorization factorization(matrixData);

    MatrixDataArray& V = factorization.GetV();
    MatrixDataArray& R = factorization.GetR();

    for (U32 i = 0; i < numFullRegistersPerCol; ++i)
        FactorizationSteps(i, V, R);

    if constexpr (numNonFullRegValues != 0)
        FactorizationSteps<0, numNonFullRegValues>(numRegistersPerCol - 1, V, R);


    DEV_EXCEPTION(simd::GetValue<(_rows - 1) % numRegisterValues>(R[R.size() - 1]) == ApproxZero<ValueType>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, Pivot _pivot>
inline typename QRDenseSIMD<_registerType, _rows, _cols, _pivot>::VectorDataArray
QRDenseSIMD<_registerType, _rows, _cols, _pivot>::Solve(const Factorization& factorization,
                                                        const VectorDataArray& rhsData)
{
    alignas(alignment) VectorDataArray vectorData = rhsData;

    //    if constexpr (_pivot != Pivot::NONE)
    //        PivotDenseSSE<_registerType, _rows>::PermuteVector(vectorData, factorization.mPermutationData);

    //    ForwardSubstitutionDenseSIMD<_registerType, _rows, true>::SolveInPlace(factorization.mVR, vectorData);
    //    BackwardSubstitutionDenseSIMD<_registerType, _rows, false>::SolveInPlace(factorization.mVR, vectorData);

    return vectorData;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, Pivot _pivot>
template <U32 _regValueIdx>
inline _registerType
QRDenseSIMD<_registerType, _rows, _cols, _pivot>::CalculateSquareSum(U32 regRowIdx, U32 colStartIdx, MatrixDataArray& R)
{
    using namespace GDL::simd;

    const VectorDataArray& col = reinterpret_cast<const VectorDataArray&>(R[colStartIdx]);

    if constexpr (_regValueIdx + 1 < numRegisterValues)
        return RegisterArraySquareSum<_regValueIdx + 1>(col, regRowIdx);
    else
        return RegisterArraySquareSum<0>(col, regRowIdx + 1);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, Pivot _pivot>
template <U32 _regValueIdx>
inline void QRDenseSIMD<_registerType, _rows, _cols, _pivot>::FactorizationStep(U32 iteration, U32 regRowIdx,
                                                                                MatrixDataArray& V, MatrixDataArray& R)
{
    using namespace GDL::simd;

    const U32 colStartIdx =
            iteration * numRegistersPerCol; // check if it might be passed directly if iteration is not needed
    const U32 actRowRegIdx = colStartIdx + regRowIdx;

    DEV_EXCEPTION(GetValue<_regValueIdx>(R[actRowRegIdx]) == ApproxZero<ValueType>(1, 100),
                  "Can't solve system - Singular matrix or inappropriate pivoting strategy.");


    _registerType squaredSum = CalculateSquareSum<_regValueIdx>(regRowIdx, colStartIdx, R);

    // const _registerType zero = _mm_setzero<_registerType>();

    _registerType colNorm = _mm_sqrt(_mm_fmadd(R[actRowRegIdx], R[actRowRegIdx], squaredSum));
    _registerType colSignedNorm = Negate(CopySign<true>(R[actRowRegIdx], colNorm));

    _registerType wPiv = BlendIndex<_regValueIdx>(R[actRowRegIdx], _mm_sub(R[actRowRegIdx], colSignedNorm));
    _registerType wNorm = BroadcastAcrossLanes<_regValueIdx>(_mm_sqrt(_mm_fmadd(wPiv, wPiv, squaredSum)));


    V[actRowRegIdx] = BlendAboveIndex<_regValueIdx>(_mm_div(wPiv, wNorm), V[actRowRegIdx]);
    R[actRowRegIdx] = BlendAboveIndex<_regValueIdx>(colSignedNorm, R[actRowRegIdx]);


    // ---------------------

    _registerType zero = _mm_setzero<_registerType>();

    std::array<_registerType, _cols> tmp = {{0}};
    for (UST c = iteration + 1; c < _cols; ++c)
        tmp[c] = BlendAboveIndex<_regValueIdx>(_mm_mul(V[actRowRegIdx], R[c]), zero);


    _registerType dot_products = RegisterMultiSum(tmp);

    // tmp[0] = _mm_mul(V[0], BroadcastAcrossLanes<0>(dot_products));
    tmp[1] = _mm_mul(V[actRowRegIdx], BroadcastAcrossLanes<1>(dot_products));
    tmp[2] = _mm_mul(V[actRowRegIdx], BroadcastAcrossLanes<2>(dot_products));
    tmp[3] = _mm_mul(V[actRowRegIdx], BroadcastAcrossLanes<3>(dot_products));

    _registerType mtwo = _mm_set1<_registerType>(-2);
    // R[0] = _mm_fmadd(mtwo, tmp[0], R[0]);
    for (UST c = iteration + 1; c < _cols; ++c)
        R[c] = BlendAboveIndex<_regValueIdx>(_mm_fmadd(mtwo, tmp[c], R[c]), R[c]);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, Pivot _pivot>
template <U32 _regValueIdx, U32 _maxRecursionDepth>
inline void QRDenseSIMD<_registerType, _rows, _cols, _pivot>::FactorizationSteps(U32 regRowIdx, MatrixDataArray& V,
                                                                                 MatrixDataArray& R)
{
    using namespace GDL::simd;

    static_assert(_maxRecursionDepth <= numRegisterValues,
                  "_maxRecursionDepth must be equal or smaller than the number of register values.");

    const U32 iteration = regRowIdx * numRegisterValues + _regValueIdx;
    const U32 colStartIdx = iteration * numRegistersPerCol;

    //    if constexpr (_pivot != Pivot::NONE)
    //        PivotDenseSSE<_registerType, _rows>::template PivotingStepRegister<_regValueIdx, _pivot>(
    //                iteration, regRowIdx, factorization.mVR, factorization.mPermutationData);

    // Set unused values of last register to 0 if it is not already done by the pivoting step
    constexpr U32 numNonFullRegValues = _rows % numRegisterValues;

    if constexpr (numNonFullRegValues != 0 && _pivot == Pivot::NONE)
    {
        const _registerType zero = _mm_setzero<_registerType>();
        U32 nonFullRegIdx = colStartIdx + numRegistersPerCol - 1;
        R[nonFullRegIdx] = BlendBelowIndex<numNonFullRegValues - 1>(R[nonFullRegIdx], zero);
    }

    FactorizationStep<_regValueIdx>(iteration, regRowIdx, V, R);

    if constexpr (_regValueIdx + 1 < _maxRecursionDepth)
        FactorizationSteps<_regValueIdx + 1, _maxRecursionDepth>(regRowIdx, V, R);
}



} // namespace GDL::Solver
