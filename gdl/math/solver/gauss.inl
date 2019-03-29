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

    constexpr U32 numFullCollReisters = _size / numRegisterValues;

    // Size is known! MAYBE recursive template call to eliminate loop and enable more optimizations!
    for (U32 i = 0; i < numFullCollReisters; ++i)
    {
        GaussStepsRegister(i, matrixData);
    }

    // Missing: Non full registers at the end of a column!

    return VectorType();
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
inline U32 GaussDense<_registerType, _size>::FindPivot(U32 stepCount, const MatrixDataArray& matrixData)
{
    // Optimize: First compare all registers and apply result on value vector and index vector. Then store index
    // vector and value vector results and compare elements. Index vector just stores register index. The element
    // index can be calculated from register index and position in register.
    const U32 colIdx = stepCount * numColRegisters;

    std::array<ValueType, _size> values;
    std::memcpy(values.data(), &matrixData[colIdx], sizeof(ValueType) * _size);

    U32 pivotIdx = stepCount;
    values[pivotIdx] = std::abs(values[pivotIdx]);

    for (U32 j = stepCount + 1; j < _size; ++j)
    {
        values[j] = std::abs(values[j]);
        if (values[pivotIdx] < values[j])
            pivotIdx = j;
    }

    return pivotIdx;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void GaussDense<_registerType, _size>::PivotingStepRegister(U32 stepCount, U32 colRegIdx,
                                                                   MatrixDataArray& matrixData)
{
    U32 pivotIdx = FindPivot(stepCount, matrixData);
    SwapRows<_regValueIdx>(pivotIdx, stepCount, colRegIdx, matrixData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void GaussDense<_registerType, _size>::GaussStepsRegister(U32 colRegIdx, MatrixDataArray& matrixData)
{

    const U32 stepCount = colRegIdx * numRegisterValues + _regValueIdx;
    PivotingStepRegister<_regValueIdx>(stepCount, colRegIdx, matrixData);

    std::cout << MatrixType(matrixData) << std::endl;

    if constexpr (_regValueIdx + 1 < numRegisterValues)
        GaussStepsRegister<_regValueIdx + 1>(colRegIdx, matrixData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx>
inline void GaussDense<_registerType, _size>::SwapRows(U32 pivotIdx, U32 stepCount, U32 colRegIdx,
                                                       MatrixDataArray& matrixData)
{
    // Pivot element is already in place
    if (pivotIdx == stepCount)
        return;

    const U32 pivotColRegIdx = pivotIdx / numRegisterValues;

    // Special case: if _stepCount == numRegistersPerCol
    if (colRegIdx == pivotColRegIdx)
        SwapRowsSwitch<_regValueIdx, true>(pivotIdx, stepCount, pivotColRegIdx, colRegIdx, matrixData);
    else
        SwapRowsSwitch<_regValueIdx, false>(pivotIdx, stepCount, pivotColRegIdx, colRegIdx, matrixData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _regValueIdx, bool _swapSameReg>
inline void GaussDense<_registerType, _size>::SwapRowsSwitch(U32 pivotIdx, U32 stepCount, U32 pivotColRegIdx,
                                                             U32 colRegIdx, MatrixDataArray& matrixData)
{
    static_assert(numRegisterValues == 2 || numRegisterValues == 4 || numRegisterValues == 8,
                  "Only registers with 2,4 or 8 values are supported.");

    if constexpr (numRegisterValues == 2)
        switch (pivotIdx % numRegisterValues)
        {
        case 0:
            SwapRowsAllRegisters<_regValueIdx, 0, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 1:
            SwapRowsAllRegisters<_regValueIdx, 1, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        }

    if constexpr (numRegisterValues == 4)
        switch (pivotIdx % numRegisterValues)
        {
        case 0:
            SwapRowsAllRegisters<_regValueIdx, 0, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 1:
            SwapRowsAllRegisters<_regValueIdx, 1, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 2:
            SwapRowsAllRegisters<_regValueIdx, 2, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 3:
            SwapRowsAllRegisters<_regValueIdx, 3, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        }

    if constexpr (numRegisterValues == 8)
        switch (pivotIdx % numRegisterValues)
        {
        case 0:
            SwapRowsAllRegisters<_regValueIdx, 0, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 1:
            SwapRowsAllRegisters<_regValueIdx, 1, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 2:
            SwapRowsAllRegisters<_regValueIdx, 2, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 3:
            SwapRowsAllRegisters<_regValueIdx, 3, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 4:
            SwapRowsAllRegisters<_regValueIdx, 4, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 5:
            SwapRowsAllRegisters<_regValueIdx, 5, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 6:
            SwapRowsAllRegisters<_regValueIdx, 6, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        case 7:
            SwapRowsAllRegisters<_regValueIdx, 7, _swapSameReg>(stepCount, pivotColRegIdx, colRegIdx, matrixData);
            break;
        }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, I32 _size>
template <U32 _rowPiv, U32 _rowSwap, bool _swapSameReg>
inline void GaussDense<_registerType, _size>::SwapRowsAllRegisters(U32 stepCount, U32 pivotColRegIdx, U32 colRegIdx,
                                                                   MatrixDataArray& matrixData)
{
    if constexpr (_swapSameReg)
    {
        if constexpr (_rowPiv < _rowSwap)
            for (U32 i = stepCount * numColRegisters + colRegIdx; i < matrixData.size(); i += numColRegisters)
                matrixData[i] = sse::Swap<_rowPiv, _rowSwap>(matrixData[i]);
        else
            THROW("Internal index error");
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
    }
}



} // namespace GDL::Solver
