#pragma once

#include "matSingle.h"

#include <algorithm>
#include <cassert>
#include <iostream>




namespace GDL
{


template <typename _type, I32 _rows, I32 _cols>
MatSingle<_type, _rows, _cols>::MatSingle()
{
}

template <typename _type, I32 _rows, I32 _cols>
template <typename... Args>
MatSingle<_type, _rows, _cols>::MatSingle(Args... args)
    : mData{{static_cast<_type>(args)...}}
{
    assert(mData.size() == sizeof...(args));
}

template <typename _type, I32 _rows, I32 _cols>
MatSingle<_type, _rows, _cols>::MatSingle(const std::array<_type, _rows * _cols>& data)
    : mData(data)
{
    assert(mData.size() == data.size());
}


template <typename _type, I32 _rows, I32 _cols>
template <I32 _rowsRhs, I32 _colsRhs>
MatSingle<_type, _rows, _colsRhs> MatSingle<_type, _rows, _cols>::
operator*(const MatSingle<_type, _rowsRhs, _colsRhs>& rhs) const
{
    static_assert(_cols == _rowsRhs, "Lhs cols != Rhs rows");

    MatSingle<_type, _rows, _colsRhs> result;
    result.SetZero();
    // loop over RHS cols
    for (U32 i = 0; i < _colsRhs; ++i)
        // loop over RHS row
        for (U32 j = 0; j < _rowsRhs; ++j)
            // loop over LHS rows
            for (U32 k = 0; k < _rows; ++k)
                result.mData[k + i * _rows] += mData[k + j * _rows] * rhs.mData[j + i * _rowsRhs];
    return result;
}


template <typename _type, I32 _rows, I32 _cols>
MatSingle<_type, _rows, _cols>& MatSingle<_type, _rows, _cols>::
operator+=(const MatSingle<_type, _rows, _cols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] += rhs.mData[i];
    return *this;
}

template <typename _type, I32 _rows, I32 _cols>
_type MatSingle<_type, _rows, _cols>::operator()(const U32 row, const U32 col) const
{
    return mData[row + col * _rows];
}

template <typename _type, I32 _rows, I32 _cols>
U32 MatSingle<_type, _rows, _cols>::Rows() const
{
    return _rows;
}

template <typename _type, I32 _rows, I32 _cols>
U32 MatSingle<_type, _rows, _cols>::Cols() const
{
    return _cols;
}

template <typename _type, I32 _rows, I32 _cols>
std::array<_type, _rows * _cols> MatSingle<_type, _rows, _cols>::Data() const
{
    return mData;
}

template <typename _type, I32 _rows, I32 _cols>
void MatSingle<_type, _rows, _cols>::SetZero()
{
    mData.fill(static_cast<_type>(0));
}


template <typename _type2, I32 _rows2, I32 _cols2>
std::ostream& operator<<(std::ostream& os, const MatSingle<_type2, _rows2, _cols2>& mat)
{
    using namespace GDL;
    for (U32 i = 0; i < _rows2; ++i)
    {
        os << "| ";
        for (U32 j = 0; j < _cols2; ++j)
            os << mat(i, j) << " ";
        os << "|" << std::endl;
    }
    return os;
}


} // namespace GDL
