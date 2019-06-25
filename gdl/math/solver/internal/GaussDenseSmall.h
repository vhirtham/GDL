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
class Mat3Serial;
template <typename>
class Mat4Serial;
class Mat3fSSE;
class Mat4fSSE;
template <typename, bool>
class Vec3Serial;
template <typename, bool>
class Vec4Serial;
template <bool>
class Vec3fSSE;
template <bool>
class Vec4fSSE;


namespace Solver
{

//! @brief Support class for Gauss solver.
//! @tparam _type: Data type
//! @tparam _size: Size of the linear system
template <typename _type, U32 _size>
class GaussDenseSmallSerial
{
    static_assert(_size == 3 || _size == 4, "Unsupported system size.");

    template <Pivot, typename _type2>
    friend Vec3Serial<_type2, true> Gauss(const Mat3Serial<_type2>& A, const Vec3Serial<_type2, true>& b);
    template <Pivot, typename _type2>
    friend Vec4Serial<_type2, true> Gauss(const Mat4Serial<_type2>& A, const Vec4Serial<_type2, true>& b);


    template <U32 _idx, Pivot _pivot = Pivot::PARTIAL>
    static inline void GaussStep(std::array<_type, _size * _size>& matrixData, std::array<_type, _size>& vectorData);

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
    static inline void EliminationStep(std::array<_type, _size * _size>& matrixData,
                                       std::array<_type, _size>& vectorData);
};



//! @brief Support class for Gauss solver.
//! @tparam _size: Size of the linear system
template <U32 _size>
class GaussDenseSmallSSE
{
    static_assert(_size == 3 || _size == 4, "Unsupported system size.");

    friend Vec4fSSE<true> GaussNoPivot(const Mat4fSSE&, const Vec4fSSE<true>&);
    friend Vec3fSSE<true> GaussPartialPivot(const Mat3fSSE&, const Vec3fSSE<true>&);
    friend Vec4fSSE<true> GaussPartialPivot(const Mat4fSSE&, const Vec4fSSE<true>&);

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
    static inline void EliminationStep(const std::array<__m128* const, _size + 1>& data);
};



//#ifdef __AVX2__

////! @brief Support class for Gauss solver.
////! @tparam _size: Size of the linear system
// class GaussDenseSmallAVX
//{

//    friend Vec4fSSE<true> GaussNoPivot(const Mat4fAVX&, const Vec4fSSE<true>&);
//    friend Vec4fSSE<true> GaussPartialPivot(const Mat4fAVX&, const Vec4fSSE<true>&);

//    //! @brief Performs the gauss elimination step starting with the specified matrix element on the main diagonal.
//    Rows
//    //! above and to the right of the specified element are not modified, since the elemination process is expected to
//    //! work from left to right and top to bottom.
//    //! @tparam _idx: Index of the element on the main diagonal of the matrix from which the elimination process is
//    //! started
//    //! @param data: Data of the linear system (matrix columns with appended rhs vector)
//    //! @remark Since the current column is not needed in subsequent elimination steps, it will not be modified to
//    //! maximize performance. Therefore the matrix is not equal to the identity matrix when all elimination steps are
//    //! done.
//    template <U32 _idx>
//    static inline void EliminationStep(const std::array<__m256* const, 3>& data);
//};

//#endif // __AVX2__



} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/internal/GaussDenseSmall.inl"
