#pragma once

#pragma once
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"

#include <x86intrin.h>

namespace GDL
{

// SSEMaxRegisterSize -------------------------------------------------------------------------------------------------

constexpr U32 SSEMaxRegisterSize()
{
#ifdef __AVX512F__
    return 512;
#elif __AVX2__
    return 256;
#else
    return 128;
#endif
}

// SSEGetFittingRegister ----------------------------------------------------------------------------------------------

template <typename _type, U32 _registerSize>
auto SSEGetFittingRegister()
{
    static_assert(_registerSize == 128 || _registerSize == 256 || _registerSize == 512,
                  "Register size must be 128, 256 or 512");

    // clang-format off
    if constexpr(std::is_same<_type, F32>::value && _registerSize == 128)
        return __m128();
    if constexpr(std::is_same<_type, F64>::value && _registerSize == 128)
        return __m128d();
    if constexpr(std::is_same<_type, I32>::value && _registerSize == 128)
        return __m128i();
#ifdef __AVX2__
    if constexpr(std::is_same<_type, F32>::value && _registerSize == 256)
        return __m256();
    if constexpr(std::is_same<_type, F64>::value && _registerSize == 256)
        return __m256d();
    if constexpr(std::is_same<_type, I32>::value && _registerSize == 256)
        return __m256i();
#ifdef __AVX512F__
    if constexpr(std::is_same<_type, F32>::value && _registerSize == 512)
        return __m512();
    if constexpr(std::is_same<_type, F64>::value && _registerSize == 512)
        return __m512d();
    if constexpr(std::is_same<_type, I32>::value && _registerSize == 512)
        return __m512i();
#endif // __AVX512F__
#endif // __AVX2__
    // clang-format on
}


// AlignmentBytes -----------------------------------------------------------------------------------------------------

template <typename _registerType>
constexpr const U32 AlignmentBytes = 0;

template <>
constexpr const U32 AlignmentBytes<__m128> = 16;

template <>
constexpr const U32 AlignmentBytes<__m128d> = 16;

template <>
constexpr const U32 AlignmentBytes<__m128i> = 16;

#ifdef __AVX2__
template <>
constexpr const U32 AlignmentBytes<__m256> = 32;

template <>
constexpr const U32 AlignmentBytes<__m256d> = 32;

template <>
constexpr const U32 AlignmentBytes<__m256i> = 32;

#ifdef __AVX512F__
template <>
constexpr const U32 AlignmentBytes<__m512> = 64;

template <>
constexpr const U32 AlignmentBytes<__m512d> = 64;

template <>
constexpr const U32 AlignmentBytes<__m512i> = 64;
#endif // __AVX512F__
#endif // __AVX2__



// GetNumRegisterEntries ----------------------------------------------------------------------------------------------

//! @brief Gets the number of values that can be stored in the register
//! @tparam _registerType: Register type
//! @return Number of values that can be stored in the register
template <typename _registerType>
constexpr U32 GetNumRegisterEntries()
{
    // clang-format off
    if constexpr(std::is_same<_registerType, __m128>::value)
        return 4;
    else if constexpr(std::is_same<_registerType, __m128d>::value)
        return 2;
    else if constexpr(std::is_same<_registerType, __m128i>::value)
        return 4;
#ifdef __AVX2__
    else if constexpr(std::is_same<_registerType, __m256>::value)
        return 8;
    else if constexpr(std::is_same<_registerType, __m256d>::value)
        return 4;
    else if constexpr(std::is_same<_registerType, __m256i>::value)
        return 8;
#ifdef __AVX512F__
    else if constexpr(std::is_same<_registerType, __m512>::value)
        return 16;
    else if constexpr(std::is_same<_registerType, __m512d>::value)
        return 8;
    else if constexpr(std::is_same<_registerType, __m512i>::value)
        return 16;
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "No number of entries known for given register type");
    // clang-format on
}



// _mmx_store_p --------------------------------------------------------------------------------------------------------

//! @brief Stores the values of a register to a given memory location
//! @tparam _type: Type of the registers values
//! @tparam _registerType: Register type
//! @param ptr: Pointer to a piece of memory where the data should be stored
//! @param reg: Register that provides the data
template <typename _type, typename _registerType>
inline void _mmx_store_p(_type* ptr, const _registerType& reg)
{
    // clang-format off
    if constexpr(std::is_same<_registerType, __m128>::value && std::is_same<_type, F32>::value)
        _mm_store_ps(ptr, reg);
    else if constexpr(std::is_same<_registerType, __m128d>::value && std::is_same<_type, F64>::value)
        _mm_store_pd(ptr, reg);
#ifdef __AVX2__
    else if constexpr(std::is_same<_registerType, __m256>::value && std::is_same<_type, F32>::value)
        _mm256_store_ps(ptr, reg);
    else if constexpr(std::is_same<_registerType, __m256d>::value && std::is_same<_type, F64>::value)
        _mm256_store_pd(ptr, reg);
#ifdef __AVX512F__
    else if constexpr(std::is_same<_registerType, __m512>::value && std::is_same<_type, F32>::value)
        _mm512_store_ps(ptr, reg);
    else if constexpr(std::is_same<_registerType, __m512d>::value && std::is_same<_type, F64>::value)
        _mm512_store_pd(ptr, reg);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
    // clang-format on
}



// _mmx_setzero_p -----------------------------------------------------------------------------------------------------

//! @brief Template to create a register with all entries set to zero
//! @tparam _registerType: Register type
//! @return Register with all entries set to zero
template <typename _registerType>
inline _registerType _mmx_setzero_p()
{
    // clang-format off
    if constexpr(std::is_same<_registerType, __m128>::value)
        return _mm_setzero_ps();
    else if constexpr(std::is_same<_registerType, __m128d>::value)
        return _mm_setzero_pd();
    else if constexpr(std::is_same<_registerType, __m128i>::value)
        return _mm_setzero_si128();
#ifdef __AVX2__
    if constexpr(std::is_same<_registerType, __m256>::value)
        return _mm256_setzero_ps();
    else if constexpr(std::is_same<_registerType, __m256d>::value)
        return _mm256_setzero_pd();
    else if constexpr(std::is_same<_registerType, __m256i>::value)
        return _mm256_setzero_si256();
#ifdef __AVX512F__
    if constexpr(std::is_same<_registerType, __m512>::value)
        return _mm512_setzero_ps();
    else if constexpr(std::is_same<_registerType, __m512d>::value)
        return _mm512_setzero_pd();
    else if constexpr(std::is_same<_registerType, __m512i>::value)
        return _mm512_setzero_si512();
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
    // clang-format on
}



// _mmx_set1_p --------------------------------------------------------------------------------------------------------

//! @brief Template to create a register with all entries set to the same value
//! @tparam _registerType: Register type
//! @tparam _type: Type of the value
//! @param value: Value that should be set
//! @return Register with all entries set to the same value
template <typename _registerType, typename _type>
inline _registerType _mmx_set1_p(_type value)
{
    // clang-format off
    if constexpr(std::is_same<_registerType, __m128>::value && std::is_same<_type, F32>::value)
        return _mm_set1_ps(value);
    else if constexpr(std::is_same<_registerType, __m128d>::value && std::is_same<_type, F64>::value)
        return _mm_set1_pd(value);
    else if constexpr(std::is_same<_registerType, __m128i>::value && std::is_same<_type, I32>::value)
        return _mm_set1_epi32(value);
#ifdef __AVX2__
    if constexpr(std::is_same<_registerType, __m256>::value && std::is_same<_type, F32>::value)
        return _mm256_set1_ps(value);
    else if constexpr(std::is_same<_registerType, __m256d>::value && std::is_same<_type, F64>::value)
        return _mm256_set1_pd(value);
    else if constexpr(std::is_same<_registerType, __m256i>::value && std::is_same<_type, I32>::value)
        return _mm256_set1_epi32(value);
#ifdef __AVX512F__
    if constexpr(std::is_same<_registerType, __m512>::value && std::is_same<_type, F32>::value)
        return _mm512_set1_ps(value);
    else if constexpr(std::is_same<_registerType, __m512d>::value && std::is_same<_type, F64>::value)
        return _mm512_set1_pd(value);
    else if constexpr(std::is_same<_registerType, __m512i>::value && std::is_same<_type, I32>::value)
        return _mm512_set1_epi32(value);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
    // clang-format on
}



// _mmx_setr_p --------------------------------------------------------------------------------------------------------

//! @brief Template to create a register with given values in reverse order
//! @tparam _registerType: Register type
//! @tparam _type: Type of the value
//! @param v0: First value
//! @param v1: Second value
//! @param v2: Third value
//! @param v3: Fourth value
//! @return Register with given values in reverse order
template <typename _registerType, typename _type>
inline _registerType _mmx_setr_p(_type v0, _type v1, _type v2, _type v3)
{
    // clang-format off
    if constexpr(std::is_same<_registerType, __m128>::value && std::is_same<_type, F32>::value)
        return _mm_setr_ps(v0,v1,v2,v3);
    else if constexpr(std::is_same<_registerType, __m128d>::value && std::is_same<_type, F64>::value)
        return _mm_setr_pd(v0,v1,v2,v3);
    else if constexpr(std::is_same<_registerType, __m128i>::value && std::is_same<_type, I32>::value)
        return _mm_setr_epi32(v0,v1,v2,v3);
#ifdef __AVX2__
    if constexpr(std::is_same<_registerType, __m256>::value && std::is_same<_type, F32>::value)
        return _mm256_setr_ps(v0,v1,v2,v3);
    else if constexpr(std::is_same<_registerType, __m256d>::value && std::is_same<_type, F64>::value)
        return _mm256_setr_pd(v0,v1,v2,v3);
    else if constexpr(std::is_same<_registerType, __m256i>::value && std::is_same<_type, I32>::value)
        return _mm256_setr_epi32(v0,v1,v2,v3);
#ifdef __AVX512F__
    if constexpr(std::is_same<_registerType, __m512>::value && std::is_same<_type, F32>::value)
        return _mm512_setr_ps(v0,v1,v2,v3);
    else if constexpr(std::is_same<_registerType, __m512d>::value && std::is_same<_type, F64>::value)
        return _mm512_setr_pd(v0,v1,v2,v3);
    else if constexpr(std::is_same<_registerType, __m512i>::value && std::is_same<_type, I32>::value)
        return _mm512_setr_epi32(v0,v1,v2,v3);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
    // clang-format on
}



// _mmx_add_p ---------------------------------------------------------------------------------------------------------

//! @brief Template for register addition
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the addition
template <typename _registerType>
inline _registerType _mmx_add_p(const _registerType& lhs, const _registerType& rhs)
{
    // clang-format off
    if constexpr(std::is_same<_registerType, __m128>::value)
        return _mm_add_ps(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m128d>::value)
        return _mm_add_pd(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m128i>::value)
        return _mm_add_epi32(lhs,rhs);
#ifdef __AVX2__
    if constexpr(std::is_same<_registerType, __m256>::value)
        return _mm256_add_ps(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m256d>::value)
        return _mm256_add_pd(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m256i>::value)
        return _mm256_add_epi32(lhs,rhs);
#ifdef __AVX512F__
    if constexpr(std::is_same<_registerType, __m512>::value)
        return _mm512_add_ps(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m512d>::value)
        return _mm512_add_pd(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m512i>::value)
        return _mm512_add_epi32(lhs,rhs);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
    // clang-format on
}



// _mmx_mul_p ---------------------------------------------------------------------------------------------------------

//! @brief Template for register multiplication
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the multiplication
template <typename _registerType>
inline _registerType _mmx_mul_p(const _registerType& lhs, const _registerType& rhs)
{
    // clang-format off
    if constexpr(std::is_same<_registerType, __m128>::value)
        return _mm_mul_ps(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m128d>::value)
        return _mm_mul_pd(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m128i>::value)
        return _mm_mul_epi32(lhs,rhs);
#ifdef __AVX2__
    if constexpr(std::is_same<_registerType, __m256>::value)
        return _mm256_mul_ps(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m256d>::value)
        return _mm256_mul_pd(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m256i>::value)
        return _mm256_mul_epi32(lhs,rhs);
#ifdef __AVX512F__
    if constexpr(std::is_same<_registerType, __m512>::value)
        return _mm512_mul_ps(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m512d>::value)
        return _mm512_mul_pd(lhs,rhs);
    else if constexpr(std::is_same<_registerType, __m512i>::value)
        return _mm512_mul_epi32(lhs,rhs);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
    // clang-format on
}



// _mmx_fmadd_p -------------------------------------------------------------------------------------------------------

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
inline _registerType _mmx_fmadd_p(const _registerType& lhsM, const _registerType& rhsM, const _registerType& add)
{
#ifndef __FMA__
    return _mmx_add_p(_mmx_mul_p(lhsM, rhsM), add);
#else
    // clang-format off
    if constexpr(std::is_same<_registerType, __m128>::value)
        return _mm_fmadd_ps(lhsM, rhsM, add);
    else if constexpr(std::is_same<_registerType, __m128d>::value)
        return _mm_fmadd_pd(lhsM, rhsM, add);
#ifdef __AVX2__
    else if constexpr(std::is_same<_registerType, __m256>::value)
        return _mm256_fmadd_ps(lhsM, rhsM, add);
    else if constexpr(std::is_same<_registerType, __m256d>::value)
        return _mm256_fmadd_pd(lhsM, rhsM, add);
#ifdef __AVX512F__
    else if constexpr(std::is_same<_registerType, __m512>::value)
        return _mm512_fmadd_ps(lhsM, rhsM, add);
    else if constexpr(std::is_same<_registerType, __m512d>::value)
        return _mm512_fmadd_pd(lhsM, rhsM, add);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
#endif // __FMA__
    // clang-format on
}



// CalcMinNumArrayRegisters -------------------------------------------------------------------------------------------

//! @brief Calculates the minimal number of registers to store a certain number of values
//! @tparam _registerType: Register type
//! @param numValues: Number of values that should be stored
//! @return Minimal number of registers
template <typename _registerType>
constexpr U32 CalcMinNumArrayRegisters(U32 numValues)
{
    constexpr U32 registerSize = GetNumRegisterEntries<_registerType>();
    return (numValues / registerSize) + ((numValues % registerSize > 0) ? 1 : 0);
}



} // namespace GDL
