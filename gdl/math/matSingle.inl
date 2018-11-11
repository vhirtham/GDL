#pragma once

#include "gdl/math/matSingle.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>



namespace GDL
{


template <typename _type, I32 _rows, I32 _cols>
MatSingle<_type, _rows, _cols>::MatSingle()
    : mData{{0}}
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
_type MatSingle<_type, _rows, _cols>::operator()(const U32 row, const U32 col) const
{
    return mData[row + col * _rows];
}



template <typename _type, I32 _rows, I32 _cols>
bool MatSingle<_type, _rows, _cols>::operator==(const MatSingle& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < mData.size(); ++i)
        result = result && mData[i] == Approx(rhs.mData[i]);
    return result;
}



template <typename _type, I32 _rows, I32 _cols>
bool MatSingle<_type, _rows, _cols>::operator!=(const MatSingle& rhs) const
{
    return !operator==(rhs);
}



template <typename _type, I32 _rows, I32 _cols>
MatSingle<_type, _rows, _cols>& MatSingle<_type, _rows, _cols>::operator+=(const MatSingle<_type, _rows, _cols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] += rhs.mData[i];
    return *this;
}



template <typename _type, I32 _rows, I32 _cols>
MatSingle<_type, _rows, _cols> MatSingle<_type, _rows, _cols>::operator+(const MatSingle<_type, _rows, _cols>& rhs)
{
    MatSingle<_type, _rows, _cols> result;
    for (U32 i = 0; i < mData.size(); ++i)
        result.mData[i] = mData[i] + rhs.mData[i];
    return result;
}



template <typename _type, I32 _rows, I32 _cols>
MatSingle<_type, _cols, _rows> MatSingle<_type, _rows, _cols>::Transpose() const
{
    MatSingle<_type, _cols, _rows> transposed;
    for (U32 i = 0; i < _rows; ++i)
        for (U32 j = 0; j < _cols; ++j)
            transposed.mData[i + j * 8] = mData[j + i * 8];
    return transposed;
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
    std::memset(&mData, 0, sizeof(mData));
}



template <typename _type, I32 _rows, I32 _cols>
std::ostream& operator<<(std::ostream& os, const MatSingle<_type, _rows, _cols>& mat)
{
    using namespace GDL;
    for (U32 i = 0; i < _rows; ++i)
    {
        os << "| ";
        for (U32 j = 0; j < _cols; ++j)
            os << mat(i, j) << " ";
        os << "|" << std::endl;
    }
    return os;
}


} // namespace GDL
