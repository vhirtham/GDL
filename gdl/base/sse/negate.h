#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/x86intrin.h"

namespace GDL::sse
{

//! @brief Creates a bit mask where only the signed bit is set to the desired value
//! @tparam _type: Type of the mask (F32 or F64)
//! @tparam _negation: 1 if the signed bit should be set. 0 otherwise.
//! @return Mask
template <typename _type, U32 _negation>
constexpr _type SignMask();

//! @brief Returns a register with all values negated
//! @tparam _registerType: Register type
//! @param source: Source register
//! @return Register with negated values
template <typename _registerType>
inline _registerType Negate(_registerType source);

//! @brief Negates the specified values of a register
//! @tparam _n0-_n1: Set to 1 if the value at the corresponding position should be negated
//! @param source: Source register
//! @return Register with negated values
template <U32 _n0, U32 _n1>
inline __m128d Negate(__m128d source);

//! @brief Negates the specified values of a register
//! @tparam _n0-_n3: Set to 1 if the value at the corresponding position should be negated
//! @tparam _registerType: Register type
//! @param source: Source register
//! @return Register with negated values
template <U32 _n0, U32 _n1, U32 _n2, U32 _n3, typename _registerType>
inline _registerType Negate(_registerType source);

#ifdef __AVX2__

//! @brief Negates the specified values of a register
//! @tparam _n0-_n7: Set to 1 if the value at the corresponding position should be negated
//! @param source: Source register
//! @return Register with negated values
template <U32 _n0, U32 _n1, U32 _n2, U32 _n3, U32 _n4, U32 _n5, U32 _n6, U32 _n7>
inline __m256 Negate(__m256 source);

#endif // __AVX2__

//! @brief Creates a mask register to negate values at specific positions by using a bitwise xor operation
//! @tparam _n0-_n1: Set to 1 if the value at the corresponding position should be negated
//! @return Mask register
template <U32 _n0, U32 _n1>
inline __m128d NegationMask();

//! @brief Creates a mask register to negate values at specific positions by using a bitwise xor operation
//! @tparam _registerType: Register type
//! @tparam _n0-_n3: Set to 1 if the value at the corresponding position should be negated
//! @return Mask register
template <typename _registerType, U32 _n0, U32 _n1, U32 _n2, U32 _n3>
inline _registerType NegationMask();

#ifdef __AVX2__

//! @brief Creates a mask register to negate values at specific positions by using a bitwise xor operation
//! @tparam _n0-_n7: Set to 1 if the value at the corresponding position should be negated
//! @return Mask register
template <U32 _n0, U32 _n1, U32 _n2, U32 _n3, U32 _n4, U32 _n5, U32 _n6, U32 _n7>
inline __m256 NegationMask();

#endif // __AVX2__

} // namespace GDL::sse

#include "gdl/base/sse/negate.inl"
