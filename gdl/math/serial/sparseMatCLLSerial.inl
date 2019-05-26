#pragma once

#include "gdl/base/exception.h"
#include "gdl/math/serial/sparseMatCLLSerial.h"

#include <iostream>

namespace GDL
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
SparseMatCLLSerial<_type>::Node::Node(U32 row, _type value)
    : mRow{row}
    , mValue{value}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline SparseMatCLLSerial<_type>::SparseMatCLLSerial(U32 rows, U32 cols)
    : mRows{rows}
    , mCols{cols}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline _type SparseMatCLLSerial<_type>::operator()(U32 row, U32 col) const
{
    DEV_EXCEPTION(row >= mRows || col >= mCols.size(), "Selected row or column exceeds matrix size");

    const ForwardList<Node>& colList = mCols[col];
    auto currValue = colList.begin();

    if (colList.empty() || row < currValue->mRow)
        return 0;

    while (currValue != colList.end() && currValue->mRow < row)
        ++currValue;

    if (currValue == colList.end() || currValue->mRow > row)
        return 0;

    return currValue->mValue;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline U32 SparseMatCLLSerial<_type>::Cols() const
{
    return mCols.size();
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline U32 SparseMatCLLSerial<_type>::CountStoredValues() const
{
    U32 numStoredValues = 0;

    for (U32 i = 0; i < mCols.size(); ++i)
        for ([[maybe_unused]] const auto& node : mCols[i])
            ++numStoredValues;

    return numStoredValues;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
void SparseMatCLLSerial<_type>::Remove(U32 row, U32 col)
{
    ForwardList<Node>& colList = mCols[col];
    auto currValue = colList.begin();

    if (currValue == colList.end())
        return;
    if (currValue->mRow == row)
        colList.pop_front();

    auto prevValue = currValue++;

    while (true)
    {
        if (currValue == colList.end() || currValue->mRow > row)
            return;
        if (currValue->mRow == row)
        {
            colList.erase_after(prevValue);
            return;
        }
        prevValue = currValue++;
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline U32 SparseMatCLLSerial<_type>::Rows() const
{
    return mRows;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline void SparseMatCLLSerial<_type>::Set(U32 row, U32 col, _type value)
{
    DEV_EXCEPTION(row >= mRows || col >= mCols.size(), "Selected row or column exceeds matrix size");

    if (value == 0.0)
    {
        Remove(row, col);
        return;
    }

    ForwardList<Node>& colList = mCols[col];
    auto currValue = colList.begin();

    // New element is in front of first
    if (colList.empty() || currValue->mRow > row)
    {
        colList.emplace_front(row, value);
        return;
    }


    auto nextValue = ++colList.begin();
    while (nextValue != colList.end() && nextValue->mRow <= row)
    {
        currValue = nextValue;
        ++nextValue;
    }


    if (currValue->mRow == row)
        currValue->mValue = value;
    else
        colList.emplace_after(currValue, row, value);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
std::ostream& operator<<(std::ostream& os, const SparseMatCLLSerial<_type>& mat)
{
    for (U32 i = 0; i < mat.Rows(); ++i)
    {
        os << "| ";
        for (U32 j = 0; j < mat.Cols(); ++j)
            os << mat(i, j) << " ";
        os << "|" << std::endl;
    }
    return os;
}


} // namespace GDL
