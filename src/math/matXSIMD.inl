#pragma once

#include "matXSIMD.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#ifndef NDEBUG
#include <iostream>
#endif

template <typename T, int tRows, int tCols>
GDL::matXSIMD<T, tRows, tCols>::matXSIMD()
{
    ConstructionChecks();
}


template <typename T, int tRows, int tCols>
template <typename... Args>
GDL::matXSIMD<T, tRows, tCols>::matXSIMD(Args... args)
{
    assert(sizeof...(args) == tRows * tCols);
    *this = matXSIMD(std::array<T, tRows * tCols>{{static_cast<T>(args)...}});
}

template <typename T, int tRows, int tCols>
GDL::matXSIMD<T, tRows, tCols>::matXSIMD(const std::array<T, tRows * tCols>& data)
{
    if (tRows % mNumRegisterEntries == 0)
    {
        assert(sizeof(mData) == sizeof(data));
        std::memcpy(&mData, &data, sizeof(data));
    }
    else
    {
        for (U32 i = 0; i < tCols; ++i)
        {
            // Set last column register to zero
            std::memset((&mData[(i + 1) * mNumRegistersPerCol - 1]), 0, sizeof(__mx));
            // Copy data into column
            std::memcpy(&mData[i * mNumRegistersPerCol], &data[i * tRows], sizeof(T) * tRows);
        }
    }

    ConstructionChecks();
}

template <typename T, int tRows, int tCols>
GDL::matXSIMD<T, tRows, tCols>& GDL::matXSIMD<T, tRows, tCols>::operator+=(const GDL::matXSIMD<T, tRows, tCols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] = _mmx_add_p<T>(rhs.mData[i], mData[i]);
    return *this;
}

template <typename T, int tRows, int tCols>
T GDL::matXSIMD<T, tRows, tCols>::operator()(const GDL::U32 row, const GDL::U32 col) const
{
    assert(row < tRows && col < tCols);
    return mData[row / mNumRegisterEntries + col * mNumRegistersPerCol][row % mNumRegisterEntries];
}

template <typename T, int tRows, int tCols>
GDL::U32 GDL::matXSIMD<T, tRows, tCols>::Rows() const
{
    return tRows;
}


template <typename T, int tRows, int tCols>
GDL::U32 GDL::matXSIMD<T, tRows, tCols>::Cols() const
{
    return tCols;
}

template <typename T, int tRows, int tCols>
void GDL::matXSIMD<T, tRows, tCols>::SetZero()
{
    std::fill(mData.begin(), mData.end(), _mmx_set1_p<T, __mx>(0.));
}



template <typename T, int tRows, int tCols>
const std::array<T, tRows * tCols> GDL::matXSIMD<T, tRows, tCols>::Data() const
{
    std::array<T, tRows * tCols> data;
    if (tRows % mNumRegisterEntries == 0)
    {
        assert(sizeof(mData) == sizeof(data));
        std::memcpy(&data, &mData, sizeof(data));
    }
    else
    {
        for (U32 i = 0; i < tCols; ++i)
        {
            std::memcpy(&data[i * tRows], &mData[i * mNumRegistersPerCol], sizeof(T) * tRows);
        }
    }
    return data;
}

template <typename T, int tRows, int tCols>
void GDL::matXSIMD<T, tRows, tCols>::ConstructionChecks() const
{
    assert(sizeof(mData) == sizeof(__mx) * mData.size()); // Array needs to be compact
#ifndef NDEBUG
    for (const auto& iRegister : mData)
        if (!is_aligned(&iRegister, 16))
            throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}


template <typename T, int tRows, int tCols>
template <int tRowsRhs, int tColsRhs>
GDL::matXSIMD<T, tRows, tColsRhs> GDL::matXSIMD<T, tRows, tCols>::
operator*(const matXSIMD<T, tRowsRhs, tColsRhs>& rhs) const
{
    static_assert(tCols == tRowsRhs, "Lhs cols != Rhs rows - Matrix multiplication not possible!");

    constexpr U32 registersPerColRhs = CalcMinNumArrayRegisters(tRowsRhs, mNumRegisterEntries);

    matXSIMD<T, tRows, tColsRhs> result;
    result.SetZero();

    if
        constexpr(mNumRegisterEntries == 4)
        {
            // loop over RHS rows (column registers)
            for (U32 j = 0; j < registersPerColRhs; ++j)
                // loop over every RHS Col
                for (U32 i = 0; i < tColsRhs; ++i)
                {
                    const U32 registerNumRhs = i * registersPerColRhs + j;
                    // those temporaries seem to have no effect on performance but they help to keep the code a little
                    // bit
                    // less
                    // messy. Also check if there is a faster way to extract single values into a new register
                    __mx tmp0 = _mmx_set1_p<T, __mx>(rhs.mData[registerNumRhs][0]);
                    __mx tmp1 = _mmx_set1_p<T, __mx>(rhs.mData[registerNumRhs][1]);
                    __mx tmp2 = _mmx_set1_p<T, __mx>(rhs.mData[registerNumRhs][2]);
                    __mx tmp3 = _mmx_set1_p<T, __mx>(rhs.mData[registerNumRhs][3]);
                    // loop over LHS rows (column registers)
                    for (U32 k = 0; k < mNumRegistersPerCol; ++k)
                    {
                        const U32 registerNumResult = i * mNumRegistersPerCol + k;
                        const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
#ifdef ENABLE_SSE4
                        result.mData[registerNumResult] = _mmx_fmadd_p(
                                mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp0,
                                _mmx_fmadd_p(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp1,
                                             _mmx_fmadd_p(mData[currentBlockLhs + 2 * mNumRegistersPerCol], tmp2,
                                                          _mmx_fmadd_p(mData[currentBlockLhs + 3 * mNumRegistersPerCol],
                                                                       tmp3, result.mData[registerNumResult]))));
#else
                        result.mData[registerNumResult] = _mmx_add_p<T>(
                                _mmx_add_p<T>(
                                        _mmx_add_p<T>(
                                                _mmx_mul_p(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp0),
                                                _mmx_mul_p(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp1)),
                                        _mmx_add_p<T>(
                                                _mmx_mul_p(mData[currentBlockLhs + 2 * mNumRegistersPerCol], tmp2),
                                                _mmx_mul_p(mData[currentBlockLhs + 3 * mNumRegistersPerCol], tmp3))),
                                result.mData[registerNumResult]);

#endif
                    }
                }
        }
    else
    {
        if
            constexpr(mNumRegisterEntries == 2)
            {
                // loop over RHS rows (column registers)
                for (U32 j = 0; j < registersPerColRhs; ++j)
                    // loop over every RHS Col
                    for (U32 i = 0; i < tColsRhs; ++i)
                    {
                        const U32 registerNumRhs = i * registersPerColRhs + j;
                        // those temporaries seem to have no effect on performance but they help to keep the code a
                        // little bit
                        // less
                        // messy. Also check if there is a faster way to extract single values into a new register
                        __mx tmp0 = _mmx_set1_p<T, __mx>(rhs.mData[registerNumRhs][0]);
                        __mx tmp1 = _mmx_set1_p<T, __mx>(rhs.mData[registerNumRhs][1]);
                        // loop over LHS rows (column registers)
                        for (U32 k = 0; k < mNumRegistersPerCol; ++k)
                        {
                            const U32 registerNumResult = i * mNumRegistersPerCol + k;
                            const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
#ifdef ENABLE_SSE4
                            result.mData[registerNumResult] =
                                    _mmx_fmadd_p(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp0,
                                                 _mmx_fmadd_p(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp1,
                                                              result.mData[registerNumResult]));
#else
                            result.mData[registerNumResult] = _mmx_add_p<T>(

                                    _mmx_add_p<T>(_mmx_mul_p(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp0),
                                                  _mmx_mul_p(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp1)),

                                    result.mData[registerNumResult]);

#endif
                        }
                    }
            }
    }
    return result;
}

#ifndef NDEBUG
template <typename T, int tRows2, int tCols2>
std::ostream& operator<<(std::ostream& os, const GDL::matXSIMD<T, tRows2, tCols2>& mat)
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
