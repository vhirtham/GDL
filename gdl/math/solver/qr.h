#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/math/solver/pivotEnum.h"
#include "gdl/math/solver/internal/qrDenseSerial.h"
#include "gdl/math/solver/internal/qrDenseSIMD.h"


#include <array>


namespace GDL
{

template <typename _type, U32, U32>
class MatSerial;
template <typename _type, U32, U32>
class MatSIMD;
template <typename _type, U32, bool>
class VecSerial;
template <typename _type, U32, bool>
class VecSIMD;

namespace Solver
{


// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type, U32 _rows, U32 _cols>
using QRFactorizationSerial = typename QRDenseSerial<_type, _rows, _cols, _pivot>::Factorization;

template <Pivot _pivot, typename _type, U32 _rows, U32 _cols>
using QRFactorizationSIMD =
        typename QRDenseSIMD<typename VecSIMD<_type, _rows, true>::RegisterType, _rows, _cols, _pivot>::Factorization;



// --------------------------------------------------------------------------------------------------------------------

//! @brief Solves the linear system A * x = r using QR decomposition
//! @tparam _type: Data type
//! @tparam _rows: Rows of the matrix
//! @tparam _cols: Columns of the matrix
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @param r: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _rows, U32 _cols>
[[nodiscard]] VecSerial<_type, _rows, true> QR(const MatSerial<_type, _rows, _cols>& A,
                                               const VecSerial<_type, _rows, true>& r);

//! @brief Solves the linear system A * x = r using QR decomposition
//! @tparam _type: Data type
//! @tparam _rows: Rows of the matrix
//! @tparam _cols: Columns of the matrix
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param factorization: Factorization of A
//! @param r: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _rows, U32 _cols>
[[nodiscard]] VecSerial<_type, _rows, true> QR(const QRFactorizationSerial<_pivot, _type, _rows, _cols>& factorization,
                                               const VecSerial<_type, _rows, true>& r);

//! @brief Calculates the QR decomposition of a matrix.
//! @tparam _type: Data type
//! @tparam _rows: Rows of the matrix
//! @tparam _cols: Columns of the matrix
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @return QR decomposition
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _rows, U32 _cols>
[[nodiscard]] QRFactorizationSerial<_pivot, _type, _rows, _cols>
QRFactorization(const MatSerial<_type, _rows, _cols>& A);



//! @brief Solves the linear system A * x = r using QR decomposition
//! @tparam _type: Data type
//! @tparam _rows: Rows of the matrix
//! @tparam _cols: Columns of the matrix
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @param r: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _rows, U32 _cols>
[[nodiscard]] VecSIMD<_type, _rows, true> QR(const MatSIMD<_type, _rows, _cols>& A,
                                             const VecSIMD<_type, _rows, true>& r);

//! @brief Solves the linear system A * x = r using QR decomposition
//! @tparam _type: Data type
//! @tparam _registerType: Register type
//! @tparam _rows: Rows of the matrix
//! @tparam _cols: Columns of the matrix
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param factorization: Factorization of A
//! @param r: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _rows, U32 _cols>
[[nodiscard]] VecSIMD<_type, _rows, true> QR(const QRFactorizationSIMD<_pivot, _type, _rows, _cols>& factorization,
                                             const VecSIMD<_type, _rows, true>& r);

//! @brief Calculates the QR decomposition of a matrix.
//! @tparam _type: Data type
//! @tparam _rows: Rows of the matrix
//! @tparam _cols: Columns of the matrix
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @return QR decomposition
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _rows, U32 _cols>
[[nodiscard]] QRFactorizationSIMD<_pivot, _type, _rows, _cols> QRFactorization(const MatSIMD<_type, _rows, _cols>& A);



} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/qr.inl"
