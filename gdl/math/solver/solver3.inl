#pragma once

#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/math/solver/solver3.h"
#include "gdl/math/sse/mat3fSSE.h"
#include "gdl/math/sse/vec3fSSE.h"


namespace GDL::Solver
{

Vec3fSSE<true> Cramer(const Mat3fSSE& A, const Vec3fSSE<true>& b)
{
    using namespace GDL::sse;

    const std::array<__m128, 3>& dataA = A.DataSSE();

    __m128 cross12 = CrossProduct(dataA[1], dataA[2]);
    __m128 detA = DotProduct<1, 1, 1, 0>(dataA[0], cross12);

    DEV_EXCEPTION(_mm_cvtss_f32(detA) == ApproxZero<F32>(), "Singular matrix - system not solveable");

    __m128 x = DotProduct<1, 1, 1, 0>(b.DataSSE(), cross12);
    x = Blend<0, 1, 0, 0>(x, DotProduct<1, 1, 1, 0>(dataA[0], CrossProduct(b.DataSSE(), dataA[2])));
    x = Blend<0, 0, 1, 0>(x, DotProduct<1, 1, 1, 0>(dataA[0], CrossProduct(dataA[1], b.DataSSE())));

    return Vec3fSSE<true>(_mmx_div_p(x, detA));
}

} // namespace GDL::Solver
