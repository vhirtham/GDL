#pragma once

#pragma once
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/pow.h"

#include <limits>
#include <x86intrin.h>

namespace GDL
{

// SSEIsRegisterType --------------------------------------------------------------------------------------------------

//! @brief Returns if the template type is a sse register type
//! @tparam _registerType: Register type
//! @return Inatance of  the underlying data type of a register
template <typename _registerType>
constexpr bool SSEIsRegisterType()
{
    if constexpr (std::is_same<_registerType, __m128>::value || std::is_same<_registerType, __m128d>::value
#ifdef __AVX2__
                  || std::is_same<_registerType, __m256>::value || std::is_same<_registerType, __m256d>::value
#ifdef __AVX512F__
                  || std::is_same<_registerType, __m512>::value || std::is_same<_registerType, __m512d>::value
#endif // __AVX512F__
#endif // __AVX2__
    )
        return true;
    return false;
}



// SSEMaxRegisterSize -------------------------------------------------------------------------------------------------

//! @brief Gets the bitsize of the largest available register
//! @return Bitsize of the largest available register
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

//! @brief Gets a register of the provided bitsize which is compatible with a given data type
//! @tparam _type: Returned register must be capable to store values of this data type
//! @tparam _registerSize: Bit size of the returned register
//! @return Register of the provided bitsize which is compatible with a given data type
//! @remark A function call should usually be combined with decltype
template <typename _type, U32 _registerSize>
auto SSEGetFittingRegister()
{
    static_assert(_registerSize == 128 || _registerSize == 256 || _registerSize == 512,
                  "Register size must be 128, 256 or 512");

    if constexpr (std::is_same<_type, F32>::value && _registerSize == 128)
        return __m128();
    if constexpr (std::is_same<_type, F64>::value && _registerSize == 128)
        return __m128d();
    if constexpr (std::is_same<_type, I32>::value && _registerSize == 128)
        return __m128i();
#ifdef __AVX2__
    if constexpr (std::is_same<_type, F32>::value && _registerSize == 256)
        return __m256();
    if constexpr (std::is_same<_type, F64>::value && _registerSize == 256)
        return __m256d();
    if constexpr (std::is_same<_type, I32>::value && _registerSize == 256)
        return __m256i();
#ifdef __AVX512F__
    if constexpr (std::is_same<_type, F32>::value && _registerSize == 512)
        return __m512();
    if constexpr (std::is_same<_type, F64>::value && _registerSize == 512)
        return __m512d();
    if constexpr (std::is_same<_type, I32>::value && _registerSize == 512)
        return __m512i();
#endif // __AVX512F__
#endif // __AVX2__
}



// SSEGetDataType -----------------------------------------------------------------------------------------------------

//! @brief Gets an inatance of the underlying data type of a register
//! @tparam _registerType: Register type
//! @tparam _returnTypeIfNoTARegister: If set to true, this function returns an instance of the template parameters type
//! if it is not a register. If set to false (default), it will throw.
//! @return Inatance of  the underlying data type of a register
//! @remark A function call should usually be combined with decltype
template <typename _registerType, bool _returnTypeIfNoTARegister = false>
constexpr auto SSEGetDataType()
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return F32{0};
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return F64{0};
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return F32{0};
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return F64{0};
#ifdef __AVX512F__
    else if constexpr (std::is_same<_registerType, __m512>::value)
        return F32{0};
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return F64{0};
#endif // __AVX512F__
#endif // __AVX2__
    else
    {
        if constexpr (_returnTypeIfNoTARegister)
            return _registerType();
        else
            throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
    }
}



// alignmentBytes -----------------------------------------------------------------------------------------------------

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

#ifdef __AVX512F__
template <>
constexpr const U32 alignmentBytes<__m512> = 64;

template <>
constexpr const U32 alignmentBytes<__m512d> = 64;

template <>
constexpr const U32 alignmentBytes<__m512i> = 64;
#endif // __AVX512F__
#endif // __AVX2__



// GetNumRegisterEntries ----------------------------------------------------------------------------------------------

//! @brief Gets the number of values that can be stored in the register
//! @tparam _registerType: Register type
//! @return Number of values that can be stored in the register
template <typename _registerType>
constexpr U32 SSEGetNumRegisterEntries()
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return 4;
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return 2;
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return 8;
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return 4;
#ifdef __AVX512F__
    else if constexpr (std::is_same<_registerType, __m512>::value)
        return 16;
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return 8;
#endif // __AVX512F__
#endif // __AVX2__
    else
        return 0;
}



// _mmx_store_p --------------------------------------------------------------------------------------------------------

//! @brief Stores the values of a register to a given memory location
//! @tparam _type: Type of the registers values
//! @tparam _registerType: Register type
//! @param ptr: Pointer to a piece of memory where the data should be stored
//! @param reg: Register that provides the data
template <typename _type, typename _registerType>
inline void _mmx_store_p(_type* ptr, _registerType reg)
{
    if constexpr (std::is_same<_registerType, __m128>::value && std::is_same<_type, F32>::value)
        _mm_store_ps(ptr, reg);
    else if constexpr (std::is_same<_registerType, __m128d>::value && std::is_same<_type, F64>::value)
        _mm_store_pd(ptr, reg);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value && std::is_same<_type, F32>::value)
        _mm256_store_ps(ptr, reg);
    else if constexpr (std::is_same<_registerType, __m256d>::value && std::is_same<_type, F64>::value)
        _mm256_store_pd(ptr, reg);
#ifdef __AVX512F__
    else if constexpr (std::is_same<_registerType, __m512>::value && std::is_same<_type, F32>::value)
        _mm512_store_ps(ptr, reg);
    else if constexpr (std::is_same<_registerType, __m512d>::value && std::is_same<_type, F64>::value)
        _mm512_store_pd(ptr, reg);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
}



// _mmx_setzero_p -----------------------------------------------------------------------------------------------------

//! @brief Template to create a register with all entries set to zero
//! @tparam _registerType: Register type
//! @return Register with all entries set to zero
template <typename _registerType>
inline _registerType _mmx_setzero_p()
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_setzero_ps();
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_setzero_pd();
    else if constexpr (std::is_same<_registerType, __m128i>::value)
        return _mm_setzero_si128();
#ifdef __AVX2__
    if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_setzero_ps();
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_setzero_pd();
    else if constexpr (std::is_same<_registerType, __m256i>::value)
        return _mm256_setzero_si256();
#ifdef __AVX512F__
    if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_setzero_ps();
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_setzero_pd();
    else if constexpr (std::is_same<_registerType, __m512i>::value)
        return _mm512_setzero_si512();
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
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
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_set1_ps(value);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_set1_pd(value);
#ifdef __AVX2__
    if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_set1_ps(value);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_set1_pd(value);
#ifdef __AVX512F__
    if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_set1_ps(value);
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_set1_pd(value);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
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
template <typename _registerType, typename... _args>
inline _registerType _mmx_setr_p(_args... args)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_setr_ps(std::forward<_args>(args)...);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_setr_pd(std::forward<_args>(args)...);
#ifdef __AVX2__
    if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_setr_ps(std::forward<_args>(args)...);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_setr_pd(std::forward<_args>(args)...);
#ifdef __AVX512F__
    if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_setr_ps(std::forward<_args>(args)...);
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_setr_pd(std::forward<_args>(args)...);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
}



// _mmx_add_p ---------------------------------------------------------------------------------------------------------

//! @brief Template for register addition
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the addition
template <typename _registerType>
inline _registerType _mmx_add_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_add_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_add_pd(lhs, rhs);
#ifdef __AVX2__
    if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_add_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_add_pd(lhs, rhs);
#ifdef __AVX512F__
    if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_add_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_add_pd(lhs, rhs);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// _mmx_sub_p ---------------------------------------------------------------------------------------------------------

//! @brief Template for register substraction
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the substraction
template <typename _registerType>
inline _registerType _mmx_sub_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_sub_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_sub_pd(lhs, rhs);
#ifdef __AVX2__
    if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_sub_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_sub_pd(lhs, rhs);
#ifdef __AVX512F__
    if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_sub_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_sub_pd(lhs, rhs);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// _mmx_mul_p ---------------------------------------------------------------------------------------------------------

//! @brief Template for register multiplication
//! @tparam _registerType: Register type
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return Result of the multiplication
template <typename _registerType>
inline _registerType _mmx_mul_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_mul_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_mul_pd(lhs, rhs);
#ifdef __AVX2__
    if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_mul_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_mul_pd(lhs, rhs);
#ifdef __AVX512F__
    if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_mul_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_mul_pd(lhs, rhs);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
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
inline _registerType _mmx_fmadd_p(_registerType lhsM, _registerType rhsM, _registerType add)
{
#ifndef __FMA__
    return _mmx_add_p(_mmx_mul_p(lhsM, rhsM), add);
#else
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_fmadd_ps(lhsM, rhsM, add);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_fmadd_pd(lhsM, rhsM, add);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_fmadd_ps(lhsM, rhsM, add);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_fmadd_pd(lhsM, rhsM, add);
#ifdef __AVX512F__
    else if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_fmadd_ps(lhsM, rhsM, add);
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_fmadd_pd(lhsM, rhsM, add);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
#endif // __FMA__
}



// _mmx_cmpeq_p -------------------------------------------------------------------------------------------------------

//! @brief Compares two registers for equality
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mmx_cmpeq_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_cmpeq_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_cmpeq_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_cmp_ps(lhs, rhs, _CMP_EQ_OS);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_cmp_pd(lhs, rhs, _CMP_EQ_OS);
#ifdef __AVX512F__
    // Check https://software.intel.com/en-us/forums/intel-isa-extensions/topic/749055
    else if constexpr (std::is_same<_registerType, __m512>::value)
        throw(__PRETTY_FUNCTION__, "Not implemented");
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        throw(__PRETTY_FUNCTION__, "Not implemented");
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// _mmx_cmple_p -------------------------------------------------------------------------------------------------------

//! @brief Compares lhs <= rhs for two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mmx_cmple_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_cmple_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_cmple_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_cmp_ps(lhs, rhs, _CMP_LE_OS);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_cmp_pd(lhs, rhs, _CMP_LE_OS);
#ifdef __AVX512F__
    // Check https://software.intel.com/en-us/forums/intel-isa-extensions/topic/749055
    else if constexpr (std::is_same<_registerType, __m512>::value)
        throw(__PRETTY_FUNCTION__, "Not implemented");
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        throw(__PRETTY_FUNCTION__, "Not implemented");
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// _mmx_cmpgt_p -------------------------------------------------------------------------------------------------------

//! @brief Compares lhs > rhs for two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the results of the comparison
template <typename _registerType>
inline auto _mmx_cmpgt_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_cmpgt_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_cmpgt_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_cmp_ps(lhs, rhs, _CMP_GT_OS);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_cmp_pd(lhs, rhs, _CMP_GT_OS);
#ifdef __AVX512F__
    // Check https://software.intel.com/en-us/forums/intel-isa-extensions/topic/749055
    else if constexpr (std::is_same<_registerType, __m512>::value)
        throw(__PRETTY_FUNCTION__, "Not implemented");
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        throw(__PRETTY_FUNCTION__, "Not implemented");
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// _mmx_max_p ---------------------------------------------------------------------------------------------------------

//! @brief Calculates the maximum values of two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the maximum values
template <typename _registerType>
inline _registerType _mmx_max_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_max_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_max_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_max_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_max_pd(lhs, rhs);
#ifdef __AVX512F__
    else if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_max_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_max_pd(lhs, rhs);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// _mmx_min_p ---------------------------------------------------------------------------------------------------------

//! @brief Calculates the minimum values of two registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register which stores the minimum values
template <typename _registerType>
inline _registerType _mmx_min_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_min_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_min_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_min_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_min_pd(lhs, rhs);
#ifdef __AVX512F__
    else if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_min_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_min_pd(lhs, rhs);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// _mmx_andnot_p ------------------------------------------------------------------------------------------------------

//! @brief Performs a and-not operation on the passed registers
//! @tparam _registerType: Register type
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Result of the and-not operation
template <typename _registerType>
inline _registerType _mmx_andnot_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_andnot_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_andnot_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_andnot_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_andnot_pd(lhs, rhs);
#ifdef __AVX512F__
    else if constexpr (std::is_same<_registerType, __m512>::value)
        return _mm512_andnot_ps(lhs, rhs);
    else if constexpr (std::is_same<_registerType, __m512d>::value)
        return _mm512_andnot_pd(lhs, rhs);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// _mmx_movemask_epi8 -------------------------------------------------------------------------------------------------

//! @brief Create mask from the most significant bit of each 8-bit element in a register and returns it
//! @tparam _registerType: Register type
//! @param reg: Register which serves as source for the mask
//! @return Mask from the most significant bit of each 8-bit element of the source register
template <typename _registerType>
inline auto _mmx_movemask_epi8(_registerType reg)
{
    if constexpr (std::is_same<_registerType, __m128i>::value)
        return static_cast<U16>(_mm_movemask_epi8(reg));
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256i>::value)
        return static_cast<U32>(_mm256_movemask_epi8(reg));
#ifdef __AVX512F__
    // Check https://software.intel.com/en-us/forums/intel-isa-extensions/topic/749055
    else if constexpr (std::is_same<_registerType, __m512i>::value)
        throw(__PRETTY_FUNCTION__, "Not implemented");
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// SSEAbs -------------------------------------------------------------------------------------------------------------

//! @brief Calculates the absolute value of each element
//! @tparam _registerType: Register type
//! @param reg: Input register
//! @return Register with absolute values
//! @remark source:
//! https://stackoverflow.com/questions/23847377/how-does-this-function-compute-the-absolute-value-of-a-float-through-a-not-and-a
template <typename _registerType>
_registerType SSEAbs(_registerType reg)
{
    const _registerType mask = _mmx_set1_p<_registerType>(-0.0);
    return _mmx_andnot_p(mask, reg);
}



// CalcMinNumArrayRegisters -------------------------------------------------------------------------------------------

//! @brief Calculates the minimal number of registers to store a certain number of values
//! @tparam _registerType: Register type
//! @param numValues: Number of values that should be stored
//! @return Minimal number of registers
template <typename _registerType>
constexpr U32 SSECalcMinNumArrayRegisters(U32 numValues)
{
    constexpr U32 registerSize = SSEGetNumRegisterEntries<_registerType>();
    return (numValues / registerSize) + ((numValues % registerSize > 0) ? 1 : 0);
}



// SSEReinterpretAsIntRegister ----------------------------------------------------------------------------------------

//! @brief Reinterprets the passed register as a integer register of the same size
//! @tparam _registerType: Register type
//! @param reg: Register that should be cast
//! @return Integer register
template <typename _registerType>
inline auto SSEReinterpretAsIntRegister(_registerType reg)
{
    if constexpr (std::is_same<_registerType, __m128>::value || std::is_same<_registerType, __m128d>::value)
        return reinterpret_cast<__m128i>(reg);
#ifdef __AVX2__
    if constexpr (std::is_same<_registerType, __m256>::value || std::is_same<_registerType, __m256d>::value)
        return reinterpret_cast<__m256i>(reg);
#ifdef __AVX512F__
    if constexpr (std::is_same<_registerType, __m512>::value || std::is_same<_registerType, __m512d>::value)
        return reinterpret_cast<__m512i>(reg);
#endif // __AVX512F__
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



// SSEGetComparisonValueAllTrue ---------------------------------------------------------------------------------------

//! @brief Calculates a value with a certain bitsignature that corresponds to the expected result from a register
//! comparison where all element comparisons return true
//! @tparam _registerType: RegisterType
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @return Value with specific bit signature
template <typename _registerType, U32 _numComparedValues = SSEGetNumRegisterEntries<_registerType>()>
constexpr auto SSECalculateComparisonValueAllTrue()
{
    using ReturnType = decltype(_mmx_movemask_epi8(SSEReinterpretAsIntRegister(_mmx_setzero_p<_registerType>())));
    constexpr U32 numRegisterEntries = SSEGetNumRegisterEntries<_registerType>();

    if constexpr (_numComparedValues == numRegisterEntries)
        return std::numeric_limits<ReturnType>::max();
    else
    {
        constexpr U32 numUnusedBits = (sizeof(ReturnType) * 8 * _numComparedValues) / numRegisterEntries;
        return static_cast<ReturnType>(Pow<numUnusedBits>(2) - 1);
    }
}



// SSECompareAllTrue -------------------------------------------------------------------------------------------------

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
inline bool SSECompareAllTrue(_registerType lhs, _registerType rhs, _compFunction compFunction)
{
    constexpr U32 numRegisterEntries = SSEGetNumRegisterEntries<_registerType>();

    static_assert(_numComparedValues > 0 && _numComparedValues <= numRegisterEntries,
                  "Invalid number of compared values ---> [1 ... numRegisterEntries]");

    auto cmpResult = _mmx_movemask_epi8(SSEReinterpretAsIntRegister(compFunction(lhs, rhs)));
    constexpr auto refResult = SSECalculateComparisonValueAllTrue<_registerType, _numComparedValues>();

    static_assert(std::is_same_v<const decltype(cmpResult), decltype(refResult)>, "Mismatching types for comparison");

    if constexpr (_numComparedValues != numRegisterEntries)
        cmpResult &= refResult; // Set bits of elemts that should not be compared to zero

    return cmpResult >= refResult;
}



// SSECompareAllEqual -------------------------------------------------------------------------------------------------


//! @brief Compares if the values of two register are equal. This function returns true if all values are equal and
//! false otherwise
//! @tparam _registerType: Register type
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return TRUE / FALSE
template <typename _registerType, U32 _numComparedValues = SSEGetNumRegisterEntries<_registerType>()>
inline bool SSECompareAllEqual(_registerType lhs, _registerType rhs)
{
    return SSECompareAllTrue<_registerType, _numComparedValues, decltype(_mmx_cmpeq_p<_registerType>)>(
            lhs, rhs, &_mmx_cmpeq_p<_registerType>);
}



// SSECompareAllLessEqual ---------------------------------------------------------------------------------------------


//! @brief Compares lhs <= rhs for two register. This function returns true if all lhs values are less or equal when
//! compared to the corresponding rhs value
//! @tparam _registerType: Register type
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return TRUE / FALSE
template <typename _registerType, U32 _numComparedValues = SSEGetNumRegisterEntries<_registerType>()>
inline bool SSECompareAllLessEqual(_registerType lhs, _registerType rhs)
{
    return SSECompareAllTrue<_registerType, _numComparedValues, decltype(_mmx_cmple_p<_registerType>)>(
            lhs, rhs, &_mmx_cmple_p<_registerType>);
}



// SSECompareAllGreaterThan
// ---------------------------------------------------------------------------------------------


//! @brief Compares lhs > rhs for two register. This function returns true if all lhs values are greater than the rhs
//! values. False if not.
//! @tparam _registerType: Register type
//! @tparam _numComparedValues: Number of register values that should be compared. If the value is smaller than the
//! number of register values, only the first elements of the register are compared.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return TRUE / FALSE
template <typename _registerType, U32 _numComparedValues = SSEGetNumRegisterEntries<_registerType>()>
inline bool SSECompareAllGreaterThan(_registerType lhs, _registerType rhs)
{
    return SSECompareAllTrue<_registerType, _numComparedValues, decltype(_mmx_cmpgt_p<_registerType>)>(
            lhs, rhs, &_mmx_cmpgt_p<_registerType>);
}



} // namespace GDL
