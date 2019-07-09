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



//! @brief Solves the linear system A * x = r using LU decomposition
//! @tparam _type: Data type
//! @tparam _size: Size of the system
//! @tparam _pivot: Enum to select the pivoting strategy
//! @param A: Matrix
//! @param r: Vector
//! @return Result vector x
template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
[[nodiscard]] VecSerial<_type, _size, true> LU(const MatSerial<_type, _size, _size>& A,
                                               const VecSerial<_type, _size, true>& r);

////! @brief Solves the linear system A * x = r using LU decomposition
////! @tparam _type: Data type
////! @tparam _size: Size of the system
////! @tparam _pivot: Enum to select the pivoting strategy
////! @param A: Matrix
////! @param r: Vector
////! @return Result vector x
// template <Pivot _pivot = Pivot::PARTIAL, typename _type, U32 _size>
//[[nodiscard]] VecSIMD<_type, _size, true> LU(const MatSIMD<_type, _size, _size>& A,
//                                             const VecSIMD<_type, _size, true>& r);



// Support classes ----------------------------------------------------------------------------------------------------

//! @brief LU solver class for dense static systems
//! @tparam _type: Data type
//! @tparam _size: Size of the linear system
//! @tparam _pivot: Enum to select the pivoting strategy
template <typename _type, U32 _size, Pivot _pivot = Pivot::PARTIAL>
class LUDenseSerial
{
    using MatrixDataArray = std::array<_type, _size * _size>;
    using VectorDataArray = std::array<_type, _size>;
    using VectorType = VecSerial<_type, _size, true>;
    using MatrixType = MatSerial<_type, _size, _size>;

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



    //! @brief Solves the linear system A * x = r
    //! @param A: Matrix
    //! @param r: Vector
    //! @return Result vector x
    [[nodiscard]] inline static VectorType Solve(const MatrixType& A, const VectorType& r);

    //! @brief Solves the linear system A * x = r
    //! @param factorization: Matrix factorization
    //! @param r: Vector
    //! @return Result vector x
    [[nodiscard]] inline static VectorType Solve(const Factorization& factorization, const VectorType& r);


private:
    //! @brief Performs the backward substitution of the solution process
    //! @param lu: Data of the LU decomposition
    //! @param r: Data of the right-hand side vector
    static inline void BackwardSubstitution(const MatrixDataArray& lu, VectorDataArray& r);

    //! @brief Calculates the LU factorization and returns it
    //! @param matrixData: Data of the matrix that should be factorized
    //! @return LU factorization
    [[nodiscard]] static inline Factorization Factorize(const MatrixDataArray& matrixData);

    //! @brief Performs a single factorization step
    //! @param iteration: Number of the current iteration
    //! @param factorization: Factorization data
    static inline void FactorizationStep(U32 iteration, Factorization& factorization);

    //! @brief Performs the forward substitution of the solution process
    //! @param lu: Data of the LU decomposition
    //! @param r: Data of the right-hand side vector
    static inline void ForwardSubstitution(const MatrixDataArray& lu, VectorDataArray& r);

    //! @brief Returns a vector that is permuted the same way as the factorized matrix
    //! @param r: Vector
    //! @param factorization: Factorization data
    //! @return Permuted verctor
    static inline VectorDataArray GetPermutedVectorData(const VectorType& r, const Factorization& factorization);
};



template <typename _registerType, U32 _size, Pivot _pivot = Pivot::PARTIAL>
class LUDenseSSE
{
};


} // namespace Solver

} // namespace GDL


#include "gdl/math/solver/lu.inl"
