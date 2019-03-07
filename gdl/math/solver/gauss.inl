#pragma once

#include "gdl/math/solver/gauss.h"

#include "gdl/math/sse/matSSE.h"
#include "gdl/math/sse/vecSSE.h"



namespace GDL::Solver
{



template <typename _type, I32 _size>
VecSSE<_type, _size, true> GaussPartialPivot(const MatSSE<_type, _size, _size>& A, const VecSSE<_type, _size, true>& b)
{
    return GaussDense<_type, _size>::SolvePartialPivot(A, b);
}



template <typename _type, I32 _size>
inline typename GaussDense<_type, _size>::VectorType GaussDense<_type, _size>::SolvePartialPivot(const MatrixType& A,
                                                                                                 const VectorType& b)
{
    alignas(alignment) MatrixDataArray matrixData = A.DataSSE();



    // for (U32 i = 0; i < _size - 1; ++i)
    for (U32 i = 0; i < 1; ++i)
    {
        PivotingStep(i, matrixData);
    }

    std::cout << MatrixType(matrixData) << std::endl;
}



template <typename _type, I32 _size>
inline U32 GaussDense<_type, _size>::FindPivot(U32 stepCount, const MatrixDataArray& matrixData)
{
    // Optimize: First compare all registers and apply result on value vector and index vector. Then store index
    // vector and value vector results and compare elements. Index vector just stores register index. The element
    // index can be calculated from register index and position in register.
    const U32 colIdx = stepCount * numColRegisters;
    std::array<_type, _size> values;
    std::memcpy(values.data(), &matrixData[colIdx], sizeof(_type) * _size);

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



template <typename _type, I32 _size>
inline void GaussDense<_type, _size>::PivotingStep(U32 stepCount, MatrixDataArray& matrixData)
{
    U32 pivotIdx = FindPivot(stepCount, matrixData);
    SwapPivot(pivotIdx, stepCount, matrixData);
}



template <typename _type, I32 _size>
inline void GaussDense<_type, _size>::SwapPivot(U32 pivotIdx, U32 stepCount, MatrixDataArray& matrixData)
{
    // Pivot element is already in place
    if (pivotIdx == stepCount)
        return;

    const U32 colIdx = stepCount * numColRegisters;
    const U32 pivotRegisterIdx = pivotIdx / numRegisterValues + colIdx;
    const U32 currRegisterIdx = stepCount / numRegisterValues + colIdx;


    std::cout << currRegisterIdx << " / " << pivotRegisterIdx << std::endl;
}



template <typename _type, I32 _size>
inline void GaussDense<_type, _size>::SwapPivotInsideRegister(U32 Idx0, U32 Idx1, MatrixDataArray& matrixData)
{
}



template <typename _type, I32 _size>
inline void GaussDense<_type, _size>::SwapPivotAcrossRegisters(U32 Idx0, U32 Idx1, MatrixDataArray& matrixData)
{
}



} // namespace GDL::Solver
