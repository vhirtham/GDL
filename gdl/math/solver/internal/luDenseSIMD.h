#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>


namespace GDL::Solver
{


//! @brief LU solver class for dense static systems
//! @tparam _registerType: Register type
//! @tparam _size: Size of the linear system
//! @tparam _pivot: Enum to select the pivoting strategy
template <typename _registerType, U32 _size, Pivot _pivot>
class LUDenseSIMD
{
    static constexpr U32 alignment = simd::alignmentBytes<_registerType>;
    static constexpr U32 numRegisterValues = simd::numRegisterValues<_registerType>;
    static constexpr U32 numColRegisters = simd::CalcMinNumArrayRegisters<_registerType>(_size);


    using MatrixDataArray = std::array<_registerType, numColRegisters * _size>;
    using VectorDataArray = std::array<_registerType, numColRegisters>;
    using ValueType = decltype(simd::GetDataType<_registerType>());


public:
    //! @brief Class that stores the LU factorization and the permutations
    class Factorization
    {
        friend class LUDenseSIMD;

        alignas(alignment) MatrixDataArray mLU;

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

    //! @brief Performs multiple backward substitution steps using template recursion
    //! @tparam _regValueIdx: Specifies the current active rows position inside of its corresponding register
    //! @param regRowIdx: Row index of the register that contains the current active row
    //! @param lu: Data of the LU decomposition
    //! @param rhsData: Data of the right-hand side vector
    template <U32 _regValueIdx = numRegisterValues - 1>
    static inline void BackwardSubstitutionSteps(U32 regRowIdx, const MatrixDataArray& lu, VectorDataArray& rhsData);

    //! @brief Main loop of the factorization procedure
    //! @param factorization: Matrix factorization
    static inline void FactorizationLoop(Factorization& factorization);

    //! @brief Performs a single factorization step
    //! @tparam _regValueIdx: Specifies the current active rows position inside of its corresponding register
    //! @param iteration: Iteration number of the factorization procedure
    //! @param regRowIdx: Row index of the register that contains the current active row
    //! @param lu: Data of the LU decomposition
    template <U32 _regValueIdx>
    static inline void FactorizationStep(U32 iteration, U32 regRowIdx, MatrixDataArray& lu);

    //! @brief Performs multiple factorization steps using template recursion
    //! @tparam _regValueIdx: Specifies the current active rows position inside of its corresponding register
    //! @tparam _maxRecursionDepth: Maximum number of template recursions
    //! @param regRowIdx: Row index of the register that contains the current active row
    //! @param factorization: Matrix factorization
    template <U32 _regValueIdx = 0, U32 _maxRecursionDepth = numRegisterValues>
    static inline void FactorizationSteps(U32 regRowIdx, Factorization& factorization);

    //! @brief Performs the forward substitution of the solution process
    //! @param lu: Data of the LU decomposition
    //! @param rhsData: Data of the right-hand side vector
    static inline void ForwardSubstitution(const MatrixDataArray& lu, VectorDataArray& rhsData);

    //! @brief Performs multiple forward substitution steps using template recursion
    //! @tparam _regValueIdx: Specifies the current active rows position inside of its corresponding register
    //! @tparam _maxRecursionDepth: Maximum number of template recursions
    //! @param regRowIdx: Row index of the register that contains the current active row
    //! @param lu: Data of the LU decomposition
    //! @param rhsData: Data of the right-hand side vector
    template <U32 _regValueIdx = 0, U32 _maxRecursionDepth = numRegisterValues>
    static inline void ForwardSubstitutionSteps(U32 regRowIdx, const MatrixDataArray& lu, VectorDataArray& rhsData);
};



} // namespace GDL::Solver


#include "gdl/math/solver/internal/luDenseSIMD.inl"
