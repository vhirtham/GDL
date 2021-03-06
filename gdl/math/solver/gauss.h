#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>


namespace GDL
{

template <typename _type, U32, U32>
class MatSerial;
template <typename _type, U32, U32>
class MatSIMD;
template <typename _type, U32, bool>
class VecSerial;
template <typename _type, U32, bool>
class VecSIMD;

namespace Solver
{



//! @brief Solves the linear system A * x = b by using a Gauss-Jordan algorithm with partial pivoting.
//! @tparam _type: Data type
//! @tparam _size: Size of the system
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
[[nodiscard]] VecSerial<_type, _size, true> Gauss(const MatSerial<_type, _size, _size>& A,
                                                  const VecSerial<_type, _size, true>& b);

//! @brief Solves the linear system A * x = b by using a vectorized Gauss-Jordan algorithm with partial pivoting.
//! @tparam _type: Data type
//! @tparam _size: Size of the system
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @param b: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
[[nodiscard]] VecSIMD<_type, _size, true> Gauss(const MatSIMD<_type, _size, _size>& A,
                                                const VecSIMD<_type, _size, true>& b);



// Support classes ----------------------------------------------------------------------------------------------------

//! @brief Gauss-Jordan solver class for dense static systems
//! @tparam _type: Data type
//! @tparam _size: Size of the linear system
//! @tparam _pivot: Enum to select the pivoting strategy
template <typename _type, U32 _size, Pivot _pivot = Pivot::PARTIAL>
class GaussDenseSerial
{
    using MatrixDataArray = std::array<_type, _size * _size>;
    using VectorDataArray = std::array<_type, _size>;
    using VectorType = VecSerial<_type, _size, true>;
    using MatrixType = MatSerial<_type, _size, _size>;

public:
    //! @brief Solves the linear system A * x = b by using a Gauss-Jordan algorithm with partial pivoting.
    //! @param A: Matrix
    //! @param b: Vector
    //! @return Result vector x
    [[nodiscard]] inline static VectorType Solve(const MatrixType& A, const VectorType& b);

private:
    //! @brief Performs the elimination step of the current iteration
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    inline static void EliminationStep(U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);

    //! @brief Performs a single Gauss step (with pivoting if selected)
    //! @param iteration: Number of the current iteration
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    inline static void GaussStep(U32 iteration, MatrixDataArray& matData, VectorDataArray& vecData);
};



//! @brief SSE based Gauss-Jordan solver class for dense static systems
//! @tparam _registerType: SSE register type
//! @tparam _size: Size of the linear system
//! @tparam _pivot: Enum to select the pivoting strategy
template <typename _registerType, U32 _size, Pivot _pivot = Pivot::PARTIAL>
class GaussDenseSSE
{
    static constexpr U32 alignment = simd::alignmentBytes<_registerType>;
    static constexpr U32 numRegisterValues = simd::numRegisterValues<_registerType>;
    static constexpr U32 numColRegisters = simd::CalcMinNumArrayRegisters<_registerType>(_size);


    using MatrixDataArray = std::array<_registerType, numColRegisters * _size>;
    using VectorDataArray = std::array<_registerType, numColRegisters>;
    using ValueType = decltype(simd::GetDataType<_registerType>());
    using VectorType = VecSIMD<ValueType, _size, true>;
    using MatrixType = MatSIMD<ValueType, _size, _size>;


public:
    //! @brief Solves the linear system A * x = b by using a Gauss-Jordan algorithm with partial pivoting.
    //! @param A: Matrix
    //! @param b: Vector
    //! @return Result vector x
    [[nodiscard]] inline static VectorType Solve(const MatrixType& A, const VectorType& b);

private:
    //! @brief Performs the elimination step of the current iteration
    //! @tparam _regValueIdx: Each column is composed of registers that contain multiple row values of that column. This
    //! parameter specifies the position of the value inside of the corresponding register that belongs to the current
    //! iterations active row.
    //! @param iteration: Number of the current iteration
    //! @param regRowIdx: This parameter specifies the position of the register that holds the value of the current
    //! iterations active row inside of each column of registers.
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <U32 _regValueIdx>
    inline static void EliminationStepRegister(U32 iteration, U32 regRowIdx, MatrixDataArray& matData,
                                               VectorDataArray& vecData);

    //! @brief This function uses template recursion to perform a gauss step (pivoting and elimination) for each value
    //! inside of a register.
    //! @tparam _regValueIdx: Each column is composed of registers that contain multiple row values of that column. This
    //! parameter specifies the position of the value inside of the corresponding register that belongs to the current
    //! iterations active row.
    //! @tparam _maxRecursionDepth: Specifies the maximal recursion depth.
    //! @param regRowIdx: This parameter specifies the position of the register that holds the value of the current
    //! iterations active row inside of each column of registers.
    //! @param matData: Matrix data array (column major ordering)
    //! @param vecData: Vector data array
    template <U32 _regValueIdx = 0, U32 _maxRecursionDepth = numRegisterValues>
    inline static void GaussStepsRegister(U32 regRowIdx, MatrixDataArray& matData, VectorDataArray& vecData);
};


} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/gauss.inl"
