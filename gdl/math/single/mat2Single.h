#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <iostream>

namespace GDL
{

template <typename, bool>
class Vec2Single;

//! @brief 2x2 Matrix
//! @tparam T: Data type
template <typename _type>
class Mat2Single
{

    std::array<_type, 4> mD;

public:
    inline Mat2Single();
    inline Mat2Single(const Mat2Single& other) = default;
    inline Mat2Single(Mat2Single&& other) = default;
    inline Mat2Single& operator=(const Mat2Single& other) = default;
    inline Mat2Single& operator=(Mat2Single&& other) = default;
    inline ~Mat2Single() = default;

    //! @brief Constructor which initializes every entry on the main diagonal with the given value
    //! @param val: Value for initialization of every entry on the main diagonal
    inline explicit Mat2Single(_type val);

    //! @brief Constructor which initializes the matrix with the provided array
    //! @param data: Array containing the data
    inline explicit Mat2Single(std::array<_type, 4> data);

    //! @brief Constructor that initializes the full matrix with specific values (column major)
    //! @param v0-v3: Matrix values in column major ordering
    inline Mat2Single(_type v0, _type v1, _type v2, _type v3);

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
    [[nodiscard]] inline bool operator==(const Mat2Single& rhs) const;

    //! @brief Compares if two matrices are NOT equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Mat2Single& rhs) const;

    //! @brief Matrix - matrix addition assignment
    //! @param rhs: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat2Single& operator+=(const Mat2Single& rhs);

    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition (this)
    [[nodiscard]] inline Mat2Single operator+(const Mat2Single& rhs) const;

    //! @brief Matrix - matrix multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Mat2Single operator*(const Mat2Single& rhs) const;

    //! @brief Matrix - vector multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Vec2Single<_type, true> operator*(const Vec2Single<_type, true>& rhs) const;

    //! @brief Returns the transposed matrix
    //! @return Transposed matrix
    [[nodiscard]] inline Mat2Single Transpose() const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    [[nodiscard]] inline const std::array<_type, 4> Data() const;
};



//! @brief Offstream operator
//! @tparam _type: Data type of the matrix
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const Mat2Single<_type>& mat);


} // namespace GDL



#include "gdl/math/single/mat2Single.inl"
