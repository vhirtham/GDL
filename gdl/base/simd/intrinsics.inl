#pragma once

#include "gdl/base/simd/intrinsics.h"

#include "gdl/base/exception.h"
#include "gdl/base/simd/constants.h"
#include "gdl/base/simd/negate.h"

#include <immintrin.h>

namespace GDL
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, typename _registerType>
inline void _mm_store(_type* ptr, _registerType reg)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, typename _type>
inline _registerType _mmx_load_p(_type* ptr)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_setzero()
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, typename _type>
inline _registerType _mm_set1(_type value)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_set1_ps(static_cast<F32>(value));
    else if constexpr (Is__m128d<_registerType>)
        return _mm_set1_pd(static_cast<F64>(value));
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_set1_ps(static_cast<F32>(value));
    else
        return _mm256_set1_pd(static_cast<F64>(value));
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, typename... _args>
inline _registerType _mm_setr(_args... args)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_add(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_sub(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_mul(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
_registerType _mm_div(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_fmadd(_registerType lhsM, _registerType rhsM, _registerType add)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

#ifndef __FMA__
    return _mm_add(_mm_mul(lhsM, rhsM), add);
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_fnmadd(_registerType lhsM, _registerType rhsM, _registerType add)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

#ifndef __FMA__
    return _mm_sub(add, _mm_mul(lhsM, rhsM));
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_fmsub(_registerType lhsM, _registerType rhsM, _registerType sub)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

#ifndef __FMA__
    return _mm_sub(_mm_mul(lhsM, rhsM), sub);
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_fnmsub(_registerType lhsM, _registerType rhsM, _registerType sub)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

#ifndef __FMA__
    return _mm_sub(Negate(_mm_mul(lhsM, rhsM)), sub);
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



// --------------------------------------------------------------------------------------------------------------------

template <U32 _mask, typename _registerType>
_registerType _mm_dp(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
_registerType _mm_sqrt(_registerType reg)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
_registerType _mm_rsqrt(_registerType reg)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_cmpeq(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_cmple(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_cmpgt(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_cmplt(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_max(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_min(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_and(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_and_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_and_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_and_ps(lhs, rhs);
    else
        return _mm256_and_pd(lhs, rhs);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_andnot(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_or(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_or_ps(lhs, rhs);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_or_pd(lhs, rhs);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_or_ps(lhs, rhs);
    else
        return _mm256_or_pd(lhs, rhs);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_xor(_registerType lhs, _registerType rhs)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_movemaskEpi8(_registerType reg)
{
    using namespace GDL::simd;
    static_assert(Is__m128i<_registerType> || Is__m256i<_registerType>,
                  "Function can only be used with __m128i and __m256i registers.");

    if constexpr (Is__m128i<_registerType>)
        return static_cast<U16>(_mm_movemask_epi8(reg));
#ifdef __AVX2__
    else if constexpr (Is__m256i<_registerType>)
        return static_cast<U32>(_mm256_movemask_epi8(reg));
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_castFI(_registerType src)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_castps_si128(src);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_castpd_si128(src);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_castps_si256(src);
    else
        return _mm256_castpd_si256(src);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, typename _intRegisterType>
inline _registerType _mm_castIF(_intRegisterType src)
{
    using namespace GDL::simd;
    static_assert(Is__m128i<_intRegisterType> || Is__m256i<_intRegisterType>,
                  "Function can only be used with compatible integer register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_castsi128_ps(src);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_castsi128_pd(src);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_castsi256_ps(src);
    else
        return _mm256_castsi256_pd(src);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_cvtsF(_registerType reg)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <I32 _blendMask, typename _registerType>
inline auto _mm_blend(_registerType src0, _registerType src1)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_blendv(_registerType src0, _registerType src1, _registerType _blendMask)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <typename _registerType>
inline _registerType _mm_broadcasts(_registerType src)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_broadcastss_ps(src);
    else if constexpr (Is__m128d<_registerType>)
        // return _mm_broadcastsd_pd(src); - missing
        // GCC bug? See:
        // https://stackoverflow.com/questions/58270381/mm-broadcastsd-pd-missing-in-gcc-avx2intrin-h-versions-x-9-2
        return _mm_movedup_pd(src);
    else if constexpr (Is__m256<_registerType>)
        return _mm256_broadcastss_ps(_mm256_castps256_ps128(src));
    else
        return _mm256_broadcastsd_pd(_mm256_castpd256_pd128(src));
}

#endif //__AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_movehdup(_registerType src)
{
    using namespace GDL::simd;
    static_assert(Is__m128<_registerType> || Is__m256<_registerType>,
                  "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_movehdup_ps(src);
#ifdef __AVX2__
    else
        return _mm256_movehdup_ps(src);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_moveldup(_registerType src)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_moveldup_ps(src);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_movedup_pd(src);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_moveldup_ps(src);
    else
        return _mm256_movedup_pd(src);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_movehl(_registerType src0, _registerType src1)
{
    using namespace GDL::simd;
    static_assert(Is__m128<_registerType> || Is__m256<_registerType>, "Only __m128 and __m256 registers supported.");

    if constexpr (Is__m128<_registerType>)
        return _mm_movehl_ps(src0, src1);
#ifdef __AVX2__
    else
        // See: https://stackoverflow.com/questions/58954801/avx-equivalent-for-mm-movelh-ps?noredirect=1#58955087
        // return _mm256_castpd_ps(_mm256_unpackhi_pd(_mm256_castps_pd(src1), _mm256_castps_pd(src0)));
        return _mm256_shuffle_ps(src1, src0, 0xee);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType _mm_movelh(_registerType src0, _registerType src1)
{
    using namespace GDL::simd;
    static_assert(Is__m128<_registerType> || Is__m256<_registerType>, "Only __m128 and __m256 registers supported.");

    if constexpr (Is__m128<_registerType>)
        return _mm_movelh_ps(src0, src1);
#ifdef __AVX2__
    else
        // See: https://stackoverflow.com/questions/58954801/avx-equivalent-for-mm-movelh-ps?noredirect=1#58955087
        // return _mm256_castpd_ps(_mm256_unpacklo_pd(_mm256_castps_pd(src0), _mm256_castps_pd(src1)));
        return _mm256_shuffle_ps(src0, src1, 0x44);

#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <I32 _permuteMask, typename _registerType>
inline auto _mm_permute(_registerType reg)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <I32 _permuteMask, typename _registerType>
inline auto _mm_permute2f128(_registerType src0, _registerType src1)
{
    using namespace GDL::simd;
    static_assert(Is__m256<_registerType> || Is__m256d<_registerType>,
                  "Function can only be used with __m256 and __m256d registes.");

    if constexpr (Is__m256<_registerType>)
        return _mm256_permute2f128_ps(src0, src1, _permuteMask);
    else
        return _mm256_permute2f128_pd(src0, src1, _permuteMask);
}



// --------------------------------------------------------------------------------------------------------------------

template <I32 _permuteMask>
inline __m256d _mm_permute4x64(__m256d src0)
{
    return _mm256_permute4x64_pd(src0, _permuteMask);
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <I32 _shuffleMask, typename _registerType>
inline auto _mm_shuffle(_registerType src0, _registerType src1)
{
    using namespace GDL::simd;
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_unpackhi(_registerType src0, _registerType src1)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_unpackhi_ps(src0, src1);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_unpackhi_pd(src0, src1);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_unpackhi_ps(src0, src1);
    else
        return _mm256_unpackhi_pd(src0, src1);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline auto _mm_unpacklo(_registerType src0, _registerType src1)
{
    using namespace GDL::simd;
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    if constexpr (Is__m128<_registerType>)
        return _mm_unpacklo_ps(src0, src1);
    else if constexpr (Is__m128d<_registerType>)
        return _mm_unpacklo_pd(src0, src1);
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return _mm256_unpacklo_ps(src0, src1);
    else
        return _mm256_unpacklo_pd(src0, src1);
#endif // __AVX2__
}



} // namespace GDL
