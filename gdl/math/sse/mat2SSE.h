#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/math/sse/vec2SSE.h"


#include <array>
#include <iostream>


namespace GDL
{

template <bool>
class Vec2SSE;

//! @brief 2x2 Matrix with SSE support
class alignas(sse::alignmentBytes<__m128>) Mat2SSE
{
    alignas(sse::alignmentBytes<__m128>) __m128 mData;

public:
    inline Mat2SSE();
    inline Mat2SSE(const Mat2SSE& other);
    inline Mat2SSE(Mat2SSE&& other) = default;
    inline Mat2SSE& operator=(const Mat2SSE& other) = default;
    inline Mat2SSE& operator=(Mat2SSE&& other) = default;
    inline ~Mat2SSE() = default;

    //! @brief Constructor which initializes the matrix with the provided array
    //! @param data: Array containing the data
    inline explicit Mat2SSE(std::array<F32, 4> data);

    //! @brief Constructor that initializes the full matrix with specific values (column major)
    //! @param v0-v3: Matrix values in column major ordering
    inline Mat2SSE(F32 v0, F32 v1, F32 v2, F32 v3);

private:
    //! @brief Constructor that initializes the matrix with a register
    //! @param reg: first column
    inline Mat2SSE(__m128 reg);

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
    [[nodiscard]] inline bool operator==(const Mat2SSE& rhs) const;

    //! @brief Compares if two matrices are NOT equal
    //! @param rhs: Matrix that should be compared
    //! @return TRUE/FALSE
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Mat2SSE& rhs) const;

    //! @brief Matrix - matrix addition assignment
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    inline Mat2SSE& operator+=(const Mat2SSE& other);

    //! @brief Matrix - matrix addition
    //! @param other: Rhs matrix
    //! @return Result of the addition (this)
    [[nodiscard]] inline Mat2SSE operator+(const Mat2SSE& other);

    //! @brief Matrix - matrix multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Mat2SSE operator*(const Mat2SSE& rhs) const;

    //! @brief Matrix - vector multiplication
    //! @param rhs: Rhs matrix
    //! @return Result of the multiplication
    [[nodiscard]] inline Vec2SSE<true> operator*(const Vec2SSE<true>& rhs) const;

    //! @brief Returns the transposed matrix
    //! @return Transposed matrix
    [[nodiscard]] inline Mat2SSE Transpose() const;

    //! @brief Gets the data array in column major ordering
    //! @return Data
    [[nodiscard]] inline const std::array<F32, 4> Data() const;

private:
    //! @brief Checks if the matrix internal data is aligned
    //! @return True / False
    inline bool IsDataAligned() const;
};



//! @brief Offstream operator
//! @param os: Reference to offstream object
//! @param mat: Matrix
//! @return Reference to offstream object
inline std::ostream& operator<<(std::ostream& os, const Mat2SSE& mat);

} // namespace GDL


#include "gdl/math/sse/mat2SSE.inl"
