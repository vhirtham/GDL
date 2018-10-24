#pragma once

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"

#include <cassert>
#include <cmath>
#include <limits>


namespace GDL
{

//! @brief Class which enables the comparison of floating point values with a certain tolerance
//! @tparam _type: Value type
//! @remark Inspired by
//! https://github.com/catchorg/Catch2/blob/dfea75fc05432e4704c9de4f7058b594db14a6dd/include/internal/catch_approx.hpp
template <typename _type>
class Approx
{
    static_assert(std::is_floating_point<_type>::value, "Approx can only be used with floating point types.");


    const _type mValue;
    const _type mBaseZero;
    const I32 mFactor;

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
    explicit constexpr Approx(const _type value, const I32 factor = 3, const _type minimalBase = static_cast<_type>(1))
        : mValue{value}
        , mBaseZero{std::abs(minimalBase)}
        , mFactor{factor}
    {
        DEV_EXCEPTION(factor < 1, "Scaling factor must be bigger than 0.");
        DEV_EXCEPTION(mBaseZero <= static_cast<_type>(0), "Minimal base can't be 0.");
    }


    //! @brief Checks if two compared values are equal within a certain tolerance, which depends on the values sizes and
    //! a prescribed scaling factor.
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    constexpr bool operator==(const _type rhs) const
    {
        constexpr _type epsilon = std::numeric_limits<_type>::epsilon();

        return std::abs(rhs - mValue) <=
               static_cast<_type>(mFactor) * epsilon * (std::max({std::abs(rhs), std::abs(mValue), mBaseZero}));
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
template <typename _typeLhs, typename _typeApprox>
constexpr bool operator==(const _typeLhs lhs, const Approx<_typeApprox>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value,
                  "Lhs type must be an integral or floating point type.");
    return rhs == static_cast<_typeApprox>(lhs);
}


//! @brief Checks if two compared values are NOT equal within a certain tolerance, which depends on the values sizes
//! and a prescribed scaling factor.
//! @tparam _typeLhs: Type of the lhs value
//! @tparam _typeApprox: Type of the Approx class
//! @param lhs: Left hand side of the comparison
//! @param rhs: Right hand side of the comparison
//! @return TRUE / FALSE
template <typename _typeLhs, typename _typeApprox>
constexpr bool operator!=(const _typeLhs lhs, const Approx<_typeApprox>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value,
                  "Lhs type must be an integral or floating point type.");
    return !(rhs == static_cast<_typeApprox>(lhs));
}



//! @brief Checks if a floating point value is close to 0
//! @tparam _type: Value type
//! @param base: Base value that is used to calculate the epsilon (minimal possible increment)
//! @param factor: The calculated minimal increment is multiplied with this factor to scale the tolerance of the
//! comparison.
template <typename _type>
Approx<_type> ApproxZero(_type base = static_cast<_type>(1), I32 factor = 3)
{
    return Approx(static_cast<_type>(0), factor, base);
}

} // namespace GDL
