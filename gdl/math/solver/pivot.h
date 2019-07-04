#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>


namespace GDL
{

// template <typename _type, I32, I32>
// class MatSerial;
// template <typename _type, I32, I32>
// class MatSIMD;
// template <typename _type, I32, bool>
// class VecSerial;
// template <typename _type, I32, bool>
// class VecSIMD;

namespace Solver
{



template <typename _type, I32 _size>
class PivotDenseSerial
{
    template <typename, I32>
    friend class GaussDenseSerial;

    using MatrixDataArray = std::array<_type, _size * _size>;
    using VectorDataArray = std::array<_type, _size>;

    //! @brief Finds the pivot element (maximum absolute value) inside of a column and returns the row where it is
    //! currently located.
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @return Row index of the pivot element
    inline static U32 FindPivot(U32 iteration, const MatrixDataArray& matData);

    //! @brief Performs the pivoting step for the given iteration
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    inline static void PivotingStep(U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);

    //! @brief Moves the row that contains the pivot element into the correct position.
    //! @param pivotRowIdx: Index of the row that contains the pivot element
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    inline static void SwapPivot(U32 pivotRowIdx, U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);
};



template <typename _registerType, I32 _size>
class PivotDenseSSE
{
    template <typename, I32>
    friend class GaussDenseSSE;

    static constexpr U32 alignment = simd::alignmentBytes<_registerType>;
    static constexpr U32 numRegisterValues = simd::numRegisterValues<_registerType>;
    static constexpr U32 numColRegisters = simd::CalcMinNumArrayRegisters<_registerType>(_size);

    using ValueType = decltype(simd::GetDataType<_registerType>());
    using MatrixDataArray = std::array<_registerType, numColRegisters * _size>;
    using VectorDataArray = std::array<_registerType, numColRegisters>;

    //! @brief Finds the pivot element (maximum absolute value) inside of a column and returns the row where it is
    //! currently located.
    //! @tparam _regValueIdx: Each column is composed of registers that contain multiple row values of that column. This
    //! parameter specifies the position of the value inside of the corresponding register that belongs to the current
    //! iterations active row.
    //! @param iteration: Number of the current iteration
    //! @param regRowIdx: This parameter specifies the position of the register that holds the value of the current
    //! iterations active row inside of each column of registers.
    //! @param matData: Matrix data array (column major ordering)
    //! @return Row index of the pivot element
    template <U32 _regValueIdx>
    inline static U32 FindPivot(U32 iteration, U32 regRowIdx, const MatrixDataArray& matData);

    //! @brief Performs the pivoting step for the given iteration
    //! @tparam _regValueIdx: Each column is composed of registers that contain multiple row values of that column. This
    //! parameter specifies the position of the value inside of the corresponding register that belongs to the current
    //! iterations active row.
    //! @param iteration: Number of the current iteration
    //! @param regRowIdx: This parameter specifies the position of the register that holds the value of the current
    //! iterations active row inside of each column of registers.
    //! iterations active row.
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <U32 _regValueIdx>
    inline static void PivotingStepRegister(U32 iteration, U32 regRowIdx, MatrixDataArray& matData,
                                            VectorDataArray& vecData);

    //! @brief Moves the row that contains the pivot element into the correct position.
    //! @tparam _regValueIdx: Each column is composed of registers that contain multiple row values of that column. This
    //! parameter specifies the position of the value inside of the corresponding register that belongs to the current
    //! iterations active row.
    //! @param pivotRowIdx: Index of the row that contains the pivot element
    //! @param iteration: Number of the current iteration
    //! @param regRowIdx: This parameter specifies the position of the register that holds the value of the current
    //! iterations active row inside of each column of registers.
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <U32 _regValueIdx>
    inline static void SwapPivot(U32 pivotRowIdx, U32 iteration, U32 regRowIdx, MatrixDataArray& matData,
                                 VectorDataArray& vecData);

    //! @brief Moves the row that contains the pivot element into the correct position by performing a swap operation
    //! for each relevant column.
    //! @tparam _regIdxDst: Each column is composed of registers that contain multiple row values of that column. This
    //! parameter specifies the position of the value inside of the corresponding register that belongs to the current
    //! iterations active row.
    //! @tparam _regIdxPiv: Each column is composed of registers that contain multiple row values of that column. This
    //! parameter specifies the position of the pivot element inside of the corresponding register.
    //! @tparam _sameReg: Specifies if the swap operation is performed inside the same register or not.
    //! @param iteration: Number of the current iteration
    //! @param pivotRegRowIdx: This parameter specifies the position of the register that holds the pivot element inside
    //! of each column of registers.
    //! @param regRowIdx: This parameter specifies the position of the register that holds the value of the current
    //! iterations active row inside of each column of registers.
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <U32 _regIdxDst, U32 _regIdxPiv, bool _sameReg>
    inline static void SwapPivotAllColumns(U32 iteration, U32 pivotRegRowIdx, U32 regRowIdx, MatrixDataArray& matData,
                                           VectorDataArray& vecData);

    //! @brief Swap operations for registers need to know the indices of the swapped elements at compile time. This
    //! function is just a switch to select the correct compile time swap function depending on the position of the
    //! pivot element that is determined at runtime.
    //! @tparam _regIdxDst: Each column is composed of registers that contain multiple row values of that column. This
    //! parameter specifies the position of the value inside of the corresponding register that belongs to the current
    //! iterations active row.
    //! @tparam _sameReg: Specifies if the swap operation is performed inside the same register or not.
    //! @param pivotRowIdx: Index of the row that contains the pivot element
    //! @param iteration: Number of the current iteration
    //! @param pivotRegRowIdx: This parameter specifies the position of the register that holds the pivot element inside
    //! of each column of registers.
    //! @param regRowIdx: This parameter specifies the position of the register that holds the value of the current
    //! iterations active row inside of each column of registers.
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <U32 _regValueIdx, bool _sameReg>
    inline static void SwapPivotSwitch(U32 pivotRowIdx, U32 iteration, U32 pivotRegRowIdx, U32 regRowIdx,
                                       MatrixDataArray& matData, VectorDataArray& vecData);
};


} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/pivot.inl"
