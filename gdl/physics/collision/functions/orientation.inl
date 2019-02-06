#pragma once

#include "gdl/physics/collision/functions/orientation.h"

#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/maskMacros.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/vec2.h"

namespace GDL
{

template <bool _isCol>
inline F32 Orientation(Vec2fSSE<_isCol> a, Vec2fSSE<_isCol> b, Vec2fSSE<_isCol> c)
{
    __m128 tmp0 = _mmx_sub_p(b.DataSSE(), c.DataSSE());

    __m128 tmp1 = _mm_shuffle_ps(tmp0, c.DataSSE(), SHUFFLE_4_MASK(1, 0, 1, 0));
    __m128 tmp2 = _mm_xor_ps(_mm_shuffle_ps(a.DataSSE(), b.DataSSE(), SHUFFLE_4_MASK(0, 1, 0, 1)),
                             _mmx_setr_p<__m128>(0.f, -0.f, 0.f, -0.f));
    return sse::DotProduct(tmp1, tmp2);
}

} // namespace GDL
