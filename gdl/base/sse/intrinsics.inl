#pragma once

#include "gdl/base/sse/intrinsics.h"

#include "gdl/base/exception.h"
#include "gdl/base/sse/constants.h"
#include "gdl/base/sse/negate.h"



namespace GDL
{

template <typename _type, typename _registerType>
inline void _mmx_store_p(_type* ptr, _registerType reg)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType> && std::is_same<_type, F32>::value)
        _mm_store_ps(ptr, reg);
    else if constexpr (Is__m128d<_registerType> && std::is_same<_type, F64>::value)
        _mm_store_pd(ptr, reg);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType> && std::is_same<_type, F32>::value)
        _mm256_store_ps(ptr, reg);
    else
        _mm256_store_pd(ptr, reg);
#endif // __AVX2__
}



template <typename _registerType, typename _type>
inline _registerType _mmx_load_p(_type* ptr)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType> && std::is_same<_type, F32>::value)
        return _mm_load_ps(ptr);
    else if constexpr (Is__m128d<_registerType> && std::is_same<_type, F64>::value)
        return _mm_load_pd(ptr);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType> && std::is_same<_type, F32>::value)
        return _mm256_load_ps(ptr);
    else
        return _mm256_load_pd(ptr);
#endif // __AVX2__
}



template <typename _registerType>
inline _registerType _mmx_setzero_p()
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_setzero_ps();
    else if constexpr (Is__m128d<_registerType>)
        return _mm_setzero_pd();
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_setzero_ps();
    else
        return _mm256_setzero_pd();
#endif // __AVX2__
}



template <typename _registerType, typename _type>
inline _registerType _mmx_set1_p(_type value)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_set1_ps(value);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_set1_pd(value);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_set1_ps(value);
    else
        return _mm256_set1_pd(value);
#endif // __AVX2__
}



template <typename _registerType, typename... _args>
inline _registerType _mmx_setr_p(_args... args)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_setr_ps(std::forward<_args>(args)...);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_setr_pd(std::forward<_args>(args)...);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_setr_ps(std::forward<_args>(args)...);
    else
        return _mm256_setr_pd(std::forward<_args>(args)...);
#endif // __AVX2__
}



template <typename _registerType>
inline _registerType _mmx_add_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_add_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_add_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_add_ps(lhs, rhs);
    else
        return _mm256_add_pd(lhs, rhs);
#endif // __AVX2__
}



template <typename _registerType>
inline _registerType _mmx_sub_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_sub_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_sub_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_sub_ps(lhs, rhs);
    else
        return _mm256_sub_pd(lhs, rhs);
#endif // __AVX2__
}



template <typename _registerType>
inline _registerType _mmx_mul_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_mul_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_mul_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_mul_ps(lhs, rhs);
    else
        return _mm256_mul_pd(lhs, rhs);
#endif // __AVX2__
}



template <typename _registerType>
_registerType _mmx_div_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_div_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_div_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_div_ps(lhs, rhs);
    else
        return _mm256_div_pd(lhs, rhs);
#endif // __AVX2__
}



template <typename _registerType>
inline _registerType _mmx_fmadd_p(_registerType lhsM, _registerType rhsM, _registerType add)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

#ifndef __FMA__
    return _mmx_add_p(_mmx_mul_p(lhsM, rhsM), add);
#else
    if constexpr (Is__m128<_registerType>)
        return _mm_fmadd_ps(lhsM, rhsM, add);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_fmadd_pd(lhsM, rhsM, add);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_fmadd_ps(lhsM, rhsM, add);
    else
        return _mm256_fmadd_pd(lhsM, rhsM, add);
#endif // __AVX2__
#endif // __FMA__
}



template <typename _registerType>
inline _registerType _mmx_fnmadd_p(_registerType lhsM, _registerType rhsM, _registerType add)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

#ifndef __FMA__
    return _mmx_sub_p(add, _mmx_mul_p(lhsM, rhsM));
#else
    if constexpr (Is__m128<_registerType>)
        return _mm_fnmadd_ps(lhsM, rhsM, add);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_fnmadd_pd(lhsM, rhsM, add);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_fnmadd_ps(lhsM, rhsM, add);
    else
        return _mm256_fnmadd_pd(lhsM, rhsM, add);
#endif // __AVX2__
#endif // __FMA__
}



template <typename _registerType>
inline _registerType _mmx_fmsub_p(_registerType lhsM, _registerType rhsM, _registerType sub)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

#ifndef __FMA__
    return _mmx_sub_p(_mmx_mul_p(lhsM, rhsM), sub);
#else
    if constexpr (Is__m128<_registerType>)
        return _mm_fmsub_ps(lhsM, rhsM, sub);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_fmsub_pd(lhsM, rhsM, sub);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_fmsub_ps(lhsM, rhsM, sub);
    else
        return _mm256_fmsub_pd(lhsM, rhsM, sub);
#endif // __AVX2__
#endif // __FMA__
}



template <typename _registerType>
inline _registerType _mmx_fnmsub_p(_registerType lhsM, _registerType rhsM, _registerType sub)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

#ifndef __FMA__
    return _mmx_sub_p(Negate(_mmx_mul_p(lhsM, rhsM)), sub);
#else
    if constexpr (Is__m128<_registerType>)
        return _mm_fnmsub_ps(lhsM, rhsM, sub);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_fnmsub_pd(lhsM, rhsM, sub);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_fnmsub_ps(lhsM, rhsM, sub);
    else
        return _mm256_fnmsub_pd(lhsM, rhsM, sub);
#endif // __AVX2__
#endif // __FMA__
}



template <U32 _mask, typename _registerType>
_registerType _mmx_dp_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(Is__m128<_registerType> || Is__m128d<_registerType> || Is__m256<_registerType>,
                  "Function can only be used with __m128, __m128d and __m256 registers.");

    if constexpr (Is__m128<_registerType>)
        return _mm_dp_ps(lhs, rhs, _mask);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_dp_pd(lhs, rhs, _mask);
#ifdef __AVX2__
    else
        return _mm256_dp_ps(lhs, rhs, _mask);
        // No intrinsics for __m256d
#endif // __AVX2__
}



template <typename _registerType>
_registerType _mmx_sqrt_p(_registerType reg)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_sqrt_ps(reg);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_sqrt_pd(reg);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_sqrt_ps(reg);
    else
        return _mm256_sqrt_pd(reg);
#endif // __AVX2__
}



template <typename _registerType>
_registerType _mmx_rsqrt_p(_registerType reg)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_rsqrt_ps(reg);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_rsqrt_pd(reg);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_rsqrt_ps(reg);
    else
        return _mm256_rsqrt_pd(reg);
#endif // __AVX2__
}



template <typename _registerType>
inline auto _mmx_cmpeq_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_cmpeq_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_cmpeq_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_cmp_ps(lhs, rhs, _CMP_EQ_OS);
    else
        return _mm256_cmp_pd(lhs, rhs, _CMP_EQ_OS);
#endif // __AVX2__
}



template <typename _registerType>
inline auto _mmx_cmple_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_cmple_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_cmple_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_cmp_ps(lhs, rhs, _CMP_LE_OS);
    else
        return _mm256_cmp_pd(lhs, rhs, _CMP_LE_OS);
#endif // __AVX2__
}



template <typename _registerType>
inline auto _mmx_cmpgt_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_cmpgt_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_cmpgt_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_cmp_ps(lhs, rhs, _CMP_GT_OS);
    else
        return _mm256_cmp_pd(lhs, rhs, _CMP_GT_OS);
#endif // __AVX2__
}



template <typename _registerType>
inline auto _mmx_cmplt_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_cmplt_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_cmplt_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_cmp_ps(lhs, rhs, _CMP_LT_OS);
    else
        return _mm256_cmp_pd(lhs, rhs, _CMP_LT_OS);
#endif // __AVX2__
}



template <typename _registerType>
inline _registerType _mmx_max_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_max_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_max_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_max_ps(lhs, rhs);
    else
        return _mm256_max_pd(lhs, rhs);
#endif // __AVX2__
}



template <typename _registerType>
inline _registerType _mmx_min_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_min_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_min_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_min_ps(lhs, rhs);
    else
        return _mm256_min_pd(lhs, rhs);
#endif // __AVX2__
}



template <typename _registerType>
inline _registerType _mmx_andnot_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_andnot_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_andnot_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_andnot_ps(lhs, rhs);
    else
        return _mm256_andnot_pd(lhs, rhs);
#endif // __AVX2__
}



template <typename _registerType>
inline _registerType _mmx_xor_p(_registerType lhs, _registerType rhs)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_xor_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_xor_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_xor_ps(lhs, rhs);
    else
        return _mm256_xor_pd(lhs, rhs);
#endif // __AVX2__
}



template <typename _registerType>
inline auto _mmx_movemask_epi8(_registerType reg)
{
    using namespace GDL::sse;
    static_assert(Is__m128i<_registerType> || Is__m256i<_registerType>,
                  "Function can only be used with __m128i and __m256i registers.");

    if constexpr (Is__m128i<_registerType>)
        return static_cast<U16>(_mm_movemask_epi8(reg));
#ifdef __AVX2__
    else if constexpr (Is__m256i<_registerType>)
        return static_cast<U32>(_mm256_movemask_epi8(reg));
#endif // __AVX2__
}



template <typename _registerType>
inline auto _mmx_cvtsx_fx(_registerType reg)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_cvtss_f32(reg);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_cvtsd_f64(reg);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_cvtss_f32(reg);
    else
        return _mm256_cvtsd_f64(reg);
#endif // __AVX2__
}



template <I32 _blendMask, typename _registerType>
inline auto _mmx_blend_p(_registerType src0, _registerType src1)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_blend_ps(src0, src1, _blendMask);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_blend_pd(src0, src1, _blendMask);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_blend_ps(src0, src1, _blendMask);
    else
        return _mm256_blend_pd(src0, src1, _blendMask);
#endif // __AVX2__
}



template <typename _registerType>
inline auto _mmx_blendv_p(_registerType src0, _registerType src1, _registerType _blendMask)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_blendv_ps(src0, src1, _blendMask);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_blendv_pd(src0, src1, _blendMask);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_blendv_ps(src0, src1, _blendMask);
    else
        return _mm256_blendv_pd(src0, src1, _blendMask);
#endif // __AVX2__
}



template <I32 _permuteMask, typename _registerType>
inline auto _mmx_permute_p(_registerType reg)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

#ifndef __AVX2__
    if constexpr (Is__m128<_registerType>)
        return _mm_shuffle_ps(reg, reg, _permuteMask);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_shuffle_pd(reg, reg, _permuteMask);
#else // __AVX2__
    if constexpr (Is__m128<_registerType>)
        return _mm_permute_ps(reg, _permuteMask);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_permute_pd(reg, _permuteMask);
    else if constexpr (Is__m256<_registerType>)
        return _mm256_permute_ps(reg, _permuteMask);
    else
        return _mm256_permute_pd(reg, _permuteMask);
#endif // __AVX2__
}



#ifdef __AVX2__

template <I32 _permuteMask, typename _registerType>
inline auto _mmx_permute2f128_p(_registerType src0, _registerType src1)
{
    using namespace GDL::sse;
    static_assert(Is__m256<_registerType> || Is__m256d<_registerType>,
                  "Function can only be used with __m256 and __m256d registes.");

    if constexpr (Is__m256<_registerType>)
        return _mm256_permute2f128_ps(src0, src1, _permuteMask);
    else
        return _mm256_permute2f128_pd(src0, src1, _permuteMask);
}

#endif // __AVX2__



template <I32 _shuffleMask, typename _registerType>
inline auto _mmx_shuffle_p(_registerType src0, _registerType src1)
{
    using namespace GDL::sse;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_shuffle_ps(src0, src1, _shuffleMask);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_shuffle_pd(src0, src1, _shuffleMask);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_shuffle_ps(src0, src1, _shuffleMask);
    else
        return _mm256_shuffle_pd(src0, src1, _shuffleMask);
#endif // __AVX2__
}



} // namespace GDL
