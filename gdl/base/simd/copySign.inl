#pragma once

#include "gdl/base/simd/copySign.h"

#include "gdl/base/exception.h"
#include "gdl/base/simd/compareAll.h"
#include "gdl/base/simd/intrinsics.h"

#include <cstring>

namespace GDL::simd
{

template <bool _srcValueAllPositive, typename _registerType>
inline _registerType CopySign(_registerType srcSign, _registerType srcAbsVal)
{
    const _registerType mask = _mm_set1<_registerType>(-0.);

    _registerType sign = _mm_and(srcSign, mask);
    if constexpr (_srcValueAllPositive)
    {
        DEV_EXCEPTION(!CompareMemoryZero(_mm_and(srcAbsVal, mask)), "Signed bit of one or more values set.");
        return _mm_or(sign, srcAbsVal);
    }
    else
    {
        _registerType value = _mm_andnot(mask, srcAbsVal);
        return _mm_or(sign, value);
    }
}



} // namespace GDL::simd
