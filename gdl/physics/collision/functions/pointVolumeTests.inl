#pragma once

#include "gdl/physics/collision/functions/pointVolumeTests.h"

#include "gdl/base/exception.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/vec3.h"
#include "gdl/physics/collision/functions/orientation.h"


namespace GDL
{

template <bool _isCol>
F32 PointInsideSphere(const Vec3fSSE<_isCol>& point, const Vec3fSSE<_isCol>& c0, const Vec3fSSE<_isCol>& c1,
                      const Vec3fSSE<_isCol>& c2, const Vec3fSSE<_isCol>& c3)
{
    using namespace GDL::sse;

    DEV_EXCEPTION(Orientation(c0, c1, c2, c3) <= 0, "Invalid order of the passed points c0, c1, c2 and c3.");

    __m128 px = Permute<0, 0, 0, 0>(point.DataSSE());
    __m128 py = Permute<1, 1, 1, 1>(point.DataSSE());
    __m128 pz = Permute<2, 2, 2, 2>(point.DataSSE());

    __m128 c01xy = _mm_unpacklo_ps(c0.DataSSE(), c1.DataSSE());
    __m128 c01z0 = _mm_unpackhi_ps(c0.DataSSE(), c1.DataSSE());
    __m128 c23xy = _mm_unpacklo_ps(c2.DataSSE(), c3.DataSSE());
    __m128 c23z0 = _mm_unpackhi_ps(c2.DataSSE(), c3.DataSSE());

    __m128 cx = Shuffle<0, 1, 0, 1>(c01xy, c23xy);
    __m128 cy = Shuffle<2, 3, 2, 3>(c01xy, c23xy);
    __m128 cz = Shuffle<0, 1, 0, 1>(c01z0, c23z0);

    __m128 tmp0 = _mmx_sub_p(cx, px);
    __m128 tmp1 = _mmx_sub_p(cy, py);
    __m128 tmp2 = _mmx_sub_p(cz, pz);

    __m128 tmp3 = _mmx_fmadd_p(tmp0, tmp0, _mmx_fmadd_p(tmp1, tmp1, _mmx_mul_p(tmp2, tmp2)));

    return Determinant4x4(tmp0, tmp1, tmp2, tmp3);
}

} // namespace GDL
