#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/sse.h"

#include <x86intrin.h>
#include <array>
#include <cmath> // Temporary as long as rotation matrix is stored here!
#include <iostream>


namespace GDL
{

//! @brief 4x4 Matrix with SSE support
class alignas(16) Mat4SIMD
{
    alignas(16) std::array<__m128, 4> mData;

public:
    //! @brief Constructor
    inline Mat4SIMD();

    //! @brief Constructor that initializes full matrix with specific values (column major)
    //! @param v0-v15: Matrix values in row major ordering
    inline Mat4SIMD(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10, F32 v11,
                    F32 v12, F32 v13, F32 v14, F32 v15);

private:
    //! @brief Constructor that initializes full matrix with specific columns
    //! @param col0: first column
    //! @param col1: second column
    //! @param col2: third column
    //! @param col3: fourth column
    inline Mat4SIMD(__m128 col0, __m128 col1, __m128 col2, __m128 col3);

public:
    //! @brief Copy constructor
    //! @param other: Object that should be copied
    inline Mat4SIMD(const Mat4SIMD& other);

    //! @brief Move constructor
    //! @param other: Object that should be moved
    inline Mat4SIMD(Mat4SIMD&& other) = default;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    inline Mat4SIMD& operator=(const Mat4SIMD& other) = default;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    inline Mat4SIMD& operator=(Mat4SIMD&& other) = default;

    //! @brief Destructor
    inline ~Mat4SIMD() = default;

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline F32 operator()(const U32 row, const U32 col) const;

    //! @brief Matrix - matrix addition assignment
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat4SIMD& operator+=(const Mat4SIMD& other);

    //! @brief Matrix - matrix addition
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat4SIMD operator+(const Mat4SIMD& other);

    //! @brief Matrix - matrix multiplication
    //! @param other: Rhs matrix
    //! @return Result of the multiplication
    inline Mat4SIMD operator*(const Mat4SIMD& other) const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    inline const std::array<F32, 16> Data() const;


    //! @brief Rotation matrix for a rotation around the z-axis
    //! @param angle: Rotation angle in rad
    //! @return Rotation matrix
    static Mat4SIMD RotationMatrixZ(F32 angle)
    {
        F32 sinAngle = std::sin(angle);
        F32 cosAngle = std::cos(angle);
        // clang-format off
        return Mat4SIMD(
             cosAngle, sinAngle,       0.,       0.,
            -sinAngle, cosAngle,       0.,       0.,
                   0.,       0.,       1.,       0.,
                   0.,       0.,       0.,       1.);
        // clang-format on
    }

private:
    //! @brief Checks if the matrix was constructed as expected
    inline void ConstructionChecks() const;
};



//! @brief Offstream operator
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
inline std::ostream& operator<<(std::ostream& os, const Mat4SIMD& mat);

} // namespace GDL


#include "gdl/math/mat4SIMD.inl"
