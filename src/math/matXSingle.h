#pragma once

#include "GDLTypedefs.h"

#include <array>

namespace GDL
{
//! @brief Matrix of arbitrary size
//! @tparam T: Data type
//! @tparam tRows: Number of rows
//! @tparam tCols: Number of columns
template <typename T, int tRows, int tCols>
class matXSingle
{
    template <typename T2, int tRows2, int tCols2>
    friend class matXSingle;

    std::array<T, tRows * tCols> mData;

public:
    //! @brief Constructor
    matXSingle();

    //! @brief Constructor to set the whole matrix
    //! @brief Array with values (column major)
    matXSingle(const std::array<T, tRows * tCols>& data);

    //! @brief Matrix - matrix multiplication
    //! @tparam tRowsRhs: Rhs matrix number of rows
    //! @tparam tColsRhs: Rhs matrix number of columns
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    template <int tRowsRhs, int tColsRhs>
    inline matXSingle<T, tRows, tColsRhs> operator*(const matXSingle<T, tRowsRhs, tColsRhs>& rhs) const;


    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    inline matXSingle& operator+=(const matXSingle& rhs);


    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline T operator()(const U32 row, const U32 col) const;

    //! @brief Gets the number of rows
    //! @return Number of rows
    inline U32 Rows() const;

    //! @brief Gets the number of columns
    //! @return Number of columns
    inline U32 Cols() const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    inline std::array<T, tRows * tCols> Data() const;

    //! @brief Sets all matrix entries to zero
    inline void SetZero();

    // private:
    //    static constexpr std::array<T, tRows * tCols> RowMajorToColumnMajor(std::array<T, tRows * tCols> rowMaj);
};

} // namespace GDL
