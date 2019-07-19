#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
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

    //! @brief Finds the pivot element (maximum absolute value) inside of a column and returns the row where it is
    //! currently located.
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @return Row index of the pivot element
    inline static U32 FindPivotPartial(U32 iteration, const MatrixDataArray& matData);

    //! @brief Performs a partial pivoting step for the given iteration
    //! @tparam _swapAllRows: Defines if the rows should be wapped in every column or just in the column to the right of
    //! the pivot element and the one of the pivot element itself.
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <bool _swapAllRows>
    inline static void PartialPivotingStep(U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);

    //! @brief Applies a permutation to a vector
    //! @param vec: Original vector
    //! @param permutation: Permutation data
    //! @return Permuted vector
    static inline VectorDataArray PermuteVector(const VectorDataArray& vec, const VectorDataArray& permutation);

    //! @brief Performs the pivoting step for the given iteration
    //! @tparam _pivot: Enum to select the pivoting strategy
    //! @tparam _swapAllRows: Defines if the rows should be wapped in every column or just in the column to the right of
    //! the pivot element and the one of the pivot element itself.
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <Pivot _pivot, bool _swapAllRows>
    inline static void PivotingStep(U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);

    //! @brief Moves the row that contains the pivot element into the correct position.
    //! @tparam _swapAllRows: Defines if the rows should be wapped in every column or just in the column to the right of
    //! the pivot element and the one of the pivot element itself.
    //! @param pivotRowIdx: Index of the row that contains the pivot element
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <bool _swapAllRows>
    inline static void SwapRows(U32 pivotRowIdx, U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);
};



} // namespace GDL::Solver




#include "gdl/math/solver/internal/pivotDenseSerial.inl"
