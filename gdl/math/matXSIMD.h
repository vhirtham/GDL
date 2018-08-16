#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/exception.h"
#include "gdl/base/SSESupportFunctions.h"

#include <array>
#include <type_traits>


namespace GDL
{


//! @brief Matrix of arbitrary size with SIMD support
//! @tparam tRows: Number of rows
//! @tparam tCols: Number of columns
template <typename T, I32 tRows, I32 tCols>
class alignas(AlignmentBytes<__m128>) matXSIMD
// TODO: if register is a template parameter set corresponding alignment in the row above!
{
    typedef typename std::conditional<std::is_same<T, F32>::value, __m128, __m128d>::type __mx;
    constexpr static U32 mNumRegisterEntries = GetNumRegisterEntries<__mx, T>();
    constexpr static U32 mNumRegistersPerCol = CalcMinNumArrayRegisters(tRows, mNumRegisterEntries);
    constexpr static U32 mNumRegisters = tCols * mNumRegistersPerCol;


    alignas(AlignmentBytes<__mx>) std::array<__mx, mNumRegisters> mData;

    template <typename T2, I32 tRows2, I32 tCols2>
    friend class matXSIMD;

public:
    //! @brief constructor
    matXSIMD();

    //! @brief Constructor to set the whole matrix
    //! @param args: Values (column major)
    //! @tparam Args: Variadic data type
    template <typename... Args>
    explicit matXSIMD(Args... args);


    //! @brief Constructor to set the whole matrix
    //! @brief Array with values (column major)
    explicit matXSIMD(const std::array<T, tRows * tCols>& data);

    //! @brief Matrix - matrix multiplication
    //! @tparam tRowsRhs: Rhs matrix number of rows
    //! @tparam tColsRhs: Rhs matrix number of columns
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    template <I32 tRowsRhs, I32 tColsRhs>
    inline matXSIMD<T, tRows, tColsRhs> operator*(const matXSIMD<T, tRowsRhs, tColsRhs>& rhs) const;

    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    inline matXSIMD& operator+=(const matXSIMD& rhs);

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

    //! @brief Sets all matrix entries to zero
    inline void SetZero();

    //! @brief Gets the data array in column major ordering
    //! @return Data
    inline const std::array<T, tRows * tCols> Data() const;

private:
    void ConstructionChecks() const;
};
} // namespace GDL
