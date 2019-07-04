#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>

namespace GDL
{


namespace Solver
{


//! @brief Helper class for pivoting strategies that are shared amongst multiple solvers
//! @tparam _type: Data type of the system
//! @tparam _size: Size of the system
template <typename _type, U32 _size>
class PivotDenseSmallSerial
{

    template <typename, U32, Pivot>
    friend class LUDenseSmallSerial;
    template <typename, U32, Pivot>
    friend class GaussDenseSmallSerial;


    //! @brief Performs partial pivoting on a matrix if necessary
    //! @tparam _idx: Index of the active row
    //! @tparam _idxColStart: Index of the first column that should be modified during pivoting.
    //! @param matrixData: Data of the matrix (column major)
    //! @param vectorData: Vector wich is permuted the same way as the matrix
    template <U32 _idx, U32 _idxColStart = 0>
    static inline void Partial(std::array<_type, _size * _size>& matrixData, std::array<_type, _size>& vectorData);

    //! @brief Applies a permutation to a vector
    //! @param vec: Original vector
    //! @param permutation: Permutation data
    //! @return Permuted vector
    static inline std::array<_type, _size> PermuteVector(const std::array<_type, _size>& vec,
                                                         const std::array<_type, _size>& permutation);

    //! @brief Performs a pivoting step on a matrix with the selected pivoting strategy
    //! @tparam _pivot: Enum to select pivoting strategy
    //! @tparam _idx: Index of the active row
    //! @tparam _idxColStart: Index of the first column that should be modified during pivoting.
    //! @param matrixData: Data of the matrix (column major)
    //! @param vectorData: Vector wich is permuted the same way as the matrix
    template <Pivot _pivot, U32 _idx, U32 _idxColStart = 0>
    static inline void PivotStep(std::array<_type, _size * _size>& matrixData, std::array<_type, _size>& vectorData);
};



//! @brief Helper class for pivoting strategies that are shared amongst multiple solvers
//! @tparam _size: Size of the system
template <U32 _size>
class PivotDenseSmallSSE
{
    template <U32, Pivot>
    friend class LUDenseSmallSSE;
    template <U32, Pivot>
    friend class GaussDenseSmallSSE;

    //! @brief Creates the permutation hash for the given permutation
    //! @param permutation: Register that contains an arbitrary permutation of 0,1,2,3
    //! @return Permutation hash
    static inline U32 CreatePermutationHash(__m128 permutation);

    //! @brief Performs partial pivoting on a matrix if necessary
    //! @tparam _idx: Index of the active row
    //! @tparam _idxColStart: Index of the first column that should be modified during pivoting.
    //! @param matrixData: Data of the matrix (column major)
    //! @param vectorData: Vector wich is permuted the same way as the matrix
    template <U32 _idx, U32 _idxColStart = 0>
    static inline void Partial(std::array<__m128, _size>& matrixData, __m128& vectorData);

    //! @brief Applies a permutation to a vector
    //! @param vec: Original vector
    //! @param permutationHash: Hash that specifies the permutation
    //! @return Permuted vector
    static inline __m128 PermuteVector(const __m128& vec, U32 permutationHash);


    //! @brief Performs a pivoting step on a matrix with the selected pivoting strategy
    //! @tparam _pivot: Enum to select pivoting strategy
    //! @tparam _idx: Index of the active row
    //! @tparam _idxColStart: Index of the first column that should be modified during pivoting.
    //! @param matrixData: Data of the matrix (column major)
    //! @param vectorData: Vector wich is permuted the same way as the matrix
    template <Pivot _pivot, U32 _idx, U32 _idxColStart = 0>
    static inline void PivotStep(std::array<__m128, _size>& matrixData, __m128& vectorData);
};

} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/internal/pivotDenseSmall.inl"
