#pragma once


#include <gdl/base/fundamentalTypes.h>

#include <array>



using namespace GDL;



//! @brief Test different values for the general transpose functions template parameters that control the offset of the
//! first matrix register inside their corresponding arrays.
//! @tparam _registerType: Register type
//! @tparam rows: Number of rows of the matrix
//! @tparam cols: Number of columns of the matrix
//! @tparam _maxOffset: Maximal offset value per input and output array that should be tested.
template <typename _registerType, U32 _rows, U32 _cols, U32 _maxOffset = 2>
inline void TestArrayOffset();



//! @brief Subfunction of the array offset test. Increases the offset inside the input array.
//! @tparam rows: Number of rows of the matrix
//! @tparam cols: Number of columns of the matrix
//! @tparam _maxOffset: Maximal offset value per input and output array that should be tested.
//! @tparam _columnOffsetIn: Offset inside the input array.
//! @tparam _registerType: Register type
//! @tparam _inputArraySize: Size of the input array
//! @param input: Input array (matrix before transposition)
template <U32 _rows, U32 _cols, U32 _maxOffset, U32 _columnOffsetIn = 0, typename _registerType, UST _inputArraySize>
inline void TestArrayOffsetIterateInputOffset(std::array<_registerType, _inputArraySize> input);



//! @brief Subfunction of the array offset test. Increases the offset inside the output array.
//! @tparam rows: Number of rows of the matrix
//! @tparam cols: Number of columns of the matrix
//! @tparam _maxOffset: Maximal offset value per input and output array that should be tested.
//! @tparam _columnOffsetIn: Offset inside the input array.
//! @tparam _columnOffsetIn: Offset inside the Output array.
//! @tparam _registerType: Register type
//! @tparam _inputArraySize: Size of the input array
//! @param input: Input array (matrix before transposition)
template <U32 _rows, U32 _cols, U32 _maxOffset, U32 _columnOffsetIn, U32 _columnOffsetOut = 0, typename _registerType,
          UST _inputArraySize>
inline void TestArrayOffsetIterateOutputOffset(std::array<_registerType, _inputArraySize> input);



//! @brief Subfunction of the array offset test. Performs the actual test with a fixed set of parameters.
//! @tparam rows: Number of rows of the matrix
//! @tparam cols: Number of columns of the matrix
//! @tparam _columnOffsetIn: Offset inside the input array.
//! @tparam _columnOffsetIn: Offset inside the Output array.
//! @tparam _registerType: Register type
//! @tparam _inputArraySize: Size of the input array
//! @param input: Input array (matrix before transposition)
template <U32 _rows, U32 _cols, U32 _columnOffsetIn, U32 _columnOffsetOut, typename _registerType, UST _inputArraySize>
inline void TestArrayOffsetTestCase(std::array<_registerType, _inputArraySize> input);



//! @brief Test different values for the general transpose functions template parameters that control the distance of
//! the matrix registers inside their corresponding arrays.
//! @tparam _registerType: Register type
//! @tparam rows: Number of rows of the matrix
//! @tparam cols: Number of columns of the matrix
//! @tparam _numStrides: Number of stride values per input and output array that should be tested.
template <typename _registerType, U32 _rows, U32 _cols, U32 _numStrides = 3>
inline void TestArrayStride();



//! @brief Subfunction of the array stride test. Increases the stride inside the input array.
//! @tparam rows: Number of rows of the matrix
//! @tparam cols: Number of columns of the matrix
//! @tparam _numStrides: Number of stride values per input and output array that should be tested.
//! @tparam _columnStrideIn: Stride inside the input array.
//! @tparam _registerType: Register type
//! @tparam _inputArraySize: Size of the input array
//! @param input: Input array (matrix before transposition)
template <U32 _rows, U32 _cols, U32 _numStrides, U32 _columnStrideIn = 1, typename _registerType, UST _inputArraySize>
inline void TestArrayStrideIterateInputStride(std::array<_registerType, _inputArraySize> input);



//! @brief Subfunction of the array stride test. Increases the stride inside the output array.
//! @tparam rows: Number of rows of the matrix
//! @tparam cols: Number of columns of the matrix
//! @tparam _numStrides: Number of stride values per input and output array that should be tested.
//! @tparam _columnStrideIn: Stride inside the input array.
//! @tparam _columnStrideOut: Stride inside the output array.
//! @tparam _registerType: Register type
//! @tparam _inputArraySize: Size of the input array
//! @param input: Input array (matrix before transposition)
template <U32 _rows, U32 _cols, U32 _numStrides, U32 _columnStrideIn, U32 _columnStrideOut = 1, typename _registerType,
          UST _inputArraySize>
inline void TestArrayStrideIterateOutputStride(std::array<_registerType, _inputArraySize> input);



//! @brief Subfunction of the array stride test. Performs the actual test with a fixed set of parameters.
//! @tparam rows: Number of rows of the matrix
//! @tparam cols: Number of columns of the matrix
//! @tparam _columnStrideIn: Stride inside the input array.
//! @tparam _columnStrideOut: Stride inside the output array.
//! @tparam _registerType: Register type
//! @tparam _inputArraySize: Size of the input array
//! @param input: Input array (matrix before transposition)
template <U32 _rows, U32 _cols, U32 _columnStrideIn, U32 _columnStrideOut, typename _registerType, UST _inputArraySize>
inline void TestArrayStrideTestCase(std::array<_registerType, _inputArraySize> input);

// helpers
// ------------------------------------------------------------------------------------------------------------

//! @brief Check the results of the transposition
//! @tparam rows: Number of rows of the matrix
//! @tparam cols: Number of columns of the matrix
//! @tparam _firstRowIn: Index of the source matrix's first row elements in their corresponding registers.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @tparam colStartIn: Index of the first matrix register inside the input array
//! @tparam colStartOut: Index of the first matrix register inside the output array
//! @tparam colStrideIn: Distance between to matrix registers inside the input array
//! @tparam colStrideOut: Distance between to matrix registers inside the output array
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be
//! overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _checkUnusedCols: If "true", unused colum registers are checked to be unmodified.
//! @tparam _inputArraySize: Input array size
//! @tparam _outputArraySize: Output array size
//! @tparam _registerType: Register type
//! @param in: Input array (matrix before transposition)
//! @param in: Output array (transposed matrix)
//! @param ref: Reference array - holds the values of the output array, before the transposed matrix was written
//! to it.
template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, U32 _colStartIn, U32 _colStartOut, U32 _colStrideIn,
          U32 _colStrideOut, bool _overwriteUnused, bool _unusedSetZero, bool _checkUnusedCols = true,
          UST _inputArraySize, UST _outputArraySize, typename _registerType>
inline void CheckResults(const std::array<_registerType, _inputArraySize>& in,
                         const std::array<_registerType, _outputArraySize>& out,
                         const std::array<_registerType, _outputArraySize>& ref);



//! @brief Create the default input array
//! @tparam _registerType: Register type
//! @tparam _arraySize: Array size
//! @return Default input array
template <typename _registerType, UST _arraySize>
[[nodiscard]] std::array<_registerType, _arraySize> GetDefaultInputArray();



//! @brief Create the default reference array
//! @tparam _registerType: Register type
//! @tparam _arraySize: Array size
//! @return Default reference array
template <typename _registerType, UST _arraySize>
[[nodiscard]] std::array<_registerType, _arraySize> GetDefaultReferenceArray();



#include "test/unit/base/simd/transposeTest.inl"
