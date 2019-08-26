#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/solver/pivotEnum.h"
#include "gdl/math/solver/internal/permutationDataSerial.h"

#include <array>


namespace GDL::Solver
{

template <typename, U32, U32, U32>
class PivotDenseSerial;

//! @brief QR solver class for dense static systems
//! @tparam _type: Data type
//! @tparam _rows: Rows of the matrix
//! @tparam _cols: Columns of the matrix
//! @tparam _pivot: Enum to select the pivoting strategy
template <typename _type, U32 _rows, U32 _cols, Pivot _pivot>
class QRDenseSerial
{
    using MatrixDataArray = std::array<_type, _rows * _cols>;
    using VectorDataArray = std::array<_type, _rows>;

    using PivotClass = PivotDenseSerial<_type, _rows, _cols + 1, 1>;

    QRDenseSerial() = delete;

public:
    //! @brief Class that stores the QR factorization and the permutations
    class Factorization
    {
        friend class QRDenseSerial;

        //! @brief Helper struct that is used inside of a union
        struct RData
        {
            VectorDataArray Offset;
            MatrixDataArray R;
        };

        //! @brief Helper struct that is used inside of a union
        struct QData
        {
            MatrixDataArray Q;
            VectorDataArray Offset;
        };

        using QRDataArray = std::array<_type, (_rows + 1) * _cols>;


        //! Union that stores the data of Q and R as compact as possible
        union QRData {
            QData mQData;
            RData mRData;
            QRDataArray mQR;

            //! @brief ctor
            //! @param dataQ: Data that should initialize Q
            QRData(const MatrixDataArray& dataQ);
        };

        static_assert(sizeof(RData) == sizeof(QData) && sizeof(RData) == sizeof(QRDataArray) &&
                              sizeof(RData) == sizeof(QRData),
                      "Internal data format error");

        QRData mQR;

        PermutationData<_type, _rows, _pivot> mPermutationData;

        //! @brief ctor
        //! @param matrixData: Data of the matrix that should be factorized
        Factorization(const MatrixDataArray& matrixData);

        //! @brief Returns a reference to Q
        //! @return Reference to Q
        MatrixDataArray& GetQ();

        //! @brief Returns a reference to Q
        //! @return Reference to Q
        const MatrixDataArray& GetQ() const;

        //! @brief Returns a reference to R
        //! @return Reference to R
        MatrixDataArray& GetR();

        //! @brief Returns a reference to R
        //! @return Reference to R
        const MatrixDataArray& GetR() const;

        //! @brief Returns a reference to the internal data array
        //! @return Reference to the internal data array
        QRDataArray& GetQR();
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
    //! @brief Applies the current iterations reflection to R
    //! @param iteration: Number of the current iteration
    //! @param colStartIdx: Array index of the active columns first element inside of R
    //! @param Q: Matrix that stores the Householder reflections of Q, not Q itself.
    //! @param R: Matrix R
    static inline void ApplyReflectionToR(U32 iteration, U32 colStartIdx, MatrixDataArray& Q, MatrixDataArray& R);

    //! @brief Calculates the current iterations householder reflection and the new diagonal entry of R
    //! @param iteration: Number of the current iteration
    //! @param pivIdx: Array index of the pivot element inside of R
    //! @param Q: Matrix that stores the Householder reflections of Q, not Q itself.
    //! @param R: Matrix R
    static inline void CalculateReflectionAndDiagonalValue(U32 iteration, U32 pivIdx, MatrixDataArray& Q,
                                                           MatrixDataArray& R);

    //! @brief Performs a single factorization step
    //! @param iteration: Number of the current iteration
    //! @param Q: Matrix that stores the Householder reflections of Q, not Q itself.
    //! @param R: Matrix R
    static inline void FactorizationStep(U32 iteration, MatrixDataArray& Q, MatrixDataArray& R);

    //! @brief Returns a vector that is permuted the same way as the factorized matrix
    //! @param rhsData: Vector
    //! @param factorization: Factorization data
    //! @return Permuted verctor
    static inline VectorDataArray GetPermutedVectorData(const VectorDataArray& rhsData,
                                                        const Factorization& factorization);

    //! @brief Multiplies a vector with the transposed of Q. The passed vector is overwritten with the result.
    //! @param Q: Matrix that stores the Householder reflections of Q, not Q itself.
    //! @param vectorData: Vector data
    static inline void MultiplyWithTransposedQ(const MatrixDataArray& Q, VectorDataArray& vectorData);
};



} // namespace GDL::Solver


#include "gdl/math/solver/internal/qrDenseSerial.inl"
