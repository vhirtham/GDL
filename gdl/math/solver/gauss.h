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
template <typename _type, I32 _size>
class GaussDense
{
    using RegisterType = decltype(sse::GetFittingRegister<_type, sse::MaxRegisterSize()>());
    constexpr static U32 alignment = sse::alignmentBytes<RegisterType>;
    static constexpr U32 numRegisterValues = sse::numRegisterValues<RegisterType>;
    static constexpr U32 numColRegisters = sse::CalcMinNumArrayRegisters<RegisterType>(_size);

    using MatrixDataArray = std::array<RegisterType, numColRegisters * _size>;
    using VectorType = VecSSE<_type, _size, true>;
    using MatrixType = MatSSE<_type, _size, _size>;


public:
    //! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
    //! @param A: Matrix
    //! @param b: Vector
    //! @return Result vector x
    inline static VectorType SolvePartialPivot(const MatrixType& A, const VectorType& b);

private:
    //! @brief Finds the pivot element of the current elimination step and returns its index
    //! @param stepCount: Number of performed elimination steps
    //! @param matrixData: Array containing the matrix data
    //! @return Index of the pivot element
    inline static U32 FindPivot(U32 stepCount, const MatrixDataArray& matrixData);

    //! @brief Performs the pivoting step of the gauss elimination procedure by modifying the passed data
    //! @param stepCount: Number of performed elimination steps
    //! @param matrixData: Array containing the matrix data
    inline static void PivotingStep(U32 stepCount, MatrixDataArray& matrixData);

    //! @brief Swaps the pivot element into the first row of the current elimination step
    //! @param pivotIdx: Current position of the pivot element
    //! @param stepCount: Number of performed elimination steps
    //! @param matrixData: Array containing the matrix data
    inline static void SwapPivot(U32 pivotIdx, U32 stepCount, MatrixDataArray& matrixData);

    inline static void SwapPivotAcrossRegisters(U32 Idx0, U32 Idx1, MatrixDataArray& matrixData);

    inline static void SwapPivotInsideRegister(U32 Idx0, U32 Idx1, MatrixDataArray& matrixData);
};


} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/gauss.inl"
