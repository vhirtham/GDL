#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/math/solver/pivotEnum.h"
#include "gdl/math/solver/internal/luDenseSerial.h"
#include "gdl/math/solver/internal/luDenseSIMD.h"

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

template <Pivot _pivot, typename _type, U32 _size>
using LUFactorizationSerial = typename LUDenseSerial<_type, _size, _pivot>::Factorization;

template <Pivot _pivot, typename _type, U32 _size>
using LUFactorizationSIMD =
        typename LUDenseSIMD<typename VecSIMD<_type, _size, true>::RegisterType, _size, _pivot>::Factorization;



// --------------------------------------------------------------------------------------------------------------------

//! @brief Solves the linear system A * x = r using LU decomposition
//! @tparam _type: Data type
//! @tparam _size: Size of the system
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @param r: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
[[nodiscard]] VecSerial<_type, _size, true> LU(const MatSerial<_type, _size, _size>& A,
                                               const VecSerial<_type, _size, true>& r);

//! @brief Solves the linear system A * x = r using LU decomposition
//! @tparam _type: Data type
//! @tparam _size: Size of the system
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param factorization: Factorization of A
//! @param r: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
[[nodiscard]] VecSerial<_type, _size, true> LU(const LUFactorizationSerial<_pivot, _type, _size>& factorization,
                                               const VecSerial<_type, _size, true>& r);

//! @brief Calculates the LU decomposition of a matrix.
//! @tparam _type: Data type
//! @tparam _size: Size of the system
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @return LU decomposition
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
[[nodiscard]] LUFactorizationSerial<_pivot, _type, _size> LUFactorization(const MatSerial<_type, _size, _size>& A);



//! @brief Solves the linear system A * x = r using LU decomposition
//! @tparam _type: Data type
//! @tparam _size: Size of the system
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @param r: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
[[nodiscard]] VecSIMD<_type, _size, true> LU(const MatSIMD<_type, _size, _size>& A,
                                             const VecSIMD<_type, _size, true>& r);

//! @brief Solves the linear system A * x = r using LU decomposition
//! @tparam _type: Data type
//! @tparam _registerType: Register type
//! @tparam _size: Size of the system
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param factorization: Factorization of A
//! @param r: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
[[nodiscard]] VecSIMD<_type, _size, true> LU(const LUFactorizationSIMD<_pivot, _type, _size>& factorization,
                                             const VecSIMD<_type, _size, true>& r);

//! @brief Calculates the LU decomposition of a matrix.
//! @tparam _type: Data type
//! @tparam _size: Size of the system
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @return LU decomposition
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
[[nodiscard]] LUFactorizationSIMD<_pivot, _type, _size> LUFactorization(const MatSIMD<_type, _size, _size>& A);



} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/lu.inl"
