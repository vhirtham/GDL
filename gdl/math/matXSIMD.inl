#pragma once

#include "matXSIMD.h"

#include "gdl/base/functions/alignment.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

namespace GDL
{



template <typename _type, I32 _rows, I32 _cols>
matXSIMD<_type, _rows, _cols>::matXSIMD()
{
    ConstructionChecks();
}



template <typename _type, I32 _rows, I32 _cols>
template <typename... _args>
matXSIMD<_type, _rows, _cols>::matXSIMD(_args... args)
{
    assert(sizeof...(args) == _rows * _cols);
    *this = matXSIMD(std::array<_type, _rows * _cols>{{static_cast<_type>(args)...}});
}



template <typename _type, I32 _rows, I32 _cols>
matXSIMD<_type, _rows, _cols>::matXSIMD(const std::array<_type, _rows * _cols>& data)
{
    if (_rows % mNumRegisterEntries == 0)
    {
        assert(sizeof(mData) == sizeof(data));
        std::memcpy(&mData, &data, sizeof(data));
    }
    else
    {
        for (U32 i = 0; i < _cols; ++i)
        {
            // Set last column register to zero
            std::memset((&mData[(i + 1) * mNumRegistersPerCol - 1]), 0, sizeof(__mx));
            // Copy data into column
            std::memcpy(&mData[i * mNumRegistersPerCol], &data[i * _rows], sizeof(_type) * _rows);
        }
    }

    ConstructionChecks();
}



template <typename _type, I32 _rows, I32 _cols>
matXSIMD<_type, _rows, _cols>& matXSIMD<_type, _rows, _cols>::operator+=(const matXSIMD<_type, _rows, _cols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] = _mmx_add_p(rhs.mData[i], mData[i]);
    return *this;
}



template <typename _type, I32 _rows, I32 _cols>
_type matXSIMD<_type, _rows, _cols>::operator()(const U32 row, const U32 col) const
{
    assert(row < _rows && col < _cols);
    return mData[row / mNumRegisterEntries + col * mNumRegistersPerCol][row % mNumRegisterEntries];
}



template <typename _type, I32 _rows, I32 _cols>
U32 matXSIMD<_type, _rows, _cols>::Rows() const
{
    return _rows;
}



template <typename _type, I32 _rows, I32 _cols>
U32 matXSIMD<_type, _rows, _cols>::Cols() const
{
    return _cols;
}



template <typename _type, I32 _rows, I32 _cols>
void matXSIMD<_type, _rows, _cols>::SetZero()
{
    mData.fill(_mmx_set1_p<__mx>(static_cast<_type>(0)));
}



template <typename _type, I32 _rows, I32 _cols>
const std::array<_type, _rows * _cols> matXSIMD<_type, _rows, _cols>::Data() const
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
void matXSIMD<_type, _rows, _cols>::ConstructionChecks() const
{
    assert(sizeof(mData) == sizeof(__mx) * mData.size()); // Array needs to be compact
#ifndef NDEBUG
    for (const auto& iRegister : mData)
        if (!IsAligned(&iRegister, 16))
            throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}



template <typename _type, I32 _rows, I32 _cols>
template <I32 _rowsRhs, I32 _colsRhs>
matXSIMD<_type, _rows, _colsRhs> matXSIMD<_type, _rows, _cols>::
operator*(const matXSIMD<_type, _rowsRhs, _colsRhs>& rhs) const
{
    static_assert(_cols == _rowsRhs, "Lhs cols != Rhs rows - Matrix multiplication not possible!");

    constexpr U32 registersPerColRhs = CalcMinNumArrayRegisters<__mx>(_rowsRhs);

    matXSIMD<_type, _rows, _colsRhs> result;
    result.SetZero();


    // Registers with 4 data entries %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if
        constexpr(mNumRegisterEntries == 4)
        {
            // loop over RHS rows (column registers)
            for (U32 j = 0; j < _rowsRhs / mNumRegisterEntries; ++j)
                // loop over every RHS Col
                for (U32 i = 0; i < _colsRhs; ++i)
                {
                    const U32 registerNumRhs = i * registersPerColRhs + j;
                    alignas(mAlignment) std::array<__mx, mNumRegisterEntries> tmp =
                            CreateRHSRegisterArray(rhs.mData[registerNumRhs]);

                    // loop over LHS rows (column registers)
                    for (U32 k = 0; k < mNumRegistersPerCol; ++k)
                    {
                        const U32 registerNumResult = i * mNumRegistersPerCol + k;
                        const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
#ifdef ENABLE_SSE4
                        result.mData[registerNumResult] = MultiplyRegisters<mNumRegisterEntries>(
                                tmp, result.mData[registerNumResult], currentBlockLhs);
#else
                        result.mData[registerNumResult] = _mmx_add_p<_type>(
                                _mmx_add_p<_type>(
                                        _mmx_add_p<_type>(
                                                _mmx_mul_p(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp[0]),
                                                _mmx_mul_p(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp[1])),
                                        _mmx_add_p<_type>(
                                                _mmx_mul_p(mData[currentBlockLhs + 2 * mNumRegistersPerCol], tmp[2]),
                                                _mmx_mul_p(mData[currentBlockLhs + 3 * mNumRegistersPerCol], tmp[3]))),
                                result.mData[registerNumResult]);

#endif
                    }
                }

            constexpr U32 remainder = _rowsRhs % mNumRegisterEntries;
            if
                constexpr(remainder == 3)
                {
                    U32 j = _rowsRhs / mNumRegisterEntries;
                    // loop over every RHS Col
                    for (U32 i = 0; i < _colsRhs; ++i)
                    {
                        const U32 registerNumRhs = i * registersPerColRhs + j;
                        alignas(mAlignment) std::array<__mx, remainder> tmp =
                                CreateRHSRegisterArray<remainder>(rhs.mData[registerNumRhs]);
                        // loop over LHS rows (column registers)
                        for (U32 k = 0; k < mNumRegistersPerCol; ++k)
                        {
                            const U32 registerNumResult = i * mNumRegistersPerCol + k;
                            const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
#ifdef ENABLE_SSE4
                            result.mData[registerNumResult] =
                                    MultiplyRegisters<remainder>(tmp, result.mData[registerNumResult], currentBlockLhs);
#else
                            result.mData[registerNumResult] = _mmx_add_p<_type>(
                                    _mmx_add_p<_type>(
                                            _mmx_mul_p(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp[0]),
                                            _mmx_mul_p(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp[1])),
                                    _mmx_add_p<_type>(
                                            _mmx_mul_p(mData[currentBlockLhs + 2 * mNumRegistersPerCol], tmp[2]),
                                            result.mData[registerNumResult]));

#endif
                        }
                    }
                }
            if
                constexpr(remainder == 2)
                {
                    U32 j = _rowsRhs / mNumRegisterEntries;
                    // loop over every RHS Col
                    for (U32 i = 0; i < _colsRhs; ++i)
                    {
                        const U32 registerNumRhs = i * registersPerColRhs + j;
                        __mx tmp0 = _mmx_set1_p<_type, __mx>(rhs.mData[registerNumRhs][0]);
                        __mx tmp1 = _mmx_set1_p<_type, __mx>(rhs.mData[registerNumRhs][1]);
                        // loop over LHS rows (column registers)
                        for (U32 k = 0; k < mNumRegistersPerCol; ++k)
                        {
                            const U32 registerNumResult = i * mNumRegistersPerCol + k;
                            const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
#ifdef ENABLE_SSE4
                            result.mData[registerNumResult] =
                                    _mmx_fmadd_p(mData[currentBlockLhs], tmp0,
                                                 _mmx_fmadd_p(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp1,
                                                              result.mData[registerNumResult]));
#else
                            result.mData[registerNumResult] = _mmx_add_p<_type>(
                                    _mmx_add_p<_type>(
                                            _mmx_mul_p(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp0),
                                            _mmx_mul_p(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp1)),
                                    result.mData[registerNumResult]);

#endif
                        }
                    }
                }
            if
                constexpr(remainder == 1)
                {
                    U32 j = _rowsRhs / mNumRegisterEntries;
                    // loop over every RHS Col
                    for (U32 i = 0; i < _colsRhs; ++i)
                    {
                        const U32 registerNumRhs = i * registersPerColRhs + j;
                        alignas(mAlignment) std::array<__mx, remainder> tmp =
                                CreateRHSRegisterArray<remainder>(rhs.mData[registerNumRhs]);
                        // loop over LHS rows (column registers)
                        for (U32 k = 0; k < mNumRegistersPerCol; ++k)
                        {
                            const U32 registerNumResult = i * mNumRegistersPerCol + k;
                            const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
#ifdef ENABLE_SSE4
                            result.mData[registerNumResult] =
                                    MultiplyRegisters<remainder>(tmp, result.mData[registerNumResult], currentBlockLhs);
#else
                            result.mData[registerNumResult] = _mmx_add_p<_type>(
                                    _mmx_mul_p(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp[0]),
                                    result.mData[registerNumResult]);

#endif
                        }
                    }
                }
        }


    // Registers with 2 data entries %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if
        constexpr(mNumRegisterEntries == 2)
        {
            // loop over RHS rows (column registers)
            for (U32 j = 0; j < _rowsRhs / mNumRegisterEntries; ++j)
                // loop over every RHS Col
                for (U32 i = 0; i < _colsRhs; ++i)
                {
                    const U32 registerNumRhs = i * registersPerColRhs + j;
                    std::array<__mx, mNumRegisterEntries> tmp = CreateRHSRegisterArray(rhs.mData[registerNumRhs]);
                    // loop over LHS rows (column registers)
                    for (U32 k = 0; k < mNumRegistersPerCol; ++k)
                    {
                        const U32 registerNumResult = i * mNumRegistersPerCol + k;
                        const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
#ifdef ENABLE_SSE4
                        result.mData[registerNumResult] = MultiplyRegisters<mNumRegisterEntries>(
                                tmp, result.mData[registerNumResult], currentBlockLhs);
#else
                        result.mData[registerNumResult] = _mmx_add_p<_type>(

                                _mmx_add_p<_type>(_mmx_mul_p(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp[0]),
                                                  _mmx_mul_p(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp[1])),

                                result.mData[registerNumResult]);

#endif
                    }
                }
            if
                constexpr(_rowsRhs % mNumRegisterEntries == 1)
                {
                    U32 j = _rowsRhs / mNumRegisterEntries;
                    for (U32 i = 0; i < _colsRhs; ++i)
                    {
                        const U32 registerNumRhs = i * registersPerColRhs + j;
                        __mx tmp0 = _mmx_set1_p<__mx>(rhs.mData[registerNumRhs][0]);
                        // loop over LHS rows (column registers)
                        for (U32 k = 0; k < mNumRegistersPerCol; ++k)
                        {
                            const U32 registerNumResult = i * mNumRegistersPerCol + k;
                            const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
#ifdef ENABLE_SSE4
                            result.mData[registerNumResult] =
                                    _mmx_fmadd_p(mData[currentBlockLhs], tmp0, result.mData[registerNumResult]);
#else
                            result.mData[registerNumResult] = _mmx_add_p<_type>(
                                    _mmx_mul_p(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp0),
                                    result.mData[registerNumResult]);

#endif
                        }
                    }
                }
        }

    return result;
}



template <typename _type, I32 _rows, I32 _cols>
template <U32 _numOperations, U32 _count>
typename matXSIMD<_type, _rows, _cols>::__mx
matXSIMD<_type, _rows, _cols>::MultiplyRegisters(const std::array<__mx, _numOperations>& values,
                                                 const __mx currentValue, const U32 currentBlockIndex) const
{
    static_assert(_numOperations <= mNumRegisterEntries && _numOperations > 0, "Invalid number of operations.");

    // clang-format off
    if
        constexpr(_count + 1 == _numOperations) return _mmx_fmadd_p(
                mData[currentBlockIndex + _count * mNumRegistersPerCol], values[_count], currentValue);
    else
        return _mmx_fmadd_p(mData[currentBlockIndex + _count * mNumRegistersPerCol], values[_count],
                            MultiplyRegisters<_numOperations, _count + 1>(values, currentValue, currentBlockIndex));
    // clang-format on
}



template <typename _type, I32 _rows, I32 _cols>
template <U32 _arraySize, U32 _count, typename... _args>
std::array<typename matXSIMD<_type, _rows, _cols>::__mx, _arraySize>
matXSIMD<_type, _rows, _cols>::CreateRHSRegisterArray(const __mx& data, const _args&... args)
{
    static_assert(_arraySize <= GetNumRegisterEntries<__mx>() && _arraySize > 0, "Invalid array size.");

    // clang-format off
    if constexpr(_arraySize == _count)
        return std::array<__mx, _arraySize>{{args...}};
    else
        return CreateRHSRegisterArray<_arraySize, _count + 1>(data, args..., _mmx_set1_p<__mx>(data[_count]));
    // clang-format on
}



template <typename _type, I32 _rowsOther, I32 _colsOther>
std::ostream& operator<<(std::ostream& os, const matXSIMD<_type, _rowsOther, _colsOther>& mat)
{
    using namespace GDL;
    for (U32 i = 0; i < _rowsOther; ++i)
    {
        os << "| ";
        for (U32 j = 0; j < _colsOther; ++j)
            os << mat(i, j) << " ";
        os << "|" << std::endl;
    }
    return os;
}



} // namespace GDL
