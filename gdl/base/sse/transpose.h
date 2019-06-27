#pragma once

#include "gdl/base/sse/x86intrin.h"



namespace GDL::sse
{

//! @brief Transposes a 2x2 matrix which consists of 2 __m128d registers
//! @param in: Input registers
//! @param out: Output registers
inline void Transpose(__m128d in0, __m128d in1, __m128d& out0, __m128d& out1);

//! @brief Transposes a 4x4 matrix which consists of 4 __m128 registers
//! @param in: Input registers
//! @param out: Output registers
inline void Transpose(__m128 in0, __m128 in1, __m128 in2, __m128 in3, __m128& out0, __m128& out1, __m128& out2,
                      __m128& out3);

#ifdef __AVX2__

//! @brief Transposes a 4x4 matrix which consists of 4 __m256d registers
//! @param in: Input registers
//! @param out: Output registers
inline void Transpose(__m256d in0, __m256d in1, __m256d in2, __m256d in3, __m256d& out0, __m256d& out1, __m256d& out2,
                      __m256d& out3);

//! @brief Transposes a 8x8 matrix which consists of 8 __m256 registers
//! @param in: Input registers
//! @param out: Output registers
//! @remark source: https://stackoverflow.com/questions/25622745/transpose-an-8x8-float-using-avx-avx2
inline void Transpose(__m256 in0, __m256 in1, __m256 in2, __m256 in3, __m256 in4, __m256 in5, __m256 in6, __m256 in7,
                      __m256& out0, __m256& out1, __m256& out2, __m256& out3, __m256& out4, __m256& out5, __m256& out6,
                      __m256& out7);

#endif // __AVX2__

} // namespace GDL::sse


#include "gdl/base/sse/transpose.inl"
