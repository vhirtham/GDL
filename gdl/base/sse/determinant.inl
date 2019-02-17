#pragma once

#include "gdl/base/sse/determinant.h"

#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/swizzle.h"

namespace GDL::sse
{

inline F32 Determinant2x2(__m128 data)
{
    __m128 tmp = _mmx_mul_p(data, Permute<3, 2, 1, 0>(data));
    return _mm_cvtss_f32(_mm_sub_ps(tmp, Permute<1, 0, 3, 1>(tmp)));
}



inline F32 Determinant2x2(__m128 col0, __m128 col1)
{
    return Determinant2x2(Shuffle<0, 1, 0, 1>(col0, col1));
}




inline F32 Determinant3x3(__m128 col0, __m128 col1, __m128 col2)
{
    __m128 r1_yzx = sse::Permute<1, 2, 0, 3>(col1);
    __m128 r2_yzx = sse::Permute<1, 2, 0, 3>(col2);

    __m128 tmp = _mmx_sub_p(_mmx_mul_p(col1, r2_yzx), _mmx_mul_p(r1_yzx, col2));

    return sse::DotProductF32<1, 1, 1, 0>(col0, sse::Permute<1, 2, 0, 3>(tmp));
}




} // namespace GDL::sse
