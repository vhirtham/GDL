#pragma once

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/sse.h"

#include <algorithm>
#include <cmath>
#include <limits>


namespace GDL
{



//! @brief Class which enables the comparison of floating point values with a certain dynamic tolerance
//! @remark Inspired by
//! https://github.com/catchorg/Catch2/blob/dfea75fc05432e4704c9de4f7058b594db14a6dd/include/internal/catch_approx.hpp
template <typename _registerType, U32 _numComparedValuesSSE = SSEGetNumRegisterEntries<_registerType>()>
class alignas(alignmentBytes<_registerType>) Approx
{


    using mElementType = decltype(SSEGetDataType<_registerType, true>());

    alignas(alignmentBytes<_registerType>) _registerType mValue;
    alignas(alignmentBytes<_registerType>) _registerType mFactor;
    alignas(alignmentBytes<_registerType>) _registerType mBaseZero;


public:
    inline Approx(const _registerType value, const I32 factor = 3,
                  const mElementType minimalBase = static_cast<mElementType>(1))
        : mValue{value}
        , mFactor{_mmx_set1_p<_registerType>(
                  static_cast<mElementType>(factor * std::numeric_limits<mElementType>::epsilon()))}
        , mBaseZero{_mmx_set1_p<_registerType>(std::abs(minimalBase))}
    {
        DEV_EXCEPTION(factor <= 0, "Scaling factor must be bigger than 0.");
        DEV_EXCEPTION(mBaseZero[0] <= static_cast<mElementType>(0), "Minimal base can't be 0.");
    }



    inline bool operator==(const _registerType rhs) const
    {
        const _registerType tolerance =
                _mmx_mul_p(mFactor, _mmx_max_p(_mmx_max_p(SSEAbs(rhs), SSEAbs(mValue)), mBaseZero));

        return SSECompareAllLessEqual<_registerType, _numComparedValuesSSE>(SSEAbs(_mmx_sub_p(rhs, mValue)), tolerance);
    }



    inline bool operator!=(const _registerType rhs) const
    {
        return !operator==(rhs);
    }
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
    constexpr Approx(const _type value, const I32 factor = 3, const _type minimalBase = static_cast<_type>(1))
        : mValue{value}
        , mFactor{static_cast<_type>(factor * std::numeric_limits<_type>::epsilon())}
        , mBaseZero{std::abs(minimalBase)}
    {
        DEV_EXCEPTION(factor <= 0, "Scaling factor must be bigger than 0.");
        DEV_EXCEPTION(mBaseZero <= static_cast<_type>(0), "Minimal base can't be 0.");
    }


    //! @brief Checks if two compared values are equal within a certain tolerance, which depends on the values sizes and
    //! a prescribed scaling factor.
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    constexpr bool operator==(const _type rhs) const
    {
        return std::abs(rhs - mValue) <= mFactor * (std::max({std::abs(rhs), std::abs(mValue), mBaseZero}));
    }


    //! @brief Checks if two compared values are NOT equal within a certain tolerance, which depends on the values sizes
    //! and a prescribed scaling factor.
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    constexpr bool operator!=(const _type rhs) const
    {
        return !(*this == rhs);
    }
};



//! @brief Checks if two compared values are equal within a certain tolerance, which depends on the values sizes and
//! a prescribed scaling factor.
//! @tparam _typeLhs: Type of the lhs value
//! @tparam _typeApprox: Type of the Approx class
//! @param lhs: Left hand side of the comparison
//! @param rhs: Right hand side of the comparison
//! @return TRUE / FALSE
template <typename _typeLhs, typename _typeApprox, U32 _numComparedValuesSSE>
constexpr bool operator==(const _typeLhs lhs, const Approx<_typeApprox, _numComparedValuesSSE>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value ||
                          SSEIsRegisterType<_typeLhs>(),
                  "Lhs type must be an integral, floating point or sse register type.");
    return rhs == static_cast<_typeApprox>(lhs);
}


//! @brief Checks if two compared values are NOT equal within a certain tolerance, which depends on the values sizes
//! and a prescribed scaling factor.
//! @tparam _typeLhs: Type of the lhs value
//! @tparam _typeApprox: Type of the Approx class
//! @param lhs: Left hand side of the comparison
//! @param rhs: Right hand side of the comparison
//! @return TRUE / FALSE
template <typename _typeLhs, typename _typeApprox, U32 _numComparedValuesSSE>
constexpr bool operator!=(const _typeLhs lhs, const Approx<_typeApprox, _numComparedValuesSSE>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value ||
                          SSEIsRegisterType<_typeLhs>(),
                  "Lhs type must be an integral, floating point or sse register type.");
    return !(rhs == static_cast<_typeApprox>(lhs));
}



//! @brief Checks if a floating point value is close to 0
//! @tparam _type: Value type
//! @param base: Base value that is used to calculate the epsilon (minimal possible increment)
//! @param factor: The calculated minimal increment is multiplied with this factor to scale the tolerance of the
//! comparison.
// template <typename _type>
// Approx<_type> ApproxZero(_type base = static_cast<_type>(1), I32 factor = 3)
//{
//    return Approx<_type>(static_cast<_type>(0), factor, static_cast<_type>(base));
//}

template <typename _registerType, U32 _numComparedValuesSSE = SSEGetNumRegisterEntries<_registerType>(),
          typename _type = decltype(SSEGetDataType<_registerType, true>())>
auto ApproxZero(_type base = static_cast<_type>(1), I32 factor = 3)
{
    // clang-format off
    if constexpr(SSEIsRegisterType<_registerType>())
        return Approx<_registerType, _numComparedValuesSSE>(_mmx_setzero_p<_registerType>(), factor, base);
    else
        return Approx<_registerType>(static_cast<_type>(0), factor, static_cast<_type>(base));
    // clang-format on
}
} // namespace GDL
