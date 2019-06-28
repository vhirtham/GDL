#pragma once

#include "gdl/math/solver/internal/lltDenseSmall.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/pow.h"
#include "gdl/base/simd/directAccess.h"


// REMOVE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "gdl/math/serial/mat4Serial.h"
#include <iostream>

namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline LLTDenseSmallSerial<_type, _size>::Factorization::Factorization(
        const std::array<_type, _size * _size>& matrixData)
    : mLLT{matrixData}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
[[nodiscard]] inline typename LLTDenseSmallSerial<_type, _size>::Factorization
LLTDenseSmallSerial<_type, _size>::Factorize(const std::array<_type, _size * _size>& matrixData)
{
    Factorization factorization(matrixData);
    FactorizeLLT(factorization);


    //    std::cout << Mat4Serial(matrixData) << std::endl;
    //    std::cout << Mat4Serial(factorization.mLLT) << std::endl;
    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
[[nodiscard]] inline std::array<_type, _size>
LLTDenseSmallSerial<_type, _size>::Solve(const Factorization& factorization, std::array<_type, _size> r)
{

    ForwardSubstitution(factorization.mLLT, r);
    BackwardSubstitution(factorization.mLLT, r);

    return r;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSerial<_type, _size>::BackwardSubstitution(const std::array<_type, _size * _size>& lu,
                                                                    std::array<_type, _size>& r)
{
    constexpr U32 pivIdx = (_size + 1) * _idx;

    r[_idx] /= lu[pivIdx];

    for (U32 i = 0; i < _idx; ++i)
        r[i] -= r[_idx] * lu[i * _size + _idx];

    if constexpr (_idx > 0)
        BackwardSubstitution<_idx - 1>(lu, r);

    //    4x4 system:
    //    r[0] = (r[0] - r[3] * LU[3] - r[2] * LU[8] - r[1] * LU[4]) / LU[0];
    //    r[1] = (r[1] - r[3] * LU[7] - r[2] * LU[9]) / LU[5];
    //    r[2] = (r[2] - r[3] * LU[11]) / LU[10];
    //    r[3] /= LU[15];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSerial<_type, _size>::FactorizationStep(Factorization& factorization)
{
    using namespace GDL::simd;

    constexpr U32 pivIdx = (_size + 1) * _idx;

    DEV_EXCEPTION(factorization.mLLT[pivIdx] == ApproxZero<F32>(1, 10), "Singular matrix - system not solveable");


    for (U32 i = _idx; i < _size; ++i)
        for (U32 j = 0; j < _idx; ++j)
            factorization.mLLT[i + _idx * _size] -=
                    factorization.mLLT[_idx + j * _size] * factorization.mLLT[i + j * _size];

    factorization.mLLT[pivIdx] = sqrt(factorization.mLLT[pivIdx]);

    for (U32 i = pivIdx + 1; i < _size * (_idx + 1); ++i)
        factorization.mLLT[i] /= factorization.mLLT[pivIdx];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSerial<_type, _size>::FactorizeLLT(Factorization& factorization)
{
    FactorizationStep<_idx>(factorization);

    if constexpr (_idx + 1 < _size)
        FactorizeLLT<_idx + 1>(factorization);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSerial<_type, _size>::ForwardSubstitution(const std::array<_type, _size * _size>& lu,
                                                                   std::array<_type, _size>& r)
{

    constexpr U32 pivIdx = (_size + 1) * _idx;

    r[_idx] /= lu[pivIdx];
    for (U32 i = _idx + 1; i < _size; ++i)
        r[i] -= lu[i + _idx * _size] * r[_idx];

    if constexpr (_idx + 1 < _size)
        ForwardSubstitution<_idx + 1>(lu, r);

    //    4x4 system:
    //    r[0] = LU[0] * r[0];
    //    r[1] = (r[1] - LU[1] * r[0]) * LU[5];
    //    r[2] = (r[2] - LU[2] * r[0] - LU[6] * r[1]) * LU[10];
    //    r[3] = (r[3] - LU[3] * r[0] - LU[7] * r[1] - LU[11] * r[2]) * LU[15];
}



} // namespace GDL::Solver
