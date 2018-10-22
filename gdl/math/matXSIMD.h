#pragma once

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/sse.h"

#include <array>
#include <type_traits>


namespace GDL
{


//! @brief Matrix of arbitrary size with SIMD support
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
template <typename _type, I32 _rows, I32 _cols>
class alignas(AlignmentBytes<__m128>) matXSIMD
// TODO: if register is a template parameter set corresponding alignment in the row above!
{
    typedef typename std::conditional<std::is_same<_type, F32>::value, __m128, __m128d>::type __mx;
    constexpr static U32 mAlignment = AlignmentBytes<__mx>;
    constexpr static U32 mNumRegisterEntries = GetNumRegisterEntries<__mx>();
    constexpr static U32 mNumRegistersPerCol = CalcMinNumArrayRegisters(_rows, mNumRegisterEntries);
    constexpr static U32 mNumRegisters = _cols * mNumRegistersPerCol;


    alignas(AlignmentBytes<__mx>) std::array<__mx, mNumRegisters> mData;

    template <typename _typeOther, I32 _rowsOther, I32 _colsOther>
    friend class matXSIMD;

public:
    //! @brief constructor
    matXSIMD();

    //! @brief Constructor to set the whole matrix
    //! @param args: Values (column major)
    //! @tparam _args: Variadic data type
    template <typename... _args>
    explicit matXSIMD(_args... args);


    //! @brief Constructor to set the whole matrix
    //! @brief Array with values (column major)
    explicit matXSIMD(const std::array<_type, _rows * _cols>& data);

    //! @brief Matrix - matrix multiplication
    //! @tparam _rowsRhs: Rhs matrix number of rows
    //! @tparam _colsRhs: Rhs matrix number of columns
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    template <I32 _rowsRhs, I32 _colsRhs>
    inline matXSIMD<_type, _rows, _colsRhs> operator*(const matXSIMD<_type, _rowsRhs, _colsRhs>& rhs) const;

    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    inline matXSIMD& operator+=(const matXSIMD& rhs);

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline _type operator()(const U32 row, const U32 col) const;

    //! @brief Gets the number of rows
    //! @return Number of rows
    inline U32 Rows() const;

    //! @brief Gets the number of columns
    //! @return Number of columns
    inline U32 Cols() const;

    //! @brief Sets all matrix entries to zero
    inline void SetZero();

    //! @brief Gets the data array in column major ordering
    //! @return Data
    inline const std::array<_type, _rows * _cols> Data() const;

private:
    //! @brief Helps to create a temporary array that is needed during matrix-matrix multiplication.
    //! @tparam _arraySize: Size of the array
    //! @tparam _count: Internal counter
    //! @tparam _args: Additional arguments. Don't provide one. They are used during recursive function calls.
    //! @param data: Register that provides the values for the array
    //! @param args: Additional arguments. Don't provide one. They are used during recursive function calls.
    //! @return Array that is needed during matrix-matrix multiplication.
    template <U32 _arraySize = mNumRegisterEntries, U32 _count = 0, typename... _args>
    static std::array<__mx, _arraySize> CreateRHSRegisterArray(const __mx& data, const _args&... args);


    //! @brief This function helps with the generalization of matrix-matrix multiplication. It calculates some in
    //! between values that depend on the number of values in the used register and adds them to the current solution.
    //! @tparam _numOperations: Number of recursive function calls.
    //! @tparam _count: Internal counter to keep track of the number of recursive function calls.
    //! @param values: Array of registers which should be multiplied and added to the result.
    //! @param currentValue: Current value in the result register
    //! @param currentBlockIndex: Index of the currently used register of the lhs matrix.
    //! @return Updated solution register
    template <U32 _numOperations = mNumRegisterEntries, U32 _count = 0>
    inline __mx MultiplyRegisters(const std::array<__mx, _numOperations>& values, const __mx currentValue,
                                  const U32 currentBlockIndex) const;

    //! @brief Checks if the matrix was constructed as expected
    void ConstructionChecks() const;
};
} // namespace GDL
