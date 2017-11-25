#pragma once

#include "matXSIMD.h"


template <int tRows, int tCols>
GDL::matXSIMD<tRows, tCols>::matXSIMD()
{
//    mData = static_cast<__mx*>(aligned_alloc(
//            64, sizeof(float) * 8 * tCols * CalcMinNumArrayRegisters(tRows, GetNumRegisterEntries<__mx, F32>())));
#ifndef NDEBUG
    if (!(is_aligned(&mData[0], 16) && is_aligned(&mData[1], 16) && is_aligned(&mData[2], 16) &&
          is_aligned(&mData[3], 16)))
        throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}

template <int tRows, int tCols>
GDL::matXSIMD<tRows, tCols>& GDL::matXSIMD<tRows, tCols>::operator+=(const GDL::matXSIMD<tRows, tCols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] = _mmx_add_ps(rhs.mData[i], mData[i]);
    return *this;
}


template <int tRows, int tCols>
template <int tRowsRhs, int tColsRhs>
GDL::matXSIMD<tRows, tColsRhs> GDL::matXSIMD<tRows, tCols>::operator*(const matXSIMD<tRowsRhs, tColsRhs>& rhs) const
{
    static_assert(tCols == tRowsRhs, "Lhs cols != Rhs rows - Matrix multiplication not possible!");

    constexpr U32 registerSize = GetNumRegisterEntries<__mx, F32>();
    constexpr U32 registersPerColLhs = CalcMinNumArrayRegisters(tRowsRhs, registerSize);
    constexpr U32 registersPerColRhs = CalcMinNumArrayRegisters(tRowsRhs, registerSize);

    matXSIMD<tRows, tColsRhs> result;
    // loop over RHS rows (column registers)
    for (U32 j = 0; j < registersPerColRhs; ++j)
    {
        // loop over every RHS Col
        for (U32 i = 0; i < tColsRhs; ++i)
        {
            // loop over LHS rows (column registers)
            for (U32 k = 0; k < registersPerColLhs; ++k)
            {
                const U32 registerNumRhs = i * registersPerColRhs + j;
                const U32 registerNumResult = i * registersPerColLhs + k;
                const U32 currentBlockLhs = j * registerSize * registersPerColLhs + k;
                result.mData[registerNumResult] = //_mmx_mul_ps(mData[0],mData[0]);
                        _mmx_add_ps(
                                _mmx_add_ps(_mmx_add_ps(_mmx_mul_ps(mData[currentBlockLhs + 0 * registersPerColLhs],
                                                                    _mmx_set1_ps<__mx>(rhs.mData[registerNumRhs][0])),
                                                        _mmx_mul_ps(mData[currentBlockLhs + 1 * registersPerColLhs],
                                                                    _mmx_set1_ps<__mx>(rhs.mData[registerNumRhs][1]))),
                                            _mmx_add_ps(_mmx_mul_ps(mData[currentBlockLhs + 2 * registersPerColLhs],
                                                                    _mmx_set1_ps<__mx>(rhs.mData[registerNumRhs][2])),
                                                        _mmx_mul_ps(mData[currentBlockLhs + 3 * registersPerColLhs],
                                                                    _mmx_set1_ps<__mx>(rhs.mData[registerNumRhs][3])))),
                                result.mData[registerNumResult]);
            }
        }
    }
    return result;
}
