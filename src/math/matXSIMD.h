#pragma once
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

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

//! @brief Matrix of arbitrary size with SIMD support
//! @tparam tRows: Number of rows
//! @tparam tCols: Number of columns
template <int tRows, int tCols>
class __attribute__((aligned(16))) matXSIMD
// TODO: if register is a template parameter set corresponding alignment in the row above!
{
    template <int tRows2, int tCols2>
    friend class matXSIMD;

    typedef __m128 __mx;
    __attribute__((aligned(16)))
    std::array<__mx, tCols * CalcMinNumArrayRegisters(tRows, GetNumRegisterEntries<__mx, F32>())>
            mData;

public:
    //! @brief constructor
    matXSIMD();

    //! @brief Constructor to set the whole matrix
    //! @param args: Values (column major)
    //! @tparam Args: Variadic data type
    template <typename... Args>
    matXSIMD(Args... args);


    //! @brief Constructor to set the whole matrix
    //! @brief Array with values (column major)
    matXSIMD(const std::array<F32, tRows * tCols>& data);

    //! @brief Matrix - matrix multiplication
    //! @tparam tRowsRhs: Rhs matrix number of rows
    //! @tparam tColsRhs: Rhs matrix number of columns
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    template <int tRowsRhs, int tColsRhs>
    inline matXSIMD<tRows, tColsRhs> operator*(const matXSIMD<tRowsRhs, tColsRhs>& rhs) const;

    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    inline matXSIMD& operator+=(const matXSIMD& rhs);

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline F32 operator()(const U32 row, const U32 col) const;

    //! @brief Gets the number of rows
    //! @return Number of rows
    inline U32 Rows() const;

    //! @brief Gets the number of columns
    //! @return Number of columns
    inline U32 Cols() const;

    //! @brief Sets all matrix entries to zero
    inline void SetZero();

    //! @brief Gets the data array in column major ordering
    //! @return Data
    inline const std::array<F32, tRows * tCols> Data() const;

private:
    void ConstructionChecks() const;
};
} // namespace GDL
