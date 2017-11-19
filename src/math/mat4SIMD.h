#pragma once
#include "GDLTypedefs.h"

#include <x86intrin.h>

#ifndef NDEBUG
#include <iostream>
#endif

namespace GDL
{

bool is_aligned(const volatile void* p, size_t n)
{
    return reinterpret_cast<std::uintptr_t>(p) % n == 0;
}


class __attribute__((aligned(16))) mat4SIMD
{
    __attribute__((aligned(16))) __m128 mCol0;
    __attribute__((aligned(16))) __m128 mCol1;
    __attribute__((aligned(16))) __m128 mCol2;
    __attribute__((aligned(16))) __m128 mCol3;

public:
    //! @brief Constructor
    mat4SIMD();

    //! @brief Constructor that initializes full matrix with specific values (row major)
    //! @param v0-v16: Matrix values in row major ordering
    mat4SIMD(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10, F32 v11, F32 v12,
             F32 v13, F32 v14, F32 v15);

private:
    //! @brief Constructor that initializes full matrix with specific columns
    //! @param col0: first column
    //! @param col1: second column
    //! @param col2: third column
    //! @param col3: fourth column
    mat4SIMD(__m128 col0, __m128 col1, __m128 col2, __m128 col3);

public:
    //! @brief Copy constructor
    //! @param other: Object that should be copied
    mat4SIMD(const mat4SIMD& other);

    //! @brief Move constructor
    //! @param other: Object that should be moved
    mat4SIMD(mat4SIMD&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    mat4SIMD& operator=(const mat4SIMD& other) = default;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    mat4SIMD& operator=(mat4SIMD&& other) = default;

    //! @brief Destructor
    ~mat4SIMD() = default;

    //! @brief Matrix - matrix multiplication
    //! @param other: Rhs matrix
    //! @return Result of the multiplication
    inline mat4SIMD operator*(const mat4SIMD& other) const;

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline F32 operator()(const U32 row, const U32 col) const;


private:
#ifndef NDEBUG
    friend std::ostream& operator<<(std::ostream&, mat4SIMD const&);
#endif
};

typedef mat4SIMD mat4f;

} // namespace GDL
