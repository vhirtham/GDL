#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <iostream>


namespace GDL
{

template <typename, bool>
class Vec4Serial;

//! @brief Vector class with 3 entries
//! @tparam _type: Data type of the vector
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
template <typename _type, bool _isCol = true>
class Vec3Serial
{
    template <typename, bool>
    friend class Vec3Serial;
    template <typename, bool>
    friend class Vec4Serial;
    template <typename>
    friend class Mat3Serial;

    std::array<_type, 3> mData;

public:
    inline Vec3Serial();
    inline Vec3Serial(const Vec3Serial& other) = default;
    inline Vec3Serial(Vec3Serial&& other) = default;
    inline Vec3Serial& operator=(const Vec3Serial& other) = default;
    inline Vec3Serial& operator=(Vec3Serial&& other) = default;
    inline ~Vec3Serial() = default;

    //! @brief Constructor which initializes the vector with the provided array
    //! @param data: Array containing the data
    inline explicit Vec3Serial(std::array<_type, 3> data);

    //! @brief Constructor that initializes the vector with specific values
    //! @param v0-v2: Vector values
    inline Vec3Serial(_type v0, _type v1, _type v2);

    //! @brief Constructor that initializes the vector with a Vec4. The last value of the vec 4 is dropped.
    //! @param other: Other Vector
    inline Vec3Serial(const Vec4Serial<_type, _isCol>& other);

public:
    //! @brief Direct access operator
    //! @param index: Index the accessed value
    //! @return Accessed value
    [[nodiscard]] inline _type operator[](const U32 index) const;

    //! @brief Compares if two vectors are equal
    //! @param rhs: Vector that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator==(const Vec3Serial& rhs) const;

    //! @brief Compares if two vectors are NOT equal
    //! @param rhs: Vector that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Vec3Serial& rhs) const;

    //! @brief Vector - vector addition assignment
    //! @param rhs: Rhs vector
    //! @return Result of the addition (this)
    inline Vec3Serial& operator+=(const Vec3Serial& rhs);

    //! @brief Vector - vector substraction assignment
    //! @param rhs: Rhs vector
    //! @return Result of the substraction (this)
    inline Vec3Serial& operator-=(const Vec3Serial& rhs);

    //! @brief Vector - vector addition
    //! @param rhs: Rhs vector
    //! @return Result of the addition
    [[nodiscard]] inline Vec3Serial operator+(const Vec3Serial& rhs) const;

    //! @brief Vector - vector substraction
    //! @param rhs: Rhs vector
    //! @return Result of the substraction
    [[nodiscard]] inline Vec3Serial operator-(const Vec3Serial& rhs) const;

    //! @brief Vector - scalar multiplication
    //! @param rhs: Rhs scalar
    //! @return Result of the multiplication
    [[nodiscard]] inline Vec3Serial operator*(F32 rhs) const;

    //! @brief Calculates the cross product of two vectors
    //! @tparam _isColRhs: True if the rhs vector is a column vector, false otherwise
    //! @param rhs: Right hand side vector
    //! @return Cross product
    [[nodiscard]] inline Vec3Serial Cross(Vec3Serial rhs) const;

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline const std::array<_type, 3> Data() const;

    //! @brief Calculates the dot product of two vectors
    //! @tparam _isColRhs: True if the rhs vector is a column vector, false otherwise
    //! @param rhs: Right hand side vector
    //! @return Dot product
    template <bool _isColRhs>
    [[nodiscard]] inline F32 Dot(Vec3Serial<_type, _isColRhs> rhs) const;

    //! @brief Calculates the length of the vector
    //! @return Length of the vector
    [[nodiscard]] inline F32 Length() const;

    //! @brief Normalizes the vector
    //! @return Reference to this
    Vec3Serial& Normalize();
};


template <bool _isCol>
using Vec3fSerial = Vec3Serial<F32, _isCol>;



//! @brief Vector - scalar multiplication
//! @tparam _type: Data type of the vector
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
//! @param lhs: Lhs scalar
//! @param rhs: Rhs vector
//! @return Result of the multiplication
template <typename _type, bool _isCol>
[[nodiscard]] inline Vec3Serial<_type, _isCol> operator*(F32 lhs, Vec3Serial<_type, _isCol> rhs);

//! @brief Offstream operator
//! @tparam _type: Data type of the vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const Vec3Serial<_type, true>& vec);

//! @brief Offstream operator
//! @tparam _type: Data type of the vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const Vec3Serial<_type, false>& vec);

} // namespace GDL

#include "gdl/math/serial/vec3Serial.inl"
