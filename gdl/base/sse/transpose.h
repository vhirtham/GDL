#pragma once

#include <x86intrin.h>


// Replace if alignmentBytes is in own header
#include "gdl/base/sse/intrinsics.h"

namespace GDL
{

//! @brief Transposes a 2x2 matrix which consists of 2 __m128d registers
//! @param in: Input registers
//! @param out: Output registers
inline void Transpose(__m128d in0, __m128d in1, __m128d& out0, __m128d& out1)
{
    out0 = _mm_unpacklo_pd(in0, in1);
    out1 = _mm_unpackhi_pd(in0, in1);
}



//! @brief Transposes a 4x4 matrix which consists of 4 __m128 registers
//! @param in: Input registers
//! @param out: Output registers
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


//! @brief Transposes a 4x4 matrix which consists of 4 __m256d registers
//! @param in: Input registers
//! @param out: Output registers
inline void Transpose(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1, __m256d& out2,
                      __m256d& out3)
{
    alignas(alignmentBytes<__m256d>) __m256d tmp0, tmp1, tmp2, tmp3;

    tmp0 = _mm256_unpacklo_pd(in0, in1);
    tmp1 = _mm256_unpackhi_pd(in0, in1);
    tmp2 = _mm256_unpacklo_pd(in2, in3);
    tmp3 = _mm256_unpackhi_pd(in2, in3);

    out0 = _mm256_permute2f128_pd(tmp0, tmp2, 16 * 2);
    out1 = _mm256_permute2f128_pd(tmp1, tmp3, 16 * 2);
    out2 = _mm256_permute2f128_pd(tmp0, tmp2, 1 + 16 * 3);
    out3 = _mm256_permute2f128_pd(tmp1, tmp3, 1 + 16 * 3);
}



//! @brief Transposes a 8x8 matrix which consists of 8 __m256 registers
//! @param in: Input registers
//! @param out: Output registers
//! @remark source: https://stackoverflow.com/questions/25622745/transpose-an-8x8-float-using-avx-avx2
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

    // a[0] = _mm256_shuffle_ps(tmp[0],tmp[2], 0x44);
    // a[1] = _mm256_shuffle_ps(tmp[0],tmp[2], 0xEE);
    // a[2] = _mm256_shuffle_ps(tmp[1],tmp[3], 0x44);
    // a[3] = _mm256_shuffle_ps(tmp[1],tmp[3], 0xEE);
    // a[4] = _mm256_shuffle_ps(tmp[4],tmp[6], 0x44);
    // a[5] = _mm256_shuffle_ps(tmp[4],tmp[6], 0xEE);
    // a[6] = _mm256_shuffle_ps(tmp[5],tmp[7], 0x44);
    // a[7] = _mm256_shuffle_ps(tmp[5],tmp[7], 0xEE);

    tmpBlend = _mm256_shuffle_ps(out0, out2, 0x4E);
    tmp0 = _mm256_blend_ps(out0, tmpBlend, 0xCC);
    tmp1 = _mm256_blend_ps(out2, tmpBlend, 0x33);
    tmpBlend = _mm256_shuffle_ps(out1, out3, 0x4E);
    tmp2 = _mm256_blend_ps(out1, tmpBlend, 0xCC);
    tmp3 = _mm256_blend_ps(out3, tmpBlend, 0x33);
    tmpBlend = _mm256_shuffle_ps(out4, out6, 0x4E);
    tmp4 = _mm256_blend_ps(out4, tmpBlend, 0xCC);
    tmp5 = _mm256_blend_ps(out6, tmpBlend, 0x33);
    tmpBlend = _mm256_shuffle_ps(out5, out7, 0x4E);
    tmp6 = _mm256_blend_ps(out5, tmpBlend, 0xCC);
    tmp7 = _mm256_blend_ps(out7, tmpBlend, 0x33);

    out0 = _mm256_permute2f128_ps(tmp0, tmp4, 16 * 2);
    out1 = _mm256_permute2f128_ps(tmp1, tmp5, 16 * 2);
    out2 = _mm256_permute2f128_ps(tmp2, tmp6, 16 * 2);
    out3 = _mm256_permute2f128_ps(tmp3, tmp7, 16 * 2);
    out4 = _mm256_permute2f128_ps(tmp0, tmp4, 1 + 16 * 3);
    out5 = _mm256_permute2f128_ps(tmp1, tmp5, 1 + 16 * 3);
    out6 = _mm256_permute2f128_ps(tmp2, tmp6, 1 + 16 * 3);
    out7 = _mm256_permute2f128_ps(tmp3, tmp7, 1 + 16 * 3);
}

#endif // __AVX2__


} // namespace GDL
