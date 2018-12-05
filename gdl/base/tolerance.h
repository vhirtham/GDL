#pragma once

#include "gdl/base/fundamentalTypes.h"

#include "gdl/base/sse/utility.h"



namespace GDL
{

//! @brief Class to check if two values are equal with a certain tolerance.
//! @tparam _registerType: Register type or data type for non sse types
//! @tparam _numComparedValuesSSE: Specifies how many values (starting with the first element) of a sse register should
//! be compared. For non sse types this value should be set to 0 (specialized case).
//! @remark There is also the Approx class which has a quiet similar purpose. The major difference is, that the approx
//! has a dynamic tolerance while this class only has a fixed tolerance
template <typename _registerType, U32 _numComparedValuesSSE = sse::numRegisterValues<_registerType>>
class alignas(sse::alignmentBytes<_registerType>) Tolerance
{
    static_assert(sse::IsRegisterType<_registerType>(), "Type is no valid sse register type");
    static_assert(_numComparedValuesSSE <= sse::numRegisterValues<_registerType>,
                  "_numComparedValues > Number of register elements.");

    alignas(sse::alignmentBytes<_registerType>) _registerType mValue;
    alignas(sse::alignmentBytes<_registerType>) _registerType mTolerance;

public:
    Tolerance() = delete;
    Tolerance(const Tolerance&) = default;
    Tolerance(Tolerance&&) = default;
    Tolerance& operator=(const Tolerance&) = default;
    Tolerance& operator=(Tolerance&&) = default;
    ~Tolerance() = default;



    //! @brief ctor
    //! @param value: Register of values that should be compared to other values
    //! @param tolerance: Tolerances for the comparison (Can be different for each element)
    Tolerance(_registerType value, _registerType tolerance);

    //! @brief ctor
    //! @tparam _type: Data type of the tolerance
    //! @param value: Register of values that should be compared to other values
    //! @param tolerance: Tolerance value for all comparisons of the register
    template <typename _type>
    Tolerance(_registerType value, _type tolerance);

    //! @brief Compares if the rhs values are inside a certain tolerance to the stored values
    //! @param rhs: Right hand side values
    //! @return Returns true if all rhs values are inside the tolerance. False otherwise
    inline bool operator==(_registerType rhs) const;

    //! @brief Compares if the rhs values are NOT inside a certain tolerance to the stored values
    //! @param rhs: Right hand side values
    //! @return Returns true if one or more rhs values are not inside the tolerance. False otherwise
    inline bool operator!=(_registerType rhs) const;
};



//! @brief Spezialization of the Tolerance class for non sse types
//! @tparam _type: Data type
template <typename _type>
class Tolerance<_type, 0>
{
    static_assert(std::is_integral<_type>::value || std::is_floating_point<_type>::value,
                  "Tolerance can only be used with integer and floating point types.");

    _type mValue;
    _type mTolerance;

public:
    Tolerance() = delete;
    Tolerance(const Tolerance&) = default;
    Tolerance(Tolerance&&) = default;
    Tolerance& operator=(const Tolerance&) = default;
    Tolerance& operator=(Tolerance&&) = default;
    ~Tolerance() = default;



    //! @brief ctor
    //! @param value: Value that should be compared to other values
    //! @param tolerance: Tolerance for the comparison
    constexpr Tolerance(const _type value, const _type tolerance);

    //! @brief Compares if the rhs value is inside a certain tolerance to the stored value
    //! @param rhs: Right hand side value
    //! @return True / false
    constexpr bool operator==(_type rhs) const;

    //! @brief Compares if the rhs value is NOT inside a certain tolerance to the stored value
    //! @param rhs: Right hand side value
    //! @return True / false
    constexpr bool operator!=(_type rhs) const;
};



//! @brief Compares if the rhs value is inside a certain tolerance to the stored value of the Tolerance class.
//! Have a look at the specific class template (sse or not) for more details.
//! @tparam _typeLhs: Type of the lhs value
//! @tparam _typeTolerance: Type of the value stored inside of the Tolerance class
//! @tparam _numComparedValuesSSE: Number of compared values of the Tolerance class
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return True / false
template <typename _typeLhs, typename _typeTolerance, U32 _numComparedValuesSSE>
constexpr bool operator==(const _typeLhs lhs, const Tolerance<_typeTolerance>& rhs);



//! @brief Compares if the rhs value is NOT inside a certain tolerance to the stored value of the Tolerance class.
//! Have a look at the specific class template (sse or not) for more details.
//! @tparam _typeLhs: Type of the lhs value
//! @tparam _typeTolerance: Type of the value stored inside of the Tolerance class
//! @tparam _numComparedValuesSSE: Number of compared values of the Tolerance class
//! @param lhs: Left hand side value
//! @param rhs: Right hand side value
//! @return True / false
template <typename _typeLhs, typename _typeTolerance, U32 _numComparedValuesSSE>
constexpr bool operator!=(const _typeLhs lhs, const Tolerance<_typeTolerance>& rhs);


} // namespace GDL

#include "gdl/base/tolerance.inl"
