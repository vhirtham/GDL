#pragma once

#ifdef __AVX2__

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"
#include "gdl/base/sse/intrinsics.h"


#include <array>
#include <iostream>

namespace GDL
{

template <bool>
class Vec4fSSE;

//! @brief 4x4 Matrix with SSE support
class alignas(sse::alignmentBytes<__m256>) Mat4fAVX
{
    alignas(sse::alignmentBytes<__m256>) std::array<__m256, 2> mData;

public:
    inline Mat4fAVX();
    inline Mat4fAVX(const Mat4fAVX& other);
    inline Mat4fAVX(Mat4fAVX&& other) = default;
    inline Mat4fAVX& operator=(const Mat4fAVX& other) = default;
    inline Mat4fAVX& operator=(Mat4fAVX&& other) = default;
    inline ~Mat4fAVX() = default;

    //! @brief Constructor which initializes the matrix with the provided array
    //! @param data: Array containing the data
    inline explicit Mat4fAVX(std::array<F32, 16> data);

    //! @brief Constructor that initializes the full matrix with specific values (column major)
    //! @param v0-v15: Matrix values in column major ordering
    inline Mat4fAVX(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10, F32 v11,
                    F32 v12, F32 v13, F32 v14, F32 v15);

private:
    //! @brief Constructor that initializes the full matrix with two registers
    //! @param col01: first 2 columns
    //! @param col23: second 2 columns
    inline Mat4fAVX(__m256 col01, __m256 col23);

public:
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
    [[nodiscard]] inline bool operator==(const Mat4fAVX& rhs) const;

    //! @brief Compares if two matrices are NOT equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Mat4fAVX& rhs) const;

    //! @brief Matrix - matrix addition assignment
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat4fAVX& operator+=(const Mat4fAVX& other);

    //! @brief Matrix - matrix addition
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    [[nodiscard]] inline Mat4fAVX operator+(const Mat4fAVX& other);

    //! @brief Matrix - matrix multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Mat4fAVX operator*(const Mat4fAVX& rhs) const;

    //! @brief Matrix - vector multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Vec4fSSE<true> operator*(const Vec4fSSE<true>& rhs) const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    [[nodiscard]] inline const std::array<F32, 16> Data() const;

    //! @brief Calculates the determinant of the matrix
    //! @return Determinant of the matrix
    [[nodiscard]] inline F32 Det() const;

    //! @brief Returns the transposed matrix
    //! @return Transposed matrix
    [[nodiscard]] inline Mat4fAVX Transpose() const;



private:
    //! @brief Checks if the matrix internal data is aligned
    //! @return True / False
    inline bool IsDataAligned() const;
};



//! @brief Offstream operator
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
inline std::ostream& operator<<(std::ostream& os, const Mat4fAVX& mat);

} // namespace GDL


#include "gdl/math/sse/mat4fAVX.inl"

#endif //__AVX2__
