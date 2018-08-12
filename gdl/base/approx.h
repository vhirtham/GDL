#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"

#include <cassert>
#include <cmath>
#include <limits>
#include <type_traits>


namespace GDL
{

//! @brief Class which enables the comparison of floating point values with a certain tolerance
//! @tparam _type: Value type
//! @remark Inspired by
//! https://github.com/catchorg/Catch2/blob/dfea75fc05432e4704c9de4f7058b594db14a6dd/include/internal/catch_approx.hpp
template <typename _type>
class Approx
{
    static_assert(std::is_same<_type, F32>::value || std::is_same<_type, F64>::value,
                  "Approx can only be used with floating point types.");


    const _type mValue;
    const _type mScale;

public:
    Approx() = delete;
    Approx(const Approx&) = default;
    Approx(Approx&&) = default;
    Approx& operator=(const Approx&) = default;
    Approx& operator=(Approx&&) = default;
    ~Approx() = default;



    //! @brief ctor
    //! @param value: Value that should be compared to other values
    //! @param scale: Factor to scale the tolerance. The tolerance is the minimal increment (epsilon) of the bigger of
    //! two compared values.
    explicit constexpr Approx(_type value, _type scale = static_cast<_type>(3))
        : mValue{value}
        , mScale{scale}
    {
        DEV_EXCEPTION(scale < static_cast<_type>(1), "Scaling factors smaller than 1 are not allowed.");
        DEV_EXCEPTION(std::abs(mValue) == static_cast<_type>(0),
                      "Can't use Approx with a value of 0. Use ApproxZero instead.");
    }

    //! @brief Checks if two compared values are equal within a certain tolerance, which depends on the values sizes and
    //! a prescribed scaling factor.
    //! @tparam _typeLhs: Type of the lhs value
    //! @param lhs: Left hand side of the comparison
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    template <typename _typeLhs>
    friend constexpr bool operator==(const _typeLhs lhs, const Approx& rhs)
    {
        constexpr _type epsilon = std::numeric_limits<_type>::epsilon();
        const _type lhsConv = static_cast<_type>(lhs);

        return std::abs(lhsConv - rhs.mValue) <
               rhs.mScale * epsilon * (std::max(std::abs(lhsConv), std::abs(rhs.mValue)));
    }

    //! @brief Checks if two compared values are NOT equal within a certain tolerance, which depends on the values sizes
    //! and a prescribed scaling factor.
    //! @tparam _typeLhs: Type of the lhs value
    //! @param lhs: Left hand side of the comparison
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    template <typename _typeLhs>
    friend constexpr bool operator!=(const _typeLhs lhs, const Approx& rhs)
    {
        return !(lhs == rhs);
    }
};



using Approx32 = Approx<F32>;
using Approx64 = Approx<F64>;



//! @brief Class which enables helps checking if a floating point value is close to 0
//! @tparam _type: Value type
template <typename _type>
class ApproxZero
{
    static_assert(std::is_same<_type, F32>::value || std::is_same<_type, F64>::value,
                  "Approx can only be used with floating point types.");


    const _type mTolerance;

public:
    ApproxZero() = delete;
    ApproxZero(const ApproxZero&) = default;
    ApproxZero(ApproxZero&&) = default;
    ApproxZero& operator=(const ApproxZero&) = default;
    ApproxZero& operator=(ApproxZero&&) = default;
    ~ApproxZero() = default;


    //! @brief ctor
    //! @param tolerance: Tolerance for comparison
    explicit constexpr ApproxZero(_type tolerance = std::numeric_limits<_type>::epsilon())
        : mTolerance{tolerance}
    {
        DEV_EXCEPTION(std::abs(mTolerance) == static_cast<_type>(0), "Tolerance must be bigger than 0.");
    }

    //! @brief ctor
    //! @param base: Value that should be compared to other values
    //! @param scale: Factor to scale the tolerance. The tolerance is the minimal increment (epsilon) of the given base.
    constexpr ApproxZero(_type base, _type scale)
        : mTolerance{std::numeric_limits<_type>::epsilon() * base * scale}
    {
        DEV_EXCEPTION(scale < static_cast<_type>(1), "Scaling factors smaller than 1 are not allowed.");
        DEV_EXCEPTION(std::abs(base) <= static_cast<_type>(0), "Base must be bigger than 0.");
        assert(mTolerance > static_cast<_type>(0));
    }

    //! @brief Checks if a value is 0 within a certain tolerance
    //! @tparam _typeLhs: Type of the lhs value
    //! @param lhs: Left hand side of the comparison
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    template <typename _typeLhs>
    friend constexpr bool operator==(const _typeLhs lhs, const ApproxZero& rhs)
    {
        const _type lhsConv = static_cast<_type>(lhs);

        return std::abs(lhsConv) < rhs.mTolerance;
    }

    //! @brief Checks if a value is NOT 0 within a certain tolerance
    //! @tparam _typeLhs: Type of the lhs value
    //! @param lhs: Left hand side of the comparison
    //! @param rhs: Right hand side of the comparison
    //! @return TRUE / FALSE
    template <typename _typeLhs>
    friend constexpr bool operator!=(const _typeLhs lhs, const ApproxZero& rhs)
    {
        return !(lhs == rhs);
    }
};



using ApproxZero32 = ApproxZero<F32>;
using ApproxZero64 = ApproxZero<F64>;

} // namespace GDL