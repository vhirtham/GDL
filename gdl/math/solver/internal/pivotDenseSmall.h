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


namespace Solver
{


//! @brief Helper class for pivoting strategies that are shared amongst multiple solvers
template <typename _type, U32 _size>
class PivotDenseSmallSerial
{

    template <typename, U32, Pivot>
    friend class LUDenseSmallSerial;
    template <typename, U32>
    friend class GaussDenseSmallSerial;

    //! @brief Performs partial pivoting on a matrix if necessary
    //! @tparam _idx: Index of the active row
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
};



} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/internal/pivotDenseSmall.inl"
