#pragma once

#include "gdl/base/simd/registerSum.h"

#include "gdl/base/simd/transpose.h"
#include "gdl/base/simd/swizzle.h"

namespace GDL::simd
{


// --------------------------------------------------------------------------------------------------------------------

template <U32 _regIdxFirst, typename _registerType, std::size_t _size>
[[nodiscard]] inline _registerType RegisterArraySum(const std::array<_registerType, _size>& arr, U32 idxFirst) noexcept
{
    const _registerType zero = _mm_setzero<_registerType>();

    _registerType sum = BlendAboveIndex<_regIdxFirst>(arr[idxFirst], zero);

    for (U32 i = idxFirst + 1; i < arr.size(); ++i)
        sum = _mm_add(sum, arr[i]);
    return RegisterSum(sum);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _regIdxFirst, typename _registerType, std::size_t _size>
[[nodiscard]] inline _registerType RegisterArraySquareSum(const std::array<_registerType, _size>& arr,
                                                          U32 idxFirst) noexcept
{
    const _registerType zero = _mm_setzero<_registerType>();

    _registerType sum = BlendAboveIndex<_regIdxFirst>(arr[idxFirst], zero);
    sum = _mm_mul(sum, sum);

    for (U32 i = idxFirst + 1; i < arr.size(); ++i)
        sum = _mm_fmadd(arr[i], arr[i], sum);
    return RegisterSum(sum);
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline __m128d RegisterMultiSum(const std::array<__m128d, 2>& data) noexcept
{
    __m128d tmp_0 = Shuffle<0, 0>(data[0], data[1]);
    __m128d tmp_1 = Shuffle<1, 1>(data[0], data[1]);
    return _mm_add(tmp_0, tmp_1);
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline __m128 RegisterMultiSum(const std::array<__m128, 4>& data) noexcept
{
    __m128 tmp_0 = Shuffle<0, 1, 0, 1>(data[0], data[1]);
    __m128 tmp_1 = Shuffle<2, 3, 2, 3>(data[0], data[1]);
    __m128 tmp_2 = Shuffle<0, 1, 0, 1>(data[2], data[3]);
    __m128 tmp_3 = Shuffle<2, 3, 2, 3>(data[2], data[3]);

    __m128 tmp_4 = _mm_add(tmp_0, tmp_1);
    __m128 tmp_5 = _mm_add(tmp_2, tmp_3);

    __m128 tmp_6 = Shuffle<0, 2, 0, 2>(tmp_4, tmp_5);
    __m128 tmp_7 = Shuffle<1, 3, 1, 3>(tmp_4, tmp_5);

    return _mm_add(tmp_6, tmp_7);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

[[nodiscard]] inline __m256d RegisterMultiSum(const std::array<__m256d, 4>& data) noexcept
{
    __m256d tmp_0 = Shuffle<0, 0>(data[0], data[1]);
    __m256d tmp_1 = Shuffle<1, 1>(data[0], data[1]);
    __m256d tmp_2 = Shuffle<0, 0>(data[2], data[3]);
    __m256d tmp_3 = Shuffle<1, 1>(data[2], data[3]);

    __m256d tmp_4 = _mm_add(tmp_0, tmp_1);
    __m256d tmp_5 = _mm_add(tmp_2, tmp_3);

    __m256d tmp_6 = Permute2F128<0, 0, 1, 0>(tmp_4, tmp_5);
    __m256d tmp_7 = Permute2F128<0, 1, 1, 1>(tmp_4, tmp_5);

    return _mm_add(tmp_6, tmp_7);
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline __m256 RegisterMultiSum(const std::array<__m256, 8>& data) noexcept
{
    __m256 tmp_0_0 = Shuffle<0, 1, 0, 1>(data[0], data[1]);
    __m256 tmp_0_1 = Shuffle<2, 3, 2, 3>(data[0], data[1]);
    __m256 tmp_0_2 = Shuffle<0, 1, 0, 1>(data[2], data[3]);
    __m256 tmp_0_3 = Shuffle<2, 3, 2, 3>(data[2], data[3]);

    __m256 tmp_0_4 = Shuffle<0, 1, 0, 1>(data[4], data[5]);
    __m256 tmp_0_5 = Shuffle<2, 3, 2, 3>(data[4], data[5]);
    __m256 tmp_0_6 = Shuffle<0, 1, 0, 1>(data[6], data[7]);
    __m256 tmp_0_7 = Shuffle<2, 3, 2, 3>(data[6], data[7]);

    __m256 tmp_1_0 = _mm_add(tmp_0_0, tmp_0_1);
    __m256 tmp_1_1 = _mm_add(tmp_0_2, tmp_0_3);
    __m256 tmp_1_2 = _mm_add(tmp_0_4, tmp_0_5);
    __m256 tmp_1_3 = _mm_add(tmp_0_6, tmp_0_7);

    __m256 tmp_2_0 = Shuffle<0, 2, 0, 2>(tmp_1_0, tmp_1_1);
    __m256 tmp_2_1 = Shuffle<1, 3, 1, 3>(tmp_1_0, tmp_1_1);
    __m256 tmp_2_2 = Shuffle<0, 2, 0, 2>(tmp_1_2, tmp_1_3);
    __m256 tmp_2_3 = Shuffle<1, 3, 1, 3>(tmp_1_2, tmp_1_3);

    __m256 tmp_3_0 = _mm_add(tmp_2_0, tmp_2_1);
    __m256 tmp_3_1 = _mm_add(tmp_2_2, tmp_2_3);

    __m256 tmp_4_0 = Permute2F128<0, 0, 1, 0>(tmp_3_0, tmp_3_1);
    __m256 tmp_4_1 = Permute2F128<0, 1, 1, 1>(tmp_3_0, tmp_3_1);

    return _mm_add(tmp_4_0, tmp_4_1);
}

#endif



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline __m128 RegisterSum(__m128 source) noexcept
{
    __m128 sum = _mm_add(source, Permute<1, 0, 3, 2>(source));
    return _mm_add(sum, Permute<2, 3, 0, 1>(sum));
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline __m128d RegisterSum(__m128d source) noexcept
{
    return _mm_add(source, Permute<1, 0>(source));
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

[[nodiscard]] inline __m256 RegisterSum(__m256 source) noexcept
{
    __m256 sum = _mm_add(source, Permute<1, 0, 3, 2>(source));
    sum = _mm_add(sum, Permute<2, 3, 0, 1>(sum));
    return _mm_add(sum, Permute2F128<1, 0>(sum));
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline __m256d RegisterSum(__m256d source) noexcept
{
    __m256d sum = _mm_add(source, Permute<1, 0>(source));
    return _mm_add(sum, Permute2F128<1, 0>(sum));
}

#endif //__AVX2__



} // namespace GDL::simd
