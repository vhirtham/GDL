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
    alignas(alignment) MatrixDataArray matrixData = A.DataSSE();
    alignas(alignment) VectorDataArray vectorData = b.DataSSE();

    constexpr U32 numFullColRegisters = _size / numRegisterValues;
    constexpr U32 numNonFullRegValues = _size % numRegisterValues;

    if constexpr (numNonFullRegValues != 0)
        for (U32 i = numColRegisters - 1; i < _size * numColRegisters; i += numColRegisters)
            matrixData[i] = BlendBelowIndex<numNonFullRegValues - 1>(matrixData[i], _mmx_set1_p<_registerType>(0));



    for (U32 i = 0; i < numFullColRegisters; ++i)
        GaussStepsRegister(i, matrixData, vectorData);

    if constexpr (numNonFullRegValues != 0)
        GaussStepsRegister<0, numNonFullRegValues>(numColRegisters - 1, matrixData, vectorData);


    return VectorType(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _idx>
inline _registerType GaussDense<_registerType, _size>::BlendIndex(_registerType reg0, _registerType reg1)
{
    using namespace GDL::sse;

    static_assert(numRegisterValues == 2 || numRegisterValues == 4 || numRegisterValues == 8,
                  "Only registers with 2, 4 or 8 values are supported.");

    if constexpr (numRegisterValues == 2)
    {
        constexpr U32 b0 = (_idx == 0) ? 1 : 0;
        constexpr U32 b1 = (_idx == 1) ? 1 : 0;

        return Blend<b0, b1>(reg0, reg1);
    }
    if constexpr (numRegisterValues == 4)
    {
        constexpr U32 b0 = (_idx == 0) ? 1 : 0;
        constexpr U32 b1 = (_idx == 1) ? 1 : 0;
        constexpr U32 b2 = (_idx == 2) ? 1 : 0;
        constexpr U32 b3 = (_idx == 3) ? 1 : 0;

        return Blend<b0, b1, b2, b3>(reg0, reg1);
    }
    if constexpr (numRegisterValues == 8)
    {
        constexpr U32 b0 = (_idx == 0) ? 1 : 0;
        constexpr U32 b1 = (_idx == 1) ? 1 : 0;
        constexpr U32 b2 = (_idx == 2) ? 1 : 0;
        constexpr U32 b3 = (_idx == 3) ? 1 : 0;
        constexpr U32 b4 = (_idx == 4) ? 1 : 0;
        constexpr U32 b5 = (_idx == 5) ? 1 : 0;
        constexpr U32 b6 = (_idx == 6) ? 1 : 0;
        constexpr U32 b7 = (_idx == 7) ? 1 : 0;

        return Blend<b0, b1, b2, b3, b4, b5, b6, b7>(reg0, reg1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _idx>
inline _registerType GaussDense<_registerType, _size>::BlendAboveIndex([[maybe_unused]] _registerType reg0,
                                                                       [[maybe_unused]] _registerType reg1)
{
    using namespace GDL::sse;

    static_assert(numRegisterValues == 2 || numRegisterValues == 4 || numRegisterValues == 8,
                  "Only registers with 2, 4 or 8 values are supported.");

    if constexpr (_idx == 0)
        return reg0;
    else if constexpr (_idx >= numRegisterValues)
        return reg1;
    else if constexpr (numRegisterValues == 2)
    {
        constexpr U32 b0 = (_idx > 0) ? 1 : 0;
        constexpr U32 b1 = (_idx > 1) ? 1 : 0;

        return Blend<b0, b1>(reg0, reg1);
    }
    else if constexpr (numRegisterValues == 4)
    {
        constexpr U32 b0 = (_idx > 0) ? 1 : 0;
        constexpr U32 b1 = (_idx > 1) ? 1 : 0;
        constexpr U32 b2 = (_idx > 2) ? 1 : 0;
        constexpr U32 b3 = (_idx > 3) ? 1 : 0;

        return Blend<b0, b1, b2, b3>(reg0, reg1);
    }
    else if constexpr (numRegisterValues == 8)
    {
        constexpr U32 b0 = (_idx > 0) ? 1 : 0;
        constexpr U32 b1 = (_idx > 1) ? 1 : 0;
        constexpr U32 b2 = (_idx > 2) ? 1 : 0;
        constexpr U32 b3 = (_idx > 3) ? 1 : 0;
        constexpr U32 b4 = (_idx > 4) ? 1 : 0;
        constexpr U32 b5 = (_idx > 5) ? 1 : 0;
        constexpr U32 b6 = (_idx > 6) ? 1 : 0;
        constexpr U32 b7 = (_idx > 7) ? 1 : 0;

        return Blend<b0, b1, b2, b3, b4, b5, b6, b7>(reg0, reg1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _idx>
inline _registerType GaussDense<_registerType, _size>::BlendBelowIndex([[maybe_unused]] _registerType reg0,
                                                                       [[maybe_unused]] _registerType reg1)
{
    using namespace GDL::sse;

    static_assert(numRegisterValues == 2 || numRegisterValues == 4 || numRegisterValues == 8,
                  "Only registers with 2, 4 or 8 values are supported.");

    constexpr U32 b0 = 0;

    if constexpr (_idx >= numRegisterValues)
        return reg0;
    else if constexpr (numRegisterValues == 2)
    {
        constexpr U32 b1 = (_idx < 1) ? 1 : 0;

        return Blend<b0, b1>(reg0, reg1);
    }
    else if constexpr (numRegisterValues == 4)
    {
        constexpr U32 b1 = (_idx < 1) ? 1 : 0;
        constexpr U32 b2 = (_idx < 2) ? 1 : 0;
        constexpr U32 b3 = (_idx < 3) ? 1 : 0;

        return Blend<b0, b1, b2, b3>(reg0, reg1);
    }
    else if constexpr (numRegisterValues == 8)
    {
        constexpr U32 b1 = (_idx < 1) ? 1 : 0;
        constexpr U32 b2 = (_idx < 2) ? 1 : 0;
        constexpr U32 b3 = (_idx < 3) ? 1 : 0;
        constexpr U32 b4 = (_idx < 4) ? 1 : 0;
        constexpr U32 b5 = (_idx < 5) ? 1 : 0;
        constexpr U32 b6 = (_idx < 6) ? 1 : 0;
        constexpr U32 b7 = (_idx < 7) ? 1 : 0;

        return Blend<b0, b1, b2, b3, b4, b5, b6, b7>(reg0, reg1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void GaussDense<_registerType, _size>::EliminationStepRegister(U32 stepCount, U32 colRegIdx,
                                                                      MatrixDataArray& matrixData,
                                                                      VectorDataArray& vectorData)
{
    using namespace GDL::sse;

    const U32 colStartIdx = stepCount * numColRegisters;
    const U32 pivRegIdx = colStartIdx + colRegIdx;

    DEV_EXCEPTION(GetValue<_regValueIdx>(matrixData[pivRegIdx]) == ApproxZero<ValueType>(1, 10),
                  "Singular matrix - system not solveable");



    // Calculate row multipliers
    alignas(alignmentBytes<_registerType>) std::array<_registerType, numColRegisters> rowMult;
    const _registerType one = _mmx_set1_p<_registerType>(1);
    const _registerType div = _mmx_div_p(one, BroadcastAcrossLanes<_regValueIdx>(matrixData[pivRegIdx]));

    for (U32 i = 0; i < numColRegisters; ++i)
        if (i == pivRegIdx % numColRegisters)
        {
            _registerType absPivM1 = _mmx_sub_p(matrixData[colStartIdx + i], one);
            rowMult[i] = _mmx_mul_p(div, BlendIndex<_regValueIdx>(matrixData[colStartIdx + i], absPivM1));
        }
        else
            rowMult[i] = _mmx_mul_p(div, matrixData[colStartIdx + i]);



    // Perform elimination
    for (U32 i = colStartIdx + numColRegisters; i < _size * numColRegisters; i += numColRegisters)
    {
        _registerType bc = BroadcastAcrossLanes<_regValueIdx>(matrixData[colRegIdx + i]);
        for (U32 j = 0; j < numColRegisters; ++j)
        {
            const U32 regIdx = i + j;
            matrixData[regIdx] = _mmx_fnmadd_p(rowMult[j], bc, matrixData[regIdx]);
        }
    }
    _registerType bc = BroadcastAcrossLanes<_regValueIdx>(vectorData[colRegIdx]);
    for (U32 i = 0; i < numColRegisters; ++i)
        vectorData[i] = _mmx_fnmadd_p(rowMult[i], bc, vectorData[i]);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline U32 GaussDense<_registerType, _size>::FindPivot(U32 stepCount, U32 colRegIdx, const MatrixDataArray& matrixData)
{


    const U32 colIdx = stepCount * numColRegisters;

    //    std::array<ValueType, _size> values;
    //    std::memcpy(values.data(), &matrixData[colIdx], sizeof(ValueType) * _size);

    //    U32 pivotIdx = stepCount;
    //    values[pivotIdx] = std::abs(values[pivotIdx]);

    //    for (U32 j = stepCount + 1; j < _size; ++j)
    //    {
    //        values[j] = std::abs(values[j]);
    //        if (values[pivotIdx] < values[j])
    //            pivotIdx = j;
    //    }

    _registerType zero = _mmx_setzero_p<_registerType>();
    _registerType cmpAbs = sse::Abs(BlendAboveIndex<_regValueIdx>(matrixData[colIdx + colRegIdx], zero));
    _registerType cmpIdx = _mmx_set1_p<_registerType>(colRegIdx);
    for (U32 i = colRegIdx + 1; i < numColRegisters; ++i)
    {
        _registerType cmpAbs2 = sse::Abs(matrixData[colIdx + i]);
        _registerType cmpRes = _mmx_cmplt_p(cmpAbs, cmpAbs2);
        cmpAbs = _mmx_blendv_p(cmpAbs, cmpAbs2, cmpRes);
        cmpIdx = _mmx_blendv_p(cmpIdx, _mmx_set1_p<_registerType>(i), cmpRes);
    }

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

    // U32 pivotIdx2 = sse::GetValue(cmpIdx, maxValIdx) * numRegisterValues + maxValIdx;

    //    std::cout << VecSSE<ValueType, numRegisterValues>(std::array<_registerType, 1>{cmpIdx}) << std::endl;
    // std::cout << pivotIdx << "/" << pivotIdx2 << std::endl;

    // return pivotIdx;
    return sse::GetValue(cmpIdx, maxValIdx) * numRegisterValues + maxValIdx;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void GaussDense<_registerType, _size>::PivotingStepRegister(U32 stepCount, U32 colRegIdx,
                                                                   MatrixDataArray& matrixData,
                                                                   VectorDataArray& vectorData)
{
    U32 pivotIdx = FindPivot<_regValueIdx>(stepCount, colRegIdx, matrixData);

    DEV_EXCEPTION(pivotIdx >= _size, "Internal error. Pivot index bigger than matrix size.");

    SwapRows<_regValueIdx>(pivotIdx, stepCount, colRegIdx, matrixData, vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx, U32 _maxSubsteps>
inline void GaussDense<_registerType, _size>::GaussStepsRegister(U32 colRegIdx, MatrixDataArray& matrixData,
                                                                 VectorDataArray& vectorData)
{

    const U32 stepCount = colRegIdx * numRegisterValues + _regValueIdx;
    PivotingStepRegister<_regValueIdx>(stepCount, colRegIdx, matrixData, vectorData);
    EliminationStepRegister<_regValueIdx>(stepCount, colRegIdx, matrixData, vectorData);

    //    // Temporary output, remove if finished implementation
    //    for (U32 i = 0; i < matrixData.size(); ++i)
    //    {
    //        for (U32 u = 0; u < numRegisterValues; ++u)
    //        {
    //            if (sse::GetValue(matrixData[i], u) == ApproxZero<F32>(100))
    //                sse::SetValue(matrixData[i], u, 0);
    //        }
    //    }
    //    std::cout << MatrixType(matrixData) << std::endl;
    //    std::cout << VectorType(vectorData) << std::endl;

    if constexpr (_regValueIdx + 1 < numRegisterValues && _regValueIdx + 1 < _maxSubsteps)
        GaussStepsRegister<_regValueIdx + 1, _maxSubsteps>(colRegIdx, matrixData, vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void GaussDense<_registerType, _size>::SwapRows(U32 pivotIdx, U32 stepCount, U32 colRegIdx,
                                                       MatrixDataArray& matrixData, VectorDataArray& vectorData)
{
    // Pivot element is already in place
    if (pivotIdx == stepCount)
        return;

    const U32 pivotColRegIdx = pivotIdx / numRegisterValues;

    // Special case: if _stepCount == numRegistersPerCol
    if (colRegIdx == pivotColRegIdx)
        SwapRowsSwitch<_regValueIdx, true>(pivotIdx, stepCount, pivotColRegIdx, colRegIdx, matrixData, vectorData);
    else
        SwapRowsSwitch<_regValueIdx, false>(pivotIdx, stepCount, pivotColRegIdx, colRegIdx, matrixData, vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx, bool _swapSameReg>
inline void GaussDense<_registerType, _size>::SwapRowsSwitch(U32 pivotIdx, U32 stepCount, U32 pivotColRegIdx,
                                                             U32 colRegIdx, MatrixDataArray& matrixData,
                                                             VectorDataArray& vectorData)
{
    static_assert(numRegisterValues == 2 || numRegisterValues == 4 || numRegisterValues == 8,
                  "Only registers with 2,4 or 8 values are supported.");

    if constexpr (numRegisterValues == 2)
        switch (pivotIdx % numRegisterValues)
        {
        case 0:
            SwapRowsAllRegisters<_regValueIdx, 0, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 1:
            SwapRowsAllRegisters<_regValueIdx, 1, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        }

    if constexpr (numRegisterValues == 4)
        switch (pivotIdx % numRegisterValues)
        {
        case 0:
            SwapRowsAllRegisters<_regValueIdx, 0, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 1:
            SwapRowsAllRegisters<_regValueIdx, 1, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 2:
            SwapRowsAllRegisters<_regValueIdx, 2, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 3:
            SwapRowsAllRegisters<_regValueIdx, 3, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        }

    if constexpr (numRegisterValues == 8)
        switch (pivotIdx % numRegisterValues)
        {
        case 0:
            SwapRowsAllRegisters<_regValueIdx, 0, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 1:
            SwapRowsAllRegisters<_regValueIdx, 1, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 2:
            SwapRowsAllRegisters<_regValueIdx, 2, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 3:
            SwapRowsAllRegisters<_regValueIdx, 3, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 4:
            SwapRowsAllRegisters<_regValueIdx, 4, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 5:
            SwapRowsAllRegisters<_regValueIdx, 5, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 6:
            SwapRowsAllRegisters<_regValueIdx, 6, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        case 7:
            SwapRowsAllRegisters<_regValueIdx, 7, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData,
                                                                vectorData);
            break;
        }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _rowPiv, U32 _rowSwap, bool _swapSameReg>
inline void GaussDense<_registerType, _size>::SwapRowsAllRegisters(U32 stepCount, U32 pivotColRegIdx, U32 colRegIdx,
                                                                   MatrixDataArray& matrixData,
                                                                   VectorDataArray& vectorData)
{
    if constexpr (_swapSameReg)
    {
        if constexpr (_rowPiv < _rowSwap)
        {
            for (U32 i = stepCount * numColRegisters + colRegIdx; i < matrixData.size(); i += numColRegisters)
                matrixData[i] = sse::Swap<_rowPiv, _rowSwap>(matrixData[i]);
            vectorData[colRegIdx] = sse::Swap<_rowPiv, _rowSwap>(vectorData[colRegIdx]);
        }
        else
            THROW("Internal index error - Probably singular matrix.");
    }
    else
    {
        DEV_EXCEPTION(pivotColRegIdx <= colRegIdx,
                      "Internal error. Pivot register has equal or lower index than current register");
        DEV_EXCEPTION(pivotColRegIdx / numColRegisters != colRegIdx / numColRegisters,
                      "Internal error. Registers are not in the same column.");

        U32 regDiff = pivotColRegIdx - colRegIdx;

        for (U32 i = stepCount * numColRegisters + colRegIdx; i < matrixData.size(); i += numColRegisters)
            sse::Exchange<_rowPiv, _rowSwap>(matrixData[i], matrixData[i + regDiff]);
        sse::Exchange<_rowPiv, _rowSwap>(vectorData[colRegIdx], vectorData[colRegIdx + regDiff]);
    }
}



} // namespace GDL::Solver
