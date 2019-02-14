#pragma once

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/compareAll.h"
#include "gdl/base/sse/intrinsics.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace GDL
{

template <typename _registerType, U32 _numComparedValuesSSE>
Approx<_registerType, _numComparedValuesSSE>::Approx(const _registerType values, const I32 factor,
                                                     const ElementType minimalBase)
    : mValues{values}
    , mFactor{_mmx_set1_p<_registerType>(
              static_cast<ElementType>(factor * std::numeric_limits<ElementType>::epsilon()))}
    , mBaseZero{_mmx_set1_p<_registerType>(std::abs(minimalBase))}
{
    DEV_EXCEPTION(factor <= 0, "Scaling factor must be bigger than 0.");
    DEV_EXCEPTION(mBaseZero[0] <= static_cast<ElementType>(0), "Minimal base can't be 0.");
}



template <typename _registerType, U32 _numComparedValuesSSE>
bool Approx<_registerType, _numComparedValuesSSE>::operator==(const _registerType rhs) const
{
    const _registerType tolerance =
            _mmx_mul_p(mFactor, _mmx_max_p(_mmx_max_p(sse::Abs(rhs), sse::Abs(mValues)), mBaseZero));

    return sse::CompareAllLessEqual<_registerType, _numComparedValuesSSE>(sse::Abs(_mmx_sub_p(rhs, mValues)),
                                                                          tolerance);
}



template <typename _registerType, U32 _numComparedValuesSSE>
bool Approx<_registerType, _numComparedValuesSSE>::operator!=(const _registerType rhs) const
{
    return !operator==(rhs);
}



template <typename _type>
constexpr Approx<_type, 0>::Approx(const _type value, const I32 factor, const _type minimalBase)
    : mValue{value}
    , mFactor{static_cast<_type>(factor * std::numeric_limits<_type>::epsilon())}
    , mBaseZero{std::abs(minimalBase)}
{
    DEV_EXCEPTION(factor <= 0, "Scaling factor must be bigger than 0.");
    DEV_EXCEPTION(mBaseZero <= static_cast<_type>(0), "Minimal base can't be 0.");
}



template <typename _type>
constexpr bool Approx<_type, 0>::operator==(const _type rhs) const
{
    return std::abs(rhs - mValue) <= mFactor * (std::max({std::abs(rhs), std::abs(mValue), mBaseZero}));
}



template <typename _type>
constexpr bool Approx<_type, 0>::operator!=(const _type rhs) const
{
    return !(*this == rhs);
}



template <typename _typeLhs, typename _typeApprox, U32 _numComparedValuesSSE>
constexpr bool operator==(const _typeLhs lhs, const Approx<_typeApprox, _numComparedValuesSSE>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value ||
                          sse::IsRegisterType<_typeLhs>,
                  "Lhs type must be an integral, floating point or sse register type.");
    return rhs == static_cast<_typeApprox>(lhs);
}



template <typename _typeLhs, typename _typeApprox, U32 _numComparedValuesSSE>
constexpr bool operator!=(const _typeLhs lhs, const Approx<_typeApprox, _numComparedValuesSSE>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value ||
                          sse::IsRegisterType<_typeLhs>,
                  "Lhs type must be an integral, floating point or sse register type.");
    return !(rhs == static_cast<_typeApprox>(lhs));
}



template <typename _classType, U32 _numComparedValuesSSE, typename _baseType>
constexpr auto ApproxZero(_baseType base, I32 factor)
{
    if constexpr (sse::IsRegisterType<_classType>)
        return Approx<_classType, _numComparedValuesSSE>(_mmx_setzero_p<_classType>(), factor, base);
    else
        return Approx<_classType>(static_cast<_baseType>(0), factor, static_cast<_baseType>(base));
}



} // namespace GDL
