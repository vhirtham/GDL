#pragma once

#include "gdl/base/insideTolerance.h"


namespace GDL
{

template <typename _type, bool _sse>
constexpr InsideTolerance<_type, _sse>::InsideTolerance(_type value, _type tolerance)
    : mValue{value}
    , mTolerance{tolerance}
{
}



template <typename _type, bool _sse>
constexpr bool InsideTolerance<_type, _sse>::operator==(_type rhs) const
{
    return std::abs(rhs - mValue) <= mTolerance;
}



template <typename _type, bool _sse>
constexpr bool InsideTolerance<_type, _sse>::operator!=(_type rhs) const
{
    return !operator==(rhs);
}



template <typename _registerType>
InsideTolerance<_registerType, true>::InsideTolerance(_registerType value, _registerType tolerance)
    : mValue{value}
    , mTolerance{tolerance}
{
}



template <typename _registerType>
template <typename _type>
InsideTolerance<_registerType, true>::InsideTolerance(_registerType value, _type tolerance)
    : mValue{value}
    , mTolerance{_mmx_set1_p<_registerType>(tolerance)}
{
    static_assert(std::is_integral<_type>::value || std::is_floating_point<_type>::value,
                  "Tolerance must be an integer or floating point type.");
}



template <typename _registerType>
bool GDL::InsideTolerance<_registerType, true>::operator==(_registerType rhs) const
{
    return SSECompareAllLessEqual(SSEAbs(_mmx_sub_p(rhs, mValue)), mTolerance);
}



template <typename _registerType>
bool GDL::InsideTolerance<_registerType, true>::operator!=(_registerType rhs) const
{
    return !operator==(rhs);
}



template <typename _typeLhs, typename _typeInsideTolerance>
constexpr bool operator==(const _typeLhs lhs, const InsideTolerance<_typeInsideTolerance>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value ||
                          SSEIsRegisterType<_typeLhs>(),
                  "Lhs type must be an integral, floating point or sse register type.");

    return rhs == lhs;
}



template <typename _typeLhs, typename _typeInsideTolerance>
constexpr bool operator!=(const _typeLhs lhs, const InsideTolerance<_typeInsideTolerance>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value ||
                          SSEIsRegisterType<_typeLhs>(),
                  "Lhs type must be an integral, floating point or sse register type.");

    return rhs != lhs;
}



} // namespace GDL
