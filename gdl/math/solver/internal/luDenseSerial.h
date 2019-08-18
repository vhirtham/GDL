#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/solver/pivotEnum.h"
#include "gdl/math/solver/internal/permutationDataSerial.h"

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

    LUDenseSerial() = delete;

public:
    //! @brief Class that stores the LU factorization and the permutations
    class Factorization
    {
        friend class LUDenseSerial;

        MatrixDataArray mLU;
        PermutationData<_type, _size, _pivot> mPermutationData;

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
    //! @brief Performs a single factorization step
    //! @param iteration: Number of the current iteration
    //! @param factorization: Factorization data
    static inline void FactorizationStep(U32 iteration, Factorization& factorization);

    //! @brief Returns a vector that is permuted the same way as the factorized matrix
    //! @param rhsData: Vector
    //! @param factorization: Factorization data
    //! @return Permuted verctor
    static inline VectorDataArray GetPermutedVectorData(const VectorDataArray& rhsData,
                                                        const Factorization& factorization);
};



} // namespace GDL::Solver


#include "gdl/math/solver/internal/luDenseSerial.inl"
