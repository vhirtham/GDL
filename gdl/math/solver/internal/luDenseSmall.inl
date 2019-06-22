#pragma once

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/math/solver/internal/luDenseSmall.h"
#include "gdl/base/sse/swizzle.h"


#include <iostream>

namespace GDL::Solver
{


// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size>::BackwardSubstitution(const std::array<_type, _size * _size>& lu,
                                                                   std::array<_type, _size>& r)
{
    constexpr U32 diagIdx = (_size + 1) * _idx;

    r[_idx] /= lu[diagIdx];

    for (U32 i = 0; i < _idx; ++i)
        r[i] -= r[_idx] * lu[i + _idx * _size];

    if constexpr (_idx != 0)
        BackwardSubstitution<_idx - 1>(lu, r);

    //    4x4 system:
    //    r[3] /= LU[15];
    //    r[2] = (r[2] - r[3] * LU[14]) / LU[10];
    //    r[1] = (r[1] - r[3] * LU[13] - r[2] * LU[9]) / LU[5];
    //    r[0] = (r[0] - r[3] * LU[12] - r[2] * LU[8] - r[1] * LU[4]) / LU[0];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline std::array<_type, _size * _size>
LUDenseSmallSerial<_type, _size>::Factorize(const std::array<_type, _size * _size>& matrixData)
{
    std::array<_type, _size* _size> lu = matrixData;
    FactorizationStep<0>(lu);
    FactorizationStep<1>(lu);
    FactorizationStep<2>(lu);

    DEV_EXCEPTION(lu[_size * _size - 1] == ApproxZero<_type>(1, 10), "Singular matrix - system not solveable");

    return lu;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size>::FactorizationStep(std::array<_type, _size * _size>& lu)
{
    constexpr U32 pivIdx = _idx * _size + _idx;
    DEV_EXCEPTION(lu[pivIdx] == ApproxZero<_type>(1, 10), "Singular matrix - system not solveable");


    for (U32 i = pivIdx + 1; i < _size * (_idx + 1); ++i)
    {
        lu[i] /= lu[pivIdx];
        for (U32 j = _size; j < (_size - _idx) * _size; j += _size)
            lu[i + j] -= lu[i] * lu[pivIdx + j];
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size>::ForwardSubstitution(const std::array<_type, _size * _size>& lu,
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
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline std::array<_type, _size> LUDenseSmallSerial<_type, _size>::Solve(const std::array<_type, _size * _size>& lu,
                                                                        std::array<_type, _size> r)
{
    ForwardSubstitution(lu, r);
    BackwardSubstitution(lu, r);
    return r;
}


} // namespace GDL::Solver
