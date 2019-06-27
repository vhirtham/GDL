#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/x86intrin.h"

namespace GDL::simd
{

//! @brief Constant which stores the necessary alignment in bytes for the provided register type
//! @tparam _registerType: RegisterType
template <typename _registerType>
constexpr const U32 alignmentBytes = 0;
template <>
constexpr const U32 alignmentBytes<__m128> = 16;
template <>
constexpr const U32 alignmentBytes<__m128d> = 16;
template <>
constexpr const U32 alignmentBytes<__m128i> = 16;
#ifdef __AVX2__
template <>
constexpr const U32 alignmentBytes<__m256> = 32;
template <>
constexpr const U32 alignmentBytes<__m256d> = 32;
template <>
constexpr const U32 alignmentBytes<__m256i> = 32;
#endif // __AVX2__



//! @brief Constant which stores the number of values in a register
//! @tparam _registerType: RegisterType
template <typename _registerType>
constexpr const U32 numRegisterValues = 0;
template <>
constexpr const U32 numRegisterValues<__m128> = 4;
template <>
constexpr const U32 numRegisterValues<__m128d> = 2;
#ifdef __AVX2__
template <>
constexpr const U32 numRegisterValues<__m256> = 8;
template <>
constexpr const U32 numRegisterValues<__m256d> = 4;
#endif // __AVX2__



//! @brief Constant which stores the number of values in a register SIMD lane
//! @tparam _registerType: RegisterType
template <typename _registerType>
constexpr const U32 numValuesPerLane = 0;
template <>
constexpr const U32 numValuesPerLane<__m128> = 4;
template <>
constexpr const U32 numValuesPerLane<__m128d> = 2;
#ifdef __AVX2__
template <>
constexpr const U32 numValuesPerLane<__m256> = 4;
template <>
constexpr const U32 numValuesPerLane<__m256d> = 2;
#endif // __AVX2__



//! @brief Constant which stores the number of SIMD lanes of a register
//! @tparam _registerType: RegisterType
template <typename _registerType>
constexpr const U32 numLanes = 0;
template <>
constexpr const U32 numLanes<__m128> = 1;
template <>
constexpr const U32 numLanes<__m128d> = 1;
#ifdef __AVX2__
template <>
constexpr const U32 numLanes<__m256> = 2;
template <>
constexpr const U32 numLanes<__m256d> = 2;
#endif // __AVX2__



//! @brief Template constant is only true if the type is a supported SSE register
//! @tparam _registerType: RegisterType
template <typename _registerType>
constexpr const bool IsRegisterType = false;
template <>
constexpr const bool IsRegisterType<__m128> = true;
template <>
constexpr const bool IsRegisterType<__m128d> = true;
#ifdef __AVX2__
template <>
constexpr const bool IsRegisterType<__m256> = true;
template <>
constexpr const bool IsRegisterType<__m256d> = true;
#endif // __AVX2__



//! @brief Template constant is only true for __m128
//! @tparam _registerType: RegisterType
//! @remark One can achieve the same result with std::is_same but the synthax is longer and it needs to be wrapped into
//! #ifdef blocks in case the architecture does not support SSE/AVX.
template <typename _registerType>
constexpr const bool Is__m128 = false;
template <>
constexpr const bool Is__m128<__m128> = true;



//! @brief Template constant is only true for __m128d
//! @tparam _registerType: RegisterType
//! @remark One can achieve the same result with std::is_same but the synthax is longer and it needs to be wrapped into
//! #ifdef blocks in case the architecture does not support SSE/AVX.
template <typename _registerType>
constexpr const bool Is__m128d = false;
template <>
constexpr const bool Is__m128d<__m128d> = true;



//! @brief Template constant is only true for __m128i
//! @tparam _registerType: RegisterType
//! @remark One can achieve the same result with std::is_same but the synthax is longer and it needs to be wrapped into
//! #ifdef blocks in case the architecture does not support SSE/AVX.
template <typename _registerType>
constexpr const bool Is__m128i = false;
template <>
constexpr const bool Is__m128i<__m128i> = true;



//! @brief Template constant is only true for __m256
//! @tparam _registerType: RegisterType
//! @remark One can achieve the same result with std::is_same but the synthax is longer and it needs to be wrapped into
//! #ifdef blocks in case the architecture does not support SSE/AVX.
template <typename _registerType>
constexpr const bool Is__m256 = false;
#ifdef __AVX2__
template <>
constexpr const bool Is__m256<__m256> = true;
#endif // __AVX2__



//! @brief Template constant is only true for __m256d
//! @tparam _registerType: RegisterType
//! @remark One can achieve the same result with std::is_same but the synthax is longer and it needs to be wrapped into
//! #ifdef blocks in case the architecture does not support SSE/AVX.
template <typename _registerType>
constexpr const bool Is__m256d = false;
#ifdef __AVX2__
template <>
constexpr const bool Is__m256d<__m256d> = true;
#endif // __AVX2__



//! @brief Template constant is only true for __m256i
//! @tparam _registerType: RegisterType
//! @remark One can achieve the same result with std::is_same but the synthax is longer and it needs to be wrapped into
//! #ifdef blocks in case the architecture does not support SSE/AVX.
template <typename _registerType>
constexpr const bool Is__m256i = false;
#ifdef __AVX2__
template <>
constexpr const bool Is__m256i<__m256i> = true;
#endif // __AVX2__


} // namespace GDL::simd
