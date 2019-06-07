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
class Mat4Serial;
class Mat4fSSE;
template <typename, bool>
class Vec4Serial;
template <bool>
class Vec4fSSE;

namespace Solver
{
//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> Cramer(const Mat4fSSE& A, const Vec4fSSE<true>& b);

//! @brief Solves the linear system A * x = b by using Gaussian elimination without pivoting.
//! @tparam: Data type
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> GaussNoPivot(const Mat4Serial<_type>& A, const Vec4Serial<_type, true>& b);

//! @brief Solves the linear system A * x = b by using Gaussian elimination without pivoting.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> GaussNoPivot(const Mat4fSSE& A, const Vec4fSSE<true>& b);

//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @tparam: Data type
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> GaussPartialPivot(const Mat4Serial<_type>& A,
                                                               const Vec4Serial<_type, true>& b);

//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> GaussPartialPivot(const Mat4fSSE& A, const Vec4fSSE<true>& b);



} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/solver4.inl"
