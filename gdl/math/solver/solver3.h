#pragma once

#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <xmmintrin.h>

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

//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @tparam: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec3Serial<_type, true> Cramer(const Mat3Serial<_type>& matA,
                                                    const Vec3Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec3fSSE<true> Cramer(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs);

//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @tparam: Data type
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec3Serial<_type, true> GaussPartialPivot(const Mat3Serial<_type>& A,
                                                               const Vec3Serial<_type, true>& b);

//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
[[nodiscard]] inline Vec3fSSE<true> GaussPartialPivot(const Mat3fSSE& A, const Vec3fSSE<true>& b);



} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/solver3.inl"
