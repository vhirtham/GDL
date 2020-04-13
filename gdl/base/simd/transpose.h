#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"

#include <array>

namespace GDL::simd
{

// TODO
// documentation of test
// add static assert for _firstCol + _rows < regVals
// add static asserts to all transpose functions, -> own function, including:
//  static_assert(not(_overwriteUnused == false && _unusedSetZero == true), "Option _unusedSetZero has no effect.");

// check possible optimization of __m128 transpose 2x2 using Insert
// check optimization of lane permutations for laneoffsetIn >=2 in __m256 version

// test all brancehes of the general transpose function
// replace Permute2F128<1,0> with SwapLanes
// move SwapLanes(If) to swizzle header and write a test
// add Permute2F128If -> like SwapLanesIf --- just let Permute2F128<0,1> return the input
// Add header entries and docstrings of internal functions
// Move internal functions to the end of the files

//! @brief Transposes a matrix represented by an column major register array. The maximal supported matrix size is NxN
//! where N is the number of values in a register. The returned array has the minimal size required to store the result.
//! @tparam _registerType: Register type
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the matrix's first column inside of the passed array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the array. Maximum size is equal to the number of values per register.
//! @param matDataI: Array containing the matrix data
//! @return Minimum sized array containing the data of the transposed matrix
template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn = 0, U32 _firstColIn = 0,
          U32 _firstRowOut = _firstRowIn, bool _unusedSetZero = false, U32 _colStrideIn = 1, U32 _colStrideOut = 1,
          std::size_t _arrSizeIn>
inline auto Transpose(const std::array<_registerType, _arrSizeIn>& matDataI);

//! @brief Transposes a matrix represented by an column major register array. The maximal supported matrix size is 2x2.
//! The returned array has the minimal size required to store the result.
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the matrix's first column inside of the passed array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the array. Maximum size is 2.
//! @param matDataI: Array containing the matrix data
//! @return Minimum sized array containing the data of the transposed matrix
template <U32 _rows = 2, U32 _cols = 2, U32 _firstRowIn = 0, U32 _firstColIn = 0, U32 _firstRowOut = _firstRowIn,
          bool _unusedSetZero = false, U32 _colStrideIn = 1, U32 _colStrideOut = 1, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m128d, _arrSizeIn>& matDataI);

//! @brief Transposes a matrix represented by an column major register array. The maximal supported matrix size is 4x4.
//! The returned array has the minimal size required to store the result.
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the matrix's first column inside of the passed array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the array. Maximum size is 4.
//! @param matDataI: Array containing the matrix data
//! @return Minimum sized array containing the data of the transposed matrix
template <U32 _rows = 4, U32 _cols = 4, U32 _firstRowIn = 0, U32 _firstColIn = 0, U32 _firstRowOut = _firstRowIn,
          bool _unusedSetZero = false, U32 _colStrideIn = 1, U32 _colStrideOut = 1, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m128, _arrSizeIn>& matDataI);

//! @brief Transposes a matrix represented by an column major register array. The maximal supported matrix size is 4x4.
//! The returned array has the minimal size required to store the result.
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the matrix's first column inside of the passed array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the array. Maximum size is 4.
//! @param matDataI: Array containing the matrix data
//! @return Minimum sized array containing the data of the transposed matrix
template <U32 _rows = 4, U32 _cols = 4, U32 _firstRowIn = 0, U32 _firstColIn = 0, U32 _firstRowOut = _firstRowIn,
          bool _unusedSetZero = false, U32 _colStrideIn = 1, U32 _colStrideOut = 1, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m256d, _arrSizeIn>& matDataI);

//! @brief Transposes a matrix represented by an column major register array. The maximal supported matrix size is 8x8.
//! The returned array has the minimal size required to store the result.
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the matrix's first column inside of the passed array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the array. Maximum size is 8.
//! @param matDataI: Array containing the matrix data
//! @return Minimum sized array containing the data of the transposed matrix
template <U32 _rows = 8, U32 _cols = 8, U32 _firstRowIn = 0, U32 _firstColIn = 0, U32 _firstRowOut = _firstRowIn,
          bool _unusedSetZero = false, U32 _colStrideIn = 1, U32 _colStrideOut = 1, std::size_t _arrSizeIn>
inline auto Transpose(const std::array<__m256, _arrSizeIn>& matDataI);

//! @brief Transposes a matrix represented by an column major register array and writes it to the passed output array.
//! The maximal supported matrix size is NxN where N is the number of values in a register.
//! @tparam _registerType: Register type
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the input matrix's first column inside of the passed input array
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _firstColOut: Index of the output matrix's first column inside of the passed output array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the input array. Maximum size is equal to the number of values per register.
//! @tparam _arrSizeOut: Size of the output array. Maximum size is equal to the number of values per register.
//! @param matDataI: Array containing the input matrix data
//! @param matDataI: Array containing the output matrix data
template <typename _registerType, U32 _rows, U32 _col, U32 _firstRowIn, U32 _firstColIn = 0,
          bool _overwriteUnused = true, bool _unusedSetZero = false, U32 _colStrideIn = 1,
          U32 _firstRowOut = _firstRowIn, U32 _firstColOut = 0, U32 _colStrideOut = 1, std::size_t _arrSizeIn = 2,
          std::size_t _arrSizeOut = 2>
inline void Transpose(const std::array<_registerType, _arrSizeIn>& matDataI,
                      std::array<_registerType, _arrSizeOut>& matDataO);

//! @brief Transposes a matrix represented by an column major register array and writes it to the passed output array.
//! The maximal supported matrix size is 2x2.
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the input matrix's first column inside of the passed input array
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _firstColOut: Index of the output matrix's first column inside of the passed output array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the input array. Maximum size is 2.
//! @tparam _arrSizeOut: Size of the output array. Maximum size is 2.
//! @param matDataI: Array containing the input matrix data
//! @param matDataI: Array containing the output matrix data
template <U32 _rows = 2, U32 _cols = 2, U32 _firstRowIn = 0, U32 _firstColIn = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = false, U32 _colStrideIn = 1, U32 _firstRowOut = _firstRowIn, U32 _firstColOut = 0,
          U32 _colStrideOut = 1, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m128d, _arrSizeIn>& matDataI, std::array<__m128d, _arrSizeOut>& matDataO);

//! @brief Transposes a matrix represented by an column major register array and writes it to the passed output array.
//! The maximal supported matrix size is 4x4.
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the input matrix's first column inside of the passed input array
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _firstColOut: Index of the output matrix's first column inside of the passed output array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the input array. Maximum size is 4.
//! @tparam _arrSizeOut: Size of the output array. Maximum size is 4.
//! @param matDataI: Array containing the input matrix data
//! @param matDataI: Array containing the output matrix data
template <U32 _rows = 4, U32 _cols = 4, U32 _firstRowIn = 0, U32 _firstColIn = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = false, U32 _colStrideIn = 1, U32 _firstRowOut = _firstRowIn, U32 _firstColOut = 0,
          U32 _colStrideOut = 1, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m128, _arrSizeIn>& matDataI, std::array<__m128, _arrSizeOut>& matDataO);

//! @brief Transposes a matrix represented by an column major register array and writes it to the passed output array.
//! The maximal supported matrix size is 4x4.
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the input matrix's first column inside of the passed input array
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _firstColOut: Index of the output matrix's first column inside of the passed output array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the input array. Maximum size is 4.
//! @tparam _arrSizeOut: Size of the output array. Maximum size is 4.
//! @param matDataI: Array containing the input matrix data
//! @param matDataI: Array containing the output matrix data
template <U32 _rows = 4, U32 _cols = 4, U32 _firstRowIn = 0, U32 _firstColIn = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = false, U32 _colStrideIn = 1, U32 _firstRowOut = _firstRowIn, U32 _firstColOut = 0,
          U32 _colStrideOut = 1, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m256d, _arrSizeIn>& matDataI, std::array<__m256d, _arrSizeOut>& matDataO);

//! @brief Transposes a matrix represented by an column major register array and writes it to the passed output array.
//! The maximal supported matrix size is 8x8.
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstColIn: Index of the input matrix's first column inside of the passed input array
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _colStrideIn: Distance between two input matrix registers inside the array
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _firstColOut: Index of the output matrix's first column inside of the passed output array
//! @tparam _colStrideOut: Distance between two output matrix registers inside the array
//! @tparam _arrSizeIn: Size of the input array. Maximum size is 8.
//! @tparam _arrSizeOut: Size of the output array. Maximum size is 8.
//! @param matDataI: Array containing the input matrix data
//! @param matDataI: Array containing the output matrix data
template <U32 _rows = 8, U32 _cols = 8, U32 _firstRowIn = 0, U32 _firstColIn = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = false, U32 _colStrideIn = 1, U32 _firstRowOut = _firstRowIn, U32 _firstColOut = 0,
          U32 _colStrideOut = 1, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m256, _arrSizeIn>& matDataI, std::array<__m256, _arrSizeOut>& matDataO);



} // namespace GDL::simd


#include "gdl/base/simd/transpose.inl"
