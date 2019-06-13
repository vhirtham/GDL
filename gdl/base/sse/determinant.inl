#pragma once

#include "gdl/base/sse/determinant.h"

#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/negate.h"
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



inline F32 Determinant4x4(__m128 col0, __m128 col1, __m128 col2, __m128 col3)
{
    __m128 d1032 = Permute<1, 0, 3, 2>(col3);
    __m128 c1032 = Permute<1, 0, 3, 2>(col2);

    __m128 tmp1 = Permute<2, 3, 0, 1>(_mmx_fmsub_p(col2, d1032, _mmx_mul_p(col3, c1032)));
    __m128 tmp2 = _mmx_fmsub_p(Permute<3, 2, 0, 1>(col2), d1032, _mmx_mul_p(Permute<3, 2, 0, 1>(col3), c1032));

    __m128 tmp3 = Negate<1, 1, 0, 0>(tmp2);

    __m128 b1032 = Permute<1, 0, 3, 2>(col1);
    __m128 b2310 = Permute<2, 3, 1, 0>(col1);
    __m128 tmp4 = Permute<3, 2, 0, 1>(_mmx_mul_p((col1), tmp3));

    __m128 tmp5 = _mmx_fmsub_p(b1032, tmp1, _mmx_fmsub_p(b2310, tmp3, tmp4));

    return DotProductF32(col0, tmp5);
}


#ifdef __AVX2__
inline F32 Determinant4x4(__m256 col01, __m256 col23)
{
    __m256 col01P1230 = Permute<1, 2, 3, 0>(col01);
    __m256 col23P1230 = Permute<1, 2, 3, 0>(col23);
    __m256 tmp0 = _mmx_fmsub_p(col01, col23P1230, _mmx_mul_p(col23, col01P1230));
    tmp0 = Negate<0, 0, 0, 0, 1, 0, 1, 0>(tmp0);

    __m256 col01P2323 = Permute<2, 3, 2, 3>(col01);
    __m256 col23P2323 = Permute<2, 3, 2, 3>(col23);
    __m256 tmp1 = _mmx_fmsub_p(col01, col23P2323, _mmx_mul_p(col23, col01P2323));

    __m256 tmp2 = Blend<0, 0, 0, 0, 1, 1, 1, 1>(tmp0, tmp1);
    __m256 tmp3 = Permute2F128<0, 1, 1, 0>(tmp0, tmp1);
    tmp3 = Permute<2, 3, 0, 1, 1, 0, 2, 3>(tmp3);

    __m256 tmp4 = DotProduct(tmp2, tmp3);

    __m256 result = _mmx_add_p(tmp4, Permute2F128<1, 0>(tmp4));
    return _mm256_cvtss_f32(result);
}
#endif // __AVX2__



} // namespace GDL::sse
