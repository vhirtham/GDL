#pragma once


namespace GDL
{

template <bool>
class Vec4fSSE;
class Mat4fSSE;

namespace Solver
{
//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
Vec4fSSE<true> Cramer(const Mat4fSSE& A, const Vec4fSSE<true>& b);

//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
Vec4fSSE<true> GaussPartialPivot(const Mat4fSSE& A, const Vec4fSSE<true>& b);

//! @brief Solves the linear system A * x = b by using Gaussian elimination without pivoting.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
Vec4fSSE<true> GaussNoPivot(const Mat4fSSE& A, const Vec4fSSE<true>& b);


} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/solver4.inl"
