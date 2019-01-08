#pragma once

#include <x86intrin.h>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"

namespace GDL::sse
{

//! @brief Returns a mask that can be used with the dot product intrinsics.
//! @tparam _registerType: Register type
//! @param numValues: Number of values (starting at the first one) that should be used.
//! @param dstIndex: Index of an element from the result register where the dot product should be stored.
//! @return Mask
template <typename _registerType>
constexpr U32 GetDotProductMask(U32 numValues, U32 dstIndex);

//! @brief Calculates the dot product of a specified number of values from two registers
//! @tparam _registerType: Register type
//! @tparam _numValues: Number of values (starting at the first one) that should be used.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Dot product
template <typename _registerType, U32 _numValues = numRegisterValues<_registerType>>
inline auto DotProduct(_registerType lhs, _registerType rhs);

} // namespace GDL::sse



#include "gdl/base/sse/dotProduct.inl"
