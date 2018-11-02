#pragma once

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/sse.h"

#include <cmath>
#include <type_traits>

namespace GDL
{

//! @brief Class to check if two values are equal with a certain tolerance
//! @tparam _type: Value type
//! @tparam _sse: Bool to specify if type is a sse register type. This is automatically detected if not explicitly set.
//! @remark There is also the Approx class which has a quiet similar purpose. The major difference is, that the approx
//! has a dynamic tolerance while this class only has a fixed tolerance
template <typename _type, bool _sse = SSEIsRegisterType<_type>()>
class InsideTolerance
{
    static_assert(std::is_integral<_type>::value || std::is_floating_point<_type>::value,
                  "InsideTolerance can only be used with integer and floating point types.");

    const _type mValue;
    const _type mTolerance;

public:
    InsideTolerance() = delete;
    InsideTolerance(const InsideTolerance&) = default;
    InsideTolerance(InsideTolerance&&) = default;
    InsideTolerance& operator=(const InsideTolerance&) = default;
    InsideTolerance& operator=(InsideTolerance&&) = default;
    ~InsideTolerance() = default;



    //! @brief ctor
    //! @param value: Value that should be compared to other values
    //! @param tolerance: Tolerance for the comparison
    constexpr InsideTolerance(const _type value, const _type tolerance);

    //! @brief Compares if the rhs value is inside a certain tolerance to the stored value
    //! @param rhs: Right hand side value
    //! @return True / false
    constexpr bool operator==(_type rhs) const;

    //! @brief Compares if the rhs value is NOT inside a certain tolerance to the stored value
    //! @param rhs: Right hand side value
    //! @return True / false
    constexpr bool operator!=(_type rhs) const;
};



//! @brief Spezialization of the InsideTolerance class for sse registers
//! @tparam _registerType: Register type
template <typename _registerType>
class alignas(alignmentBytes<_registerType>) InsideTolerance<_registerType, true>
{
    static_assert(SSEIsRegisterType<_registerType>(), "Type is no valid sse register type");

    alignas(alignmentBytes<_registerType>) _registerType mValue;
    alignas(alignmentBytes<_registerType>) _registerType mTolerance;

public:
    InsideTolerance() = delete;
    InsideTolerance(const InsideTolerance&) = default;
    InsideTolerance(InsideTolerance&&) = default;
    InsideTolerance& operator=(const InsideTolerance&) = default;
    InsideTolerance& operator=(InsideTolerance&&) = default;
    ~InsideTolerance() = default;



    //! @brief ctor
    //! @param value: Register of values that should be compared to other values
    //! @param tolerance: Tolerances for the comparison (Can be different for each element)
    InsideTolerance(_registerType value, _registerType tolerance);

    //! @brief ctor
    //! @tparam _type: Data type of the tolerance
    //! @param value: Register of values that should be compared to other values
    //! @param tolerance: Tolerance value for all comparisons of the register
    template <typename _type>
    InsideTolerance(_registerType value, _type tolerance);

    //! @brief Compares if the rhs values are inside a certain tolerance to the stored values
    //! @param rhs: Right hand side values
    //! @return Returns true if all rhs values are inside the tolerance. False otherwise
    inline bool operator==(_registerType rhs) const;

    //! @brief Compares if the rhs values are NOT inside a certain tolerance to the stored values
    //! @param rhs: Right hand side values
    //! @return Returns true if one or more rhs values are not inside the tolerance. False otherwise
    inline bool operator!=(_registerType rhs) const;
};



//! @brief Compares if the rhs value is inside a certain tolerance to the stored value of the InsideTolerance class.
//! Have a look at the specific class template (sse or not) for more details.
//! @tparam _typeLhs: Type of the lhs value
//! @tparam _typeInsideTolerance: Type of the value stored inside of the InsideTolerance class
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return True / false
template <typename _typeLhs, typename _typeInsideTolerance>
constexpr bool operator==(const _typeLhs lhs, const InsideTolerance<_typeInsideTolerance>& rhs);



//! @brief Compares if the rhs value is NOT inside a certain tolerance to the stored value of the InsideTolerance class.
//! Have a look at the specific class template (sse or not) for more details.
//! @tparam _typeLhs: Type of the lhs value
//! @tparam _typeInsideTolerance: Type of the value stored inside of the InsideTolerance class
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return True / false
template <typename _typeLhs, typename _typeInsideTolerance>
constexpr bool operator!=(const _typeLhs lhs, const InsideTolerance<_typeInsideTolerance>& rhs);


} // namespace GDL

#include "gdl/base/insideTolerance.inl"
