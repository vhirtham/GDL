#pragma once

#include "gdl/base/tolerance.h"
#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/compareAll.h"
#include "gdl/base/sse/intrinsics.h"

#include <cmath>

namespace GDL
{

template <typename _registerType, U32 _numComparedValuesSSE>
Tolerance<_registerType, _numComparedValuesSSE>::Tolerance(_registerType value, _registerType tolerance)
    : mValue{value}
    , mTolerance{tolerance}
{
    DEV_EXCEPTION(!simd::CompareAllGreaterThan(tolerance, _mm_setzero<_registerType>()),
                  "Tolerance values must be greater than 0.");
}



template <typename _registerType, U32 _numComparedValuesSSE>
template <typename _type>
Tolerance<_registerType, _numComparedValuesSSE>::Tolerance(_registerType value, _type tolerance)
    : mValue{value}
    , mTolerance{_mm_set1<_registerType>(tolerance)}
{
    static_assert(std::is_integral<_type>::value || std::is_floating_point<_type>::value,
                  "Tolerance must be an integer or floating point type.");
    DEV_EXCEPTION(tolerance <= 0, "Tolerance must be greater than 0.");
}



template <typename _registerType, U32 _numComparedValuesSSE>
bool Tolerance<_registerType, _numComparedValuesSSE>::operator==(_registerType rhs) const
{
    return simd::CompareAllLessEqual<_registerType, _numComparedValuesSSE>(simd::Abs(_mm_sub(rhs, mValue)),
                                                                          mTolerance);
}



template <typename _registerType, U32 _numComparedValuesSSE>
bool Tolerance<_registerType, _numComparedValuesSSE>::operator!=(_registerType rhs) const
{
    return !operator==(rhs);
}



template <typename _type>
constexpr Tolerance<_type, 0>::Tolerance(const _type value, const _type tolerance)
    : mValue{value}
    , mTolerance{tolerance}
{
    DEV_EXCEPTION(mTolerance <= 0, "Tolerance must be greater than 0.");
}



template <typename _type>
constexpr bool Tolerance<_type, 0>::operator==(_type rhs) const
{
    return std::abs(mValue - rhs) <= mTolerance;
}



template <typename _type>
constexpr bool Tolerance<_type, 0>::operator!=(_type rhs) const
{
    return !operator==(rhs);
}



template <typename _typeLhs, typename _typeTolerance, U32 _numComparedValuesSSE>
constexpr bool operator==(const _typeLhs lhs, const Tolerance<_typeTolerance, _numComparedValuesSSE>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value ||
                          simd::IsRegisterType<_typeLhs>,
                  "Lhs type must be an integral, floating point or sse register type.");

    return rhs == lhs;
}



template <typename _typeLhs, typename _typeTolerance, U32 _numComparedValuesSSE>
constexpr bool operator!=(const _typeLhs lhs, const Tolerance<_typeTolerance, _numComparedValuesSSE>& rhs)
{
    static_assert(std::is_floating_point<_typeLhs>::value || std::is_integral<_typeLhs>::value ||
                          simd::IsRegisterType<_typeLhs>,
                  "Lhs type must be an integral, floating point or sse register type.");

    return rhs != lhs;
}



} // namespace GDL
