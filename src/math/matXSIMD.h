#pragma once

#include "math/mat4SIMD.inl"

#include "GDLTypedefs.h"

#include <x86intrin.h>
#include <array>

namespace GDL
{



constexpr const unsigned int CalcBlockArraySize(unsigned int pMatSize)
{
    return (pMatSize / 4) + ((pMatSize % 4 > 0) ? 1 : 0);
}

constexpr const unsigned int CalcMinimalNumRegisters(U32 numElements, U32 registerSize)
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

template <int tRows, int tCols>
class __attribute__((aligned(16))) matXSIMD
{
    // std::array<std::array<mat4SIMD,CalcBlockArraySize(tCols)>,CalcBlockArraySize(tRows)> mData;
    // std::array<mat4SIMD, CalcBlockArraySize(tCols) * CalcBlockArraySize(tRows)> mData;
    __attribute__((aligned(16)))
    std::array<__m128, tCols * CalcMinimalNumRegisters(tRows, GetNumRegisterEntries<__m128, F32>())>
            mData;

public:
    matXSIMD();

    //! @brief Matrix matrix multiplication
    template <int tRowsOther, int tColsOther>
    matXSIMD<tRows, tColsOther> operator*(const matXSIMD<tRowsOther, tColsOther>& other) const;
};
} // namespace GDL
