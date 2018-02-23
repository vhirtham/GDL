#pragma once

#include "GDLTypedefs.h"
#include <x86intrin.h>

namespace GDL
{

constexpr const unsigned int CalcMinNumArrayRegisters(U32 numElements, U32 registerSize)
{
    return (numElements / registerSize) + ((numElements % registerSize > 0) ? 1 : 0);
}

template <typename RegisterType>
constexpr const U32 GetAlignmentBytes()
{
    throw Exception(__PRETTY_FUNCTION__, "No alignment known for given data type.");
    return 0;
}

template <>
constexpr const U32 GetAlignmentBytes<__m128>()
{
    return 16;
}

template <>
constexpr const U32 GetAlignmentBytes<__m256>()
{
    return 32;
}

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

template <typename T>
inline T _mmx_add_ps(T a, T b)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type");
    return 0;
}

template <>
inline __m128 _mmx_add_ps<__m128>(__m128 a, __m128 b)
{
    return _mm_add_ps(a, b);
}

#ifdef ENABLE_AVX
template <>
inline __m256 _mmx_add_ps<__m256>(__m256 a, __m256 b)
{
    return _mm256_add_ps(a, b);
}
#endif



template <typename T>
inline T _mmx_mul_ps(T a, T b)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type");
    return 0;
}

template <>
inline __m128 _mmx_mul_ps<__m128>(__m128 a, __m128 b)
{
    return _mm_mul_ps(a, b);
}
#ifdef ENABLE_AVX
template <>
inline __m256 _mmx_mul_ps<__m256>(__m256 a, __m256 b)
{
    return _mm256_mul_ps(a, b);
}
#endif

template <typename T>
inline T _mmx_fmadd_ps(T a, T b, T c)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type");
    return 0;
}

template <>
inline __m128 _mmx_fmadd_ps<__m128>(__m128 a, __m128 b, __m128 c)
{
    return _mm_fmadd_ps(a, b, c);
}

#ifdef ENABLE_AVX
template <>
inline __m256 _mmx_fmadd_ps<__m256>(__m256 a, __m256 b, __m256 c)
{
    return _mm256_fmadd_ps(a, b, c);
}
#endif

template <typename T>
inline T _mmx_set1_ps(float f)
{
    throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type");
    return 0;
}

template <>
inline __m128 _mmx_set1_ps(float f)
{
    return _mm_set1_ps(f);
}
#ifdef ENABLE_AVX
template <>
inline __m256 _mmx_set1_ps(float f)
{
    return _mm256_set1_ps(f);
}
#endif

}
