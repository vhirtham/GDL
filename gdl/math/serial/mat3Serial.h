#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <iostream>

namespace GDL
{

template <typename, bool>
class Vec3Serial;


//! @brief 3x3 Matrix
//! @tparam T: Data type
template <typename _type>
class Mat3Serial
{

    std::array<_type, 9> mD;

public:
    inline Mat3Serial();
    inline Mat3Serial(const Mat3Serial& other) = default;
    inline Mat3Serial(Mat3Serial&& other) = default;
    inline Mat3Serial& operator=(const Mat3Serial& other) = default;
    inline Mat3Serial& operator=(Mat3Serial&& other) = default;
    inline ~Mat3Serial() = default;

    //! @brief Constructor which initializes every entry on the main diagonal with the given value
    //! @param val: Value for initialization of every entry on the main diagonal
    inline explicit Mat3Serial(_type val);

    //! @brief Constructor which initializes the matrix with the provided array
    //! @param data: Array containing the data
    inline explicit Mat3Serial(std::array<_type, 9> data);

    //! @brief Constructor that initializes the full matrix with specific values (column major)
    //! @param v0-v8: Matrix values in column major ordering
    inline Mat3Serial(_type v0, _type v1, _type v2, _type v3, _type v4, _type v5, _type v6, _type v7, _type v8);

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
    [[nodiscard]] inline bool operator==(const Mat3Serial& rhs) const;

    //! @brief Compares if two matrices are NOT equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Mat3Serial& rhs) const;

    //! @brief Matrix - matrix addition assignment
    //! @param rhs: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat3Serial& operator+=(const Mat3Serial& rhs);

    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition (this)
    [[nodiscard]] inline Mat3Serial operator+(const Mat3Serial& rhs) const;

    //! @brief Matrix - matrix multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Mat3Serial operator*(const Mat3Serial& rhs) const;

    //! @brief Matrix - vector multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Vec3Serial<_type, true> operator*(const Vec3Serial<_type, true>& rhs) const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    [[nodiscard]] inline const std::array<_type, 9> Data() const;

    //! @brief Calculates the determinant of the matrix
    //! @return Determinant of the matrix
    [[nodiscard]] inline F32 Det() const;

    //! @brief Returns the transposed matrix
    //! @return Transposed matrix
    [[nodiscard]] inline Mat3Serial Transpose() const;
};


using Mat3fSerial = Mat3Serial<F32>;
using Mat3dSerial = Mat3Serial<F64>;



//! @brief Offstream operator
//! @tparam _type: Data type of the matrix
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const Mat3Serial<_type>& mat);


} // namespace GDL



#include "gdl/math/serial/mat3Serial.inl"
