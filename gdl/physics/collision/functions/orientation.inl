#pragma once

#include "gdl/physics/collision/functions/orientation.h"

#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/sse/vec2fSSE.h"

namespace GDL
{

template <bool _isCol>
inline F32 Orientation(Vec2fSSE<_isCol> a, Vec2fSSE<_isCol> b, Vec2fSSE<_isCol> c)
{
    Vec2fSSE tmp0 = a - c;
    Vec2fSSE tmp1 = b - c;
    return sse::Determinant2x2(tmp0.DataSSE(), tmp1.DataSSE());
}

} // namespace GDL
