#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"



namespace GDL
{

//! @brief Stores the values of a register to a given memory location
//! @tparam _type: Type of the registers values
//! @tparam _registerType: Register type
//! @param ptr: Pointer to a piece of memory where the data should be stored
//! @param reg: Register that provides the data
template <typename _type, typename _registerType>
inline void _mm_store(_type* ptr, _registerType reg);

//! @brief Template to create a register with all entries set to zero
//! @tparam _registerType: Register type
//! @return Register with all entries set to zero
template <typename _registerType>
inline _registerType _mm_setzero();

//! @brief Template to create a register with all entries set to the same value
//! @tparam _registerType: Register type
//! @tparam _type: Type of the value
//! @param value: Value that should be set
//! @return Register with all entries set to the same value
template <typename _registerType, typename _type>
inline _registerType _mm_set1(_type value);

//! @brief Template to create a register with given values in reverse order
//! @tparam _registerType: Register type
//! @tparam _type: Type of the value
//! @param v0: First value
//! @param v1: Second value
//! @param v2: Third value
//! @param v3: Fourth value
//! @return Register with given values in reverse order
template <typename _registerType, typename... _args>
inline _registerType _mm_setr(_args... args);


//! @brief Extracts the lowest element of a register. In case of 256 bit registers, the lowest element of the first
//! 128bit is returned.
//! @tparam _registerType: Register type
//! @param reg: Source register
//! @return Lowest element of a register
template <typename _registerType>
inline auto _mm_cvtsF(_registerType reg);

//! @brief Template for register addition
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the addition
template <typename _registerType>
inline _registerType _mm_add(_registerType lhs, _registerType rhs);

//! @brief Template for register substraction
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the substraction
template <typename _registerType>
inline _registerType _mm_sub(_registerType lhs, _registerType rhs);

//! @brief Template for register multiplication
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the multiplication
template <typename _registerType>
inline _registerType _mm_mul(_registerType lhs, _registerType rhs);

//! @brief Template for register division
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the division
template <typename _registerType>
inline _registerType _mm_div(_registerType lhs, _registerType rhs);

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
inline _registerType _mm_fmadd(_registerType lhsM, _registerType rhsM, _registerType add);

//! @brief Template for multiplication of two registers with subsequent negation of the result and addition of a third
//! register (fused negate multiply add)
//! @tparam _registerType: Register type
//! @param lhsM: Left hand side value of the multiplication
//! @param rhsM: Right hand side value of the multiplication
//! @param add: Value that is added to the result of the negated multiplication
//! @return Result of the operation
//! @remark If fmadd intrinsics are not available the function still works. It performs the necessary operations
//! seperately.
template <typename _registerType>
inline _registerType _mm_fnmadd(_registerType lhsM, _registerType rhsM, _registerType add);

//! @brief Template for multiplication of two registers with subsequent substraction of a third register (fused multiply
//! sub)
//! @tparam _registerType: Register type
//! @param lhsM: Left hand side value of the multiplication
//! @param rhsM: Right hand side value of the multiplication
//! @param sub: Value that is substracted from the result of the multiplication
//! @return Result of the operation
//! @remark If fmsub intrinsics are not available the function still works. It performs the necessary operations
//! seperately.
template <typename _registerType>
inline _registerType _mm_fmsub(_registerType lhsM, _registerType rhsM, _registerType sub);

//! @brief Template for multiplication of two registers with subsequent negation of the result and substraction of a
//! third register (fused negate multiply sub)
//! @tparam _registerType: Register type
//! @param lhsM: Left hand side value of the multiplication
//! @param rhsM: Right hand side value of the multiplication
//! @param sub: Value that is substracted from the result of the multiplication
//! @return Result of the operation
//! @remark If fnmsub intrinsics are not available the function still works. It performs the necessary operations
//! seperately.
template <typename _registerType>
inline _registerType _mm_fnmsub(_registerType lhsM, _registerType rhsM, _registerType sub);

//! @brief Template for register dot product
//! @tparam _mask: Mask for the calculation of the dot product
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Dot product
template <U32 _mask, typename _registerType>
inline _registerType _mm_dp(_registerType lhs, _registerType rhs);

//! @brief Calculates the square root of a register
//! @tparam _registerType: Register type
//! @param reg: Source register
//! @return Result register
template <typename _registerType>
inline _registerType _mm_sqrt(_registerType reg);

//! @brief Calculates the reciprocal square root of a register
//! @tparam _registerType: Register type
//! @param reg: Source register
//! @return Result register
template <typename _registerType>
inline _registerType _mm_rsqrt(_registerType reg);

//! @brief Compares two registers for equality
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mm_cmpeq(_registerType lhs, _registerType rhs);

//! @brief Compares lhs <= rhs for two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mm_cmple(_registerType lhs, _registerType rhs);

//! @brief Compares lhs > rhs for two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mm_cmpgt(_registerType lhs, _registerType rhs);

//! @brief Compares lhs > rhs for two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mm_cmplt(_registerType lhs, _registerType rhs);

//! @brief Calculates the maximum values of two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the maximum values
template <typename _registerType>
inline _registerType _mm_max(_registerType lhs, _registerType rhs);

//! @brief Calculates the minimum values of two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the minimum values
template <typename _registerType>
inline _registerType _mm_min(_registerType lhs, _registerType rhs);

//! @brief Performs a and operation on the passed registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Result of the and-not operation
template <typename _registerType>
inline _registerType _mm_and(_registerType lhs, _registerType rhs);

//! @brief Performs a and-not operation on the passed registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Result of the and-not operation
template <typename _registerType>
inline _registerType _mm_andnot(_registerType lhs, _registerType rhs);

//! @brief Performs a or operation on the passed registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Result of the xor operation
template <typename _registerType>
inline _registerType _mm_or(_registerType lhs, _registerType rhs);

//! @brief Performs a xor operation on the passed registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Result of the xor operation
template <typename _registerType>
inline _registerType _mm_xor(_registerType lhs, _registerType rhs);

//! @brief Create mask from the most significant bit of each 8-bit element in a register and returns it
//! @tparam _registerType: Register type
//! @param reg: Register which serves as source for the mask
//! @return Mask from the most significant bit of each 8-bit element of the source register
template <typename _registerType>
inline auto _mm_movemaskEpi8(_registerType reg);


//! @brief Creates a new register where each value is selected from the two source registers values at the same
//! position.
//! @tparam _blendMask: Bitmask that determines the value composition. Have a look at Intels documentation of
//! _mm_blend_ps, _mm_blend_pd, _mm256_blend_ps or _mm256_blend_ps for detailed informations.
//! @tparam _registerType: Register type
//! @param src0: First source register
//! @param src1: second source register
//! @return Register with blended values
template <I32 _blendMask, typename _registerType>
inline auto _mm_blend(_registerType src0, _registerType src1);

//! @brief Creates a new register where each value is selected from the two source registers values at the same
//! position.
//! @tparam _registerType: Register type
//! @param src0: First source register
//! @param src1: second source register
//! @param _blendMask: Register that determines which value is selected from which source register.
//! @return Register with blended values
template <typename _registerType>
inline auto _mm_blendv(_registerType src0, _registerType src1, _registerType _blendMask);

//! @brief Creates a new register by overwriting even indexed values with the following odd indexed value.
//! @tparam _registerType: Register type
//! @param src: Source register
//! @return Register with shuffled values
template <typename _registerType>
inline auto _mm_movehdup(_registerType src);

//! @brief Creates a new register by overwriting odd indexed values with the previous even indexed value.
//! @tparam _registerType: Register type
//! @param src: Source register
//! @return Register with shuffled values
template <typename _registerType>
inline auto _mm_moveldup(_registerType src);

//! @brief Returns a register with an arbitrary combination of values from the input register as long as the source
//! values and the destination are in the same SIMD lane.
//! @tparam _permuteMask: Bitmask that determines the value composition. Have a look at Intels documentation of
//! _mm_permute_ps or_mm_shuffle_ps for detailed informations
//! @tparam _registerType: Register type
//! @param reg: Register which serves as source
//! @return Register with an arbitrary combination of values from the input register.
template <I32 _permuteMask, typename _registerType>
inline auto _mm_permute(_registerType reg);

#ifdef __AVX2__

//! @brief Returns a 256 bit AVX register with an arbitrary combination of the two source registers lanes
//! @tparam _permuteMask: Bitmask that determines the value composition. Have a look at Intels documentation of
//! _mm_permute2f128_ps or _mm_permute2f128_pd for detailed informations
//! @tparam _registerType: Register type
//! @param src0: First register which serves as source
//! @param src1: Second register which serves as source
//! @return AVX register with an arbitrary combination of the two source registers lanes
template <I32 _permuteMask, typename _registerType>
inline auto _mm_permute2f128(_registerType src0, _registerType src1);

//! @brief Returns a __m256d register with an arbitrary combination of the source registers values.
//! @tparam _permuteMask: Bitmask that determines the value composition. Have a look at Intels documentation of
//! _mm_permute4x64_ps for detailed informations
//! @param src: Source register
//! @return __m256d register with an arbitrary combination of the source registers values.
template <I32 _permuteMask>
inline __m256d _mm_permute4x64(__m256d src0);

#endif // __AVX2__

//! @brief Creates a new register with its first 64 bits per lane being selected from the first registers values
//! from the same lane. The second 64 bits are taken from the second register in the same manner.
//! @tparam _shuffleMask: Bitmask that determines the value composition. Have a look at Intels documentation of
//! _mm_shuffle_ps, _mm_shuffle_pd, _mm256_shuffle_ps or _mm256_shuffle_ps for detailed informations.
//! @tparam _registerType: Register type
//! @param src0: First source register
//! @param src1: second source register
//! @return Register with shuffled values
template <I32 _shuffleMask, typename _registerType>
inline auto _mm_shuffle(_registerType src0, _registerType src1);

//! @brief Creates a new register with an interleaved combination of the source registers second halves.
//! @tparam _registerType: Register type
//! @param src0: First source register
//! @param src1: second source register
//! @return Register with shuffled values
template <typename _registerType>
inline auto _mm_unpackhi(_registerType src0, _registerType src1);

//! @brief Creates a new register with an interleaved combination of the source registers first halves.
//! @tparam _registerType: Register type
//! @param src0: First source register
//! @param src1: second source register
//! @return Register with shuffled values
template <typename _registerType>
inline auto _mm_unpacklo(_registerType src0, _registerType src1);


} // namespace GDL

#include "gdl/base/simd/intrinsics.inl"
