#pragma once


#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <xmmintrin.h>


namespace GDL
{
class Mat4fAVX;
template <typename>
class Mat4Serial;
class Mat4fSSE;
template <typename, bool>
class Vec4Serial;
template <bool>
class Vec4fSSE;


// Missing solver types:
// LL/Cholesky
// LDL
// LU


// Missing solver variants:
// - GaussAVX

// Misc
// - Optimize Cramer Serial the same way as AVX variant

namespace Solver
{

//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> Cramer(const Mat4Serial<_type>& A, const Vec4Serial<_type, true>& b);

//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> Cramer(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs);

#ifdef __AVX2__

//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> Cramer(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs);

#endif // __AVX2__

//! @brief Solves the linear system A * x = b by using Gaussian elimination without pivoting.
//! @tparam _type: Data type
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
//! @tparam _type: Data type
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
