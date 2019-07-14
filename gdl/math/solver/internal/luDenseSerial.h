#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>


namespace GDL::Solver
{



//! @brief LU solver class for dense static systems
//! @tparam _type: Data type
//! @tparam _size: Size of the linear system
//! @tparam _pivot: Enum to select the pivoting strategy
template <typename _type, U32 _size, Pivot _pivot>
class LUDenseSerial
{
    using MatrixDataArray = std::array<_type, _size * _size>;
    using VectorDataArray = std::array<_type, _size>;

public:
    //! @brief Class that stores the LU factorization and the permutations
    class Factorization
    {
        friend class LUDenseSerial;

        MatrixDataArray mLU;
        VectorDataArray mPermutation;

        //! @brief ctor
        //! @param matrixData: Data of the matrix that should be factorized
        Factorization(const MatrixDataArray& matrixData);
    };



    //! @brief Calculates the LU factorization and returns it
    //! @param matrixData: Data of the matrix that should be factorized
    //! @return LU factorization
    [[nodiscard]] static inline Factorization Factorize(const MatrixDataArray& matrixData);

    //! @brief Solves the linear system A * x = r
    //! @param factorization: Matrix factorization
    //! @param rhsData: Data of the right-hand side vector
    //! @return Result vector x
    [[nodiscard]] inline static VectorDataArray Solve(const Factorization& factorization,
                                                      const VectorDataArray& rhsData);

private:
    //! @brief Performs the backward substitution of the solution process
    //! @param lu: Data of the LU decomposition
    //! @param rhsData: Data of the right-hand side vector
    static inline void BackwardSubstitution(const MatrixDataArray& lu, VectorDataArray& rhsData);

    //! @brief Performs a single factorization step
    //! @param iteration: Number of the current iteration
    //! @param factorization: Factorization data
    static inline void FactorizationStep(U32 iteration, Factorization& factorization);

    //! @brief Performs the forward substitution of the solution process
    //! @param lu: Data of the LU decomposition
    //! @param rhsData: Data of the right-hand side vector
    static inline void ForwardSubstitution(const MatrixDataArray& lu, VectorDataArray& rhsData);

    //! @brief Returns a vector that is permuted the same way as the factorized matrix
    //! @param rhsData: Vector
    //! @param factorization: Factorization data
    //! @return Permuted verctor
    static inline VectorDataArray GetPermutedVectorData(const VectorDataArray& rhsData,
                                                        const Factorization& factorization);
};



} // namespace GDL::Solver


#include "gdl/math/solver/internal/luDenseSerial.inl"
