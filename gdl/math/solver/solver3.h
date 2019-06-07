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
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
template <typename _type>
[[nodiscard]] inline Vec3Serial<_type, true> Cramer(const Mat3Serial<_type>& A, const Vec3Serial<_type, true>& b);

//! @brief Solves the linear system A * x = b by using Cramers rule.
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
[[nodiscard]] inline Vec3fSSE<true> Cramer(const Mat3fSSE& A, const Vec3fSSE<true>& b);

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



// Support classes ----------------------------------------------------------------------------------------------------



//! @brief Support class for Gauss solver.
//! @tparam: Data type
template <typename _type>
class Gauss3Serial
{
    template <typename _type2>
    friend Vec3Serial<_type2, true> GaussPartialPivot(const Mat3Serial<_type2>&, const Vec3Serial<_type2, true>&);

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
    static inline void EliminationStep(std::array<_type, 9>& matrixData, std::array<_type, 3>& vectorData);
};



// --------------------------------------------------------------------------------------------------------------------

//! @brief Support class for Gauss solver.
class Gauss3SSE
{
    friend Vec3fSSE<true> GaussPartialPivot(const Mat3fSSE&, const Vec3fSSE<true>&);

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
    static inline void EliminationStep(const std::array<__m128* const, 4>& data);
};


} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/solver3.inl"
