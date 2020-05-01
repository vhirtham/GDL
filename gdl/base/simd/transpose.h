#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"

#include <array>

namespace GDL::simd
{

// TODOs for functions of this file
// - check docstring
// - rename headers that contain the tests


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
template <U32 _rows, U32 _cols, U32 _firstRowIn = 0, U32 _firstRowOut = 0, U32 _firstColIn = 0, U32 _firstColOut = 0,
          U32 _colStrideIn = 1, U32 _colStrideOut = 1, bool _overwriteUnused = true, bool _unusedSetZero = false,
          typename _registerType, UST _arrSizeIn, UST _arrSizeOut>
inline void Transpose(const std::array<_registerType, _arrSizeIn>& matDataI,
                      std::array<_registerType, _arrSizeOut>& matDataO) noexcept;



} // namespace GDL::simd


#include "gdl/base/simd/transpose.inl"
