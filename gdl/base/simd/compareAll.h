#pragma once

#include "gdl/base/functions/pow.h"
#include "gdl/base/simd/utility.h"


#include <limits>

namespace GDL::simd
{


//! @brief Calculates a value with a certain bitsignature that corresponds to the expected result from a register
//! comparison where all element comparisons return true
//! @tparam _registerType: RegisterType
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @return Value with specific bit signature
template <typename _registerType, U32 _numComparedValues = numRegisterValues<_registerType>>
constexpr auto SSECalculateComparisonValueAllTrue();

//! @brief Compares two register with the passed comparison function. If a single element comparison returns false, this
//! function will also return false.
//! @tparam _registerType: Register type
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @tparam _compFunction: Type of the comparison function
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @param compFunction: Comparison function
//! @return TRUE / FALSE
template <typename _registerType, U32 _numComparedValues, typename _compFunction>
inline bool CompareAllTrue(_registerType lhs, _registerType rhs, _compFunction compFunction);

//! @brief Compares if the values of two register are equal. This function returns true if all values are equal and
//! false otherwise
//! @tparam _registerType: Register type
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return TRUE / FALSE
template <typename _registerType, U32 _numComparedValues = numRegisterValues<_registerType>>
inline bool CompareAllEqual(_registerType lhs, _registerType rhs);

//! @brief Compares lhs <= rhs for two register. This function returns true if all lhs values are less or equal when
//! compared to the corresponding rhs value
//! @tparam _registerType: Register type
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return TRUE / FALSE
template <typename _registerType, U32 _numComparedValues = numRegisterValues<_registerType>>
inline bool CompareAllLessEqual(_registerType lhs, _registerType rhs);

//! @brief Compares lhs < rhs for two register. This function returns true if all lhs values are less than the
//! corresponding rhs values
//! @tparam _registerType: Register type
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return TRUE / FALSE
template <typename _registerType, U32 _numComparedValues = numRegisterValues<_registerType>>
inline bool CompareAllLessThan(_registerType lhs, _registerType rhs);

//! @brief Compares lhs > rhs for two register. This function returns true if all lhs values are greater than the rhs
//! values. False if not.
//! @tparam _registerType: Register type
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return TRUE / FALSE
template <typename _registerType, U32 _numComparedValues = numRegisterValues<_registerType>>
inline bool CompareAllGreaterThan(_registerType lhs, _registerType rhs);


//! @brief Checks if all bytes of a register are set to zero
//! @tparam _registerType: Register type
//! @param reg: Register that should be checked
//! @return TRUE if all bytes of the register are set to zero, FALSE otherwise
template <typename _registerType>
inline bool CompareMemoryZero(const _registerType& reg);

} // namespace GDL::simd

#include "gdl/base/simd/compareAll.inl"
