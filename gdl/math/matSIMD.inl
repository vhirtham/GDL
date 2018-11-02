#pragma once

#include "gdl/math/matSIMD.h"

#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"

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
    alignas(mAlignment) std::array<_type, mNumRegisterEntries> registerValues;

    // loop over every RHS Col
    for (U32 i = 0; i < _colsRhs; ++i)
    {
        const U32 registerNumRhs = i * registersPerColRhs + j;
        _mmx_store_p(registerValues.data(), rhs.mData[registerNumRhs]);
        alignas(mAlignment) std::array<__mx, _numMultipliedRegisters> tmp =
                MultiplicationCreateRHSArray<_numMultipliedRegisters>(registerValues);

        // loop over LHS rows (column registers)
        for (U32 k = 0; k < mNumRegistersPerCol; ++k)
        {
            const U32 registerNumResult = i * mNumRegistersPerCol + k;
            const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
            result.mData[registerNumResult] = MultiplyAddRegisters<_numMultipliedRegisters>(
                    tmp, result.mData[registerNumResult], currentBlockLhs);
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
template <U32 _arraySize, U32 _count, typename... _args>
std::array<typename MatSIMD<_type, _rows, _cols>::__mx, _arraySize>
MatSIMD<_type, _rows, _cols>::MultiplicationCreateRHSArray(const std::array<_type, mNumRegisterEntries>& data,
                                                           const _args&... args)
{
    static_assert(_arraySize <= SSEGetNumRegisterEntries<__mx>() && _arraySize > 0, "Invalid array size.");

    // clang-format off
    if constexpr(_arraySize == _count)
        return std::array<__mx, _arraySize>{{args...}};
    else
        return MultiplicationCreateRHSArray<_arraySize, _count + 1>(data, args..., _mmx_set1_p<__mx>(data[_count]));
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