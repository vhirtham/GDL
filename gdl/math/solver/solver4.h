#pragma once


#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/solver/pivot.h"

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



namespace Solver
{

//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> Cramer(const Mat4Serial<_type>& matA,
                                                    const Vec4Serial<_type, true>& vecRhs);

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
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> GaussNoPivot(const Mat4Serial<_type>& matA,
                                                          const Vec4Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = b by using Gaussian elimination without pivoting.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> GaussNoPivot(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs);

#ifdef __AVX2__

//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> GaussNoPivot(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs);

#endif // __AVX2__


//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> GaussPartialPivot(const Mat4Serial<_type>& matA,
                                                               const Vec4Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> GaussPartialPivot(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs);

//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> GaussPartialPivot(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs);

//! @brief Solves the linear system A * x = b by using LU decomposition without pivoting.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> LU(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = b by using LU decomposition without pivoting.
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> LUNoPivot(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs);

} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/solver4.inl"
