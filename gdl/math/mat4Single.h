#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <iostream>

namespace GDL
{

//! @brief 4x4 Matrix
//! @tparam T: Data type
template <typename _type>
class Mat4Single
{

    std::array<_type, 16> mD;

public:
    //! @brief constructor
    inline Mat4Single();

    //! @brief constructor which initializes every entry on the main diagonal with the given value
    //! @param val: Value for initialization of every entry on the main diagonal
    inline explicit Mat4Single(_type val);

    //! @brief Constructor that initializes full matrix with specific values (column major)
    //! @param v0-v16: Matrix values in row major ordering
    inline Mat4Single(_type v0, _type v1, _type v2, _type v3, _type v4, _type v5, _type v6, _type v7, _type v8,
                      _type v9, _type v10, _type v11, _type v12, _type v13, _type v14, _type v15);

    //! @brief Copy Constructor
    //! @param other: Object that should be copied
    inline Mat4Single(const Mat4Single& other) = default;

    //! @brief Move Constructor
    //! @param other: Object that should be moved
    inline Mat4Single(Mat4Single&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    inline Mat4Single& operator=(const Mat4Single& other) = default;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    inline Mat4Single& operator=(Mat4Single&& other) = default;

    //! @brief Destructor
    inline ~Mat4Single() = default;

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline _type operator()(const U32 row, const U32 col) const;

    //! @brief Matrix - matrix addition assignment
    //! @param rhs: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat4Single& operator+=(const Mat4Single& rhs);

    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat4Single operator+(const Mat4Single& rhs);

    //! @brief Matrix - matrix multiplication
    //! @param other: Rhs matrix
    //! @return Result of the multiplication
    inline Mat4Single operator*(const Mat4Single& other) const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    inline const std::array<_type, 16> Data() const;
};



//! @brief Offstream operator
//! @tparam _type: Data type of the matrix
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const Mat4Single<_type>& mat);


} // namespace GDL



#include "gdl/math/mat4Single.inl"
