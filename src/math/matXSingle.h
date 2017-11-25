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
    std::array<T, tRows * tCols> mData;

public:
    matXSingle();

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
    matXSingle& operator+=(const matXSingle& rhs);
};

} // namespace GDL
