#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <iostream>


namespace GDL
{

//! @brief Vector class with 3 entries
//! @tparam _type: Data type of the vector
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
template <typename _type, bool _isCol = true>
class Vec3Single
{
    template <typename, bool>
    friend class Vec3Single;

    std::array<_type, 3> mData;

public:
    inline Vec3Single();
    inline Vec3Single(const Vec3Single& other) = default;
    inline Vec3Single(Vec3Single&& other) = default;
    inline Vec3Single& operator=(const Vec3Single& other) = default;
    inline Vec3Single& operator=(Vec3Single&& other) = default;
    inline ~Vec3Single() = default;

    //! @brief Constructor which initializes the vector with the provided array
    //! @param data: Array containing the data
    inline explicit Vec3Single(std::array<_type, 3> data);

    //! @brief Constructor that initializes the vector with specific values
    //! @param v0-v2: Vector values
    inline Vec3Single(_type v0, _type v1, _type v2);

public:
    //! @brief Compares if two vectors are equal
    //! @param rhs: Vector that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator==(const Vec3Single& rhs) const;

    //! @brief Compares if two vectors are NOT equal
    //! @param rhs: Vector that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const Vec3Single& rhs) const;

    //! @brief Direct access operator
    //! @param index: Index the accessed value
    //! @return Accessed value
    [[nodiscard]] inline _type operator[](const U32 index) const;

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline const std::array<_type, 3> Data() const;

    //! @brief Calculates the dot product of two vectors
    //! @tparam _isColRhs: True if the rhs vector is a column vector, false otherwise
    //! @param rhs: Right hand side vector
    //! @return Dot product
    template <bool _isColRhs>
    [[nodiscard]] inline F32 Dot(Vec3Single<_type, _isColRhs> rhs) const;

    //! @brief Calculates the length of the vector
    //! @return Length of the vector
    [[nodiscard]] inline F32 Length() const;

    //! @brief Normalizes the vector
    void Normalize();
};


template <bool _isCol>
using Vec3fSingle = Vec3Single<F32, _isCol>;

//! @brief Offstream operator
//! @tparam _type: Data type of the vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const Vec3Single<_type, true>& vec);

//! @brief Offstream operator
//! @tparam _type: Data type of the vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const Vec3Single<_type, false>& vec);

} // namespace GDL

#include "gdl/math/single/vec3Single.inl"
