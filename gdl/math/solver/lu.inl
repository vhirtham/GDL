#pragma once

#include "gdl/math/solver/lu.h"

#include "gdl/base/approx.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/swizzle.h"
#include "gdl/math/serial/matSerial.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/solver/pivot.h"


//#include <cmath>

namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _size>
VecSerial<_type, _size, true> LU(const MatSerial<_type, _size, _size>& A, const VecSerial<_type, _size, true>& r)
{
    auto factorization = LUFactorization(A);
    return LU(factorization, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _size>
[[nodiscard]] VecSerial<_type, _size, true> LU(const LUFactorizationSerial<_pivot, _type, _size>& factorization,
                                               const VecSerial<_type, _size, true>& r)
{
    using LUSolver = LUDenseSerial<_type, _size, _pivot>;

    return VecSerial<_type, _size, true>(LUSolver::Solve(factorization, r.Data()));
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _size>
LUFactorizationSerial<_pivot, _type, _size> LUFactorization(const MatSerial<_type, _size, _size>& A)
{
    using LUSolver = LUDenseSerial<_type, _size, _pivot>;

    return LUSolver::Factorize(A.Data());
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _size>
VecSIMD<_type, _size, true> LU(const MatSIMD<_type, _size, _size>& A, const VecSIMD<_type, _size, true>& r)
{
    auto factorization = LUFactorization(A);
    return LU(factorization, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _size>
[[nodiscard]] VecSIMD<_type, _size, true> LU(const LUFactorizationSIMD<_pivot, _type, _size>& factorization,
                                             const VecSIMD<_type, _size, true>& r)
{
    using RegisterType = typename MatSIMD<_type, _size, _size>::RegisterType;
    using LUSolver = LUDenseSIMD<RegisterType, _size, _pivot>;

    return VecSIMD<_type, _size, true>(LUSolver::Solve(factorization, r.DataSSE()));
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _size>
LUFactorizationSIMD<_pivot, _type, _size> LUFactorization(const MatSIMD<_type, _size, _size>& A)
{
    using RegisterType = typename MatSIMD<_type, _size, _size>::RegisterType;
    using LUSolver = LUDenseSIMD<RegisterType, _size, _pivot>;

    return LUSolver::Factorize(A.DataSSE());
}

} // namespace GDL::Solver
