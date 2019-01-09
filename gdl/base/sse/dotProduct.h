#pragma once

#include <x86intrin.h>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"

namespace GDL::sse
{

//! @brief Creates the destination part of the dot product mask.
//! @tparam _dstIndex: Index of an element from the result register wherethe dot product should be stored. If the value
//! is -1, the result will e stored in every element.
//! @return Destination part of the dot product mask.
template <I32 _dstIndex = -1>
constexpr U32 GetDotProductDestinationMask();

//! @brief Returns a mask that can be used with the dot product intrinsics.
//! @tparam _registerType: Register type
//! @tparam _numValues: Number of values (starting at the first one) that should be used.
//! @tparam _dstIndex: Index of an element from the result register wherethe dot product should be stored. If the value
//! is -1, the result will e stored in every element.
//! @return Mask
template <typename _registerType, U32 _numValues, I32 _dstIndex = -1>
constexpr U32 GetDotProductMask();

//! @brief Calculates the dot product of a specified number of values from two registers
//! @tparam _registerType: Register type
//! @tparam _numValues: Number of values (starting at the first one) that should be used.
//! @tparam _returnRegister: If true the function returns an sse register where the lowest element contains the result.
//! If false (default) a literal containing the value is returned.
//! @tparam _dstIndex: Index of an element from the result register wherethe dot product should be stored. If the value
//! is -1, the result will e stored in every element. This value has no effect if _returnRegister is false
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Dot product
template <typename _registerType, U32 _numValues = numRegisterValues<_registerType>, bool _returnRegister = false,
          I32 _dstIndex = -1>
inline auto DotProduct(const _registerType& lhs, const _registerType& rhs);

} // namespace GDL::sse



#include "gdl/base/sse/dotProduct.inl"
