#pragma once

#include "gdl/physics/collision/functions/orientation.h"

#include "gdl/base/simd/determinant.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/swizzle.h"
#include "gdl/math/simd/vec2fSSE.h"
#include "gdl/math/simd/vec3fSSE.h"

namespace GDL
{

template <bool _isCol>
inline F32 Orientation(Vec2fSSE<_isCol> a, Vec2fSSE<_isCol> b, Vec2fSSE<_isCol> c)
{
    Vec2fSSE tmp0 = a - c;
    Vec2fSSE tmp1 = b - c;
    return simd::Determinant2x2(tmp0.DataSSE(), tmp1.DataSSE());
}



template <bool _isCol>
inline F32 Orientation(Vec3fSSE<_isCol> a, Vec3fSSE<_isCol> b, Vec3fSSE<_isCol> c, Vec3fSSE<_isCol> d)
{
    Vec3fSSE tmp0 = a - d;
    Vec3fSSE tmp1 = b - d;
    Vec3fSSE tmp2 = c - d;

    return simd::Determinant3x3(tmp0.DataSSE(), tmp1.DataSSE(), tmp2.DataSSE());
}


} // namespace GDL
