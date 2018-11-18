#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"
#include "gdl/base/sse/intrinsics.h"

#include <x86intrin.h>
#include <array>
#include <cmath> // Temporary as long as rotation matrix is stored here!
#include <iostream>


namespace GDL
{

//! @brief 4x4 Matrix with SSE support
class alignas(sse::alignmentBytes<__m128>) Mat4SSE
{
    alignas(sse::alignmentBytes<__m128>) std::array<__m128, 4> mData;

public:
    inline Mat4SSE();
    inline Mat4SSE(const Mat4SSE& other);
    inline Mat4SSE(Mat4SSE&& other) = default;
    inline Mat4SSE& operator=(const Mat4SSE& other) = default;
    inline Mat4SSE& operator=(Mat4SSE&& other) = default;
    inline ~Mat4SSE() = default;

    //! @brief Constructor which initializes the matrix with the provided array
    //! @param data: Array containing the data
    inline explicit Mat4SSE(std::array<F32, 16> data);

    //! @brief Constructor that initializes full matrix with specific values (column major)
    //! @param v0-v15: Matrix values in column major ordering
    inline Mat4SSE(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10, F32 v11,
                    F32 v12, F32 v13, F32 v14, F32 v15);

private:
    //! @brief Constructor that initializes full matrix with specific columns
    //! @param col0: first column
    //! @param col1: second column
    //! @param col2: third column
    //! @param col3: fourth column
    inline Mat4SSE(__m128 col0, __m128 col1, __m128 col2, __m128 col3);

public:
    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    inline F32 operator()(const U32 row, const U32 col) const;

    //! @brief Compares if two matrices are equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator==(const Mat4SSE& rhs) const;

    //! @brief Compares if two matrices are NOT equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Mat4SSE& rhs) const;

    //! @brief Matrix - matrix addition assignment
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat4SSE& operator+=(const Mat4SSE& other);

    //! @brief Matrix - matrix addition
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    [[nodiscard]] inline Mat4SSE operator+(const Mat4SSE& other);

    //! @brief Matrix - matrix multiplication
    //! @param other: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Mat4SSE operator*(const Mat4SSE& other) const;

    //! @brief Returns the transposed matrix
    //! @return Transposed matrix
    [[nodiscard]] inline Mat4SSE Transpose() const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    [[nodiscard]] inline const std::array<F32, 16> Data() const;


    //! @brief Rotation matrix for a rotation around the z-axis
    //! @param angle: Rotation angle in rad
    //! @return Rotation matrix
    static Mat4SSE RotationMatrixZ(F32 angle)
    {
        F32 sinAngle = std::sin(angle);
        F32 cosAngle = std::cos(angle);
        // clang-format off
        return Mat4SSE(
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
inline std::ostream& operator<<(std::ostream& os, const Mat4SSE& mat);

} // namespace GDL


#include "gdl/math/mat4SSE.inl"
