#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>


namespace GDL::Solver
{



//! @brief Backward substitution solver class for dense static systems
//! @tparam _type: Data type
//! @tparam _size: Size of the linear system
//! @tparam _isUnit: If TRUE, the solver is optimized for unit triangular matrices.
template <typename _type, U32 _size, bool _isUnit = false>
class BackwardSubstitutionDenseSerial
{
    using MatrixDataArray = std::array<_type, _size * _size>;
    using VectorDataArray = std::array<_type, _size>;

    BackwardSubstitutionDenseSerial() = delete;

public:
    //! @brief Solves the linear system A * x = r with A being an upper triangular matrix
    //! @param matrixData: Matrix data
    //! @param rhsData: Data of the right-hand side vector
    //! @return Result vector x
    //    [[nodiscard]] inline static VectorDataArray Solve(const MatrixDataArray& matrixData,
    //                                                      const VectorDataArray& rhsData);

    //! @brief Solves the linear system A * x = r with A being an upper triangular matrix. The result is written into
    //! the passed vector data
    //! @param matrixData: Matrix data
    //! @param rhsData: Data of the right-hand side vector. The passed data is overwritten with the result.
    inline static void SolveInPlace(const MatrixDataArray& matrixData, VectorDataArray& rhsData);
};



} // namespace GDL::Solver


#include "gdl/math/solver/internal/backwardSubstitutionDenseSerial.inl"
