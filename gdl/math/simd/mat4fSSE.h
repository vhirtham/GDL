#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/base/simd/intrinsics.h"


#include <array>
#include <iostream>


namespace GDL
{

template <bool>
class Vec4fSSE;

//! @brief 4x4 Matrix with SSE support
class alignas(simd::alignmentBytes<__m128>) Mat4fSSE
{
    alignas(simd::alignmentBytes<__m128>) std::array<__m128, 4> mData;

public:
    inline Mat4fSSE();
    inline Mat4fSSE(const Mat4fSSE& other);
    inline Mat4fSSE(Mat4fSSE&& other) = default;
    inline Mat4fSSE& operator=(const Mat4fSSE& other) = default;
    inline Mat4fSSE& operator=(Mat4fSSE&& other) = default;
    inline ~Mat4fSSE() = default;

    //! @brief Constructor which initializes the matrix with the provided array
    //! @param data: Array containing the data
    inline explicit Mat4fSSE(std::array<F32, 16> data);

    //! @brief Constructor that initializes the full matrix with specific values (column major)
    //! @param v0-v15: Matrix values in column major ordering
    inline Mat4fSSE(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10, F32 v11,
                    F32 v12, F32 v13, F32 v14, F32 v15);

    //! @brief Constructor that initializes the full matrix with specific columns
    //! @param col0: first column
    //! @param col1: second column
    //! @param col2: third column
    //! @param col3: fourth column
    inline Mat4fSSE(__m128 col0, __m128 col1, __m128 col2, __m128 col3);

    //! @brief Direct access operator
    //! @param row: Row of the accessed value
    //! @param col: Column of the accessed value
    //! @return Accessed value
    [[nodiscard]] inline F32 operator()(const U32 row, const U32 col) const;

    //! @brief Compares if two matrices are equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator==(const Mat4fSSE& rhs) const;

    //! @brief Compares if two matrices are NOT equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Mat4fSSE& rhs) const;

    //! @brief Matrix - matrix addition assignment
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat4fSSE& operator+=(const Mat4fSSE& other);

    //! @brief Matrix - matrix addition
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    [[nodiscard]] inline Mat4fSSE operator+(const Mat4fSSE& other);

    //! @brief Matrix - matrix multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Mat4fSSE operator*(const Mat4fSSE& rhs) const;

    //! @brief Matrix - vector multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Vec4fSSE<true> operator*(const Vec4fSSE<true>& rhs) const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    [[nodiscard]] inline const std::array<F32, 16> Data() const;

    //! @brief Gets the underlying array of SSE registers
    //! @return Data array
    inline const std::array<__m128, 4>& DataSSE() const;

    //! @brief Calculates the determinant of the matrix
    //! @return Determinant of the matrix
    [[nodiscard]] inline F32 Det() const;

    //! @brief Returns the transposed matrix
    //! @return Transposed matrix
    [[nodiscard]] inline Mat4fSSE Transpose() const;



private:
    //! @brief Checks if the matrix internal data is aligned
    //! @return True / False
    inline bool IsDataAligned() const;
};



//! @brief Offstream operator
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
inline std::ostream& operator<<(std::ostream& os, const Mat4fSSE& mat);

} // namespace GDL


#include "gdl/math/simd/mat4fSSE.inl"
