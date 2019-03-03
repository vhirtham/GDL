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
class VecSingle
{
    template <typename _type2, I32 _size2, bool _isCol2>
    friend class VecSingle;

    std::array<_type, _size> mData;

public:
    inline VecSingle();
    inline VecSingle(const VecSingle& other) = default;
    inline VecSingle(VecSingle&& other) = default;
    inline VecSingle& operator=(const VecSingle& other) = default;
    inline VecSingle& operator=(VecSingle&& other) = default;
    inline ~VecSingle() = default;

    //! @brief Constructor to set the whole vector
    //! @tparam _args: Variadic data type
    //! @param args: Values
    template <typename... _args>
    explicit VecSingle(_args... args);

    //! @brief Constructor to set the whole vector
    //! @param data: Array with values
    explicit VecSingle(const std::array<_type, _size>& data);

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline const std::array<_type, _size>& Data() const;
};

} // namespace GDL


#include "gdl/math/single/vecSingle.inl"
