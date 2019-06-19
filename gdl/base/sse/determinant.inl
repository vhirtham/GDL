#pragma once

#include "gdl/base/sse/determinant.h"

#include "gdl/base/sse/crossProduct.h"
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



inline F32 Determinant2x2(__m128 a, __m128 b)
{
    return Determinant2x2(Shuffle<0, 1, 0, 1>(a, b));
}



inline F32 Determinant3x3(__m128 a, __m128 b, __m128 c)
{
    return DotProductF32<1, 1, 1, 0>(a, CrossProduct(b, c));
}



inline F32 Determinant4x4(__m128 a, __m128 b, __m128 c, __m128 d)
{
    // Calculate sum of first 4 terms
    __m128 aP1230 = Permute<1, 2, 3, 0>(a);
    __m128 bP1230 = Permute<1, 2, 3, 0>(b);
    __m128 cP1230 = Permute<1, 2, 3, 0>(c);
    __m128 dP1230 = Permute<1, 2, 3, 0>(d);

    __m128 ab03 = _mmx_fmsub_p(a, bP1230, _mm_mul_ps(aP1230, b));
    __m128 cd03 = _mmx_fmsub_p(c, dP1230, _mm_mul_ps(cP1230, d));

    __m128 cd03P2301 = Permute<2, 3, 0, 1>(cd03);
    __m128 cd03P2301N = Negate<0, 1, 0, 1>(cd03P2301);

    __m128 sum03 = DotProduct(ab03, cd03P2301N);


    // Calculate sum of last 2 terms
    __m128 acB0011 = Blend<0, 0, 1, 1>(a, c);
    __m128 acB1100 = Blend<1, 1, 0, 0>(a, c);
    __m128 bdB0011 = Blend<0, 0, 1, 1>(b, d);
    __m128 bdB1100 = Blend<1, 1, 0, 0>(b, d);

    __m128 acB1100P2301 = Permute<2, 3, 0, 1>(acB1100);
    __m128 bdB1100P2301 = Permute<2, 3, 0, 1>(bdB1100);

    __m128 abcd45 = _mmx_fmsub_p(acB0011, bdB1100P2301, _mm_mul_ps(acB1100P2301, bdB0011));
    __m128 abcd45P3210 = Permute<3, 2, 1, 0>(abcd45);

    __m128 products45 = _mm_mul_ps(abcd45, abcd45P3210);
    __m128 sum45 = _mm_add_ps(products45, Permute<1, 0, 3, 2>(products45));


    // Calculate and return determinant
    return _mm_cvtss_f32(sum03) + _mm_cvtss_f32(sum45);

    // Alternative version

    //    __m128 d1032 = Permute<1, 0, 3, 2>(d);
    //    __m128 c1032 = Permute<1, 0, 3, 2>(c);

    //    __m128 tmp1 = Permute<2, 3, 0, 1>(_mmx_fmsub_p(c, d1032, _mmx_mul_p(d, c1032)));
    //    __m128 tmp2 = _mmx_fmsub_p(Permute<3, 2, 0, 1>(c), d1032, _mmx_mul_p(Permute<3, 2, 0, 1>(d), c1032));

    //    __m128 tmp3 = Negate<1, 1, 0, 0>(tmp2);

    //    __m128 b1032 = Permute<1, 0, 3, 2>(b);
    //    __m128 b2310 = Permute<2, 3, 1, 0>(b);
    //    __m128 tmp4 = Permute<3, 2, 0, 1>(_mmx_mul_p((b), tmp3));

    //    __m128 tmp5 = _mmx_fmsub_p(b1032, tmp1, _mmx_fmsub_p(b2310, tmp3, tmp4));

    //    return DotProductF32(a, tmp5);
}


#ifdef __AVX2__
inline F32 Determinant4x4(__m256 ab, __m256 cd)
{
    // Calculate the components of the first 4 terms
    __m256 abP1230 = Permute<1, 2, 3, 0>(ab);
    __m256 cdP1230 = Permute<1, 2, 3, 0>(cd);

    __m256 acbd03 = _mmx_fmsub_p(ab, cdP1230, _mmx_mul_p(cd, abP1230));
    __m256 acbd03N = Negate<0, 0, 0, 0, 1, 0, 1, 0>(acbd03);


    // Calculate the components of the last 2 terms
    __m256 abP2323 = Permute<2, 3, 2, 3>(ab);
    __m256 cdP2323 = Permute<2, 3, 2, 3>(cd);
    __m256 acbd45 = _mmx_fmsub_p(ab, cdP2323, _mmx_mul_p(cd, abP2323));


    // Calculate determinant
    __m256 ac03bd45 = Blend<0, 0, 0, 0, 1, 1, 1, 1>(acbd03N, acbd45);
    __m256 bd03ac45 = Permute2F128<0, 1, 1, 0>(acbd03N, acbd45);
    __m256 bd03ac45P = Permute<2, 3, 0, 1, 1, 0, 2, 3>(bd03ac45);

    __m256 sums = DotProduct(ac03bd45, bd03ac45P);

    __m256 determinant = _mmx_add_p(sums, Permute2F128<1, 0>(sums));

    return _mm256_cvtss_f32(determinant);
}
#endif // __AVX2__



} // namespace GDL::sse
