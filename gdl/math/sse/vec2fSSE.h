#pragma once



#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/utility.h"

#include <array>
#include <iostream>


namespace GDL
{



//! @brief Vector class with 2 entries and SSE support
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
template <bool _isCol = true>
class alignas(sse::alignmentBytes<__m128>) Vec2fSSE
{
    template <bool>
    friend class Vec2fSSE;
    friend class Mat2fSSE;

    alignas(sse::alignmentBytes<__m128>) __m128 mData;

public:
    inline Vec2fSSE();
    inline Vec2fSSE(const Vec2fSSE& other);
    inline Vec2fSSE(Vec2fSSE&& other) = default;
    inline Vec2fSSE& operator=(const Vec2fSSE& other) = default;
    inline Vec2fSSE& operator=(Vec2fSSE&& other) = default;
    inline ~Vec2fSSE() = default;

    //! @brief Constructor which initializes the vector with the provided array
    //! @param data: Array containing the data
    inline explicit Vec2fSSE(std::array<F32, 2> data);

    //! @brief Constructor that initializes the vector with specific values
    //! @param v0-v1: Vector values
    inline Vec2fSSE(F32 v0, F32 v1);



private:
    //! @brief Constructor that initializes the vector
    //! @param data: Data
    inline Vec2fSSE(__m128 data);

public:
    //! @brief Direct access operator
    //! @param index: Index of the accessed value
    //! @return Accessed value
    [[nodiscard]] inline F32 operator[](const U32 index) const;

    //! @brief Compares if two vectors are equal
    //! @param rhs: Vector that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator==(const Vec2fSSE& rhs) const;

    //! @brief Compares if two vectors are NOT equal
    //! @param rhs: Vector that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Vec2fSSE& rhs) const;

    //! @brief Vector - vector addition assignment
    //! @param other: Rhs vector
    //! @return Result of the addition (this)
    inline Vec2fSSE& operator+=(const Vec2fSSE& rhs);

    //! @brief Vector - vector substraction assignment
    //! @param other: Rhs vector
    //! @return Result of the substraction (this)
    inline Vec2fSSE& operator-=(const Vec2fSSE& rhs);

    //! @brief Vector - vector addition
    //! @param other: Rhs vector
    //! @return Result of the addition
    [[nodiscard]] inline Vec2fSSE operator+(const Vec2fSSE& rhs) const;

    //! @brief Vector - vector substraction
    //! @param other: Rhs vector
    //! @return Result of the substraction
    [[nodiscard]] inline Vec2fSSE operator-(const Vec2fSSE& rhs) const;

    //! @brief Vector - scalar multiplication
    //! @param rhs: Rhs scalar
    //! @return Result of the multiplication
    [[nodiscard]] inline Vec2fSSE operator*(F32 rhs) const;

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline const std::array<F32, 2> Data() const;

    //! @brief Gets the data register
    //! @return Data
    inline __m128 DataSSE() const;

    //! @brief Calculates the dot product of two vectors
    //! @tparam _isColRhs: True if the rhs vector is a column vector, false otherwise
    //! @param rhs: Right hand side vector
    //! @return Dot product
    template <bool _isColRhs>
    [[nodiscard]] inline F32 Dot(Vec2fSSE<_isColRhs> rhs) const;

    //! @brief Calculates the length of the vector
    //! @return Length of the vector
    [[nodiscard]] inline F32 Length() const;

    //! @brief Normalizes the vector
    //! @return Reference to this
    Vec2fSSE& Normalize();

private:
    //! @brief Checks if the vectors internal data is aligned
    //! @return True / False
    inline bool IsDataAligned() const;
};



//! @brief Vector - scalar multiplication
//! @tparam _type: Data type of the vector
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
//! @param lhs: Lhs scalar
//! @param rhs: Rhs vector
//! @return Result of the multiplication
template <bool _isCol>
[[nodiscard]] inline Vec2fSSE<_isCol> operator*(F32 lhs, Vec2fSSE<_isCol> rhs);


//! @brief Offstream operator
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <bool _isCol = true>
inline std::ostream& operator<<(std::ostream& os, const Vec2fSSE<_isCol>& vec);

} // namespace GDL


#include "gdl/math/sse/vec2fSSE.inl"
