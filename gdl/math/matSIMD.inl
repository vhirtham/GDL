#pragma once

#include "gdl/math/matSIMD.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/transpose.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

namespace GDL
{



template <typename _type, I32 _rows, I32 _cols>
MatSIMD<_type, _rows, _cols>::MatSIMD()
    : mData{{0}}
{
    ConstructionChecks();
}



template <typename _type, I32 _rows, I32 _cols>
template <typename... _args>
MatSIMD<_type, _rows, _cols>::MatSIMD(_args... args)
{
    assert(sizeof...(args) == _rows * _cols);
    *this = MatSIMD(std::array<_type, _rows * _cols>{{static_cast<_type>(args)...}});
}



template <typename _type, I32 _rows, I32 _cols>
MatSIMD<_type, _rows, _cols>::MatSIMD(const std::array<_type, _rows * _cols>& data)
{
    if
        constexpr(_rows % mNumRegisterEntries == 0)
        {
            assert(sizeof(mData) == sizeof(data));
            std::memcpy(&mData, &data, sizeof(data));
        }
    else
        for (U32 i = 0; i < _cols; ++i)
        {
            // Set last column register to zero
            std::memset((&mData[(i + 1) * mNumRegistersPerCol - 1]), 0, sizeof(__mx));
            // Copy data into column
            std::memcpy(&mData[i * mNumRegistersPerCol], &data[i * _rows], sizeof(_type) * _rows);
        }

    ConstructionChecks();
}



template <typename _type, I32 _rows, I32 _cols>
MatSIMD<_type, _rows, _cols>::MatSIMD(bool)
{
}



template <typename _type, I32 _rows, I32 _cols>
_type MatSIMD<_type, _rows, _cols>::operator()(const U32 row, const U32 col) const
{
    assert(row < _rows && col < _cols);
    return mData[row / mNumRegisterEntries + col * mNumRegistersPerCol][row % mNumRegisterEntries];
}



template <typename _type, I32 _rows, I32 _cols>
bool MatSIMD<_type, _rows, _cols>::operator==(const MatSIMD& rhs) const
{
    bool result = true;

    // clang-format off
    if constexpr(_rows % mNumRegisterEntries == 0)
        {
            for (U32 i = 0; i < mData.size(); ++i)
                result = result && mData[i] == Approx(rhs.mData[i]);
        }
    else
        for (U32 i = 0; i < _cols; ++i)
        {
            for (U32 j = 0; j < mNumRegistersPerCol - 1; ++j)
            {
                U32 index = i * mNumRegistersPerCol + j;
                result = result && (mData[index] == Approx(rhs.mData[index]));
            }
            U32 index = (i + 1) * mNumRegistersPerCol - 1;
            result = result && mData[index] == (Approx<__mx, _rows % mNumRegisterEntries>(rhs.mData[index]));
        }
    // clang-format on

    return result;
}



template <typename _type, I32 _rows, I32 _cols>
bool MatSIMD<_type, _rows, _cols>::operator!=(const MatSIMD& rhs) const
{
    return !operator==(rhs);
}



template <typename _type, I32 _rows, I32 _cols>
MatSIMD<_type, _rows, _cols>& MatSIMD<_type, _rows, _cols>::operator+=(const MatSIMD<_type, _rows, _cols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] = _mmx_add_p(rhs.mData[i], mData[i]);
    return *this;
}



template <typename _type, I32 _rows, I32 _cols>
MatSIMD<_type, _rows, _cols> MatSIMD<_type, _rows, _cols>::operator+(const MatSIMD<_type, _rows, _cols>& rhs) const
{
    MatSIMD<_type, _rows, _cols> result{true};
    for (U32 i = 0; i < mNumRegisters; ++i)
        result.mData[i] = _mmx_add_p(rhs.mData[i], mData[i]);
    return result;
}



template <typename _type, I32 _rows, I32 _cols>
void MatSIMD<_type, _rows, _cols>::TransposeFullBlocks(MatSIMD<_type, _cols, _rows>& result) const
{
    constexpr U32 rowsBlock = _rows / mNumRegisterEntries;
    constexpr U32 colsBlock = _cols / mNumRegisterEntries;


    // clang-format off
    for (U32 iR = 0; iR < rowsBlock; ++iR)
        for (U32 iC = 0; iC < colsBlock; ++iC)
        {
            const U32 firstRegisterIndexSrc = iR + iC * mNumRegisterEntries * mNumRegistersPerCol;
            const U32 firstRegisterIndexRes = iC + iR * mNumRegisterEntries * result.mNumRegistersPerCol;

            if constexpr(mNumRegisterEntries == 2)
            {
                GDL::Transpose(mData[firstRegisterIndexSrc + 0 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 1 * mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 0 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 1 * result.mNumRegistersPerCol]);
            }
            else if constexpr(mNumRegisterEntries == 4)
            {
                GDL::Transpose(mData[firstRegisterIndexSrc + 0 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 1 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 2 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 3 * mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 0 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 1 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 2 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 3 * result.mNumRegistersPerCol]);
            }
            else if constexpr(mNumRegisterEntries == 8)
            {
                GDL::Transpose(mData[firstRegisterIndexSrc + 0 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 1 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 2 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 3 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 4 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 5 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 6 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 7 * mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 0 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 1 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 2 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 3 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 4 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 5 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 6 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 7 * result.mNumRegistersPerCol]);
            }
            else
                EXCEPTION(true, "Not implemented for given register size");
        }
    // clang-format on
}



template <typename _type, I32 _rows, I32 _cols>
template <U32 _count, typename... _args>
void MatSIMD<_type, _rows, _cols>::TransposeSparseBlocks(MatSIMD<_type, _cols, _rows>& result, U32 i,
                                                         _args&... args) const
{
    constexpr U32 iC = _cols / mNumRegisterEntries;
    const U32 firstRegisterIndexSrc = i + iC * mNumRegisterEntries * mNumRegistersPerCol;
    // clang-format off
    if constexpr(_count <mNumRegisterEntries)
    {
        if constexpr(_count < _cols % mNumRegisterEntries)
            TransposeSparseBlocks<_count+1>(result,i,args...,mData[firstRegisterIndexSrc + _count * mNumRegistersPerCol]);
        else
        {
            alignas(mAlignment) __mx tmp = _mmx_setzero_p<__mx>();
            TransposeSparseBlocks<_count+1>(result,i,args...,tmp);
        }
    }
    else if constexpr(_count < mNumRegisterEntries *2)
    {
        const U32 firstRegisterIndexRes = iC + i * mNumRegisterEntries * result.mNumRegistersPerCol;
        TransposeSparseBlocks<_count+1>(result,i,args...,result.mData[firstRegisterIndexRes + (_count -mNumRegisterEntries) * result.mNumRegistersPerCol]);
    }
    else
    GDL::Transpose(args...);
    // clang-format on
}



template <typename _type, I32 _rows, I32 _cols>
template <U32 _count, typename... _args>
void MatSIMD<_type, _rows, _cols>::TransposeSparseBlocks2(MatSIMD<_type, _cols, _rows>& result, U32 i,
                                                          _args&... args) const
{
    constexpr U32 iR = _rows / mNumRegisterEntries;
    const U32 firstRegisterIndexSrc = iR + i * mNumRegisterEntries * mNumRegistersPerCol;
    // clang-format off
    if constexpr(_count <mNumRegisterEntries)
    {
            TransposeSparseBlocks2<_count+1>(result,i,args...,mData[firstRegisterIndexSrc + _count * mNumRegistersPerCol]);
    }
    else if constexpr(_count < mNumRegisterEntries *2)
    {
        if constexpr(_count-mNumRegisterEntries < _rows % mNumRegisterEntries)
        {
            const U32 firstRegisterIndexRes = i + iR * mNumRegisterEntries * result.mNumRegistersPerCol;
            TransposeSparseBlocks2<_count+1>(result,i,args...,result.mData[firstRegisterIndexRes + (_count -mNumRegisterEntries) * result.mNumRegistersPerCol]);
        }
        else
        {
            alignas(mAlignment) __mx tmp = _mmx_setzero_p<__mx>();
            TransposeSparseBlocks2<_count+1>(result,i,args...,tmp);
        }
    }
    else
    GDL::Transpose(args...);
    // clang-format on
}



template <typename _type, I32 _rows, I32 _cols>
template <U32 _count, typename... _args>
void MatSIMD<_type, _rows, _cols>::TransposeSparseBlocks3(MatSIMD<_type, _cols, _rows>& result, _args&... args) const
{
    constexpr U32 iR = _rows / mNumRegisterEntries;
    constexpr U32 iC = _cols / mNumRegisterEntries;
    const U32 firstRegisterIndexSrc = iR + iC * mNumRegisterEntries * mNumRegistersPerCol;
    // clang-format off
    if constexpr(_count <mNumRegisterEntries)
    {
        if constexpr(_count < _cols % mNumRegisterEntries && _cols % mNumRegisterEntries>0)
            TransposeSparseBlocks3<_count+1>(result,args...,mData[firstRegisterIndexSrc + _count * mNumRegistersPerCol]);
        else
        {
            alignas(mAlignment) __mx tmp = _mmx_setzero_p<__mx>();
            TransposeSparseBlocks3<_count+1>(result,args...,tmp);
        }
    }
    else if constexpr(_count < mNumRegisterEntries *2)
    {
        if constexpr(_count-mNumRegisterEntries < _rows % mNumRegisterEntries && _rows % mNumRegisterEntries>0)
        {
            const U32 firstRegisterIndexRes = iC + iR * mNumRegisterEntries * result.mNumRegistersPerCol;
            TransposeSparseBlocks3<_count+1>(result,args...,result.mData[firstRegisterIndexRes + (_count -mNumRegisterEntries) * result.mNumRegistersPerCol]);
        }
        else
        {
            alignas(mAlignment) __mx tmp = _mmx_setzero_p<__mx>();
            TransposeSparseBlocks3<_count+1>(result,args...,tmp);
        }
    }
    else
    GDL::Transpose(args...);
    // clang-format on
}


template <typename _type, I32 _rows, I32 _cols>
MatSIMD<_type, _cols, _rows> MatSIMD<_type, _rows, _cols>::Transpose() const
{
    MatSIMD<_type, _cols, _rows> transposed;

    TransposeFullBlocks(transposed);

    // clang-format off
    if constexpr(_cols % mNumRegisterEntries != 0 )

        for(U32 i=0; i<_rows/mNumRegisterEntries;++i)
            TransposeSparseBlocks(transposed,i);

    if constexpr( _rows % mNumRegisterEntries != 0)
        for(U32 i=0; i<_cols/mNumRegisterEntries;++i)
            TransposeSparseBlocks2(transposed,i);

    if constexpr(_rows % mNumRegisterEntries != 0 && _cols % mNumRegisterEntries != 0)
        TransposeSparseBlocks3(transposed);
// clang-format on

return transposed;
}



template <typename _type, I32 _rows, I32 _cols>
U32 MatSIMD<_type, _rows, _cols>::Rows() const
{
    return _rows;
}



template <typename _type, I32 _rows, I32 _cols>
U32 MatSIMD<_type, _rows, _cols>::Cols() const
{
    return _cols;
}



template <typename _type, I32 _rows, I32 _cols>
void MatSIMD<_type, _rows, _cols>::SetZero()
{
    std::memset(&mData, 0, sizeof(mData));
}



template <typename _type, I32 _rows, I32 _cols>
const std::array<_type, _rows * _cols> MatSIMD<_type, _rows, _cols>::Data() const
{
    std::array<_type, _rows * _cols> data;
    if
        constexpr(_rows % mNumRegisterEntries == 0)
        {
            assert(sizeof(mData) == sizeof(data));
            std::memcpy(&data, &mData, sizeof(data));
        }
    else
    {
        for (U32 i = 0; i < _cols; ++i)
        {
            std::memcpy(&data[i * _rows], &mData[i * mNumRegistersPerCol], sizeof(_type) * _rows);
        }
    }
    return data;
}



template <typename _type, I32 _rows, I32 _cols>
void MatSIMD<_type, _rows, _cols>::ConstructionChecks() const
{
    assert(sizeof(mData) == sizeof(__mx) * mData.size()); // Array needs to be compact
#ifndef NDEVEXCEPTION
    for (const auto& reg : mData)
        DEV_EXCEPTION(!IsAligned(&reg, mAlignment), "One or more registers of the matrix are not aligned correctly");
#endif
}



template <typename _type, I32 _rows, I32 _cols>
template <I32 _rowsRhs, I32 _colsRhs>
MatSIMD<_type, _rows, _colsRhs> MatSIMD<_type, _rows, _cols>::
operator*(const MatSIMD<_type, _rowsRhs, _colsRhs>& rhs) const
{
    static_assert(_cols == _rowsRhs, "Lhs cols != Rhs rows");

    MatSIMD<_type, _rows, _colsRhs> result;


    // loop over RHS rows (column registers)
    for (U32 j = 0; j < _rowsRhs / mNumRegisterEntries; ++j)
        MultiplicationInnerLoops<_rowsRhs, _colsRhs, mNumRegisterEntries>(result, rhs, j);


    // clang-format off
    constexpr U32 remainder = _rowsRhs % mNumRegisterEntries;
    if constexpr(remainder > 0)
        MultiplicationInnerLoops<_rowsRhs, _colsRhs, remainder>(result, rhs, _rowsRhs / mNumRegisterEntries);
    // clang-format on

    return result;
}



template <typename _type, I32 _rows, I32 _cols>
template <I32 _rowsRhs, I32 _colsRhs, U32 _numMultipliedRegisters>
inline void MatSIMD<_type, _rows, _cols>::MultiplicationInnerLoops(MatSIMD<_type, _rows, _colsRhs>& result,
                                                                   const MatSIMD<_type, _rowsRhs, _colsRhs>& rhs,
                                                                   U32 j) const
{
    constexpr U32 registersPerColRhs = SSECalcMinNumArrayRegisters<__mx>(_rowsRhs);


    for (U32 i = 0; i < _colsRhs; ++i)
    {
        const U32 indexRegisterRhs = i * registersPerColRhs + j;
        alignas(mAlignment) std::array<_type, mNumRegisterEntries> rhsRegisterValues;
        _mmx_store_p(rhsRegisterValues.data(), rhs.mData[indexRegisterRhs]);

        std::array<__mx, _numMultipliedRegisters> tmp;
        for (U32 k = 0; k < _numMultipliedRegisters; ++k)
            tmp[k] = _mmx_set1_p<__mx>(rhsRegisterValues[k]);

        // loop over LHS rows (column registers)
        for (U32 k = 0; k < mNumRegistersPerCol; ++k)
        {
            const U32 indexRegisterResult = i * mNumRegistersPerCol + k;
            const U32 indexFirstRegisterLhsBlock = j * mNumRegisterEntries * mNumRegistersPerCol + k;
            result.mData[indexRegisterResult] = MultiplyAddRegisters<_numMultipliedRegisters>(
                    tmp, result.mData[indexRegisterResult], indexFirstRegisterLhsBlock);
        }
    }
}



template <typename _type, I32 _rows, I32 _cols>
template <U32 _numOperations, U32 _count>
typename MatSIMD<_type, _rows, _cols>::__mx
MatSIMD<_type, _rows, _cols>::MultiplyAddRegisters(const std::array<__mx, _numOperations>& values,
                                                   const __mx currentValue, const U32 currentBlockIndex) const
{
    static_assert(_numOperations <= mNumRegisterEntries && _numOperations > 0, "Invalid number of operations.");

    // clang-format off
    if constexpr(_count + 1 == _numOperations)
        return _mmx_fmadd_p(mData[currentBlockIndex + _count * mNumRegistersPerCol], values[_count], currentValue);
    else
        return _mmx_fmadd_p(mData[currentBlockIndex + _count * mNumRegistersPerCol], values[_count],
                            MultiplyAddRegisters<_numOperations, _count + 1>(values, currentValue, currentBlockIndex));
    // clang-format on
}



template <typename _type, I32 _rows, I32 _cols>
std::ostream& operator<<(std::ostream& os, const MatSIMD<_type, _rows, _cols>& mat)
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
