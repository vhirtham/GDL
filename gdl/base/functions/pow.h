#pragma once

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
inline constexpr _type Pow(const _type, std::integral_constant<U32, 0>)
{
    return 1;
}

//! @brief Overloaded helper function to calculate the power of a given base (spezialization exponent = 1)
//! @tparam _type: Type of the base
//! @param base: Base
//! @return Base;
//! @remark: std::integral_constant is used for recursive function overloading
template <typename _type>
inline constexpr _type Pow(const _type base, std::integral_constant<U32, 1>)
{
    return base;
}

//! @brief Overloaded helper function to calculate the power of a given base
//! @tparam _type: Type of the base
//! @tparam _exponent: Exponent
//! @param base: Base
//! @return Power of the passed value;
//! @remark: std::integral_constant is used for recursive function overloading
template <typename _type, U32 _exponent>
inline constexpr _type Pow(const _type base, std::integral_constant<U32, _exponent>)
{
    return Pow(base, std::integral_constant<U32, _exponent - 1>()) * base;
}


//! @brief Calculates the power of a given base with an integer exponent (compile time version)
//! @tparam _exponent: Exponent
//! @tparam _type: Type of the base
//! @param base: Base
//! @return Power of the passed value;
template <U32 _exponent, typename _type>
inline constexpr _type Pow(const _type base)
{
    return Pow(base, std::integral_constant<U32, _exponent>());
}


//! @brief Calculates the power of a given base with an integer exponent (run time version)
//! @tparam _type: Type of the base
//! @param base: Base
//! @param exponent: Exponent
//! @return Power of the passed value;
template <typename _type>
inline constexpr _type Pow(const _type base, const U32 exponent)
{
    return (exponent == 0) ? 1 : (exponent == 1) ? base : (base * Pow(base, exponent - 1));
}


//! @brief Calculates the square of a given base
//! @tparam _type: Type of the base
//! @param base: Value which should be squared
//! @return Squared value;
template <typename _type>
inline constexpr _type Square(const _type base)
{
    return Pow<2>(base);
}

// Alternative Implementation 2 -------------------------------------------------------------------

// template <typename _type, U32 _exponent>
// struct Power
//{
//    static constexpr _type Pow(const _type base)
//    {
//        return Power<_type, _exponent - 1>::Pow(base) * base;
//    }
//};

// template <typename _type>
// struct Power<_type, 1>
//{
//    static constexpr _type Pow(const _type base)
//    {
//        return base;
//    }
//};

// template <typename _type>
// struct Power<_type, 0>
//{
//    static constexpr _type Pow(const _type)
//    {
//        return 1;
//    }
//};

// template <U32 _exponent, typename _type>
// inline _type Pow(_type base)
//{
//    return Power<_type, _exponent>::Pow(base);
//}

// template <typename _type>
// inline constexpr _type Square(const _type base)
//{
//    return Pow<2>(base);
//}

} // namespace GDL
