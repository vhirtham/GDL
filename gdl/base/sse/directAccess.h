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
//! @return Value of a register at the desired position
template <U32 _index>
inline F32 GetValue(__m128 reg);

//! @brief Gets the value of a register at the desired position (run time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
inline F32 GetValue(__m128 reg, const U32 index);

//! @brief Gets the value of a register at the desired position (compile time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
template <U32 _index>
inline F64 GetValue(__m128d reg);

//! @brief Gets the value of a register at the desired position (run time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
inline F64 GetValue(__m128d reg, U32 index);

#ifdef __AVX2__

//! @brief Gets the value of a register at the desired position (compile time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
template <U32 _index>
inline F32 GetValue(__m256 reg);

//! @brief Gets the value of a register at the desired position (run time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
inline F32 GetValue(__m256 reg, U32 index);

//! @brief Gets the value of a register at the desired position (compile time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
template <U32 _index>
inline F64 GetValue(__m256d reg);

//! @brief Gets the value of a register at the desired position (run time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
inline F64 GetValue(__m256d reg, U32 index);

#endif // __AVX2__
} // namespace GDL::sse

#include "gdl/base/sse/directAccess.inl"
