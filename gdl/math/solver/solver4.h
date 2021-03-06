#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"
#include "gdl/math/solver/pivotEnum.h"
#include "gdl/math/solver/internal/ldltDenseSmall.h"
#include "gdl/math/solver/internal/lltDenseSmall.h"
#include "gdl/math/solver/internal/luDenseSmall.h"

#include <array>


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
// Full factorizationS



namespace Solver
{

// --------------------------------------------------------------------------------------------------------------------
// Cramers Rule
// --------------------------------------------------------------------------------------------------------------------

//! @brief Solves the linear system A * x = r by using Cramers rule.
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> Cramer(const Mat4Serial<_type>& matA,
                                                    const Vec4Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = r by using Cramers rule.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> Cramer(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs);

#ifdef __AVX2__

//! @brief Solves the linear system A * x = r by using Cramers rule.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> Cramer(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs);

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------
// Gaussian Elimination
// --------------------------------------------------------------------------------------------------------------------

//! @brief Solves the linear system A * x = r by using Gaussian elimination with partial pivoting.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> Gauss(const Mat4Serial<_type>& matA,
                                                   const Vec4Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = r by using Gaussian elimination with partial pivoting.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline Vec4fSSE<true> Gauss(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs);

#ifdef __AVX2__

//! @brief Solves the linear system A * x = r by using Gaussian elimination with partial pivoting.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline Vec4fSSE<true> Gauss(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs);


#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------
// LDLT decomposition
// --------------------------------------------------------------------------------------------------------------------

//! @brief Solves the linear system A * x = r by using the Cholesky LDLT decomposition.
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> LDLT(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = r by using the Cholesky LDLT decomposition.
//! @tparam _type: Data type
//! @param factorization: Factorization of A
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true>
LDLT(const typename LDLTDenseSmallSerial<_type, 4>::Factorization& factorization,
     const Vec4Serial<_type, true>& vecRhs);

//! @brief Calculates the Cholesky LDLT decomposition of a matrix.
//! @tparam _type: Data type
//! @param matA: Matrix
//! @return LDLT factorization of the matrix
template <typename _type>
[[nodiscard]] inline typename LDLTDenseSmallSerial<_type, 4>::Factorization
LDLTFactorization(const Mat4Serial<_type>& matA);

//! @brief Solves the linear system A * x = r by using the Cholesky LDLT decomposition.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> LDLT(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs);

//! @brief Solves the linear system A * x = r by using the Cholesky LDLT decomposition.
//! @param factorization: Factorization of A
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> LDLT(const typename LDLTDenseSmallSSE<4>::Factorization& factorization,
                                         const Vec4fSSE<true>& vecRhs);

//! @brief Calculates the Cholesky LDLT decomposition of a matrix.
//! @param matA: Matrix
//! @return LDLT factorization of the matrix
[[nodiscard]] inline typename LDLTDenseSmallSSE<4>::Factorization LDLTFactorization(const Mat4fSSE& matA);

#ifdef __AVX2__

//! @brief Solves the linear system A * x = r by using the Cholesky LDLT decomposition.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> LDLT(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs);

//! @brief Calculates the Cholesky LDLT decomposition of a matrix.
//! @param matA: Matrix
//! @return LDLT factorization of the matrix
[[nodiscard]] inline typename LDLTDenseSmallSSE<4>::Factorization LDLTFactorization(const Mat4fAVX& matA);

#endif //  __AVX2__



// --------------------------------------------------------------------------------------------------------------------
// LLT decomposition
// --------------------------------------------------------------------------------------------------------------------

//! @brief Solves the linear system A * x = r by using the Cholesky LLT decomposition.
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> LLT(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = r by using the Cholesky LLT decomposition.
//! @tparam _type: Data type
//! @param factorization: Factorization of A
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true>
LLT(const typename LLTDenseSmallSerial<_type, 4>::Factorization& factorization, const Vec4Serial<_type, true>& vecRhs);

//! @brief Calculates the Cholesky LLT decomposition of a matrix.
//! @tparam _type: Data type
//! @param matA: Matrix
//! @return LLT factorization of the matrix
template <typename _type>
[[nodiscard]] inline typename LLTDenseSmallSerial<_type, 4>::Factorization
LLTFactorization(const Mat4Serial<_type>& matA);

//! @brief Solves the linear system A * x = r by using the Cholesky LLT decomposition.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> LLT(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs);

//! @brief Solves the linear system A * x = r by using the Cholesky LLT decomposition.
//! @param factorization: Factorization of A
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> LLT(const typename LLTDenseSmallSSE<4>::Factorization& factorization,
                                        const Vec4fSSE<true>& vecRhs);

//! @brief Calculates the Cholesky LLT decomposition of a matrix.
//! @param matA: Matrix
//! @return LLT factorization of the matrix
[[nodiscard]] inline typename LLTDenseSmallSSE<4>::Factorization LLTFactorization(const Mat4fSSE& matA);

#ifdef __AVX2__

//! @brief Solves the linear system A * x = r by using the Cholesky LLT decomposition.
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
[[nodiscard]] inline Vec4fSSE<true> LLT(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs);

//! @brief Calculates the Cholesky LLT decomposition of a matrix.
//! @param matA: Matrix
//! @return LLT factorization of the matrix
[[nodiscard]] inline typename LLTDenseSmallSSE<4>::Factorization LLTFactorization(const Mat4fAVX& matA);

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------
// LU decomposition
// --------------------------------------------------------------------------------------------------------------------

//! @brief Solves the linear system A * x = r by using LU decomposition.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam _type: Data type
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> LU(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs);

//! @brief Solves the linear system A * x = r by using LU decomposition.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam _type: Data type
//! @param factorization: Factorization of A
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type>
[[nodiscard]] inline Vec4Serial<_type, true>
LU(const typename LUDenseSmallSerial<_type, 4, _pivot>::Factorization& factorization,
   const Vec4Serial<_type, true>& vecRhs);

//! @brief Calculates the LU decomposition of a matrix.
//! @tparam _pivot: Enum to select pivoting strategy
//! @tparam _type: Data type
//! @param matA: Matrix
//! @return LU factorization of the matrix
template <Pivot _pivot = Pivot::PARTIAL, typename _type>
[[nodiscard]] inline typename LUDenseSmallSerial<_type, 4, _pivot>::Factorization
LUFactorization(const Mat4Serial<_type>& matA);

//! @brief Solves the linear system A * x = r by using LU decomposition.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline Vec4fSSE<true> LU(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs);


//! @brief Solves the linear system A * x = r by using LU decomposition.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param factorization: Factorization of A
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline Vec4fSSE<true> LU(const typename LUDenseSmallSSE<4, _pivot>::Factorization& factorization,
                                       const Vec4fSSE<true>& vecRhs);

//! @brief Calculates the LU decomposition of a matrix.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param matA: Matrix
//! @return LU factorization of the matrix
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline typename LUDenseSmallSSE<4, _pivot>::Factorization LUFactorization(const Mat4fSSE& matA);

#ifdef __AVX2__

//! @brief Solves the linear system A * x = r by using LU decomposition.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param matA: Matrix
//! @param vecRhs: Right-hand side vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline Vec4fSSE<true> LU(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs);

//! @brief Calculates the LU decomposition of a matrix.
//! @tparam _pivot: Enum to select pivoting strategy
//! @param matA: Matrix
//! @return LU factorization of the matrix
template <Pivot _pivot = Pivot::PARTIAL>
[[nodiscard]] inline typename LUDenseSmallSSE<4, _pivot>::Factorization LUFactorization(const Mat4fAVX& matA);

#endif // __AVX2__

} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/solver4.inl"
