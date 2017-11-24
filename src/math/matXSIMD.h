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
constexpr const U32 GetNumRegisterEntries<__m128, F64>()
{
    return 2;
}

//! @brief Matrix of arbitrary size with SIMD support
//! @tparam tRows: Number of rows
//! @tparam tCols: Number of columns
template <int tRows, int tCols>
class __attribute__((aligned(16))) matXSIMD
{
    __attribute__((aligned(16)))
    std::array<__m128, tCols * CalcMinNumArrayRegisters(tRows, GetNumRegisterEntries<__m128, F32>())>
            mData;

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
