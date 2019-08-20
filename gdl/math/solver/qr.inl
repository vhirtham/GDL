#pragma once

#include "gdl/math/solver/qr.h"

#include "gdl/math/serial/matSerial.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/simd/vecSIMD.h"


//#include <cmath>

namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _rows, U32 _cols>
VecSerial<_type, _rows, true> QR(const MatSerial<_type, _rows, _cols>& A, const VecSerial<_type, _rows, true>& r)
{
    auto factorization = QRFactorization<_pivot, _type, _rows, _cols>(A);
    return QR<_pivot, _type, _rows, _cols>(factorization, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _rows, U32 _cols>
[[nodiscard]] VecSerial<_type, _rows, true> QR(const QRFactorizationSerial<_pivot, _type, _rows, _cols>& factorization,
                                               const VecSerial<_type, _rows, true>& r)
{
    using QRSolver = QRDenseSerial<_type, _rows, _cols, _pivot>;

    return VecSerial<_type, _rows, true>(QRSolver::Solve(factorization, r.Data()));
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _rows, U32 _cols>
QRFactorizationSerial<_pivot, _type, _rows, _cols> QRFactorization(const MatSerial<_type, _rows, _cols>& A)
{
    using QRSolver = QRDenseSerial<_type, _rows, _cols, _pivot>;

    return QRSolver::Factorize(A.Data());
}



// --------------------------------------------------------------------------------------------------------------------

// template <Pivot _pivot, typename _type, U32 _size>
// VecSIMD<_type, _size, true> QR(const MatSIMD<_type, _size, _size>& A, const VecSIMD<_type, _size, true>& r)
//{
//    auto factorization = QRFactorization<_pivot, _type, _size>(A);
//    return QR<_pivot, _type, _size>(factorization, r);
//}



// --------------------------------------------------------------------------------------------------------------------

// template <Pivot _pivot, typename _type, U32 _size>
//[[nodiscard]] VecSIMD<_type, _size, true> QR(const QRFactorizationSIMD<_pivot, _type, _size>& factorization,
//                                             const VecSIMD<_type, _size, true>& r)
//{
//    using RegisterType = typename MatSIMD<_type, _size, _size>::RegisterType;
//    using QRSolver = QRDenseSIMD<RegisterType, _size, _pivot>;

//    return VecSIMD<_type, _size, true>(QRSolver::Solve(factorization, r.DataSSE()));
//}



// --------------------------------------------------------------------------------------------------------------------

// template <Pivot _pivot, typename _type, U32 _size>
// QRFactorizationSIMD<_pivot, _type, _size> QRFactorization(const MatSIMD<_type, _size, _size>& A)
//{
//    using RegisterType = typename MatSIMD<_type, _size, _size>::RegisterType;
//    using QRSolver = QRDenseSIMD<RegisterType, _size, _pivot>;

//    return QRSolver::Factorize(A.DataSSE());
//}

} // namespace GDL::Solver
