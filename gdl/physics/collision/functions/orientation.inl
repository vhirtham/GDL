#pragma once

#include "gdl/physics/collision/functions/orientation.h"

#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/maskMacros.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/vec2.h"

namespace GDL
{

template <bool _isCol>
inline F32 Orientation(Vec2fSSE<_isCol> a, Vec2fSSE<_isCol> b, Vec2fSSE<_isCol> c)
{
    __m128 tmp0 = _mm_shuffle_ps(a.DataSSE(),b.DataSSE(),SHUFFLE_4_MASK(0,1,0,1));
    __m128 tmp1 = _mm_shuffle_ps(c.DataSSE(),c.DataSSE(),SHUFFLE_4_MASK(0,1,0,1));
    __m128 tmp2 =_mmx_sub_p(tmp0,tmp1);
    __m128 tmp3 = _mmx_mul_p(tmp2, sse::Swizzle<3, 2, 1, 0>(tmp2));
    return _mm_cvtss_f32(_mm_sub_ps(tmp3, sse::Swizzle<1, 0, 3, 1>(tmp3)));
}

} // namespace GDL
