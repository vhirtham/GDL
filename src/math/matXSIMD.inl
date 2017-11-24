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

    //    constexpr const U32 resRB = CalcBlockArraySize(tRows); // num result row blocks
    //    constexpr const U32 resCB = CalcBlockArraySize(tColsOther); // num result column blocks
    //    constexpr const U32 numMul = CalcBlockArraySize(tCols);

    //    matXSIMD<tRows, tColsOther> result;
    //    for (unsigned int i = 0; i < resRB; ++i)
    //    {
    //        for (unsigned int k = 0; k < resCB; ++k)
    //        {
    //            mat4SIMD tmp = (*this).mData[i] * other.mData[k * resCB];
    //            for (unsigned int j = 1; j < numMul; ++j)
    //            {
    //                // result.mData[i][k].SetZero();

    //                tmp += (*this).mData[i + j * numMul] * other.mData[j + k * resCB];
    ////                result.mData[i + k * resRB] += (*this).mData[i + j * numMul] * other.mData[j + k * resCB];

    //                //                std::cout << "I ROCK!!!!" << std::endl;
    //            }
    //            result.mData[i + k * resRB] = tmp;
    //        }
    //    }
    //    return result;

    constexpr U32 registerSize = GetNumRegisterEntries<__m128, F32>();
    constexpr U32 registersPerColThis = CalcMinimalNumRegisters(tRowsOther, registerSize);
    constexpr U32 registersPerColOther = CalcMinimalNumRegisters(tRowsOther, registerSize);

    matXSIMD<tRows, tColsOther> result;
    // loop over every LHS row
    for (U32 j = 0; j < registersPerColOther; ++j)
    {
        // loop over every LHS Col
        for (U32 i = 0; i < tColsOther; ++i)
        {
            // loop over RHS rows (column registers)
            for (U32 k = 0; k < registersPerColThis; ++k)
            {
                const U32 registerNumOther = i * registersPerColOther + j;
                const U32 registerNumResult = i * registersPerColThis + k;
                const U32 registerBlockThis = j * registerSize * registersPerColThis + k;
                result.mData[registerNumResult] =
                        _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(mData[registerBlockThis + 0 * registersPerColThis],
                                                                    _mm_set1_ps(other.mData[registerNumOther][0])),
                                                         _mm_mul_ps(mData[registerBlockThis + 1 * registersPerColThis],
                                                                    _mm_set1_ps(other.mData[registerNumOther][1]))),
                                              _mm_add_ps(_mm_mul_ps(mData[registerBlockThis + 2 * registersPerColThis],
                                                                    _mm_set1_ps(other.mData[registerNumOther][2])),
                                                         _mm_mul_ps(mData[registerBlockThis + 3 * registersPerColThis],
                                                                    _mm_set1_ps(other.mData[registerNumOther][3])))),
                                   result.mData[registerNumResult]);
            }
        }
    }
    return result;
}

template <int tRows, int tCols>
GDL::matXSIMD<tRows, tCols>::matXSIMD()
{
    //    for (U32 i = 0; i < mData.size(); ++i)
    //        mData[i] = _mm_setr_ps(1. + i * 4., 2. + i * 4., 3. + i * 4, 4. + i * 4);
}
