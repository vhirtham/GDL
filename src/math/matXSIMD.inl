#pragma once

#include "matXSIMD.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#ifndef NDEBUG
#include <iostream>
#endif

template <int tRows, int tCols>
GDL::matXSIMD<tRows, tCols>::matXSIMD()
{
    ConstructionChecks();
}


template <int tRows, int tCols>
template <typename... Args>
GDL::matXSIMD<tRows, tCols>::matXSIMD(Args... args)
{
    assert(sizeof...(args) == tRows * tCols);
    *this = matXSIMD(std::array<F32, tRows * tCols>{{static_cast<F32>(args)...}});
}

template <int tRows, int tCols>
GDL::matXSIMD<tRows, tCols>::matXSIMD(const std::array<F32, tRows * tCols>& data)
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
            std::memcpy(&mData[i * mNumRegistersPerCol], &data[i * tRows], sizeof(F32) * tRows);
        }
    }

    ConstructionChecks();
}

template <int tRows, int tCols>
GDL::matXSIMD<tRows, tCols>& GDL::matXSIMD<tRows, tCols>::operator+=(const GDL::matXSIMD<tRows, tCols>& rhs)
{
    for (U32 i = 0; i < mData.size(); ++i)
        mData[i] = _mmx_add_ps(rhs.mData[i], mData[i]);
    return *this;
}

template <int tRows, int tCols>
GDL::F32 GDL::matXSIMD<tRows, tCols>::operator()(const GDL::U32 row, const GDL::U32 col) const
{
    assert(row < tRows && col < tCols);
    return mData[row / mNumRegisterEntries + col * mNumRegistersPerCol][row % mNumRegisterEntries];
}

template <int tRows, int tCols>
GDL::U32 GDL::matXSIMD<tRows, tCols>::Rows() const
{
    return tRows;
}


template <int tRows, int tCols>
GDL::U32 GDL::matXSIMD<tRows, tCols>::Cols() const
{
    return tCols;
}

template <int tRows, int tCols>
void GDL::matXSIMD<tRows, tCols>::SetZero()
{
    std::fill(mData.begin(), mData.end(), _mm_set1_ps(0.));
}



template <int tRows, int tCols>
const std::array<GDL::F32, tRows * tCols> GDL::matXSIMD<tRows, tCols>::Data() const
{
    std::array<GDL::F32, tRows * tCols> data;
    if (tRows % mNumRegisterEntries == 0)
    {
        assert(sizeof(mData) == sizeof(data));
        std::memcpy(&data, &mData, sizeof(data));
    }
    else
    {
        for (U32 i = 0; i < tCols; ++i)
        {
            std::memcpy(&data[i * tRows], &mData[i * mNumRegistersPerCol], sizeof(F32) * tRows);
        }
    }
    return data;
}

template <int tRows, int tCols>
void GDL::matXSIMD<tRows, tCols>::ConstructionChecks() const
{
    assert(sizeof(mData) == sizeof(__mx) * mData.size()); // Array needs to be compact
#ifndef NDEBUG
    for (const auto& iRegister : mData)
        if (!is_aligned(&iRegister, 16))
            throw Exception(__PRETTY_FUNCTION__, "One or more registers of mat4SIMD are not 16 byte aligned");
#endif
}


template <int tRows, int tCols>
template <int tRowsRhs, int tColsRhs>
GDL::matXSIMD<tRows, tColsRhs> GDL::matXSIMD<tRows, tCols>::operator*(const matXSIMD<tRowsRhs, tColsRhs>& rhs) const
{
    static_assert(tCols == tRowsRhs, "Lhs cols != Rhs rows - Matrix multiplication not possible!");

    constexpr U32 registersPerColRhs = CalcMinNumArrayRegisters(tRowsRhs, mNumRegisterEntries);

    matXSIMD<tRows, tColsRhs> result;
    result.SetZero();
    // loop over RHS rows (column registers)
    for (U32 j = 0; j < registersPerColRhs; ++j)
        // loop over every RHS Col
        for (U32 i = 0; i < tColsRhs; ++i)
        {
            const U32 registerNumRhs = i * registersPerColRhs + j;
            // those temporaries seem to have no effect on performance but they help to keep the code a little bit less
            // messy. Also check if there is a faster way to extract single values into a new register
            __mx tmp0 = _mmx_set1_ps<__mx>(rhs.mData[registerNumRhs][0]);
            __mx tmp1 = _mmx_set1_ps<__mx>(rhs.mData[registerNumRhs][1]);
            __mx tmp2 = _mmx_set1_ps<__mx>(rhs.mData[registerNumRhs][2]);
            __mx tmp3 = _mmx_set1_ps<__mx>(rhs.mData[registerNumRhs][3]);
            // loop over LHS rows (column registers)
            for (U32 k = 0; k < mNumRegistersPerCol; ++k)
            {
                const U32 registerNumResult = i * mNumRegistersPerCol + k;
                const U32 currentBlockLhs = j * mNumRegisterEntries * mNumRegistersPerCol + k;
#ifdef ENABLE_SSE4
                result.mData[registerNumResult] = _mmx_fmadd_ps(
                        mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp0,
                        _mmx_fmadd_ps(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp1,
                                      _mmx_fmadd_ps(mData[currentBlockLhs + 2 * mNumRegistersPerCol], tmp2,
                                                    _mmx_fmadd_ps(mData[currentBlockLhs + 3 * mNumRegistersPerCol], tmp3,
                                                                  result.mData[registerNumResult]))));
#else
                result.mData[registerNumResult] = _mmx_add_ps(
                        _mmx_add_ps(_mmx_add_ps(_mmx_mul_ps(mData[currentBlockLhs + 0 * mNumRegistersPerCol], tmp0),
                                                _mmx_mul_ps(mData[currentBlockLhs + 1 * mNumRegistersPerCol], tmp1)),
                                    _mmx_add_ps(_mmx_mul_ps(mData[currentBlockLhs + 2 * mNumRegistersPerCol], tmp2),
                                                _mmx_mul_ps(mData[currentBlockLhs + 3 * mNumRegistersPerCol], tmp3))),
                        result.mData[registerNumResult]);

#endif
            }
        }

    return result;
}

#ifndef NDEBUG
template <int tRows2, int tCols2>
std::ostream& operator<<(std::ostream& os, const GDL::matXSIMD<tRows2, tCols2>& mat)
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
