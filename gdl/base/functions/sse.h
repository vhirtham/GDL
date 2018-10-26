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
    throw Exception(__PRETTY_FUNCTION__, "No number of entries known for given register type");
    return 0;
}

template <>
constexpr U32 GetNumRegisterEntries<__m128>()
{
    return 4;
}

template <>
constexpr U32 GetNumRegisterEntries<__m128d>()
{
    return 2;
}

template <>
constexpr U32 GetNumRegisterEntries<__m128i>()
{
    return 4;
}

#ifdef __AVX2__
template <>
constexpr U32 GetNumRegisterEntries<__m256>()
{
    return 8;
}

template <>
constexpr U32 GetNumRegisterEntries<__m256d>()
{
    return 4;
}

template <>
constexpr U32 GetNumRegisterEntries<__m256i>()
{
    return 8;
}

#ifdef __AVX512F__
template <>
constexpr U32 GetNumRegisterEntries<__m512>()
{
    return 16;
}

template <>
constexpr U32 GetNumRegisterEntries<__m512d>()
{
    return 8;
}

template <>
constexpr U32 GetNumRegisterEntries<__m512i>()
{
    return 16;
}
#endif // __AVX512F__
#endif // __AVX2__



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



// _mmx_set1_p --------------------------------------------------------------------------------------------------------

//! @brief Template to create a register with all entries set to the same value
//! @tparam _registerType: Register type
//! @tparam _type: Type of the value
//! @param value: Value that should be set
//! @return Register with all entries set to the same value
template <typename _registerType, typename _type>
inline _registerType _mmx_set1_p([[maybe_unused]] _type value)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
}

template <>
inline __m128 _mmx_set1_p(F32 value)
{
    return _mm_set1_ps(value);
}

template <>
inline __m128d _mmx_set1_p(F64 value)
{
    return _mm_set1_pd(value);
}

template <>
inline __m128i _mmx_set1_p(I32 value)
{
    return _mm_set1_epi32(value);
}

#ifdef __AVX2__
template <>
inline __m256 _mmx_set1_p(F32 value)
{
    return _mm256_set1_ps(value);
}

template <>
inline __m256d _mmx_set1_p(F64 value)
{
    return _mm256_set1_pd(value);
}

template <>
inline __m256i _mmx_set1_p(I32 value)
{
    return _mm256_set1_epi32(value);
}

#ifdef __AVX512F__
template <>
inline __m512 _mmx_set1_p(F32 value)
{
    return _mm512_set1_ps(value);
}

template <>
inline __m512d _mmx_set1_p(F64 value)
{
    return _mm512_set1_pd(value);
}

template <>
inline __m512i _mmx_set1_p(I32 value)
{
    return _mm512_set1_epi32(value);
}
#endif // __AVX512F__
#endif // __AVX2__



// _mmx_add_p ---------------------------------------------------------------------------------------------------------

//! @brief Template for register addition
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the addition
template <typename _registerType>
inline _registerType _mmx_add_p([[maybe_unused]] _registerType lhs, [[maybe_unused]] _registerType rhs)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type");
}

template <>
inline __m128 _mmx_add_p(__m128 lhs, __m128 rhs)
{
    return _mm_add_ps(lhs, rhs);
}

template <>
inline __m128d _mmx_add_p(__m128d lhs, __m128d rhs)
{
    return _mm_add_pd(lhs, rhs);
}

template <>
inline __m128i _mmx_add_p(__m128i lhs, __m128i rhs)
{
    return _mm_add_epi32(lhs, rhs);
}

#ifdef __AVX2__
template <>
inline __m256 _mmx_add_p(__m256 lhs, __m256 rhs)
{
    return _mm256_add_ps(lhs, rhs);
}

template <>
inline __m256d _mmx_add_p(__m256d lhs, __m256d rhs)
{
    return _mm256_add_pd(lhs, rhs);
}

template <>
inline __m256i _mmx_add_p(__m256i lhs, __m256i rhs)
{
    return _mm256_add_epi32(lhs, rhs);
}

#ifdef __AVX512F__
template <>
inline __m512 _mmx_add_p(__m512 lhs, __m512 rhs)
{
    return _mm512_add_ps(lhs, rhs);
}

template <>
inline __m512d _mmx_add_p(__m512d lhs, __m512d rhs)
{
    return _mm512_add_pd(lhs, rhs);
}

template <>
inline __m512i _mmx_add_p(__m512i lhs, __m512i rhs)
{
    return _mm512_add_epi32(lhs, rhs);
}
#endif // __AVX512F__
#endif // __AVX2__



// _mmx_mul_p ---------------------------------------------------------------------------------------------------------

//! @brief Template for register multiplication
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the multiplication
template <typename _registerType>
inline _registerType _mmx_mul_p([[maybe_unused]] _registerType lhs, [[maybe_unused]] _registerType rhs)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type");
}

template <>
inline __m128 _mmx_mul_p(__m128 lhs, __m128 rhs)
{
    return _mm_mul_ps(lhs, rhs);
}

template <>
inline __m128d _mmx_mul_p(__m128d lhs, __m128d rhs)
{
    return _mm_mul_pd(lhs, rhs);
}

template <>
inline __m128i _mmx_mul_p(__m128i lhs, __m128i rhs)
{
    return _mm_mul_epi32(lhs, rhs);
}

#ifdef __AVX2__
template <>
inline __m256 _mmx_mul_p(__m256 lhs, __m256 rhs)
{
    return _mm256_mul_ps(lhs, rhs);
}

template <>
inline __m256d _mmx_mul_p(__m256d lhs, __m256d rhs)
{
    return _mm256_mul_pd(lhs, rhs);
}

template <>
inline __m256i _mmx_mul_p(__m256i lhs, __m256i rhs)
{
    return _mm256_mul_epi32(lhs, rhs);
}

#ifdef __AVX512F__
template <>
inline __m512 _mmx_mul_p(__m512 lhs, __m512 rhs)
{
    return _mm512_mul_ps(lhs, rhs);
}

template <>
inline __m512d _mmx_mul_p(__m512d lhs, __m512d rhs)
{
    return _mm512_mul_pd(lhs, rhs);
}

template <>
inline __m512i _mmx_mul_p(__m512i lhs, __m512i rhs)
{
    return _mm512_mul_epi32(lhs, rhs);
}
#endif // __AVX512F__
#endif // __AVX2__



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
inline _registerType _mmx_fmadd_p(_registerType lhsM, _registerType rhsM, _registerType add)
{
    return _mmx_add_p(_mmx_mul_p(lhsM, rhsM), add);
}

#ifdef __FMA__
template <>
inline __m128 _mmx_fmadd_p(__m128 lhsM, __m128 rhsM, __m128 add)
{
    return _mm_fmadd_ps(lhsM, rhsM, add);
}

template <>
inline __m128d _mmx_fmadd_p(__m128d lhsM, __m128d rhsM, __m128d add)
{
    return _mm_fmadd_pd(lhsM, rhsM, add);
}

#ifdef __AVX2__
template <>
inline __m256 _mmx_fmadd_p(__m256 lhsM, __m256 rhsM, __m256 add)
{
    return _mm256_fmadd_ps(lhsM, rhsM, add);
}

template <>
inline __m256d _mmx_fmadd_p(__m256d lhsM, __m256d rhsM, __m256d add)
{
    return _mm256_fmadd_pd(lhsM, rhsM, add);
}

#ifdef __AVX512F__
template <>
inline __m512 _mmx_fmadd_p(__m512 lhsM, __m512 rhsM, __m512 add)
{
    return _mm512_fmadd_ps(lhsM, rhsM, add);
}

template <>
inline __m512d _mmx_fmadd_p(__m512d lhsM, __m512d rhsM, __m512d add)
{
    return _mm512_fmadd_pd(lhsM, b, add);
}

#endif // __FMA__
#endif // __AVX512F__
#endif // __AVX2__

} // namespace GDL
