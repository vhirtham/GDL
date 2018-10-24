#pragma once

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"

#include <type_traits>

// Source:
// https://stackoverflow.com/questions/16443682/c-power-of-integer-template-meta-programming#comment23585816_16443823

namespace GDL
{

//! @brief Overloaded helper function to calculate the power of a given base (spezialization exponent = 1)
//! @tparam _type: Type of the base
//! @return 1;
//! @remark: std::integral_constant is used for recursive function overloading
template <typename _type>
constexpr _type Pow(const _type, std::integral_constant<I32, 0>)
{
    return 1;
}

//! @brief Overloaded helper function to calculate the power of a given base (spezialization exponent = 1)
//! @tparam _type: Type of the base
//! @param base: Base
//! @return Base;
//! @remark: std::integral_constant is used for recursive function overloading
template <typename _type>
constexpr _type Pow(const _type base, std::integral_constant<I32, 1>)
{
    return base;
}

//! @brief Overloaded helper function to calculate the power of a given base (spezialization exponent = -1)
//! @tparam _type: Type of the base
//! @param base: Base
//! @return Base;
//! @remark: std::integral_constant is used for recursive function overloading
template <typename _type>
constexpr _type Pow(const _type base, std::integral_constant<I32, -1>)
{
    return 1 / base;
}

//! @brief Overloaded helper function to calculate the power of a given base
//! @tparam _type: Type of the base
//! @tparam _exponent: Exponent
//! @param base: Base
//! @return Power of the passed value;
//! @remark: std::integral_constant is used for recursive function overloading
template <typename _type, I32 _exponent>
constexpr _type Pow(const _type base, std::integral_constant<I32, _exponent>)
{
    // clang-format off
    if constexpr(_exponent < 0)
    {
        static_assert(std::is_floating_point<_type>::value,
                      "Negative exponents are only supported for floating point types");
        return Pow(base, std::integral_constant<I32, _exponent + 1>()) / base;
    }
    else
        return Pow(base, std::integral_constant<I32, _exponent - 1>()) * base;
    // clang-format on
}


//! @brief Calculates the power of a given base with an integer exponent (compile time version)
//! @tparam _exponent: Exponent
//! @tparam _type: Type of the base
//! @param base: Base
//! @return Power of the passed value;
template <I32 _exponent, typename _type>
constexpr _type Pow(const _type base)
{
    return Pow(base, std::integral_constant<I32, _exponent>());
}


//! @brief Calculates the power of a given base with an integer exponent (run time version)
//! @tparam _type: Type of the base
//! @param base: Base
//! @param exponent: Exponent
//! @return Power of the passed value;
template <typename _type>
constexpr _type Pow(const _type base, const I32 exponent)
{
    if (exponent > 0)
        return (exponent == 1) ? base : (Pow(base, exponent - 1) * base);

    if (exponent < 0)
    {
        DEV_EXCEPTION(!std::is_floating_point<_type>::value,
                      "Negative exponents are only supported for floating point types");
        return (exponent == -1) ? 1 / base : (Pow(base, exponent + 1) / base);
    }
    return 1;
}


//! @brief Calculates the square of a given base
//! @tparam _type: Type of the base
//! @param base: Value which should be squared
//! @return Squared value;
template <typename _type>
constexpr _type Square(const _type base)
{
    return Pow<2>(base);
}

} // namespace GDL
