#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>


namespace GDL::Solver
{



//! @brief Helper class for pivoting strategies that are shared amongst multiple solvers
//! @tparam _type: Data type of the system
//! @tparam _size: Size of the system
template <typename _type, U32 _size>
class PivotDenseSerial
{
    template <typename, U32, Pivot>
    friend class GaussDenseSerial;
    template <typename, U32, Pivot>
    friend class LUDenseSerial;



    using MatrixDataArray = std::array<_type, _size * _size>;
    using VectorDataArray = std::array<_type, _size>;



    //! @brief Finds the maximum absolute value inside of the column containing the pivot element and returns its index.
    //! Rows above the pivot element are not considered
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @return Index of the row containing the maximum absolute value
    inline static U32 FindMaxAbsValueCol(U32 iteration, const MatrixDataArray& matData);

    //! @brief Performs a partial pivoting step for the given iteration
    //! @tparam _swapAllCols: If FALSE, row elements in columns left of the pivot element are not swapped
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <bool _swapAllCols>
    inline static void PartialPivotingStep(U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);

    //! @brief Permutes a vector with the passed permutation data and returns it
    //! @param vectorData: Vector data
    //! @param permutation: Permutation data
    //! @return Permuted vector
    static inline VectorDataArray PermuteVector(const VectorDataArray& vectorData, const VectorDataArray& permutation);

    //! @brief Performs the pivoting step for the given iteration
    //! @tparam _pivot: Enum to select the pivoting strategy
    //! @tparam _swapAllCols: If FALSE, row elements in columns left of the pivot element are not swapped
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <Pivot _pivot, bool _swapAllCols>
    inline static void PivotingStep(U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);

    //! @brief Swaps the row of the pivot element with the row of the value that should replace the current pivot
    //! element
    //! @tparam _swapAllCols: If FALSE, row elements in columns left of the pivot element are not swapped
    //! @param rowIdxSwp: Index of the row that will be swapped with the pivot elements row
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <bool _swapAllCols>
    inline static void SwapRowPivot(U32 rowIdxSwp, U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);
};



} // namespace GDL::Solver



#include "gdl/math/solver/internal/pivotDenseSerial.inl"
