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
    constexpr static U32 mNumRegisterEntries = GetNumRegisterEntries<__mx, _type>();
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
    void ConstructionChecks() const;
};
} // namespace GDL
