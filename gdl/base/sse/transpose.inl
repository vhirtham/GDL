#pragma once

#include "gdl/base/sse/transpose.h"

#include "gdl/base/sse/maskMacros.h"
#include "gdl/base/sse/utility.h"

#include <x86intrin.h>

#include <iostream>

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

    out0 = _mm256_permute2f128_pd(tmp0, tmp2, PERMUTE_MASK_2F128(0, 0, 1, 0));
    out1 = _mm256_permute2f128_pd(tmp1, tmp3, PERMUTE_MASK_2F128(0, 0, 1, 0));
    out2 = _mm256_permute2f128_pd(tmp0, tmp2, PERMUTE_MASK_2F128(0, 1, 1, 1));
    out3 = _mm256_permute2f128_pd(tmp1, tmp3, PERMUTE_MASK_2F128(0, 1, 1, 1));
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

    //     tmp0 = _mm256_shuffle_ps(out0,out2, SHUFFLE_MASK_4(0, 1, 0, 1));
    //     tmp1 = _mm256_shuffle_ps(out0,out2, SHUFFLE_MASK_4(2, 3, 2, 3));
    //     tmp2 = _mm256_shuffle_ps(out1,out3, SHUFFLE_MASK_4(0, 1, 0, 1));
    //     tmp3 = _mm256_shuffle_ps(out1,out3, SHUFFLE_MASK_4(2, 3, 2, 3));
    //     tmp4 = _mm256_shuffle_ps(out4,out6, SHUFFLE_MASK_4(0, 1, 0, 1));
    //     tmp5 = _mm256_shuffle_ps(out4,out6, SHUFFLE_MASK_4(2, 3, 2, 3));
    //     tmp6 = _mm256_shuffle_ps(out5,out7, SHUFFLE_MASK_4(0, 1, 0, 1));
    //     tmp7 = _mm256_shuffle_ps(out5,out7, SHUFFLE_MASK_4(2, 3, 2, 3));

    // Faster than pure shuffle. Have a look at the link in the function description
    tmpBlend = _mm256_shuffle_ps(out0, out2, SHUFFLE_MASK_4(2, 3, 0, 1));
    tmp0 = _mm256_blend_ps(out0, tmpBlend, BLEND_MASK_8(0, 0, 1, 1, 0, 0, 1, 1));
    tmp1 = _mm256_blend_ps(out2, tmpBlend, BLEND_MASK_8(1, 1, 0, 0, 1, 1, 0, 0));
    tmpBlend = _mm256_shuffle_ps(out1, out3, SHUFFLE_MASK_4(2, 3, 0, 1));
    tmp2 = _mm256_blend_ps(out1, tmpBlend, BLEND_MASK_8(0, 0, 1, 1, 0, 0, 1, 1));
    tmp3 = _mm256_blend_ps(out3, tmpBlend, BLEND_MASK_8(1, 1, 0, 0, 1, 1, 0, 0));
    tmpBlend = _mm256_shuffle_ps(out4, out6, SHUFFLE_MASK_4(2, 3, 0, 1));
    tmp4 = _mm256_blend_ps(out4, tmpBlend, BLEND_MASK_8(0, 0, 1, 1, 0, 0, 1, 1));
    tmp5 = _mm256_blend_ps(out6, tmpBlend, BLEND_MASK_8(1, 1, 0, 0, 1, 1, 0, 0));
    tmpBlend = _mm256_shuffle_ps(out5, out7, SHUFFLE_MASK_4(2, 3, 0, 1));
    tmp6 = _mm256_blend_ps(out5, tmpBlend, BLEND_MASK_8(0, 0, 1, 1, 0, 0, 1, 1));
    tmp7 = _mm256_blend_ps(out7, tmpBlend, BLEND_MASK_8(1, 1, 0, 0, 1, 1, 0, 0));

    out0 = _mm256_permute2f128_ps(tmp0, tmp4, PERMUTE_MASK_2F128(0, 0, 1, 0));
    out1 = _mm256_permute2f128_ps(tmp1, tmp5, PERMUTE_MASK_2F128(0, 0, 1, 0));
    out2 = _mm256_permute2f128_ps(tmp2, tmp6, PERMUTE_MASK_2F128(0, 0, 1, 0));
    out3 = _mm256_permute2f128_ps(tmp3, tmp7, PERMUTE_MASK_2F128(0, 0, 1, 0));
    out4 = _mm256_permute2f128_ps(tmp0, tmp4, PERMUTE_MASK_2F128(0, 1, 1, 1));
    out5 = _mm256_permute2f128_ps(tmp1, tmp5, PERMUTE_MASK_2F128(0, 1, 1, 1));
    out6 = _mm256_permute2f128_ps(tmp2, tmp6, PERMUTE_MASK_2F128(0, 1, 1, 1));
    out7 = _mm256_permute2f128_ps(tmp3, tmp7, PERMUTE_MASK_2F128(0, 1, 1, 1));
}

#endif // __AVX2__

} // namespace GDL::sse
