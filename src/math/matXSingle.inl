#pragma once

#include "matXSingle.h"

#include <algorithm>
#ifndef NDEBUG
#include <iostream>
#endif

template <typename T, int tRows, int tCols>
GDL::matXSingle<T, tRows, tCols>::matXSingle()
{
}

template <typename T, int tRows, int tCols>
GDL::matXSingle<T, tRows, tCols>::matXSingle(const std::array<T, tRows * tCols>& data)
    : mData(data)
{
}


template <typename T, int tRows, int tCols>
template <int tRowsRhs, int tColsRhs>
GDL::matXSingle<T, tRows, tColsRhs> GDL::matXSingle<T, tRows, tCols>::
operator*(const matXSingle<T, tRowsRhs, tColsRhs>& rhs) const
{
    static_assert(tCols == tRowsRhs, "Lhs cols != Rhs rows - Matrix multiplication not possible!");

    matXSingle<T, tRows, tColsRhs> result;
    result.SetZero();
    // loop over RHS cols
    for (U32 i = 0; i < tColsRhs; ++i)
        // loop over RHS row
        for (U32 j = 0; j < tRowsRhs; ++j)
            // loop over LHS rows
            for (U32 k = 0; k < tRows; ++k)
                result.mData[k + i * tRows] += mData[k + j * tRows] * rhs.mData[j + i * tRowsRhs];
    return result;
}


template <typename T, int tRows, int tCols>
GDL::matXSingle<T, tRows, tCols>& GDL::matXSingle<T, tRows, tCols>::operator+=(const matXSingle<T, tRows, tCols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] += rhs.mData[i];
    return *this;
}

template <typename T, int tRows, int tCols>
T GDL::matXSingle<T, tRows, tCols>::operator()(const GDL::U32 row, const GDL::U32 col) const
{
    return mData[row + col * tRows];
}

template <typename T, int tRows, int tCols>
GDL::U32 GDL::matXSingle<T, tRows, tCols>::Rows() const
{
    return tRows;
}

template <typename T, int tRows, int tCols>
GDL::U32 GDL::matXSingle<T, tRows, tCols>::Cols() const
{
    return tCols;
}

template <typename T, int tRows, int tCols>
const std::array<T, tRows * tCols>& GDL::matXSingle<T, tRows, tCols>::Data() const
{
    return mData;
}

template<typename T, int tRows, int tCols>
void GDL::matXSingle<T, tRows, tCols>::SetZero()
{
    std::fill(mData.begin(),mData.end(),0.);
}

// template <typename T, int tRows, int tCols>
// constexpr std::array<T, tRows * tCols>
// GDL::matXSingle<T, tRows, tCols>::RowMajorToColumnMajor(std::array<T, tRows * tCols> rowMaj)
//{
//    std::array<T, tRows * tCols> colMaj={};
//    for (U32 i = 0; i < tRows * tCols; ++i)
//    {
//        U32 currentCol = i / tRows;
//        U32 currentRow = i % tRows;
//        colMaj[i] = rowMaj[currentCol+currentRow*tCols];
//    }
//    return colMaj;
//}

#ifndef NDEBUG
template <typename T2, int tRows2, int tCols2>
std::ostream& operator<<(std::ostream& os, const GDL::matXSingle<T2, tRows2, tCols2>& mat)
{
    using namespace GDL;
    for (U32 i = 0; i < tRows2; ++i)
    {
        os << "| ";
        for (U32 j = 0; j < tCols2; ++j)
            os << mat(i, j) << " ";
        os << "|" << std::endl;
    }
    return os;
}
#endif
