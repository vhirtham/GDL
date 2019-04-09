#pragma once

#include "gdl/base/fundamentalTypes.h"

#include "gdl/base/sse/utility.h"

#include <array>


namespace GDL
{

template <typename _type, I32 _size, bool _isCol>
class VecSSE;
template <typename _type, I32 _rows, I32 _cols>
class MatSSE;

namespace Solver
{



//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
template <typename _type, I32 _size>
VecSSE<_type, _size, true> GaussPartialPivot(const MatSSE<_type, _size, _size>& A, const VecSSE<_type, _size, true>& b);



//! @brief Gaussian elimination solver class for dense static systems
//! @tparam _type: Basic data type. Can either be F32 or F64
//! @tparam _size: Size of the linear system
template <typename _registerType, I32 _size>
class GaussDense
{
    // using RegisterType = decltype(sse::GetFittingRegister<_type, sse::MaxRegisterSize()>());
    constexpr static U32 alignment = sse::alignmentBytes<_registerType>;
    static constexpr U32 numRegisterValues = sse::numRegisterValues<_registerType>;
    static constexpr U32 numColRegisters = sse::CalcMinNumArrayRegisters<_registerType>(_size);


    using MatrixDataArray = std::array<_registerType, numColRegisters * _size>;
    using ValueType = decltype(sse::GetDataType<_registerType>());
    using VectorType = VecSSE<ValueType, _size, true>;
    using MatrixType = MatSSE<ValueType, _size, _size>;


public:
    //! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
    //! @param A: Matrix
    //! @param b: Vector
    //! @return Result vector x
    inline static VectorType SolvePartialPivot(const MatrixType& A, const VectorType& b);

private:
    template <U32 _idx>
    inline static _registerType BlendPivot(_registerType reg0, _registerType reg1);

    template <U32 _regValueIdx>
    inline static void EliminationStepRegister(U32 stepCount, U32 colRegIdx, MatrixDataArray& matrixData);

    template <U32 _regValueIdx = 0>
    inline static void GaussStepsRegister(U32 colRegIdx, MatrixDataArray& matrixData);

    inline static U32 FindPivot(U32 stepCount, const MatrixDataArray& matrixData);

    template <U32 _regValueIdx>
    inline static void PivotingStepRegister(U32 stepCount, U32 colRegIdx, MatrixDataArray& matrixData);

    template <U32 _regValueIdx>
    inline static void SwapRows(U32 pivotIdx, U32 stepCount, U32 colRegIdx, MatrixDataArray& matrixData);


    template <U32 _regValueIdx, bool _swapSameReg>
    inline static void SwapRowsSwitch(U32 pivotIdx, U32 stepCount, U32 pivotColRegIdx, U32 colRegIdx,
                                      MatrixDataArray& matrixData);

    template <U32 _rowPiv, U32 _rowSwap, bool _swapSameReg>
    inline static void SwapRowsAllRegisters(U32 stepCount, U32 pivotColRegIdx, U32 colRegIdx,
                                            MatrixDataArray& matrixData);
};


} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/gauss.inl"
