#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>

namespace GDL
{

//! @brief Vector class with arbitrary number of elements
//! @tparam _type: Data type of the vector
//! @tparam _size: Number of rows
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
template <typename _type, I32 _size, bool _isCol = true>
class VecSerial
{
    template <typename _type2, I32 _size2, bool _isCol2>
    friend class VecSerial;

    std::array<_type, _size> mData;

public:
    inline VecSerial();
    inline VecSerial(const VecSerial& other) = default;
    inline VecSerial(VecSerial&& other) = default;
    inline VecSerial& operator=(const VecSerial& other) = default;
    inline VecSerial& operator=(VecSerial&& other) = default;
    inline ~VecSerial() = default;

    //! @brief Constructor to set the whole vector
    //! @tparam _args: Variadic data type
    //! @param args: Values
    template <typename... _args>
    explicit VecSerial(_args... args);

    //! @brief Constructor to set the whole vector
    //! @param data: Array with values
    explicit VecSerial(const std::array<_type, _size>& data);

    //! @brief Direct access operator
    //! @param index: Index of the accessed value
    //! @return Accessed value
    [[nodiscard]] inline _type operator[](const U32 index) const;

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline const std::array<_type, _size>& Data() const;
};

//! @brief Offstream operator
//! @tparam _type: Data type of the vector
//! @tparam _size: Number of rows
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <typename _type, I32 _size, bool _isCol>
inline std::ostream& operator<<(std::ostream& os, const VecSerial<_type, _size, _isCol>& vec);



} // namespace GDL


#include "gdl/math/serial/vecSerial.inl"
