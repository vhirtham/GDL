#pragma once

#include "gdl/base/sse/intrinsics.h"

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"

#include <x86intrin.h>



namespace GDL
{

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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
}



template <typename _registerType, typename _type>
inline _registerType _mmx_load_p(_type* ptr)
{
    if constexpr (std::is_same<_registerType, __m128>::value && std::is_same<_type, F32>::value)
        return _mm_load_ps(ptr);
    else if constexpr (std::is_same<_registerType, __m128d>::value && std::is_same<_type, F64>::value)
        return _mm_load_pd(ptr);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value && std::is_same<_type, F32>::value)
        return _mm256_load_ps(ptr);
    else if constexpr (std::is_same<_registerType, __m256d>::value && std::is_same<_type, F64>::value)
        return _mm256_load_pd(ptr);
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
#endif // __FMA__
}



template <U32 _mask, typename _registerType>
_registerType _mmx_dp_p(_registerType lhs, _registerType rhs)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_dp_ps(lhs, rhs, _mask);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_dp_pd(lhs, rhs, _mask);
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



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
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



template <typename _registerType>
inline auto _mmx_movemask_epi8(_registerType reg)
{
    if constexpr (std::is_same<_registerType, __m128i>::value)
        return static_cast<U16>(_mm_movemask_epi8(reg));
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256i>::value)
        return static_cast<U32>(_mm256_movemask_epi8(reg));
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



template <typename _registerType>
inline auto _mmx_cvtsx_fx(_registerType reg)
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mm_cvtss_f32(reg);
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mm_cvtsd_f64(reg);
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_cvtss_f32(reg);
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_cvtsd_f64(reg);
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



} // namespace GDL
