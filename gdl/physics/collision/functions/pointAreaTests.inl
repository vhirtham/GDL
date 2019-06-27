#pragma once

#include "gdl/physics/collision/functions/pointAreaTests.h"

#include "gdl/base/exception.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/vec2.h"
#include "gdl/physics/collision/functions/orientation.h"

namespace GDL
{
template <bool _isCol>
F32 PointInsideCircle(const Vec2fSSE<_isCol>& point, const Vec2fSSE<_isCol>& c0, const Vec2fSSE<_isCol>& c1,
                      const Vec2fSSE<_isCol>& c2)
{
    using namespace GDL::simd;

    DEV_EXCEPTION(Orientation(c0, c1, c2) <= 0,
                  "The passed points c0, c1 and c2 must be in counter clockwise ordering if viewed as triangle");

    __m128 px = Permute<0, 0, 0, 3>(point.DataSSE());
    __m128 py = Permute<1, 1, 1, 3>(point.DataSSE());

    __m128 c01 = _mm_unpacklo_ps(c0.DataSSE(), c1.DataSSE());

    __m128 cx = Shuffle<0, 1, 0, 3>(c01, c2.DataSSE());
    __m128 cy = Shuffle<2, 3, 1, 3>(c01, c2.DataSSE());

    __m128 tmp0 = _mmx_sub_p(cx, px);
    __m128 tmp1 = _mmx_sub_p(cy, py);

    __m128 tmp2 = _mmx_fmadd_p(tmp0, tmp0, _mmx_mul_p(tmp1, tmp1));

    return Determinant3x3(tmp0, tmp1, tmp2);
}


} // namespace GDL
