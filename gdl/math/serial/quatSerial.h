#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <iostream>


namespace GDL
{

//! @brief Quaternion class with x, y, z, w ordering
//! @tparam _type: Data type of the quaternion
template <typename _type>
class QuatSerial
{
    std::array<_type, 4> mData;

public:
    inline QuatSerial();
    inline QuatSerial(const QuatSerial& other) = default;
    inline QuatSerial(QuatSerial&& other) = default;
    inline QuatSerial& operator=(const QuatSerial& other) = default;
    inline QuatSerial& operator=(QuatSerial&& other) = default;
    inline ~QuatSerial() = default;

    //! @brief Constructor which initializes the quaternion with the provided array
    //! @param data: Array containing the data
    inline explicit QuatSerial(std::array<_type, 4> data);

    //! @brief Constructor that initializes the quaternion with specific values
    //! @param x, y, z, w: Quaternion values
    inline QuatSerial(_type x, _type y, _type z, _type w);

public:
    //! @brief Direct access operator
    //! @param index: Index of the accessed value
    //! @return Accessed value
    [[nodiscard]] inline _type operator[](const U32 index) const;

    //! @brief Compares if two quaternions are equal
    //! @param rhs: Quaternion that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator==(const QuatSerial& rhs) const;

    //! @brief Compares if two quaternions are NOT equal
    //! @param rhs: Quaternion that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const QuatSerial& rhs) const;

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline const std::array<_type, 4> Data() const;

};

using QuatfSerial = QuatSerial<F32>;




//! @brief Offstream operator
//! @tparam _type: Data type of the quaternion
//! @param os: Reference to offstream object
//! @param quat: Quaternion
//! @return Reference to offstream object
template <typename _type>
inline std::ostream& operator<<(std::ostream& os, const QuatSerial<_type>& quat);



} // namespace GDL

#include "gdl/math/serial/quatSerial.inl"
