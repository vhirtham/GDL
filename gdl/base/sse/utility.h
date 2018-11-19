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



// Functions ----------------------------------------------------------------------------------------------------------


//! @brief Calculates the minimal number of registers to store a certain number of values
//! @tparam _registerType: Register type
//! @param numValues: Number of values that should be stored
//! @return Minimal number of registers
template <typename _registerType>
constexpr U32 CalcMinNumArrayRegisters(U32 numValues);

//! @brief Gets the value of a register at the desired position (compile time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
template <U32 _index>
inline F32 GetValue(__m128 reg);

//! @brief Gets the value of a register at the desired position (run time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
inline F32 GetValue(__m128 reg, U32 index);

//! @brief Gets the value of a register at the desired position (compile time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
template <U32 _index>
inline F64 GetValue(__m128d reg);

//! @brief Gets the value of a register at the desired position (run time version)
//! @tparam _index: Index of the value
//! @return Value of a register at the desired position
inline F64 GetValue(__m128d reg, U32 index);

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

template <I32 _i0, I32 _i1>
constexpr I32 ShuffleMask();

constexpr I32 ShuffleMask(I32 i0, I32 i1);

template <I32 _i0, I32 _i1, I32 _i2, I32 _i3>
constexpr I32 ShuffleMask();

constexpr I32 ShuffleMask(I32 i0, I32 i1, I32 i2, I32 i3);

//! @brief Returns if the template type is a sse register type
//! @tparam _registerType: Register type
//! @return Inatance of  the underlying data type of a register
template <typename _registerType>
constexpr bool IsRegisterType();

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
