#pragma once

#include "gdl/base/simd/sum.h"

#include "gdl/base/simd/swizzle.h"

namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

inline __m128 Sum(__m128 source)
{
    __m128 sum = _mm_add(source, Permute<1, 0, 3, 2>(source));
    return _mm_add(sum, Permute<2, 3, 0, 1>(sum));
}



// --------------------------------------------------------------------------------------------------------------------

inline __m128d Sum(__m128d source)
{
    return _mm_add(source, Permute<1, 0>(source));
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

inline __m256 Sum(__m256 source)
{
    __m256 sum = _mm_add(source, Permute<1, 0, 3, 2>(source));
    sum = _mm_add(sum, Permute<2, 3, 0, 1>(sum));
    return _mm_add(sum, Permute2F128<1, 0>(sum));
}



// --------------------------------------------------------------------------------------------------------------------

inline __m256d Sum(__m256d source)
{
    __m256d sum = _mm_add(source, Permute<1, 0>(source));
    return _mm_add(sum, Permute2F128<1, 0>(sum));
}

#endif //__AVX2__



} // namespace GDL::simd
