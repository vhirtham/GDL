#pragma once

#include "gdl/math/solver/gauss.h"

#include "gdl/base/sse/swizzle.h"
#include "gdl/math/sse/matSSE.h"
#include "gdl/math/sse/vecSSE.h"



namespace GDL::Solver
{



template <typename _type, I32 _size>
VecSSE<_type, _size, true> GaussPartialPivot(const MatSSE<_type, _size, _size>& A, const VecSSE<_type, _size, true>& b)
{
    using RegisterType = typename MatSSE<_type, _size, _size>::RegisterType;
    return GaussDense<RegisterType, _size>::SolvePartialPivot(A, b);
}



template <typename _registerType, I32 _size>
inline typename GaussDense<_registerType, _size>::VectorType
GaussDense<_registerType, _size>::SolvePartialPivot(const MatrixType& A, const VectorType& b)
{
    alignas(alignment) MatrixDataArray<> matrixData = A.DataSSE();



    // Size is known! MAYBE recursive template call to eliminate loop and enable more optimizations!
    for (U32 i = 0; i < 1; ++i)
    {
        PivotingStep(i, matrixData);
    }

    std::cout << MatrixType(matrixData) << std::endl;
}



template <typename _registerType, I32 _size>
inline U32 GaussDense<_registerType, _size>::FindPivot(U32 stepCount, const MatrixDataArray<>& matrixData)
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



template <typename _registerType, I32 _size>
template <U32 _rowIdx>
inline void GaussDense<_registerType, _size>::PivotingStepRegister(U32 regCount, MatrixDataArray<>& matrixData)
{
    U32 swapIdx = FindPivot(regCount * numRegisterValues + _rowIdx, matrixData);
    SwapPivot<_rowIdx>(swapIdx, regCount, matrixData);
}



template <typename _registerType, I32 _size>
inline void GaussDense<_registerType, _size>::PivotingStep(U32 regCount, MatrixDataArray<>& matrixData)
{
    PivotingStepRegister<0>(regCount, matrixData);
}



template <typename _registerType, I32 _size>
template <U32 _rowIdx>
inline void GaussDense<_registerType, _size>::SwapPivot(U32 swapIdx, U32 regCount, MatrixDataArray<>& matrixData)
{
    const U32 pivotIdx = regCount * numRegisterValues + _rowIdx;

    // Pivot element is already in place
    if (swapIdx == pivotIdx)
        return;


    const U32 swapColRegisterIdx = swapIdx / numRegisterValues;
    const U32 pivotColRegisterIdx = pivotIdx / numRegisterValues;

    // Special case: if _rowIdx == numRegistersPerCol
    if (pivotColRegisterIdx == swapColRegisterIdx)
        SwapPivotInsideRegister<_rowIdx>(swapIdx, regCount, matrixData);
    else
        THROW("Not implemented");
    // SwapPivotAcrossRegisters(pivotIdx, stepCount, matrixData);
}



template <typename _registerType, I32 _size>
template <U32 _rowIdx>
inline void GaussDense<_registerType, _size>::SwapPivotInsideRegister(U32 swapIdx, U32 regCount,
                                                                      MatrixDataArray<__m256>& matrixData)
{
    using namespace GDL::sse;

    static_assert(sse::Is__m256<_registerType>, "Invalid register type");

    swapIdx = swapIdx % numRegisterValues;

    U32 registerColIdx = regCount * numRegisterValues + _rowIdx;


    switch (swapIdx)
    {
    case 1:
        SwapMatrixRowsInsideRegister<_rowIdx, 1>(registerColIdx, matrixData);
        break;
    case 2:
        SwapMatrixRowsInsideRegister<_rowIdx, 2>(registerColIdx, matrixData);
        break;
    case 3:
        SwapMatrixRowsInsideRegister<_rowIdx, 3>(registerColIdx, matrixData);
        break;
    case 4:
        SwapMatrixRowsInsideRegister<_rowIdx, 4>(registerColIdx, matrixData);
        break;
    case 5:
        SwapMatrixRowsInsideRegister<_rowIdx, 5>(registerColIdx, matrixData);
        break;
    case 6:
        SwapMatrixRowsInsideRegister<_rowIdx, 6>(registerColIdx, matrixData);
        break;
    case 7:
        SwapMatrixRowsInsideRegister<_rowIdx, 7>(registerColIdx, matrixData);
        break;
    }
}



template <typename _registerType, I32 _size>
inline void GaussDense<_registerType, _size>::SwapPivotAcrossRegisters(U32 pivotIdx, U32 stepCount,
                                                                       MatrixDataArray<>& matrixData)
{
}



template <typename _registerType, I32 _size>
template <U32 _rowPiv, U32 _rowSwap>
inline void GaussDense<_registerType, _size>::SwapMatrixRowsInsideRegister(U32 registerColIdx,
                                                                           MatrixDataArray<>& matrixData)
{

    static_assert(_rowPiv < _rowSwap, "Index error");


    for (U32 i = registerColIdx; i < matrixData.size(); i += numColRegisters)
        matrixData[i] = sse::Swap<_rowPiv, _rowSwap>(matrixData[i]);
}



} // namespace GDL::Solver
