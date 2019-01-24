#pragma once

#include "gdl/base/sse/swizzle.h"

#include "gdl/base/sse/maskMacros.h"

namespace GDL
{


template <U32 _x0, U32 _x1, U32 _x2, U32 _x3>
inline __m128 Swizzle(__m128 source)
{
    static_assert(_x0 < 4 && _x1 < 4 && _x2 < 4 && _x3 < 4, "Values _x0-_x3 must be in the interval [0,3]");
    return _mm_shuffle_ps(source, source, SHUFFLE_4_MASK(_x0, _x1, _x2, _x3));
}



template <U32 _index>
inline __m128 Swizzle1(__m128 reg)
{
    static_assert(_index < 4, "Value x0-x3 must be in the interval [0,3]");
    return _mm_shuffle_ps(reg, reg, SHUFFLE_4_MASK(_index, _index, _index, _index));
}
} // namespace GDL
