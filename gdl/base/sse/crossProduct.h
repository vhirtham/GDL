#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/x86intrin.h"


namespace GDL::simd
{

//! @brief Calculates the cross product of first 3 values of 2 registers
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Cross product
//! @remark Since the cross product is only defined for 3d vectors the fourth entry of the result register is undefined
//! and should not be used. If both input registers contain a 0 in the fourth element, the fourth value of the result
//! register will also be 0.
[[nodiscard]] inline __m128 CrossProduct(__m128 lhs, __m128 rhs);

} // namespace GDL::simd

#include "gdl/base/sse/crossProduct.inl"
