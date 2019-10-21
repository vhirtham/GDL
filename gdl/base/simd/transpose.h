#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"

#include <array>

namespace GDL::simd
{

// TODO
// documentation of test
// add missing sizes
// add static assert for _firstCol + _rows < regVals
// check possible optimization of __m128 transpose 2x2 using Insert

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
          U32 _firstRowOut = _firstRowIn, bool _unusedSetZero = true, U32 _colStrideIn = 1, U32 _colStrideOut = 1,
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
          bool _unusedSetZero = true, U32 _colStrideIn = 1, U32 _colStrideOut = 1, std::size_t _arrSizeIn>
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
          bool _unusedSetZero = true, U32 _colStrideIn = 1, U32 _colStrideOut = 1, std::size_t _arrSizeIn>
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
          bool _unusedSetZero = true, U32 _colStrideIn = 1, U32 _colStrideOut = 1, std::size_t _arrSizeIn>
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
          bool _unusedSetZero = true, U32 _colStrideIn = 1, U32 _colStrideOut = 1, std::size_t _arrSizeIn>
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
          bool _overwriteUnused = true, bool _unusedSetZero = _overwriteUnused, U32 _colStrideIn = 1,
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
          bool _unusedSetZero = _overwriteUnused, U32 _colStrideIn = 1, U32 _firstRowOut = _firstRowIn,
          U32 _firstColOut = 0, U32 _colStrideOut = 1, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
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
          bool _unusedSetZero = _overwriteUnused, U32 _colStrideIn = 1, U32 _firstRowOut = _firstRowIn,
          U32 _firstColOut = 0, U32 _colStrideOut = 1, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
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
          bool _unusedSetZero = _overwriteUnused, U32 _colStrideIn = 1, U32 _firstRowOut = _firstRowIn,
          U32 _firstColOut = 0, U32 _colStrideOut = 1, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
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
          bool _unusedSetZero = _overwriteUnused, U32 _colStrideIn = 1, U32 _firstRowOut = _firstRowIn,
          U32 _firstColOut = 0, U32 _colStrideOut = 1, std::size_t _arrSizeIn, std::size_t _arrSizeOut>
inline void Transpose(const std::array<__m256, _arrSizeIn>& matDataI, std::array<__m256, _arrSizeOut>& matDataO);



// 1x1 ----------------------------------------------------------------------------------------------------------------

//! @brief Transposes a 1x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _registerType: Register type
//! @param out: Output register
//! @remark Unused rows are set to zero. Even though transposing a 1x1 matrix does not make that much sense, the purpose
//! of this function is to guarantee a consisten behavior of the generalized transpose functions (setting unused values
//! to zero).
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused, typename _registerType>
inline void Transpose1x1(_registerType in, _registerType& out);



// 1x2 ----------------------------------------------------------------------------------------------------------------

//! @brief Transposes a 1x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose1x2(const __m128& in0, const __m128& in1, __m128& out0);

//! @brief Transposes a 1x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose1x2(const __m128d& in0, const __m128d& in1, __m128d& out0);

#ifdef __AVX2__

//! @brief Transposes a 1x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _registerType: Register type
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose1x2(const __m256& in0, const __m256& in1, __m256& out0);

//! @brief Transposes a 1x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _registerType: Register type
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose1x2(const __m256d& in0, const __m256d& in1, __m256d& out0);

#endif //__AVX2__



// 2x1 ----------------------------------------------------------------------------------------------------------------

//! @brief Transposes a 2x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose2x1(const __m128& in0, __m128& out0, __m128& out1);

//! @brief Transposes a 2x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose2x1(const __m128d& in0, __m128d& out0, __m128d& out1);

#ifdef __AVX2__

//! @brief Transposes a 2x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose2x1(const __m256& in0, __m256& out0, __m256& out1);

//! @brief Transposes a 2x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose2x1(const __m256d& in0, __m256d& out0, __m256d& out1);

#endif //__AVX2__

// 2x2 ----------------------------------------------------------------------------------------------------------------

//! @brief Transposes a 2x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose2x2(const __m128& in0, const __m128& in1, __m128& out0, __m128& out1);

//! @brief Transposes a 2x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose2x2(const __m128d& in0, const __m128d& in1, __m128d& out0, __m128d& out1);

#ifdef __AVX2__

//! @brief Transposes a 2x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose2x2(const __m256& in0, const __m256& in1, __m256& out0, __m256& out1);

//! @brief Transposes a 2x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true,
          bool _unusedSetZero = _overwriteUnused>
inline void Transpose2x2(const __m256d& in0, const __m256d& in1, __m256d& out0, __m256d& out1);

#endif // __AVX2__



// 4x4 ----------------------------------------------------------------------------------------------------------------

//! @brief Transposes a 4x4 matrix which consists of 4 __m128 registers
//! @param in: Input registers
//! @param out: Output registers
inline void Transpose4x4(__m128 in0, __m128 in1, __m128 in2, __m128 in3, __m128& out0, __m128& out1, __m128& out2,
                         __m128& out3);

#ifdef __AVX2__

//! @brief Transposes a 4x4 matrix which consists of 4 __m256d registers
//! @param in: Input registers
//! @param out: Output registers
inline void Transpose4x4(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1,
                         __m256d& out2, __m256d& out3);



// 8x8 ----------------------------------------------------------------------------------------------------------------

//! @brief Transposes a 8x8 matrix which consists of 8 __m256 registers
//! @param in: Input registers
//! @param out: Output registers
//! @remark source: https://stackoverflow.com/questions/25622745/transpose-an-8x8-float-using-avx-avx2
inline void Transpose8x8(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                         __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5,
                         __m256& out6, __m256& out7);

#endif // __AVX2__

} // namespace GDL::simd


#include "gdl/base/simd/transpose.inl"
