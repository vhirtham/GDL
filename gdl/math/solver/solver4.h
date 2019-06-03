#pragma once


#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <xmmintrin.h>


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



// Support classes ----------------------------------------------------------------------------------------------------

//! @brief Support class for Gauss solver.
class Gauss4SSE
{
    friend Vec4fSSE<true> GaussPartialPivot(const Mat4fSSE&, const Vec4fSSE<true>&);
    friend Vec4fSSE<true> GaussNoPivot(const Mat4fSSE&, const Vec4fSSE<true>&);

    //! @brief Performs the gauss elimination step starting with the specified matrix element on the main diagonal. Rows
    //! above and to the right of the specified element are not modified, since the elemination process is expected to
    //! work from left to right and top to bottom.
    //! @tparam _idx: Index of the element on the main diagonal of the matrix from which the elimination process is
    //! started
    //! @param data: Data of the linear system (matrix columns with appended rhs vector)
    //! @remark Since the current column is not needed in subsequent elimination steps, it will not be modified to
    //! maximize performance. Therefore the matrix is not equal to the identity matrix when all elimination steps are
    //! done.
    template <U32 _idx>
    static inline void EliminationStep(const std::array<__m128* const, 5>& data);
};

} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/solver4.inl"
