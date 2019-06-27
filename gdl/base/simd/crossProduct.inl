#pragma once

#include "gdl/base/simd/crossProduct.h"
#include "gdl/base/simd/swizzle.h"


namespace GDL::simd
{
[[nodiscard]] inline __m128 CrossProduct(__m128 lhs, __m128 rhs)
{
    // source: http://threadlocalmutex.com/?p=8
    __m128 lhs_yzx = Permute<1, 2, 0, 3>(lhs);
    __m128 rhs_yzx = Permute<1, 2, 0, 3>(rhs);

    __m128 tmp = _mm_fmsub(lhs, rhs_yzx, _mm_mul(lhs_yzx, rhs));

    return Permute<1, 2, 0, 3>(tmp);
}

} // namespace GDL::simd
