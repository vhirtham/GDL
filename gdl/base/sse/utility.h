#pragma once

#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/base/fundamentalTypes.h"

#include <type_traits>
#include <x86intrin.h>



namespace GDL::sse
{

// Constants ----------------------------------------------------------------------------------------------------------

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

// Functions ----------------------------------------------------------------------------------------------------------


//! @brief Calculates the minimal number of registers to store a certain number of values
//! @tparam _registerType: Register type
//! @param numValues: Number of values that should be stored
//! @return Minimal number of registers
template <typename _registerType>
constexpr U32 CalcMinNumArrayRegisters(U32 numValues);

//! @brief Gets a register of the provided bitsize which is compatible with a given data type
//! @tparam _type: Returned register must be capable to store values of this data type
//! @tparam _registerSize: Bit size of the returned register
//! @return Register of the provided bitsize which is compatible with a given data type
//! @remark A function call should usually be combined with decltype
template <typename _type, U32 _registerSize>
auto GetFittingRegister();

//! @brief Gets an inatance of the underlying data type of a register
//! @tparam _registerType: Register type
//! @tparam _returnTypeIfNoTARegister: If set to true, this function returns an instance of the template parameters type
//! if it is not a register. If set to false (default), it will throw.
//! @return Inatance of  the underlying data type of a register
//! @remark A function call should usually be combined with decltype
template <typename _registerType, bool _returnTypeIfNoTARegister = false>
constexpr auto GetDataType();

//! @brief Gets the bitsize of the largest available register
//! @return Bitsize of the largest available register
constexpr U32 MaxRegisterSize();

//! @brief Reinterprets the passed register as a integer register of the same size
//! @tparam _registerType: Register type
//! @param reg: Register that should be cast
//! @return Integer register
template <typename _registerType>
inline auto ReinterpretAsIntRegister(_registerType reg);



} // namespace GDL::sse

#include "gdl/base/sse/utility.inl"
