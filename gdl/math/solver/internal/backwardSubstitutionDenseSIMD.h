#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"

#include <array>


namespace GDL::Solver
{



//! @brief Backward substitution solver class for dense static systems
//! @tparam _type: Data type
//! @tparam _size: Size of the linear system
//! @tparam _isUnit: If TRUE, the solver is optimized for unit triangular matrices.
template <typename _registerType, U32 _size, bool _isUnit = false>
class BackwardSubstitutionDenseSIMD
{
    static constexpr U32 alignment = simd::alignmentBytes<_registerType>;
    static constexpr U32 numRegisterValues = simd::numRegisterValues<_registerType>;
    static constexpr U32 numRegistersPerCol = simd::CalcMinNumArrayRegisters<_registerType>(_size);


    using MatrixDataArray = std::array<_registerType, numRegistersPerCol * _size>;
    using VectorDataArray = std::array<_registerType, numRegistersPerCol>;
    using ValueType = decltype(simd::GetDataType<_registerType>());

    BackwardSubstitutionDenseSIMD() = delete;

public:
    //! @brief Solves the linear system A * x = r with A being a lower triangular matrix
    //! @param matrixData: Matrix data
    //! @param rhsData: Data of the right-hand side vector
    //! @return Result vector x
    //    [[nodiscard]] inline static VectorDataArray Solve(const MatrixDataArray& matrixData,
    //                                                      const VectorDataArray& rhsData);

    //! @brief Solves the linear system A * x = r with A being a lower triangular matrix. The result is written into the
    //! passed vector data
    //! @param matrixData: Matrix data
    //! @param rhsData: Data of the right-hand side vector. The passed data is overwritten with the result.
    inline static void SolveInPlace(const MatrixDataArray& matrixData, VectorDataArray& rhsData);

private:
    //! @brief Performs a single backward substitution step
    //! @tparam _regValueIdx: Specifies the current active rows position inside of its corresponding register
    //! @param regRowIdx: Row index of the register that contains the current active row
    //! @param matrixData: MatrixData
    //! @param rhsData: Data of the right-hand side vector
    template <U32 _regValueIdx>
    static inline void SubstitutionStep(U32 regRowIdx, const MatrixDataArray& matrixData, VectorDataArray& rhsData);


    //! @brief Performs multiple backward substitution steps using template recursion
    //! @tparam _regValueIdx: Specifies the current active rows position inside of its corresponding register
    //! @param regRowIdx: Row index of the register that contains the current active row
    //! @param matrixData: MatrixData
    //! @param rhsData: Data of the right-hand side vector
    template <U32 _regValueIdx = numRegisterValues - 1>
    static inline void SubstitutionSteps(U32 regRowIdx, const MatrixDataArray& matrixData, VectorDataArray& rhsData);
};



} // namespace GDL::Solver


#include "gdl/math/solver/internal/backwardSubstitutionDenseSIMD.inl"
