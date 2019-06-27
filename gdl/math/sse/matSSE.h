#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"
#include "gdl/base/sse/intrinsics.h"

#include <array>
#include <type_traits>


namespace GDL
{


//! @brief Matrix of arbitrary size with SIMD support
//! @tparam _type: Data type of the matrix
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
template <typename _type, I32 _rows, I32 _cols>
class alignas(simd::alignmentBytes<decltype(simd::GetFittingRegister<_type, simd::MaxRegisterSize()>())>) MatSSE
{
    static_assert(std::is_floating_point<_type>::value, "Matrix can only be created with floating point types");

public:
    using RegisterType = decltype(simd::GetFittingRegister<_type, simd::MaxRegisterSize()>());
    constexpr static U32 mAlignment = simd::alignmentBytes<RegisterType>;
    constexpr static U32 mNumRegisterEntries = simd::numRegisterValues<RegisterType>;
    constexpr static U32 mNumRegistersPerCol = simd::CalcMinNumArrayRegisters<RegisterType>(_rows);
    constexpr static U32 mNumRegisters = _cols * mNumRegistersPerCol;

private:
    using DataArray = std::array<RegisterType, mNumRegisters>;

    alignas(mAlignment) DataArray mData;

    template <typename _typeOther, I32 _rowsOther, I32 _colsOther>
    friend class MatSSE;

public:
    //! @brief constructor
    MatSSE();

    //! @brief Constructor to set the whole matrix
    //! @tparam _args: Variadic data type
    //! @param args: Values (column major)
    template <typename... _args>
    explicit MatSSE(_args... args);

    //! @brief Constructor to set the whole matrix
    //! @param data: Array with values (column major)
    explicit MatSSE(const std::array<_type, _rows * _cols>& data);

    //! @brief Constructor to set the whole matrix
    //! @param data: Array with values (column major)
    explicit MatSSE(const DataArray& data);

private:
    //! @brief Private helper ctor to initialize a MatSSE without initializing the memory to zero or any other value.
    //! @remark Find a better solution than using bool as parameter to distinguish from default ctor
    explicit MatSSE(bool);

public:
    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    [[nodiscard]] inline _type operator()(const U32 row, const U32 col) const;

    //! @brief Compares if two matrices are equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator==(const MatSSE& rhs) const;

    //! @brief Compares if two matrices are NOT equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const MatSSE& rhs) const;

    //! @brief Matrix - matrix addition assignment
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    inline MatSSE& operator+=(const MatSSE& rhs);

    //! @brief Matrix - matrix addition assignment
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    [[nodiscard]] inline MatSSE operator+(const MatSSE& rhs) const;

    //! @brief Matrix - matrix multiplication
    //! @tparam _rowsRhs: Rhs matrix number of rows
    //! @tparam _colsRhs: Rhs matrix number of columns
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    template <I32 _rowsRhs, I32 _colsRhs>
    [[nodiscard]] inline MatSSE<_type, _rows, _colsRhs> operator*(const MatSSE<_type, _rowsRhs, _colsRhs>& rhs) const;

    //! @brief Returns the transposed matrix
    //! @return Transposed matrix
    [[nodiscard]] inline MatSSE<_type, _cols, _rows> Transpose() const;

    //! @brief Gets the number of rows
    //! @return Number of rows
    [[nodiscard]] inline U32 Rows() const;

    //! @brief Gets the number of columns
    //! @return Number of columns
    [[nodiscard]] inline U32 Cols() const;

    //! @brief Sets all matrix entries to zero
    inline void SetZero();

    //! @brief Gets the data array in column major ordering
    //! @return Data
    [[nodiscard]] inline const std::array<_type, _rows * _cols> Data() const;

    //! @brief Gets the underlying array of SSE registers
    //! @return Data array
    const DataArray& DataSSE() const;

private:
    //! @brief Transposes all completly filled blocks of the matrix and writes them to the corresponding position of the
    //! passed matrix
    //! @param result: Matrix that stores the results
    inline void TransposeFullBlocks(MatSSE<_type, _cols, _rows>& result) const;

    //! @brief Transposes all partially filled blocks of the matrix and writes them to the corresponding position of the
    //! passed matrix
    //! @tparam _hasZeroRows: defines if the transposed block has rows that are completely filled with zeros
    //! @tparam _hasZeroCols: defines if the transposed block has columns that are completely filled with zeros
    //! @tparam _count: internal recursion counter
    //! @tparam _args: Additional argument type
    //! @param result: Matrix that stores the results
    //! @param indexBlock: Row / column index of the current block.
    //! @param args: This parameter pack is used to gather the parameters for the sse transpose function.
    template <bool _hasSparseRows, bool _hasSparseCols, U32 _count = 0, typename... _args>
    inline void TransposeSparseBlocks(MatSSE<_type, _cols, _rows>& result, U32 indexBlock, _args&... args) const;

    //! @brief Processes the inner two loops of matrix-matrix multiplication.
    //! @tparam _rowsRhs: Rhs matrix number of rows
    //! @tparam _colsRhs: Rhs matrix number of columns
    //! @tparam _numMultipliedRegisters Number of registers that are multiplied and added to the result
    //! @param result: Reference to the result matrix
    //! @param rhs: Rhs matrix
    //! @param j: Current outer loop value
    template <I32 _rowsRhs, I32 _colsRhs, U32 _numMultipliedRegisters = mNumRegisterEntries>
    inline void MultiplicationInnerLoops(MatSSE<_type, _rows, _colsRhs>& result,
                                         const MatSSE<_type, _rowsRhs, _colsRhs>& rhs, U32 j) const;

    //! @brief This function helps with the generalization of matrix-matrix multiplication. It calculates some in
    //! between values that depend on the number of values in the used register and adds them to the current solution.
    //! @tparam _numOperations: Number of recursive function calls.
    //! @tparam _count: Internal counter to keep track of the number of recursive function calls.
    //! @param rhsValues: Rhs register that is needed for the current multiplication step
    //! @param currentValue: Current value in the result register
    //! @param currentBlockIndex: Index of the currently used register of the lhs matrix.
    //! @return Updated solution register
    template <U32 _numOperations = mNumRegisterEntries, U32 _count = 0>
    inline RegisterType MultiplyAddRegisters(const RegisterType rhsValues, const RegisterType currentValue,
                                             const U32 currentBlockIndex) const;


    //! @brief Checks if the matrix was constructed as expected
    bool IsInternalDataValid() const;
};



//! @brief Offstream operator
//! @tparam _type: Data type of the matrix
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
template <typename _type, I32 _rows, I32 _cols>
std::ostream& operator<<(std::ostream& os, const MatSSE<_type, _rows, _cols>& mat);


} // namespace GDL



#include "gdl/math/sse/matSSE.inl"
