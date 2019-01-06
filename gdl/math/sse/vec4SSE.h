#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"
#include "gdl/base/sse/intrinsics.h"

#include <array>
#include <iostream>


namespace GDL
{

//! @brief Vector class with 4 entries and SSE support
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
template <bool _isCol = true>
class alignas(sse::alignmentBytes<__m128>) Vec4SSE
{
    alignas(sse::alignmentBytes<__m128>) __m128 mData;

public:
    inline Vec4SSE();
    inline Vec4SSE(const Vec4SSE& other);
    inline Vec4SSE(Vec4SSE&& other) = default;
    inline Vec4SSE& operator=(const Vec4SSE& other) = default;
    inline Vec4SSE& operator=(Vec4SSE&& other) = default;
    inline ~Vec4SSE() = default;

    //! @brief Constructor which initializes the vector with the provided array
    //! @param data: Array containing the data
    inline explicit Vec4SSE(std::array<F32, 4> data);

    //! @brief Constructor that initializes the vector with specific values
    //! @param v0-v3: Vector values
    inline Vec4SSE(F32 v0, F32 v1, F32 v2, F32 v3);

private:
    //! @brief Constructor that initializes the vector
    //! @param data: Data
    inline Vec4SSE(__m128 data);

public:
    //! @brief Direct access operator
    //! @param index: Index of the accessed value
    //! @return Accessed value
    [[nodiscard]] inline F32 operator[](const U32 index) const;

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline const std::array<F32, 4> Data() const;

private:
    //! @brief Checks if the vectors internal data is aligned
    //! @return True / False
    inline bool IsDataAligned() const;
};



template <bool _isCol>
using Vec4fSSE = Vec4SSE<_isCol>;




//! @brief Offstream operator
//! @tparam _isCol: If true, the vector is treated as column vector, otherwise as row vector
//! @param os: Reference to offstream object
//! @param vec: Vector
//! @return Reference to offstream object
template <bool _isCol = true>
inline std::ostream& operator<<(std::ostream& os, const Vec4SSE<_isCol>& vec);

} // namespace GDL


#include "gdl/math/sse/vec4SSE.inl"
