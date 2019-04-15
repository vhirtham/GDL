#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"

#include <array>
#include <iostream>

namespace GDL
{

//! @brief Vector class with arbitrary number of elements
//! @tparam _type: Data type of the vector
//! @tparam _size: Number of rows
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
template <typename _type, I32 _size, bool _isCol = true>
class VecSSE
{

    static_assert(std::is_floating_point<_type>::value, "Matrix can only be created with floating point types");

public:
    using RegisterType = decltype(sse::GetFittingRegister<_type, sse::MaxRegisterSize()>());
    constexpr static U32 mAlignment = sse::alignmentBytes<RegisterType>;
    constexpr static U32 mNumRegisterEntries = sse::numRegisterValues<RegisterType>;
    constexpr static U32 mNumRegisters = sse::CalcMinNumArrayRegisters<RegisterType>(_size);

private:
    template <typename _type2, I32 _size2, bool _isCol2>
    friend class VecSSE;

    using DataArray = std::array<RegisterType, mNumRegisters>;

    alignas(mAlignment) DataArray mData;


public:
    inline VecSSE();
    inline VecSSE(const VecSSE& other) = default;
    inline VecSSE(VecSSE&& other) = default;
    inline VecSSE& operator=(const VecSSE& other) = default;
    inline VecSSE& operator=(VecSSE&& other) = default;
    inline ~VecSSE() = default;

    //! @brief Constructor to set the whole vector
    //! @tparam _args: Variadic data type
    //! @param args: Values
    template <typename... _args>
    explicit VecSSE(_args... args);

    //! @brief Constructor to set the whole vector
    //! @brief Array with values
    explicit VecSSE(const std::array<_type, _size>& data);

    //! @brief Constructor to set the whole vector
    //! @brief Array with values
    explicit VecSSE(const DataArray& data);

    //! @brief Direct access operator
    //! @param index: Index of the accessed value
    //! @return Accessed value
    [[nodiscard]] inline _type operator[](const U32 index) const;

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline std::array<_type, _size> Data() const;

    //! @brief Gets the underlying array of SSE registers
    //! @return Data array
    inline const DataArray& DataSSE() const;

private:
    //! @brief Checks if the vector was constructed as expected
    bool IsInternalDataValid() const;
};



//! @brief Offstream operator
//! @tparam _type: Data type of the vector
//! @tparam _size: Number of rows
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <typename _type, I32 _size, bool _isCol>
inline std::ostream& operator<<(std::ostream& os, const VecSSE<_type, _size, _isCol>& vec);


} // namespace GDL


#include "gdl/math/sse/vecSSE.inl"
