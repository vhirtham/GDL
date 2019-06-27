#pragma once

#include "gdl/base/fundamentalTypes.h"

#include "gdl/base/simd/utility.h"



namespace GDL
{



//! @brief Class which enables the comparison of floating point values with a certain dynamic tolerance. Use the
//! InsideTolerance class for fixed Tolerances.
//! @tparam _registerType: Register type or data type for non sse types
//! @tparam _numComparedValuesSSE: Specifies how many values (starting with the first element) of a sse register should
//! be compared. For non sse types this value should be set to 0 (specialized case).
//! @remark Inspired by
//! https://github.com/catchorg/Catch2/blob/dfea75fc05432e4704c9de4f7058b594db14a6dd/include/internal/catch_approx.hpp
template <typename _registerType, U32 _numComparedValuesSSE = simd::numRegisterValues<_registerType>>
class alignas(simd::alignmentBytes<_registerType>) Approx
{
    using ElementType = decltype(simd::GetDataType<_registerType, true>());

    alignas(simd::alignmentBytes<_registerType>) _registerType mValues;
    alignas(simd::alignmentBytes<_registerType>) _registerType mFactor;
    alignas(simd::alignmentBytes<_registerType>) _registerType mBaseZero;


public:
    //! @brief ctor
    //! @param values: Register with values that should be compared to other values
    //! @param factor: During a comparison the minimal increment (epsilon) of the larger of both values is calculated.
    //! The factor is multiplied with the epsilon to determine the tolerance of the comparison. A value of 3 means, that
    //! both values can only differ by 3 minimal increments of the larger value to be treated as equal.
    //! @param minimalBase: If both values of the comparison are smaller than this value, it will be used to calculate
    //! the epsilon instead of the maximum of both values. This is important if one of the values is 0 or close to 0.
    //! Without this value a comparison to 0 would be impossible.
    inline Approx(const _registerType values, const I32 factor = 3,
                  const ElementType minimalBase = static_cast<ElementType>(1));

    //! @brief Checks if two compared registers are equal within a certain tolerance, which depends on the values sizes
    //! and
    //! a prescribed scaling factor.
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    inline bool operator==(const _registerType rhs) const;

    //! @brief Checks if two compared values are NOT equal within a certain tolerance, which depends on the values sizes
    //! and a prescribed scaling factor.
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    inline bool operator!=(const _registerType rhs) const;
};



//! @tparam _type: Value type
template <typename _type>
class Approx<_type, 0>
{
    static_assert(std::is_floating_point<_type>::value, "Approx can only be used with floating point types.");


    const _type mValue;
    const _type mFactor;
    const _type mBaseZero;

public:
    Approx() = delete;
    Approx(const Approx&) = default;
    Approx(Approx&&) = default;
    Approx& operator=(const Approx&) = default;
    Approx& operator=(Approx&&) = default;
    ~Approx() = default;



    //! @brief ctor
    //! @param value: Value that should be compared to other values
    //! @param factor: During a comparison the minimal increment (epsilon) of the larger of both values is calculated.
    //! The factor is multiplied with the epsilon to determine the tolerance of the comparison. A value of 3 means, that
    //! both values can only differ by 3 minimal increments of the larger value to be treated as equal.
    //! @param minimalBase: If both values of the comparison are smaller than this value, it will be used to calculate
    //! the epsilon instead of the maximum of both values. This is important if one of the values is 0 or close to 0.
    //! Without this value a comparison to 0 would be impossible.
    constexpr Approx(const _type value, const I32 factor = 3, const _type minimalBase = static_cast<_type>(1));

    //! @brief Checks if two compared values are equal within a certain tolerance, which depends on the values sizes and
    //! a prescribed scaling factor.
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    constexpr bool operator==(const _type rhs) const;

    //! @brief Checks if two compared values are NOT equal within a certain tolerance, which depends on the values sizes
    //! and a prescribed scaling factor.
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    constexpr bool operator!=(const _type rhs) const;
};



//! @brief Checks if two compared values/registers are equal within a certain tolerance, which depends on the values
//! sizes and a prescribed scaling factor.
//! @tparam _typeLhs: Type of the lhs value
//! @tparam _typeApprox: Type of the Approx class
//! @tparam _numComparedValuesSSE: Number of compared values of the Approx class
//! @param lhs: Left hand side of the comparison
//! @param rhs: Right hand side of the comparison
//! @return TRUE / FALSE
template <typename _typeLhs, typename _typeApprox, U32 _numComparedValuesSSE>
constexpr bool operator==(const _typeLhs lhs, const Approx<_typeApprox, _numComparedValuesSSE>& rhs);



//! @brief Checks if two compared values are NOT/registers equal within a certain tolerance, which depends on the values
//! sizes and a prescribed scaling factor.
//! @tparam _typeLhs: Type of the lhs value
//! @tparam _typeApprox: Type of the Approx class
///! @tparam _numComparedValuesSSE: Number of compared values of the Approx class
//! @param lhs: Left hand side of the comparison
//! @param rhs: Right hand side of the comparison
//! @return TRUE / FALSE
template <typename _typeLhs, typename _typeApprox, U32 _numComparedValuesSSE>
constexpr bool operator!=(const _typeLhs lhs, const Approx<_typeApprox, _numComparedValuesSSE>& rhs);



//! @brief Checks if a floating point value or a register of floating point values is close to 0
//! @tparam _classType: Type of the Approx class
//! @tparam _numComparedValuesSSE: Number of compared values of the Approx class
//! @tparam _baseType: If the Approx class type is a sse register, this is the registers element type. If not, it is the
//! same as the Approx class type.
//! @param base: Base value that is used to calculate the epsilon (minimal possible increment)
//! @param factor: The calculated minimal increment is multiplied with this factor to scale the tolerance of the
//! comparison.
template <typename _classType, U32 _numComparedValuesSSE = simd::numRegisterValues<_classType>,
          typename _baseType = decltype(simd::GetDataType<_classType, true>())>
constexpr auto ApproxZero(_baseType base = static_cast<_baseType>(1), I32 factor = 3);
} // namespace GDL


#include "gdl/base/approx.inl"
