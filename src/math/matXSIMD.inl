#pragma once

#include "matXSIMD.h"

template <int tRows, int tCols>
template <int tRowsOther, int tColsOther>
GDL::matXSIMD<tRows, tColsOther> GDL::matXSIMD<tRows, tCols>::
operator*(const matXSIMD<tRowsOther, tColsOther>& other) const
{
    static_assert(tCols == tRowsOther, "Matrix sizes not compatible for multiplication");

//    matXSIMD<tRows, tColsOther> result;
//    for (unsigned int i = 0; i < CalcBlockArraySize(tRows); ++i)
//    {
//        for (unsigned int k = 0; k < CalcBlockArraySize(tRows); ++k)
//        {
//            mat4SIMD tmp;
//            for (unsigned int j = 0; j < CalcBlockArraySize(tRows); ++j)
//            {
//                // result.mData[i][k].SetZero();
//                tmp += (*this).mData[i][j] * other.mData[j][k];
//                //result.mData[i][k] += (*this).mData[i][j] * other.mData[j][k];

//                //                std::cout << "I ROCK!!!!" << std::endl;
//            }
//            result.mData[i][k] = tmp;
//        }
//    }
//    return result;

    constexpr const U32 resRB = CalcBlockArraySize(tRows); // num result row blocks
    constexpr const U32 resCB = CalcBlockArraySize(tColsOther); // num result column blocks
    constexpr const U32 numMul = CalcBlockArraySize(tCols);

    matXSIMD<tRows, tColsOther> result;
    for (unsigned int i = 0; i < resRB; ++i)
    {
        for (unsigned int k = 0; k < resCB; ++k)
        {
            mat4SIMD tmp = (*this).mData[i] * other.mData[k * resCB];
            for (unsigned int j = 1; j < numMul; ++j)
            {
                // result.mData[i][k].SetZero();

                tmp += (*this).mData[i + j * numMul] * other.mData[j + k * resCB];
//                result.mData[i + k * resRB] += (*this).mData[i + j * numMul] * other.mData[j + k * resCB];

                //                std::cout << "I ROCK!!!!" << std::endl;
            }
            result.mData[i + k * resRB] = tmp;
        }
    }
    return result;
}
