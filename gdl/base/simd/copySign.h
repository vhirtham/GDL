#pragma once

namespace GDL::simd
{

//! @brief Composes a register with the signs from a first source register and the absolute values of a second register
//! @tparam _srcValueAllPositive: Set to true if all values of 'srcValue' are positive to gain additional performance
//! @tparam _registerType: Register Type
//! @param srcSign: Register that provides the sign
//! @param srcAbsVal:Register that provides the absolute value
//! @param Composed register
template <bool _srcValueAllPositive = false, typename _registerType>
inline _registerType CopySign(_registerType srcSign, _registerType srcAbsVal);


} // namespace GDL::simd

#include "gdl/base/simd/copySign.inl"
