#pragma once

#include "gdl/math/matSSE.h"

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
MatSSE<_type, _rows, _cols>::MatSSE()
    : mData{{0}}
{
    ConstructionChecks();
}



template <typename _type, I32 _rows, I32 _cols>
template <typename... _args>
MatSSE<_type, _rows, _cols>::MatSSE(_args... args)
{
    assert(sizeof...(args) == _rows * _cols);
    *this = MatSSE(std::array<_type, _rows * _cols>{{static_cast<_type>(args)...}});
}



template <typename _type, I32 _rows, I32 _cols>
MatSSE<_type, _rows, _cols>::MatSSE(const std::array<_type, _rows * _cols>& data)
{
    if constexpr (_rows % mNumRegisterEntries == 0)
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
MatSSE<_type, _rows, _cols>::MatSSE(bool)
{
}



template <typename _type, I32 _rows, I32 _cols>
_type MatSSE<_type, _rows, _cols>::operator()(const U32 row, const U32 col) const
{
    assert(row < _rows && col < _cols);
    return mData[row / mNumRegisterEntries + col * mNumRegistersPerCol][row % mNumRegisterEntries];
}



template <typename _type, I32 _rows, I32 _cols>
bool MatSSE<_type, _rows, _cols>::operator==(const MatSSE& rhs) const
{
    bool result = true;

    if constexpr (_rows % mNumRegisterEntries == 0)
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

    return result;
}



template <typename _type, I32 _rows, I32 _cols>
bool MatSSE<_type, _rows, _cols>::operator!=(const MatSSE& rhs) const
{
    return !operator==(rhs);
}



template <typename _type, I32 _rows, I32 _cols>
MatSSE<_type, _rows, _cols>& MatSSE<_type, _rows, _cols>::operator+=(const MatSSE<_type, _rows, _cols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] = _mmx_add_p(rhs.mData[i], mData[i]);
    return *this;
}



template <typename _type, I32 _rows, I32 _cols>
MatSSE<_type, _rows, _cols> MatSSE<_type, _rows, _cols>::operator+(const MatSSE<_type, _rows, _cols>& rhs) const
{
    MatSSE<_type, _rows, _cols> result{true};
    for (U32 i = 0; i < mNumRegisters; ++i)
        result.mData[i] = _mmx_add_p(rhs.mData[i], mData[i]);
    return result;
}



template <typename _type, I32 _rows, I32 _cols>
void MatSSE<_type, _rows, _cols>::TransposeFullBlocks(MatSSE<_type, _cols, _rows>& result) const
{
    constexpr U32 rowsBlock = _rows / mNumRegisterEntries;
    constexpr U32 colsBlock = _cols / mNumRegisterEntries;

    for (U32 iR = 0; iR < rowsBlock; ++iR)
        for (U32 iC = 0; iC < colsBlock; ++iC)
        {
            const U32 firstRegisterIndexSrc = iR + iC * mNumRegisterEntries * mNumRegistersPerCol;
            const U32 firstRegisterIndexRes = iC + iR * mNumRegisterEntries * result.mNumRegistersPerCol;

            if constexpr (mNumRegisterEntries == 2)
            {
                GDL::Transpose(mData[firstRegisterIndexSrc + 0 * mNumRegistersPerCol],
                               mData[firstRegisterIndexSrc + 1 * mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 0 * result.mNumRegistersPerCol],
                               result.mData[firstRegisterIndexRes + 1 * result.mNumRegistersPerCol]);
            }
            else if constexpr (mNumRegisterEntries == 4)
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
            else if constexpr (mNumRegisterEntries == 8)
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
}



template <typename _type, I32 _rows, I32 _cols>
template <bool _hasSparseRows, bool _hasSparseCols, U32 _count, typename... _args>
void MatSSE<_type, _rows, _cols>::TransposeSparseBlocks(MatSSE<_type, _cols, _rows>& result, U32 indexBlock,
                                                         _args&... args) const
{
    static_assert(_hasSparseRows || _hasSparseCols, "This function cant be used with non sparse blocks");

    const U32 iC = (_hasSparseRows && !_hasSparseCols) ? indexBlock : _cols / mNumRegisterEntries;
    const U32 iR = (_hasSparseCols && !_hasSparseRows) ? indexBlock : _rows / mNumRegisterEntries;

    if constexpr (_count < mNumRegisterEntries)
    {
        if constexpr (_hasSparseCols && _count >= _cols % mNumRegisterEntries)
        {
            alignas(mAlignment) __mx tmp = _mmx_setzero_p<__mx>();
            TransposeSparseBlocks<_hasSparseRows, _hasSparseCols, _count + 1>(result, indexBlock, args..., tmp);
        }
        else
        {
            const U32 registerIndexSrc = iR + (iC * mNumRegisterEntries + _count) * mNumRegistersPerCol;
            TransposeSparseBlocks<_hasSparseRows, _hasSparseCols, _count + 1>(result, indexBlock, args...,
                                                                              mData[registerIndexSrc]);
        }
    }
    else if constexpr (_count < mNumRegisterEntries * 2)
    {
        if constexpr (_hasSparseRows && _count >= _rows % mNumRegisterEntries + mNumRegisterEntries)
        {
            alignas(mAlignment) __mx tmp = _mmx_setzero_p<__mx>();
            TransposeSparseBlocks<_hasSparseRows, _hasSparseCols, _count + 1>(result, indexBlock, args..., tmp);
        }
        else
        {
            const U32 registerIndexRes = iC + ((iR - 1) * mNumRegisterEntries + _count) * result.mNumRegistersPerCol;
            TransposeSparseBlocks<_hasSparseRows, _hasSparseCols, _count + 1>(result, indexBlock, args...,
                                                                              result.mData[registerIndexRes]);
        }
    }
    else
        GDL::Transpose(args...);
}



template <typename _type, I32 _rows, I32 _cols>
MatSSE<_type, _cols, _rows> MatSSE<_type, _rows, _cols>::Transpose() const
{
    MatSSE<_type, _cols, _rows> transposed;

    TransposeFullBlocks(transposed);

    if constexpr (_cols % mNumRegisterEntries != 0)
        for (U32 i = 0; i < _rows / mNumRegisterEntries; ++i)
            TransposeSparseBlocks<false, true>(transposed, i);

    if constexpr (_rows % mNumRegisterEntries != 0)
        for (U32 i = 0; i < _cols / mNumRegisterEntries; ++i)
            TransposeSparseBlocks<true, false>(transposed, i);

    if constexpr (_rows % mNumRegisterEntries != 0 && _cols % mNumRegisterEntries != 0)
        TransposeSparseBlocks<true, true>(transposed, 0);

    return transposed;
}



template <typename _type, I32 _rows, I32 _cols>
U32 MatSSE<_type, _rows, _cols>::Rows() const
{
    return _rows;
}



template <typename _type, I32 _rows, I32 _cols>
U32 MatSSE<_type, _rows, _cols>::Cols() const
{
    return _cols;
}



template <typename _type, I32 _rows, I32 _cols>
void MatSSE<_type, _rows, _cols>::SetZero()
{
    std::memset(&mData, 0, sizeof(mData));
}



template <typename _type, I32 _rows, I32 _cols>
const std::array<_type, _rows * _cols> MatSSE<_type, _rows, _cols>::Data() const
{
    std::array<_type, _rows * _cols> data;
    if constexpr (_rows % mNumRegisterEntries == 0)
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
void MatSSE<_type, _rows, _cols>::ConstructionChecks() const
{
    assert(sizeof(mData) == sizeof(__mx) * mData.size()); // Array needs to be compact
#ifndef NDEVEXCEPTION
    for (const auto& reg : mData)
        DEV_EXCEPTION(!IsAligned(&reg, mAlignment), "One or more registers of the matrix are not aligned correctly");
#endif
}



template <typename _type, I32 _rows, I32 _cols>
template <I32 _rowsRhs, I32 _colsRhs>
MatSSE<_type, _rows, _colsRhs> MatSSE<_type, _rows, _cols>::
operator*(const MatSSE<_type, _rowsRhs, _colsRhs>& rhs) const
{
    static_assert(_cols == _rowsRhs, "Lhs cols != Rhs rows");

    MatSSE<_type, _rows, _colsRhs> result;


    // loop over RHS rows (column registers)
    for (U32 j = 0; j < _rowsRhs / mNumRegisterEntries; ++j)
        MultiplicationInnerLoops<_rowsRhs, _colsRhs, mNumRegisterEntries>(result, rhs, j);

    constexpr U32 remainder = _rowsRhs % mNumRegisterEntries;
    if constexpr (remainder > 0)
        MultiplicationInnerLoops<_rowsRhs, _colsRhs, remainder>(result, rhs, _rowsRhs / mNumRegisterEntries);

    return result;
}



template <typename _type, I32 _rows, I32 _cols>
template <I32 _rowsRhs, I32 _colsRhs, U32 _numMultipliedRegisters>
inline void MatSSE<_type, _rows, _cols>::MultiplicationInnerLoops(MatSSE<_type, _rows, _colsRhs>& result,
                                                                   const MatSSE<_type, _rowsRhs, _colsRhs>& rhs,
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
typename MatSSE<_type, _rows, _cols>::__mx
MatSSE<_type, _rows, _cols>::MultiplyAddRegisters(const std::array<__mx, _numOperations>& values,
                                                   const __mx currentValue, const U32 currentBlockIndex) const
{
    static_assert(_numOperations <= mNumRegisterEntries && _numOperations > 0, "Invalid number of operations.");


    if constexpr (_count + 1 == _numOperations)
        return _mmx_fmadd_p(mData[currentBlockIndex + _count * mNumRegistersPerCol], values[_count], currentValue);
    else
        return _mmx_fmadd_p(mData[currentBlockIndex + _count * mNumRegistersPerCol], values[_count],
                            MultiplyAddRegisters<_numOperations, _count + 1>(values, currentValue, currentBlockIndex));
}



template <typename _type, I32 _rows, I32 _cols>
std::ostream& operator<<(std::ostream& os, const MatSSE<_type, _rows, _cols>& mat)
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
