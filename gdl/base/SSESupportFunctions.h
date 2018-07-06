#pragma once

#pragma once
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"

#include <x86intrin.h>

namespace GDL
{
inline bool is_aligned(const volatile void* p, size_t n)
{
    return reinterpret_cast<std::uintptr_t>(p) % n == 0;
}

constexpr const U32 CalcMinNumArrayRegisters(U32 numElements, U32 registerSize)
{
    return (numElements / registerSize) + ((numElements % registerSize > 0) ? 1 : 0);
}

template <typename _Register>
constexpr const U32 AlignmentBytes = 0;

template <>
constexpr const U32 AlignmentBytes<__m128> = 16;
template <>
constexpr const U32 AlignmentBytes<__m128d> = 16;

template <>
constexpr const U32 AlignmentBytes<__m256> = 32;

template <>
constexpr const U32 AlignmentBytes<__m512> = 64;

template <typename DataType, typename RegisterType>
constexpr const U32 GetNumRegisterEntries()
{
    throw Exception(__PRETTY_FUNCTION__, "No number of entries known for given combination.");
    return 0;
}

template <>
constexpr const U32 GetNumRegisterEntries<__m128, F32>()
{
    return 4;
}

template <>
constexpr const U32 GetNumRegisterEntries<__m256, F32>()
{
    return 8;
}

template <>
constexpr const U32 GetNumRegisterEntries<__m512, F32>()
{
    return 16;
}

template <>
constexpr const U32 GetNumRegisterEntries<__m128d, F64>()
{
    return 2;
}

template <>
constexpr const U32 GetNumRegisterEntries<__m256d, F64>()
{
    return 4;
}

template <>
constexpr const U32 GetNumRegisterEntries<__m512d, F64>()
{
    return 8;
}


template <typename T, typename TReg>
inline TReg _mmx_add_p(TReg a, TReg b)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type");
    return TReg();
}

template <>
inline __m128 _mmx_add_p<F32>(__m128 a, __m128 b)
{
    return _mm_add_ps(a, b);
}

template <>
inline __m128d _mmx_add_p<F64>(__m128d a, __m128d b)
{
    return _mm_add_pd(a, b);
}

#ifdef ENABLE_AVX
template <>
inline __m256 _mmx_add_p<__m256>(__m256 a, __m256 b)
{
    return _mm256_add_ps(a, b);
}
#endif



template <typename T>
inline T _mmx_mul_p(T a, T b)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type");
    return 0;
}

template <>
inline __m128 _mmx_mul_p<__m128>(__m128 a, __m128 b)
{
    return _mm_mul_ps(a, b);
}
template <>
inline __m128d _mmx_mul_p<__m128d>(__m128d a, __m128d b)
{
    return _mm_mul_pd(a, b);
}
#ifdef ENABLE_AVX
template <>
inline __m256 _mmx_mul_p<__m256>(__m256 a, __m256 b)
{
    return _mm256_mul_ps(a, b);
}
#endif

template <typename T>
inline T _mmx_fmadd_p(T a, T b, T c)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type");
    return 0;
}

template <>
inline __m128 _mmx_fmadd_p<__m128>(__m128 a, __m128 b, __m128 c)
{
    return _mm_fmadd_ps(a, b, c);
}

template <>
inline __m128d _mmx_fmadd_p<__m128d>(__m128d a, __m128d b, __m128d c)
{
    return _mm_fmadd_pd(a, b, c);
}

#ifdef ENABLE_AVX
template <>
inline __m256 _mmx_fmadd_p<__m256>(__m256 a, __m256 b, __m256 c)
{
    return _mm256_fmadd_ps(a, b, c);
}
#endif

template <typename T, typename TReg>
inline TReg _mmx_set1_p(T f)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected combination of register type and data type");
    return 0;
}

template <>
inline __m128 _mmx_set1_p(F32 f)
{
    return _mm_set1_ps(f);
}

template <>
inline __m128d _mmx_set1_p(F64 f)
{
    return _mm_set1_pd(f);
}

#ifdef ENABLE_AVX
template <>
inline __m256 _mmx_set1_p(F32 f)
{
    return _mm256_set1_ps(f);
}
#endif
}
