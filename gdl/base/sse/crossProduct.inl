#pragma once

#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/swizzle.h"


namespace GDL::sse
{
[[nodiscard]] inline __m128 CrossProduct(__m128 lhs, __m128 rhs)
{
    // source: http://threadlocalmutex.com/?p=8
    __m128 lhs_yzx = Permute<1, 2, 0, 3>(lhs);
    __m128 rhs_yzx = Permute<1, 2, 0, 3>(rhs);

    __m128 tmp = _mmx_fmsub_p(lhs, rhs_yzx, _mmx_mul_p(lhs_yzx, rhs));

    return Permute<1, 2, 0, 3>(tmp);
}

} // namespace GDL::sse
