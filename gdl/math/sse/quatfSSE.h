#pragma once

#include "gdl/base/approx.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"
#include "gdl/base/sse/intrinsics.h"

#include <array>
#include <iostream>


namespace GDL
{

//! @brief Quaternion class with x, y, z, w ordering and SSE support
class alignas(sse::alignmentBytes<__m128>) QuatfSSE
{

    alignas(sse::alignmentBytes<__m128>) __m128 mData;

public:
    inline QuatfSSE();
    inline QuatfSSE(const QuatfSSE& other);
    inline QuatfSSE(QuatfSSE&& other) = default;
    inline QuatfSSE& operator=(const QuatfSSE& other) = default;
    inline QuatfSSE& operator=(QuatfSSE&& other) = default;
    inline ~QuatfSSE() = default;

    //! @brief Constructor which initializes the quaternion with the provided array
    //! @param data: Array containing the data
    inline explicit QuatfSSE(std::array<F32, 4> data);

    //! @brief Constructor that initializes the quaternion with specific values
    //! @param x, y, z, w: Quaternion values
    inline QuatfSSE(F32 x, F32 y, F32 z, F32 w);

private:
    //! @brief Constructor that initializes the quaternion
    //! @param data: Data
    inline QuatfSSE(__m128 data);

public:
    //! @brief Direct access operator
    //! @param index: Index of the accessed value
    //! @return Accessed value
    [[nodiscard]] inline F32 operator[](const U32 index) const;

    //! @brief Compares if two quaternions are equal
    //! @param rhs: Quaternion that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator==(const QuatfSSE& rhs) const;

    //! @brief Compares if two quaternions are NOT equal
    //! @param rhs: Quaternion that should be compared
    //! @return True / False
    //! @remark This function uses the Approx class internally. The default minimal base is used. This might be changed
    //! in the future. A global minimal base for linear algebra comparison might be introduced.
    [[nodiscard]] inline bool operator!=(const QuatfSSE& rhs) const;

    //! @brief Gets the data array
    //! @return Data
    [[nodiscard]] inline const std::array<F32, 4> Data() const;


private:
    //! @brief Checks if the quaternions internal data is aligned
    //! @return True / False
    inline bool IsDataAligned() const;
};



//! @brief Offstream operator
//! @param os: Reference to offstream object
//! @param quat: Quaternion
//! @return Reference to offstream object
inline std::ostream& operator<<(std::ostream& os, const QuatfSSE& quat);

} // namespace GDL


#include "gdl/math/sse/quatfSSE.inl"
