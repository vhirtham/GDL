#pragma once

#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include <x86intrin.h>



namespace GDL
{

//! @brief Stores the values of a register to a given memory location
//! @tparam _type: Type of the registers values
//! @tparam _registerType: Register type
//! @param ptr: Pointer to a piece of memory where the data should be stored
//! @param reg: Register that provides the data
template <typename _type, typename _registerType>
inline void _mmx_store_p(_type* ptr, _registerType reg);

//! @brief Template to create a register with all entries set to zero
//! @tparam _registerType: Register type
//! @return Register with all entries set to zero
template <typename _registerType>
inline _registerType _mmx_setzero_p();

//! @brief Template to create a register with all entries set to the same value
//! @tparam _registerType: Register type
//! @tparam _type: Type of the value
//! @param value: Value that should be set
//! @return Register with all entries set to the same value
template <typename _registerType, typename _type>
inline _registerType _mmx_set1_p(_type value);

//! @brief Template to create a register with given values in reverse order
//! @tparam _registerType: Register type
//! @tparam _type: Type of the value
//! @param v0: First value
//! @param v1: Second value
//! @param v2: Third value
//! @param v3: Fourth value
//! @return Register with given values in reverse order
template <typename _registerType, typename... _args>
inline _registerType _mmx_setr_p(_args... args);

//! @brief Template for register addition
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the addition
template <typename _registerType>
inline _registerType _mmx_add_p(_registerType lhs, _registerType rhs);

//! @brief Template for register substraction
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the substraction
template <typename _registerType>
inline _registerType _mmx_sub_p(_registerType lhs, _registerType rhs);

//! @brief Template for register multiplication
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the multiplication
template <typename _registerType>
inline _registerType _mmx_mul_p(_registerType lhs, _registerType rhs);

//! @brief Template for multiplication of two registers with subsequent addition of a third register (fused multiply
//! add)
//! @tparam _registerType: Register type
//! @param lhsM: Left hand side value of the multiplication
//! @param rhsM: Right hand side value of the multiplication
//! @param add: Value that is added to the result of the multiplication
//! @return Result of the operation
//! @remark If fmadd intrinsics are not available the function still works. It performs the necessary operations
//! seperately.
template <typename _registerType>
inline _registerType _mmx_fmadd_p(_registerType lhsM, _registerType rhsM, _registerType add);

//! @brief Compares two registers for equality
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mmx_cmpeq_p(_registerType lhs, _registerType rhs);

//! @brief Compares lhs <= rhs for two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mmx_cmple_p(_registerType lhs, _registerType rhs);

//! @brief Compares lhs > rhs for two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mmx_cmpgt_p(_registerType lhs, _registerType rhs);

//! @brief Calculates the maximum values of two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the maximum values
template <typename _registerType>
inline _registerType _mmx_max_p(_registerType lhs, _registerType rhs);

//! @brief Calculates the minimum values of two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the minimum values
template <typename _registerType>
inline _registerType _mmx_min_p(_registerType lhs, _registerType rhs);

//! @brief Performs a and-not operation on the passed registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Result of the and-not operation
template <typename _registerType>
inline _registerType _mmx_andnot_p(_registerType lhs, _registerType rhs);

//! @brief Create mask from the most significant bit of each 8-bit element in a register and returns it
//! @tparam _registerType: Register type
//! @param reg: Register which serves as source for the mask
//! @return Mask from the most significant bit of each 8-bit element of the source register
template <typename _registerType>
inline auto _mmx_movemask_epi8(_registerType reg);


} // namespace GDL

#include "gdl/base/sse/intrinsics.inl"
