#pragma once

#include "math/mat4SIMD.inl"

#include "GDLTypedefs.h"

#include <x86intrin.h>
#include <array>

namespace GDL
{


constexpr const unsigned int CalcMinNumArrayRegisters(U32 numElements, U32 registerSize)
{
    return (numElements / registerSize) + ((numElements % registerSize > 0) ? 1 : 0);
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

template <>
inline __m256 _mmx_add_ps<__m256>(__m256 a, __m256 b)
{
    return _mm256_add_ps(a, b);
}



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

template <>
inline __m256 _mmx_mul_ps<__m256>(__m256 a, __m256 b)
{
    return _mm256_mul_ps(a, b);
}



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

template <>
inline __m256 _mmx_set1_ps(float f)
{
    return _mm256_set1_ps(f);
}


//! @brief Matrix of arbitrary size with SIMD support
//! @tparam tRows: Number of rows
//! @tparam tCols: Number of columns
template <int tRows, int tCols>
class __attribute__((aligned(32))) matXSIMD
{
    typedef __m128 __mx;
    alignas(32) std::array<__mx, tCols * CalcMinNumArrayRegisters(tRows, GetNumRegisterEntries<__mx, F32>())> mData;
    //    __mx* mData;

public:
    matXSIMD();

    //! @brief Matrix - matrix multiplication
    //! @tparam tRowsRhs: Rhs matrix number of rows
    //! @tparam tColsRhs: Rhs matrix number of columns
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    template <int tRowsRhs, int tColsRhs>
    matXSIMD<tRows, tColsRhs> operator*(const matXSIMD<tRowsRhs, tColsRhs>& rhs) const;

    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    matXSIMD& operator+=(const matXSIMD& rhs);
};
} // namespace GDL
