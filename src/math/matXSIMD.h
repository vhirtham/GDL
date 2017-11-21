#pragma once

#include "math/mat4SIMD.inl"

#include <array>

namespace GDL
{



static constexpr const unsigned int CalcBlockArraySize(unsigned int pMatSize)
{
    return (pMatSize/4) + ((pMatSize % 4>0)?1:0);
}

template <int tRows, int tCols>
class __attribute__((aligned(16))) matXSIMD
{
//    std::array<std::array<mat4SIMD,CalcBlockArraySize(tCols)>,CalcBlockArraySize(tRows)> mData;
    std::array<mat4SIMD,CalcBlockArraySize(tCols)*CalcBlockArraySize(tRows)> mData;
public:
    matXSIMD() = default;

    //! @brief Matrix matrix multiplication
    template <int tRowsOther, int tColsOther>
    matXSIMD<tRows, tColsOther> operator*(const matXSIMD<tRowsOther, tColsOther>& other) const;

};
}// namespace GDL
