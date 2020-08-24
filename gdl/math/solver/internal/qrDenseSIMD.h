#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/math/solver/internal/pivotDenseSIMD.h"
#include "gdl/math/solver/pivotEnum.h"

#include <array>


namespace GDL::Solver
{


//! @brief QR solver class for dense static systems
//! @tparam _registerType: Register type
//! @tparam _size: Size of the linear system
//! @tparam _pivot: Enum to select the pivoting strategy
template <typename _registerType, U32 _rows, U32 _cols, Pivot _pivot>
class QRDenseSIMD
{
    static constexpr U32 alignment = simd::alignmentBytes<_registerType>;
    static constexpr U32 numRegisterValues = simd::numRegisterValues<_registerType>;
    static constexpr U32 numRegistersPerCol = simd::CalcMinNumArrayRegisters<_registerType>(_rows);


    using MatrixDataArray = std::array<_registerType, numRegistersPerCol * _cols>;
    using VectorDataArray = std::array<_registerType, numRegistersPerCol>;
    using ValueType = decltype(simd::GetDataType<_registerType>());

    QRDenseSIMD() = delete;

public:
    //! @brief Class that stores the QR factorization and the permutations
    class Factorization
    {
        friend class QRDenseSIMD;

        using PermutationDataArray = typename PivotDenseSSE<_registerType, _rows>::VectorPermutationDataArray;


        //! @brief Helper struct that is used inside of a union
        struct RData
        {
            VectorDataArray Offset;
            MatrixDataArray R;
        };

        //! @brief Helper struct that is used inside of a union
        struct VData
        {
            MatrixDataArray V;
            VectorDataArray Offset;
        };

        using VRDataArray = std::array<_registerType, numRegistersPerCol*(_cols + 1)>;

        //! Union that stores the data of Q and R as compact as possible
        union alignas(alignment) VRData
        {
            VData mVData;
            RData mRData;
            VRDataArray mVR;

            //! @brief ctor
            //! @param dataQ: Data that should initialize Q
            VRData(const MatrixDataArray& dataQ);
        };


        VRData mVR;
        PermutationDataArray mPermutationData;

        //! @brief ctor
        //! @param matrixData: Data of the matrix that should be factorized
        Factorization(const MatrixDataArray& matrixData);

        //! @brief Returns a reference to V
        //! @return Reference to V
        MatrixDataArray& GetV();

        //! @brief Returns a reference to V
        //! @return Reference to V
        const MatrixDataArray& GetV() const;

        //! @brief Returns a reference to R
        //! @return Reference to R
        MatrixDataArray& GetR();

        //! @brief Returns a reference to R
        //! @return Reference to R
        const MatrixDataArray& GetR() const;

        //! @brief Returns a reference to the internal data array
        //! @return Reference to the internal data array
        VRDataArray& GetVR();
    };



    //! @brief Calculates the QR factorization and returns it
    //! @param matrixData: Data of the matrix that should be factorized
    //! @return QR factorization
    [[nodiscard]] static inline Factorization Factorize(const MatrixDataArray& matrixData);

    //! @brief Solves the linear system A * x = r
    //! @param factorization: Matrix factorization
    //! @param rhsData: Data of the right-hand side vector
    //! @return Result vector x
    [[nodiscard]] inline static VectorDataArray Solve(const Factorization& factorization,
                                                      const VectorDataArray& rhsData);

private:
    template <U32 _regValueIdx>
    static inline _registerType CalculateSquareSum(U32 regRowIdx, U32 colStartIdx, MatrixDataArray& R);

    //! @brief Performs a single factorization step
    //! @tparam _regValueIdx: Specifies the current active rows position inside of its corresponding register
    //! @param iteration: Iteration number of the factorization procedure
    //! @param regRowIdx: Row index of the register that contains the current active row
    //! @param V: Matrix that stores the Householder reflections.
    //! @param R: Matrix R
    template <U32 _regValueIdx>
    static inline void FactorizationStep(U32 iteration, U32 regRowIdx, MatrixDataArray& V, MatrixDataArray& R);

    //! @brief Performs multiple factorization steps using template recursion
    //! @tparam _regValueIdx: Specifies the current active rows position inside of its corresponding register
    //! @tparam _maxRecursionDepth: Maximum number of template recursions
    //! @param regRowIdx: Row index of the register that contains the current active row
    //! @param factorization: Matrix factorization
    template <U32 _regValueIdx = 0, U32 _maxRecursionDepth = numRegisterValues>
    static inline void FactorizationSteps(U32 regRowIdx, MatrixDataArray& V, MatrixDataArray& R);

    static inline void MultiplyWithTransposedQ(const MatrixDataArray& V, VectorDataArray& vectorData);
};



} // namespace GDL::Solver


#include "gdl/math/solver/internal/qrDenseSIMD.inl"
