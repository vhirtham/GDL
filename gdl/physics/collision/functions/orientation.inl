#pragma once

#include "gdl/physics/collision/functions/orientation.h"

#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/sse/mat2fSSE.h"
#include "gdl/math/sse/vec2fSSE.h"

namespace GDL
{

template <bool _isCol>
inline F32 Orientation(Vec2fSSE<_isCol> a, Vec2fSSE<_isCol> b, Vec2fSSE<_isCol> c)
{
    Vec2fSSE tmp0 = a - c;
    Vec2fSSE tmp1 = b - c;
    __m128 tmp2 = sse::Shuffle<0, 1, 0, 1>(tmp0.DataSSE(), tmp1.DataSSE());
    return Mat2fSSE(tmp2).Det();
}

} // namespace GDL
