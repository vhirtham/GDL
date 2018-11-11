#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/sse.h"

#include <array>
#include <type_traits>


namespace GDL
{


//! @brief Matrix of arbitrary size with SIMD support
//! @tparam _type: Data type of the matrix
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
template <typename _type, I32 _rows, I32 _cols>
class alignas(alignmentBytes<decltype(SSEGetFittingRegister<_type, SSEMaxRegisterSize()>())>) MatSIMD
{
    static_assert(std::is_floating_point<_type>::value, "Matrix can only be created with floating point types");

    using __mx = decltype(SSEGetFittingRegister<_type, SSEMaxRegisterSize()>());
    constexpr static U32 mAlignment = alignmentBytes<__mx>;
    constexpr static U32 mNumRegisterEntries = SSEGetNumRegisterEntries<__mx>();
    constexpr static U32 mNumRegistersPerCol = SSECalcMinNumArrayRegisters<__mx>(_rows);
    constexpr static U32 mNumRegisters = _cols * mNumRegistersPerCol;


    alignas(mAlignment) std::array<__mx, mNumRegisters> mData;

    template <typename _typeOther, I32 _rowsOther, I32 _colsOther>
    friend class MatSIMD;

public:
    //! @brief constructor
    MatSIMD();

    //! @brief Constructor to set the whole matrix
    //! @param args: Values (column major)
    //! @tparam _args: Variadic data type
    template <typename... _args>
    explicit MatSIMD(_args... args);

    //! @brief Constructor to set the whole matrix
    //! @brief Array with values (column major)
    explicit MatSIMD(const std::array<_type, _rows * _cols>& data);

private:
    //! @brief Private helper ctor to initialize a MatSIMD without initializing the memory to zero or any other value.
    //! @remark Find a better solution than using bool as parameter to distinguish from default ctor
    explicit MatSIMD(bool);

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
    [[nodiscard]] inline bool operator==(const MatSIMD& rhs) const;

    //! @brief Compares if two matrices are NOT equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const MatSIMD& rhs) const;

    //! @brief Matrix - matrix addition assignment
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    inline MatSIMD& operator+=(const MatSIMD& rhs);

    //! @brief Matrix - matrix addition assignment
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    [[nodiscard]] inline MatSIMD operator+(const MatSIMD& rhs) const;

    //! @brief Matrix - matrix multiplication
    //! @tparam _rowsRhs: Rhs matrix number of rows
    //! @tparam _colsRhs: Rhs matrix number of columns
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    template <I32 _rowsRhs, I32 _colsRhs>
    [[nodiscard]] inline MatSIMD<_type, _rows, _colsRhs> operator*(const MatSIMD<_type, _rowsRhs, _colsRhs>& rhs) const;

    //! @brief Returns the transposed matrix
    //! @return Transposed matrix
    [[nodiscard]] inline MatSIMD<_type, _cols, _rows> Transpose() const;

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

private:
    //! @brief Processes the inner two loops of matrix-matrix multiplication.
    //! @tparam _rowsRhs: Rhs matrix number of rows
    //! @tparam _colsRhs: Rhs matrix number of columns
    //! @tparam _numMultipliedRegisters Number of registers that are multiplied and added to the result
    //! @param result: Reference to the result matrix
    //! @param rhs: Rhs matrix
    //! @param j: Current outer loop value
    template <I32 _rowsRhs, I32 _colsRhs, U32 _numMultipliedRegisters = mNumRegisterEntries>
    inline void MultiplicationInnerLoops(MatSIMD<_type, _rows, _colsRhs>& result,
                                         const MatSIMD<_type, _rowsRhs, _colsRhs>& rhs, U32 j) const;

    //! @brief Creates a temporary array that is needed during matrix-matrix multiplication.
    //! @tparam _arraySize: Size of the array
    //! @tparam _count: Internal counter
    //! @tparam _args: Additional arguments. Don't provide one. They are used during recursive function calls.
    //! @param data: Register that provides the values for the array
    //! @param args: Additional arguments. Don't provide one. They are used during recursive function calls.
    //! @return Array that is needed during matrix-matrix multiplication.
    template <U32 _arraySize = mNumRegisterEntries, U32 _count = 0, typename... _args>
    static std::array<__mx, _arraySize> MultiplicationCreateRHSArray(const std::array<_type, mNumRegisterEntries>& data,
                                                                     const _args&... args);


    //! @brief This function helps with the generalization of matrix-matrix multiplication. It calculates some in
    //! between values that depend on the number of values in the used register and adds them to the current solution.
    //! @tparam _numOperations: Number of recursive function calls.
    //! @tparam _count: Internal counter to keep track of the number of recursive function calls.
    //! @param values: Array of registers which should be multiplied and added to the result.
    //! @param currentValue: Current value in the result register
    //! @param currentBlockIndex: Index of the currently used register of the lhs matrix.
    //! @return Updated solution register
    template <U32 _numOperations = mNumRegisterEntries, U32 _count = 0>
    inline __mx MultiplyAddRegisters(const std::array<__mx, _numOperations>& values, const __mx currentValue,
                                     const U32 currentBlockIndex) const;

    //! @brief Helper function to transposes a small block matrix
    //! @tparam _countIn: Counts the unpacked input registers
    //! @tparam _countOut: Counts the unpacked output registers
    //! @tparam _args: Type of additional arguments
    //! @param transposed: Matrix that should store the transposed data
    //! @param rowIndexBlock: Row index of the current block matrix
    //! @param colIndexBlock: Column index of the current block matrix
    //! @param args: Additional arguments
    //! @remark: The parameter pack is used to generate the inputs for the sse transpose function.
    template <U32 _countIn = 0, U32 _countOut = 0, typename... _args>
    void TransposeBlock(MatSIMD<_type, _cols, _rows>& transposed, U32 rowIndexBlock, U32 colIndexBlock,
                        _args&... args) const;

    //! @brief Checks if the matrix was constructed as expected
    void ConstructionChecks() const;
};



//! @brief Offstream operator
//! @tparam _type: Data type of the matrix
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
template <typename _type, I32 _rows, I32 _cols>
std::ostream& operator<<(std::ostream& os, const MatSIMD<_type, _rows, _cols>& mat);


} // namespace GDL



#include "gdl/math/matSIMD.inl"
