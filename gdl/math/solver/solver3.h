#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/x86intrin.h"
#include "gdl/math/solver/pivot.h"
#include "gdl/math/solver/internal/luDenseSmall.h"

#include <array>

namespace GDL
{

template <typename>
class Mat3Serial;
class Mat3fSSE;
template <typename, bool>
class Vec3Serial;
template <bool>
class Vec3fSSE;

namespace Solver
{

//! @brief Solves the linear system A * x = r by using Cramers rule.
//! @tparam: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec3Serial<_type, true> Cramer(const Mat3Serial<_type>& matA,
                                                    const Vec3Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = r by using Cramers rule.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec3fSSE<true> Cramer(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs);

//! @brief Solves the linear system A * x = r by using Gaussian elimination with partial pivoting.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type>
[[nodiscard]] inline Vec3Serial<_type, true> Gauss(const Mat3Serial<_type>& matA,
                                                   const Vec3Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = r by using Gaussian elimination with partial pivoting.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline Vec3fSSE<true> Gauss(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs);

//! @brief Solves the linear system A * x = r by using LU decomposition.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type>
[[nodiscard]] inline Vec3Serial<_type, true> LU(const Mat3Serial<_type>& matA, const Vec3Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = r by using LU decomposition.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam _type: Data type
//! @param factorization: Factorization of A
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type>
[[nodiscard]] inline Vec3Serial<_type, true>
LU(const typename LUDenseSmallSerial<_type, 3, _pivot>::Factorization& factorization,
   const Vec3Serial<_type, true>& vecRhs);

//! @brief Calculates the LU decomposition of a matrix.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam _type: Data type
//! @param matA: Matrix
//! @return LU factorization of the matrix
template <Pivot _pivot = Pivot::PARTIAL, typename _type>
[[nodiscard]] inline typename LUDenseSmallSerial<_type, 3, _pivot>::Factorization
LUFactorization(const Mat3Serial<_type>& matA);

//! @brief Solves the linear system A * x = r by using LU decomposition.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline Vec3fSSE<true> LU(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs);


//! @brief Solves the linear system A * x = r by using LU decomposition.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param factorization: Factorization of A
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline Vec3fSSE<true> LU(const typename LUDenseSmallSSE<3, _pivot>::Factorization& factorization,
                                       const Vec3fSSE<true>& vecRhs);

//! @brief Calculates the LU decomposition of a matrix.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param matA: Matrix
//! @return LU factorization of the matrix
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline typename LUDenseSmallSSE<3, _pivot>::Factorization LUFactorization(const Mat3fSSE& matA);



} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/solver3.inl"
