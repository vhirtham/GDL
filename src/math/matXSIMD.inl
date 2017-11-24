#pragma once

#include "matXSIMD.h"

template <int tRows, int tCols>
GDL::matXSIMD<tRows, tCols>::matXSIMD()
{
    //    for (U32 i = 0; i < mData.size(); ++i)
    //        mData[i] = _mm_setr_ps(1. + i * 4., 2. + i * 4., 3. + i * 4, 4. + i * 4);
}

template <int tRows, int tCols>
GDL::matXSIMD<tRows, tCols>& GDL::matXSIMD<tRows, tCols>::operator+=(const GDL::matXSIMD<tRows, tCols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] = _mm_add_ps(rhs.mData[i], mData[i]);
    return *this;
}


template <int tRows, int tCols>
template <int tRowsRhs, int tColsRhs>
GDL::matXSIMD<tRows, tColsRhs> GDL::matXSIMD<tRows, tCols>::operator*(const matXSIMD<tRowsRhs, tColsRhs>& rhs) const
{
    static_assert(tCols == tRowsRhs, "Lhs cols != Rhs rows - Matrix multiplication not possible!");

    constexpr U32 registerSize = GetNumRegisterEntries<__m128, F32>();
    constexpr U32 registersPerColLhs = CalcMinNumArrayRegisters(tRowsRhs, registerSize);
    constexpr U32 registersPerColRhs = CalcMinNumArrayRegisters(tRowsRhs, registerSize);

    matXSIMD<tRows, tColsRhs> result;
    // loop over every LHS row
    for (U32 j = 0; j < registersPerColRhs; ++j)
    {
        // loop over every LHS Col
        for (U32 i = 0; i < tColsRhs; ++i)
        {
            // loop over RHS rows (column registers)
            for (U32 k = 0; k < registersPerColLhs; ++k)
            {
                const U32 registerNumRhs = i * registersPerColRhs + j;
                const U32 registerNumResult = i * registersPerColLhs + k;
                const U32 currentBlockLhs = j * registerSize * registersPerColLhs + k;
                result.mData[registerNumResult] =
                        _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(mData[currentBlockLhs + 0 * registersPerColLhs],
                                                                    _mm_set1_ps(rhs.mData[registerNumRhs][0])),
                                                         _mm_mul_ps(mData[currentBlockLhs + 1 * registersPerColLhs],
                                                                    _mm_set1_ps(rhs.mData[registerNumRhs][1]))),
                                              _mm_add_ps(_mm_mul_ps(mData[currentBlockLhs + 2 * registersPerColLhs],
                                                                    _mm_set1_ps(rhs.mData[registerNumRhs][2])),
                                                         _mm_mul_ps(mData[currentBlockLhs + 3 * registersPerColLhs],
                                                                    _mm_set1_ps(rhs.mData[registerNumRhs][3])))),
                                   result.mData[registerNumResult]);
            }
        }
    }
    return result;
}
