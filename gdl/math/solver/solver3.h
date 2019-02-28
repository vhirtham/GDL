#pragma once


namespace GDL
{

template <bool>
class Vec3fSSE;
class Mat3fSSE;

namespace Solver
{

//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
Vec3fSSE<true> Cramer(const Mat3fSSE& A, const Vec3fSSE<true>& b);

//! @brief Solves the linear system A * x = b by using Gaussian elimination with partial pivoting.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
Vec3fSSE<true> GaussPartialPivot(const Mat3fSSE& A, const Vec3fSSE<true>& b);


} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/solver3.inl"
