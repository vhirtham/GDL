#pragma once

#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/base/fundamentalTypes.h"
#include <x86intrin.h>



namespace GDL::sse
{

//! @brief Gets the value of a register at the desired position (compile time version)
//! @tparam _index: Index of the value
//! @tparam _registerType: Register type
//! @param reg: Register
//! @return Value of a register at the desired position
template <U32 _index, typename _registerType>
inline auto GetValue(const _registerType reg);

//! @brief Gets the value of a register at the desired position (run time version)
//! @tparam _registerType: Register type
//! @param reg: Register
//! @param index: Index of the value
//! @return Value of a register at the desired position
template <typename _registerType>
inline auto GetValue(const _registerType reg, const U32 index);

//! @brief Sets the value of a register at the desired position (compile time version)
//! @tparam _index: Index of the value
//! @tparam _registerType: Register type
//! @tparam _type: Type of the passed value
//! @param reg: Register
//! @param value: Value that should be set
template <U32 _index, typename _registerType, typename _type>
inline auto SetValue(_registerType& reg, const _type value);

//! @brief Sets the value of a register at the desired position (compile time version)
//! @tparam _registerType: Register type
//! @tparam _type: Type of the passed value
//! @param reg: Register
//! @param value: Value that should be set
//! @param index: Index of the value
template <typename _registerType, typename _type>
inline auto SetValue(_registerType& reg, const _type value, const U32 _index);

} // namespace GDL::sse

#include "gdl/base/sse/directAccess.inl"
