#pragma once

#include "gdl/GDLTypedefs.h"

#include <array>

#ifndef NDEBUG
#include <iostream>
#endif

namespace GDL
{

//! @brief 4x4 Matrix
//! @tparam T: Data type
template <class T>
class mat4Single
{

    std::array<T, 16> mA;

public:
    //! @brief constructor
    inline mat4Single();

    //! @brief constructor which initializes every entry on the main diagonal with the given value
    //! @param val: Value for initialization of every entry on the main diagonal
    inline explicit mat4Single(T val);

    //! @brief Constructor that initializes full matrix with specific values (column major)
    //! @param v0-v16: Matrix values in row major ordering
    inline mat4Single(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14,
                      T v15);

    //! @brief Copy Constructor
    //! @param other: Object that should be copied
    inline mat4Single(const mat4Single& other) = default;

    //! @brief Move Constructor
    //! @param other: Object that should be moved
    inline mat4Single(mat4Single&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    inline mat4Single& operator=(const mat4Single& other) = default;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    inline mat4Single& operator=(mat4Single&& other) = default;

    //! @brief Destructor
    inline ~mat4Single() = default;


    //! @brief Matrix - matrix multiplication
    //! @param other: Rhs matrix
    //! @return Result of the multiplication
    inline mat4Single operator*(const mat4Single& other) const;

    //! @brief Matrix - matrix addition
    //! @param rhs: Rhs matrix
    //! @return Result of the addition (this)
    inline mat4Single& operator+=(const mat4Single& rhs);

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline T operator()(const U32 row, const U32 col) const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    inline const std::array<T, 16> Data() const;

private:
#ifndef NDEBUG
    template <typename T2>
    friend std::ostream& operator<<(std::ostream&, mat4Single<T2> const&);
#endif
};
} // namespace GDL
