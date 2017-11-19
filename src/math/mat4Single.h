#pragma once

#include "GDLTypedefs.h"

#include <array>

#ifndef NDEBUG
#include <iostream>
#endif

namespace GDL
{


template <class T>
class mat4Single
{

    std::array<T, 16> mA;

public:
    //! @brief constructor
    mat4Single();

    //! @brief constructor which initializes every entry on the main diagonal with the given value
    //! @param val: Value for initialization of every entry on the main diagonal
    explicit mat4Single(T val);

    //! @brief constructor which initializes the whole matrix with specific values
    //! @param val: Value array (column major ordering)
    mat4Single( std::array<T, 16> val);

    //! @brief Copy Constructor
    //! @param other: Object that should be copied
    mat4Single(const mat4Single& other) = default;

    //! @brief Move Constructor
    //! @param other: Object that should be moved
    mat4Single(mat4Single&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    mat4Single& operator=(const mat4Single& other) = default;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    mat4Single& operator=(mat4Single&& other) = default;

    //! @brief Destructor
    ~mat4Single() = default;


    //! @brief Matrix - matrix multiplication
    //! @param other: Rhs matrix
    //! @return Result of the multiplication
    inline mat4Single operator*(const mat4Single& other) const;

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline F32 operator()(const U32 row, const U32 col) const;

private:
#ifndef NDEBUG
    template <typename T2>
    friend std::ostream& operator<<(std::ostream&, mat4Single<T2> const&);
#endif
};
} // namespace GDL
