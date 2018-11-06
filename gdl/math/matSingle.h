#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>

namespace GDL
{


//! @brief Matrix of arbitrary size
//! @tparam T: Data type
//! @tparam _rows: Number of rows
//! @tparam _cols: Number of columns
template <typename _type, I32 _rows, I32 _cols>
class MatSingle
{
    template <typename _type2, I32 _rows2, I32 _cols2>
    friend class MatSingle;

    std::array<_type, _rows * _cols> mData;

public:
    //! @brief Constructor
    MatSingle();

    //! @brief Constructor to set the whole matrix
    //! @param args: Values (column major)
    //! @tparam Args: Variadic data type
    template <typename... Args>
    explicit MatSingle(Args... args);

    //! @brief Constructor to set the whole matrix
    //! @param data: Array with values (column major)
    explicit MatSingle(const std::array<_type, _rows * _cols>& data);

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
    [[nodiscard]] inline bool operator==(const MatSingle& rhs) const;

    //! @brief Compares if two matrices are NOT equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const MatSingle& rhs) const;

    //! @brief Matrix - matrix addition assignment
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    inline MatSingle& operator+=(const MatSingle& rhs);

    //! @brief Matrix - matrix addition assignment
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    [[nodiscard]] inline MatSingle operator+(const MatSingle& rhs);

    //! @brief Matrix - matrix multiplication
    //! @tparam _rowsRhs: Rhs matrix number of rows
    //! @tparam _colsRhs: Rhs matrix number of columns
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    template <I32 _rowsRhs, I32 _colsRhs>
    [[nodiscard]] inline MatSingle<_type, _rows, _colsRhs>
    operator*(const MatSingle<_type, _rowsRhs, _colsRhs>& rhs) const;

    //! @brief Gets the number of rows
    //! @return Number of rows
    [[nodiscard]] inline U32 Rows() const;

    //! @brief Gets the number of columns
    //! @return Number of columns
    [[nodiscard]] inline U32 Cols() const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    [[nodiscard]] inline std::array<_type, _rows * _cols> Data() const;

    //! @brief Sets all matrix entries to zero
    inline void SetZero();
};



//! @brief Offstream operator
//! @tparam _type: Data type of the matrix
//! @tparam _rows: Number of rows of the matrix
//! @tparam _cols: Number of columns of the matrix
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
template <typename _type, I32 _rows, I32 _cols>
std::ostream& operator<<(std::ostream& os, const MatSingle<_type, _rows, _cols>& mat);



} // namespace GDL



#include "gdl/math/matSingle.inl"
