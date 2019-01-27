#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <iostream>


namespace GDL
{


//! @brief Vector class with 2 entries
//! @tparam _type: Data type of the vector
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
template <typename _type, bool _isCol = true>
class Vec2Single
{
    template <typename, bool>
    friend class Vec2Single;
    template <typename>
    friend class Mat2Single;

    std::array<_type, 2> mData;

public:
    inline Vec2Single();
    inline Vec2Single(const Vec2Single& other) = default;
    inline Vec2Single(Vec2Single&& other) = default;
    inline Vec2Single& operator=(const Vec2Single& other) = default;
    inline Vec2Single& operator=(Vec2Single&& other) = default;
    inline ~Vec2Single() = default;

    //! @brief Constructor which initializes the vector with the provided array
    //! @param data: Array containing the data
    inline explicit Vec2Single(std::array<_type, 2> data);

    //! @brief Constructor that initializes the vector with specific values
    //! @param v0-v2: Vector values
    inline Vec2Single(_type v0, _type v1);


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
    [[nodiscard]] inline bool operator==(const Vec2Single& rhs) const;

    //! @brief Compares if two vectors are NOT equal
    //! @param rhs: Vector that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Vec2Single& rhs) const;

    //! @brief Vector - vector addition assignment
    //! @param rhs: Rhs vector
    //! @return Result of the addition (this)
    inline Vec2Single& operator+=(const Vec2Single& rhs);

    //! @brief Vector - vector substraction assignment
    //! @param rhs: Rhs vector
    //! @return Result of the substraction (this)
    inline Vec2Single& operator-=(const Vec2Single& rhs);

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline const std::array<_type, 2> Data() const;

    //! @brief Calculates the dot product of two vectors
    //! @tparam _isColRhs: True if the rhs vector is a column vector, false otherwise
    //! @param rhs: Right hand side vector
    //! @return Dot product
    template <bool _isColRhs>
    [[nodiscard]] inline F32 Dot(Vec2Single<_type, _isColRhs> rhs) const;

    //! @brief Calculates the length of the vector
    //! @return Length of the vector
    [[nodiscard]] inline F32 Length() const;

    //! @brief Normalizes the vector
    //! @return Reference to this
    Vec2Single& Normalize();
};


template <bool _isCol>
using Vec2fSingle = Vec2Single<F32, _isCol>;

//! @brief Offstream operator
//! @tparam _type: Data type of the vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const Vec2Single<_type, true>& vec);

//! @brief Offstream operator
//! @tparam _type: Data type of the vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const Vec2Single<_type, false>& vec);

} // namespace GDL

#include "gdl/math/single/vec2Single.inl"
