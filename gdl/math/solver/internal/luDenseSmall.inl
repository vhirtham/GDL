#pragma once

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/math/solver/internal/luDenseSmall.h"
#include "gdl/base/sse/swizzle.h"


#include <iostream>

namespace GDL::Solver
{

template <typename _type, U32 _size, Pivot _pivot>
inline LUDenseSmallSerial<_type, _size, _pivot>::Factorization::Factorization(
        const std::array<_type, _size * _size>& lu)
    : mLU{lu}
    , mPermutation{InitializePermutation()}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline std::array<_type, _size> LUDenseSmallSerial<_type, _size, _pivot>::Factorization::InitializePermutation()
{
    static_assert(_size == 3 || _size == 4, "System size not supported");
    if constexpr (_size == 3)
        return {{0, 1, 2}};
    if constexpr (_size == 4)
        return {{0, 1, 2, 3}};
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size, _pivot>::BackwardSubstitution(const std::array<_type, _size * _size>& lu,
                                                                           std::array<_type, _size>& r)
{
    constexpr U32 pivIdx = (_size + 1) * _idx;

    r[_idx] /= lu[pivIdx];

    for (U32 i = 0; i < _idx; ++i)
        r[i] -= r[_idx] * lu[i + _idx * _size];

    if constexpr (_idx > 0)
        BackwardSubstitution<_idx - 1>(lu, r);

    //    4x4 system:
    //    r[3] /= LU[15];
    //    r[2] = (r[2] - r[3] * LU[14]) / LU[10];
    //    r[1] = (r[1] - r[3] * LU[13] - r[2] * LU[9]) / LU[5];
    //    r[0] = (r[0] - r[3] * LU[12] - r[2] * LU[8] - r[1] * LU[4]) / LU[0];


    // Alternative version:

    //    for (U32 i = 0; i < _idx; ++i)
    //        r[i] -= r[_idx] * lu[i + _idx * _size];

    //    if constexpr (_idx > 1)
    //        BackwardSubstitution<_idx - 1>(lu, r);

    //    //    4x4 system:
    //    //    r[2] = (r[2] - r[3] * LU[14]);
    //    //    r[1] = (r[1] - r[3] * LU[13] - r[2] * LU[9]);
    //    //    r[0] = (r[0] - r[3] * LU[12] - r[2] * LU[8] - r[1] * LU[4]);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline typename LUDenseSmallSerial<_type, _size, _pivot>::Factorization
LUDenseSmallSerial<_type, _size, _pivot>::Factorize(const std::array<_type, _size * _size>& matrixData)
{
    Factorization fac(matrixData);
    FactorizeLU(fac);

    return fac;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size, _pivot>::FactorizeLU(Factorization& factorization)
{
    FactorizationStep<_idx>(factorization);

    if constexpr (_idx + 1 < _size)
        FactorizeLU<_idx + 1>(factorization);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size, _pivot>::FactorizationStep(Factorization& factorization)
{
    constexpr U32 pivIdx = _idx * _size + _idx;
    DEV_EXCEPTION(factorization.mLU[pivIdx] == ApproxZero<_type>(1, 10), "Singular matrix - system not solveable");


    for (U32 i = pivIdx + 1; i < _size * (_idx + 1); ++i)
    {
        factorization.mLU[i] /= factorization.mLU[pivIdx];
        for (U32 j = _size; j < (_size - _idx) * _size; j += _size)
            factorization.mLU[i + j] -= factorization.mLU[i] * factorization.mLU[pivIdx + j];
    }


    // Alternative version:

    //    constexpr U32 pivIdx = _idx * _size + _idx;
    //    DEV_EXCEPTION(lu[pivIdx] == ApproxZero<_type>(1, 10), "Singular matrix - system not solveable");


    //    lu[pivIdx] = 1 / lu[pivIdx];


    //    for (U32 i = pivIdx + _size; i < _size * _size; i += _size)
    //        lu[i] *= lu[pivIdx];

    //    for (U32 i = pivIdx + 1; i < _size * (_idx + 1); ++i)
    //        for (U32 j = _size; j < (_size - _idx) * _size; j += _size)
    //            lu[i + j] -= lu[i] * lu[pivIdx + j];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size, _pivot>::ForwardSubstitution(const std::array<_type, _size * _size>& lu,
                                                                          std::array<_type, _size>& r)
{
    for (U32 i = _idx + 1; i < _size; ++i)
        r[i] -= lu[i + _idx * _size] * r[_idx];

    if constexpr (_idx + 2 < _size)
        ForwardSubstitution<_idx + 1>(lu, r);

    //    4x4 system:
    //    r[1] -= LU[1] * r[0];
    //    r[2] -= LU[2] * r[0] + LU[6] * r[1];
    //    r[3] -= LU[3] * r[0] + LU[7] * r[1] + LU[11] * r[2];



    // Alternative version:

    //    constexpr U32 pivIdx = (_size + 1) * _idx;

    //    r[_idx] *= lu[pivIdx];
    //    for (U32 i = _idx + 1; i < _size; ++i)
    //        r[i] -= lu[i + _idx * _size] * r[_idx];

    //    if constexpr (_idx + 1 < _size)
    //        ForwardSubstitution<_idx + 1>(lu, r);

    //    //    4x4 system:
    //    //    r[0] = LU[0] * r[0];
    //    //    r[1] = (r[1] - LU[1] * r[0]) * LU[5];
    //    //    r[2] = (r[2] - LU[2] * r[0] - LU[6] * r[1]) * LU[10];
    //    //    r[3] = (r[3] - LU[3] * r[0] - LU[7] * r[1] - LU[11] * r[2]) * LU[15];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline std::array<_type, _size> LUDenseSmallSerial<_type, _size, _pivot>::Solve(const Factorization& factorization,
                                                                                std::array<_type, _size> r)
{
    ForwardSubstitution(factorization.mLU, r);
    BackwardSubstitution(factorization.mLU, r);
    return r;
}


} // namespace GDL::Solver
