#pragma once

#include "matXSingle.h"


template <typename T, int tRows, int tCols>
GDL::matXSingle<T, tRows, tCols>::matXSingle()
{
}


template <typename T, int tRows, int tCols>
template <int tRowsRhs, int tColsRhs>
GDL::matXSingle<T, tRows, tColsRhs> GDL::matXSingle<T, tRows, tCols>::
operator*(const matXSingle<T, tRowsRhs, tColsRhs>& rhs) const
{
    static_assert(tCols == tRowsRhs, "Lhs cols != Rhs rows - Matrix multiplication not possible!");

    matXSingle<T, tRows, tColsRhs> result;
    // loop over RHS cols
    for (U32 i = 0; i < tColsRhs; ++i)
        // loop over RHS row
        for (U32 j = 0; j < tRowsRhs; ++j)
            // loop over LHS rows
            for (U32 k = 0; k < tRows; ++k)
                result.mData[k + i * tRows] += mData[k + j * tCols] * rhs.mData[j + i * tColsRhs];
    return result;
}


template <typename T, int tRows, int tCols>
GDL::matXSingle<T, tRows, tCols>& GDL::matXSingle<T, tRows, tCols>::operator+=(const matXSingle<T, tRows, tCols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] += rhs.mData[i];
    return *this;
}
