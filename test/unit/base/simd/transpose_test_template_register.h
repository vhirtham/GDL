#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>



using namespace GDL;

// Test function ------------------------------------------------------------------------------------------------------

//! @brief Test all possible matrix transpositions for the given matrix size.
//! @tparam _registerType: Register type
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
template <typename _registerType, U32 _rows, U32 _cols>
void TestTranspose();



// Internal functions used by the test --------------------------------------------------------------------------------

//! @brief Recursive function that iterates over all possible values of the _firstRowIn template parameter.
//! @tparam _registerType: Register type
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register (increased recursively by the function)
//! @param in: Data array of the matrix that should be transposed
template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn = 0>
void TestTransposeSelectFirstRowIn(std::array<_registerType, _cols>& in);



//! @brief Recursive function that iterates over all possible values of the _firstRowOut template parameter.
//! @tparam _registerType: Register type
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register (increased recursively by the
//! function)
//! @param in: Data array of the matrix that should be transposed
template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut = 0>
void TestTransposeSelectFirstRowOut(std::array<_registerType, _cols>& in);



//! @brief Test all valid combinations of the _overwriteUnused and _unusedSetZero template parameters.
//! @tparam _registerType: Register type
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @param in: Data array of the matrix that should be transposed
template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut>
void TestTransposeTestAllOutputs(std::array<_registerType, _cols>& in);



//! @brief Perform a matrix transposition with the given setup and check the results.
//! @tparam _registerType: Register type
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Data array of the matrix that should be transposed
template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused,
          bool _unusedSetZero>
void TestTransposeTestCase(std::array<_registerType, _cols>& in);



//! @brief Select and execute the correct transposition function
//! @tparam _registerType: Register type
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Data array of the matrix that should be transposed
//! @param out: Data array that stores the result
template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused,
          bool _unusedSetZero>
void TransposeData(std::array<_registerType, _cols>& in, std::array<_registerType, _rows>& out);



//! @brief Check the results of the transposition
//! @tparam _registerType: Register type
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Data array of the matrix that should be transposed
//! @param out: Data array that stores the result
//! @param ref: State of the out variable before the transposition
template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused,
          bool _unusedSetZero>
void CheckResults(const std::array<_registerType, _cols>& in, const std::array<_registerType, _rows>& out,
                  const std::array<_registerType, _rows>& ref);



#include "test/unit/base/simd/transpose_test_template_register.inl"
