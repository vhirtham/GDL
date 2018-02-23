#pragma once
#if defined __GNUC__ && __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

#include "GDLTypedefs.h"
#include "base/SSESupportFunctions.h"

#include <array>


namespace GDL
{

//! @brief Matrix of arbitrary size with SIMD support
//! @tparam tRows: Number of rows
//! @tparam tCols: Number of columns
template <int tRows, int tCols>
class __attribute__((aligned(16))) matXSIMD
// TODO: if register is a template parameter set corresponding alignment in the row above!
{
    typedef __m128 __mx;
    constexpr static U32 mNumRegisters = tCols * CalcMinNumArrayRegisters(tRows, GetNumRegisterEntries<__mx, F32>());

    __attribute__((aligned(16))) std::array<__mx, mNumRegisters> mData;

    template <int tRows2, int tCols2>
    friend class matXSIMD;

public:
    //! @brief constructor
    matXSIMD();

    //! @brief Constructor to set the whole matrix
    //! @param args: Values (column major)
    //! @tparam Args: Variadic data type
    template <typename... Args>
    matXSIMD(Args... args);


    //! @brief Constructor to set the whole matrix
    //! @brief Array with values (column major)
    matXSIMD(const std::array<F32, tRows * tCols>& data);

    //! @brief Matrix - matrix multiplication
    //! @tparam tRowsRhs: Rhs matrix number of rows
    //! @tparam tColsRhs: Rhs matrix number of columns
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    template <int tRowsRhs, int tColsRhs>
    inline matXSIMD<tRows, tColsRhs> operator*(const matXSIMD<tRowsRhs, tColsRhs>& rhs) const;

    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition
    inline matXSIMD& operator+=(const matXSIMD& rhs);

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline F32 operator()(const U32 row, const U32 col) const;

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
    inline const std::array<F32, tRows * tCols> Data() const;

private:
    void ConstructionChecks() const;
};
} // namespace GDL
