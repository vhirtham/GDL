#pragma once

#include "gdl/base/sse/transpose.h"

#include "gdl/base/sse/swizzle.h"
#include "gdl/base/sse/utility.h"
#include "gdl/base/sse/x86intrin.h"



namespace GDL::sse
{

inline void Transpose(__m128d in0, __m128d in1, __m128d& out0, __m128d& out1)
{
    out0 = _mm_unpacklo_pd(in0, in1);
    out1 = _mm_unpackhi_pd(in0, in1);
}



inline void Transpose(__m128 in0, __m128 in1, __m128 in2, __m128 in3, __m128& out0, __m128& out1, __m128& out2,
                      __m128& out3)
{
    alignas(alignmentBytes<__m128>) __m128 tmp0, tmp1, tmp2, tmp3;

    tmp0 = _mm_unpacklo_ps(in0, in1);
    tmp1 = _mm_unpackhi_ps(in0, in1);
    tmp2 = _mm_unpacklo_ps(in2, in3);
    tmp3 = _mm_unpackhi_ps(in2, in3);

    out0 = _mm_movelh_ps(tmp0, tmp2);
    out1 = _mm_movehl_ps(tmp2, tmp0);
    out2 = _mm_movelh_ps(tmp1, tmp3);
    out3 = _mm_movehl_ps(tmp3, tmp1);
}


#ifdef __AVX2__

inline void Transpose(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1, __m256d& out2,
                      __m256d& out3)
{
    alignas(alignmentBytes<__m256d>) __m256d tmp0, tmp1, tmp2, tmp3;

    tmp0 = _mm256_unpacklo_pd(in0, in1);
    tmp1 = _mm256_unpackhi_pd(in0, in1);
    tmp2 = _mm256_unpacklo_pd(in2, in3);
    tmp3 = _mm256_unpackhi_pd(in2, in3);

    out0 = Permute2F128<0, 0, 1, 0>(tmp0, tmp2);
    out1 = Permute2F128<0, 0, 1, 0>(tmp1, tmp3);
    out2 = Permute2F128<0, 1, 1, 1>(tmp0, tmp2);
    out3 = Permute2F128<0, 1, 1, 1>(tmp1, tmp3);
}



inline void Transpose(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                      __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6,
                      __m256& out7)
{
    alignas(alignmentBytes<__m256>) __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmpBlend;

    out0 = _mm256_unpacklo_ps(in0, in1);
    out1 = _mm256_unpackhi_ps(in0, in1);
    out2 = _mm256_unpacklo_ps(in2, in3);
    out3 = _mm256_unpackhi_ps(in2, in3);
    out4 = _mm256_unpacklo_ps(in4, in5);
    out5 = _mm256_unpackhi_ps(in4, in5);
    out6 = _mm256_unpacklo_ps(in6, in7);
    out7 = _mm256_unpackhi_ps(in6, in7);

    //    tmp0 = Shuffle<0, 1, 0, 1>(out0, out2);
    //    tmp1 = Shuffle<2, 3, 2, 3>(out0, out2);
    //    tmp2 = Shuffle<0, 1, 0, 1>(out1, out3);
    //    tmp3 = Shuffle<2, 3, 2, 3>(out1, out3);
    //    tmp4 = Shuffle<0, 1, 0, 1>(out4, out6);
    //    tmp5 = Shuffle<2, 3, 2, 3>(out4, out6);
    //    tmp6 = Shuffle<0, 1, 0, 1>(out5, out7);
    //    tmp7 = Shuffle<2, 3, 2, 3>(out5, out7);

    // Faster than pure shuffle (Around 6-8%). Have a look at the link in the function description
    tmpBlend = Shuffle<2, 3, 0, 1>(out0, out2);
    tmp0 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out0, tmpBlend);
    tmp1 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out2, tmpBlend);
    tmpBlend = Shuffle<2, 3, 0, 1>(out1, out3);
    tmp2 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out1, tmpBlend);
    tmp3 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out3, tmpBlend);
    tmpBlend = Shuffle<2, 3, 0, 1>(out4, out6);
    tmp4 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out4, tmpBlend);
    tmp5 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out6, tmpBlend);
    tmpBlend = Shuffle<2, 3, 0, 1>(out5, out7);
    tmp6 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(out5, tmpBlend);
    tmp7 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(out7, tmpBlend);

    out0 = Permute2F128<0, 0, 1, 0>(tmp0, tmp4);
    out1 = Permute2F128<0, 0, 1, 0>(tmp1, tmp5);
    out2 = Permute2F128<0, 0, 1, 0>(tmp2, tmp6);
    out3 = Permute2F128<0, 0, 1, 0>(tmp3, tmp7);
    out4 = Permute2F128<0, 1, 1, 1>(tmp0, tmp4);
    out5 = Permute2F128<0, 1, 1, 1>(tmp1, tmp5);
    out6 = Permute2F128<0, 1, 1, 1>(tmp2, tmp6);
    out7 = Permute2F128<0, 1, 1, 1>(tmp3, tmp7);
}

#endif // __AVX2__

} // namespace GDL::sse
