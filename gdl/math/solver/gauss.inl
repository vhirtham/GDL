#pragma once

#include "gdl/math/solver/gauss.h"

#include "gdl/base/sse/swizzle.h"
#include "gdl/math/sse/matSSE.h"
#include "gdl/math/sse/vecSSE.h"



namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, I32 _size>
VecSSE<_type, _size, true> GaussPartialPivot(const MatSSE<_type, _size, _size>& A, const VecSSE<_type, _size, true>& b)
{
    using RegisterType = typename MatSSE<_type, _size, _size>::RegisterType;
    return GaussDense<RegisterType, _size>::SolvePartialPivot(A, b);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
inline typename GaussDense<_registerType, _size>::VectorType
GaussDense<_registerType, _size>::SolvePartialPivot(const MatrixType& A, const VectorType& b)
{
    using namespace GDL::sse;

    alignas(alignment) MatrixDataArray matData = A.DataSSE();
    alignas(alignment) VectorDataArray vecData = b.DataSSE();

    constexpr U32 numRowsFullRegisters = _size / numRegisterValues;
    constexpr U32 numNonFullRegValues = _size % numRegisterValues;

    // Set memory of unused register values to zero -> Important for pivoting step
    if constexpr (numNonFullRegValues != 0)
        for (U32 i = numColRegisters - 1; i < _size * numColRegisters; i += numColRegisters)
            matData[i] = BlendBelowIndex<numNonFullRegValues - 1>(matData[i], _mmx_set1_p<_registerType>(0));

    // Perform Gauss steps for all registers that do not contain unused values
    for (U32 i = 0; i < numRowsFullRegisters; ++i)
        GaussStepsRegister(i, matData, vecData);

    // Perform Gauss steps for remaining rows that are stored in registers with unused values
    if constexpr (numNonFullRegValues != 0)
        GaussStepsRegister<0, numNonFullRegValues>(numColRegisters - 1, matData, vecData);


    return VectorType(vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void GaussDense<_registerType, _size>::EliminationStepRegister(U32 iteration, U32 regRowIdx,
                                                                      MatrixDataArray& matData,
                                                                      VectorDataArray& vecData)
{
    using namespace GDL::sse;

    const U32 colStartIdx = iteration * numColRegisters;
    const U32 actRowRegIdx = colStartIdx + regRowIdx;

    DEV_EXCEPTION(GetValue<_regValueIdx>(matData[actRowRegIdx]) == ApproxZero<ValueType>(1, 10),
                  "Singular matrix - system not solveable");



    // Calculate row multipliers
    alignas(alignmentBytes<_registerType>) std::array<_registerType, numColRegisters> rowMult;
    const _registerType one = _mmx_set1_p<_registerType>(1);
    const _registerType div = _mmx_div_p(one, BroadcastAcrossLanes<_regValueIdx>(matData[actRowRegIdx]));

    for (U32 i = 0; i < numColRegisters; ++i)
        if (i == actRowRegIdx % numColRegisters)
        {
            _registerType absPivM1 = _mmx_sub_p(matData[colStartIdx + i], one);
            rowMult[i] = _mmx_mul_p(div, BlendIndex<_regValueIdx>(matData[colStartIdx + i], absPivM1));
        }
        else
            rowMult[i] = _mmx_mul_p(div, matData[colStartIdx + i]);



    // Perform elimination for all relevant columns
    for (U32 i = colStartIdx + numColRegisters; i < _size * numColRegisters; i += numColRegisters)
    {
        _registerType bc = BroadcastAcrossLanes<_regValueIdx>(matData[regRowIdx + i]);
        for (U32 j = 0; j < numColRegisters; ++j)
        {
            const U32 regIdx = i + j;
            matData[regIdx] = _mmx_fnmadd_p(rowMult[j], bc, matData[regIdx]);
        }
    }
    _registerType bc = BroadcastAcrossLanes<_regValueIdx>(vecData[regRowIdx]);
    for (U32 i = 0; i < numColRegisters; ++i)
        vecData[i] = _mmx_fnmadd_p(rowMult[i], bc, vecData[i]);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline U32 GaussDense<_registerType, _size>::FindPivot(U32 iteration, U32 regRowIdx, const MatrixDataArray& matData)
{
    using namespace GDL::sse;

    const U32 colStartIdx = iteration * numColRegisters;


    // Vectorized comparisons
    _registerType zero = _mmx_setzero_p<_registerType>();
    _registerType cmpAbs = Abs(BlendAboveIndex<_regValueIdx>(matData[colStartIdx + regRowIdx], zero));
    _registerType cmpIdx = _mmx_set1_p<_registerType>(regRowIdx);
    for (U32 i = regRowIdx + 1; i < numColRegisters; ++i)
    {
        _registerType cmpAbs2 = Abs(matData[colStartIdx + i]);
        _registerType cmpRes = _mmx_cmplt_p(cmpAbs, cmpAbs2);
        cmpAbs = _mmx_blendv_p(cmpAbs, cmpAbs2, cmpRes);
        cmpIdx = _mmx_blendv_p(cmpIdx, _mmx_set1_p<_registerType>(i), cmpRes);
    }

    // Find pivot in result register
    alignas(alignment) std::array<ValueType, numRegisterValues> values2;
    _mmx_store_p(values2.data(), cmpAbs);
    ValueType maxVal = values2[0];
    U32 maxValIdx = 0;
    for (U32 i = 1; i < numRegisterValues; ++i)
        if (maxVal < values2[i])
        {
            maxVal = values2[i];
            maxValIdx = i;
        }


    return GetValue(cmpIdx, maxValIdx) * numRegisterValues + maxValIdx;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx, U32 _maxRecursionDepth>
inline void GaussDense<_registerType, _size>::GaussStepsRegister(U32 regRowIdx, MatrixDataArray& matData,
                                                                 VectorDataArray& vecData)
{
    static_assert(_maxRecursionDepth <= numRegisterValues,
                  "_maxRecursionDepth must be equal or smaller than the number of register values.");

    const U32 iteration = regRowIdx * numRegisterValues + _regValueIdx;
    PivotingStepRegister<_regValueIdx>(iteration, regRowIdx, matData, vecData);
    EliminationStepRegister<_regValueIdx>(iteration, regRowIdx, matData, vecData);

    if constexpr (_regValueIdx + 1 < _maxRecursionDepth)
        GaussStepsRegister<_regValueIdx + 1, _maxRecursionDepth>(regRowIdx, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void GaussDense<_registerType, _size>::PivotingStepRegister(U32 iteration, U32 regRowIdx,
                                                                   MatrixDataArray& matData, VectorDataArray& vecData)
{
    U32 pivotRowIdx = FindPivot<_regValueIdx>(iteration, regRowIdx, matData);

    DEV_EXCEPTION(pivotRowIdx >= _size, "Internal error. Pivot index bigger than matrix size.");

    SwapPivot<_regValueIdx>(pivotRowIdx, iteration, regRowIdx, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void GaussDense<_registerType, _size>::SwapPivot(U32 pivotRowIdx, U32 iteration, U32 regRowIdx,
                                                        MatrixDataArray& matData, VectorDataArray& vecData)
{
    // Pivot element is already in place
    if (pivotRowIdx == iteration)
        return;

    const U32 pivotRegRowIdx = pivotRowIdx / numRegisterValues;
    if (regRowIdx == pivotRegRowIdx)
        SwapPivotSwitch<_regValueIdx, true>(pivotRowIdx, iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
    else
        SwapPivotSwitch<_regValueIdx, false>(pivotRowIdx, iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regIdxDst, U32 _regIdxPiv, bool _sameReg>
inline void GaussDense<_registerType, _size>::SwapPivotAllColumns(U32 iteration, U32 pivotRegRowIdx, U32 regRowIdx,
                                                                  MatrixDataArray& matData, VectorDataArray& vecData)
{
    using namespace GDL::sse;

    if constexpr (_sameReg)
    {
        if constexpr (_regIdxDst < _regIdxPiv)
        {
            for (U32 i = iteration * numColRegisters + regRowIdx; i < matData.size(); i += numColRegisters)
                matData[i] = Swap<_regIdxDst, _regIdxPiv>(matData[i]);
            vecData[regRowIdx] = Swap<_regIdxDst, _regIdxPiv>(vecData[regRowIdx]);
        }
        else
            THROW("Internal index error - Probably singular matrix."); // LCOV_EXCL_LINE
    }
    else
    {
        DEV_EXCEPTION(pivotRegRowIdx <= regRowIdx,
                      "Internal error. Pivot register has equal or lower index than current register");
        DEV_EXCEPTION(pivotRegRowIdx / numColRegisters != regRowIdx / numColRegisters,
                      "Internal error. Registers are not in the same column.");

        U32 regDiff = pivotRegRowIdx - regRowIdx;

        for (U32 i = iteration * numColRegisters + regRowIdx; i < matData.size(); i += numColRegisters)
            Exchange<_regIdxDst, _regIdxPiv>(matData[i], matData[i + regDiff]);
        Exchange<_regIdxDst, _regIdxPiv>(vecData[regRowIdx], vecData[regRowIdx + regDiff]);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx, bool _sameReg>
inline void GaussDense<_registerType, _size>::SwapPivotSwitch(U32 pivotRowIdx, U32 iteration, U32 pivotRegRowIdx,
                                                              U32 regRowIdx, MatrixDataArray& matData,
                                                              VectorDataArray& vecData)
{
    static_assert(numRegisterValues == 2 || numRegisterValues == 4 || numRegisterValues == 8,
                  "Only registers with 2,4 or 8 values are supported.");

    if constexpr (numRegisterValues == 2)
        switch (pivotRowIdx % numRegisterValues)
        {
        case 0:
            SwapPivotAllColumns<_regValueIdx, 0, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 1:
            SwapPivotAllColumns<_regValueIdx, 1, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        }

    if constexpr (numRegisterValues == 4)
        switch (pivotRowIdx % numRegisterValues)
        {
        case 0:
            SwapPivotAllColumns<_regValueIdx, 0, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 1:
            SwapPivotAllColumns<_regValueIdx, 1, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 2:
            SwapPivotAllColumns<_regValueIdx, 2, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 3:
            SwapPivotAllColumns<_regValueIdx, 3, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        }

    if constexpr (numRegisterValues == 8)
        switch (pivotRowIdx % numRegisterValues)
        {
        case 0:
            SwapPivotAllColumns<_regValueIdx, 0, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 1:
            SwapPivotAllColumns<_regValueIdx, 1, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 2:
            SwapPivotAllColumns<_regValueIdx, 2, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 3:
            SwapPivotAllColumns<_regValueIdx, 3, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 4:
            SwapPivotAllColumns<_regValueIdx, 4, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 5:
            SwapPivotAllColumns<_regValueIdx, 5, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 6:
            SwapPivotAllColumns<_regValueIdx, 6, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        case 7:
            SwapPivotAllColumns<_regValueIdx, 7, _sameReg>(iteration, pivotRegRowIdx, regRowIdx, matData, vecData);
            break;
        }
}


} // namespace GDL::Solver
