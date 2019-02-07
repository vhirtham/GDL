#pragma once
#include "gdl/base/sse/swizzle.h"

#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/maskMacros.h"
#include "gdl/base/sse/utility.h"

namespace GDL::sse
{


template <U32 _x0, U32 _x1, U32 _x2, U32 _x3>
inline __m128 Swizzle(__m128 source)
{
    static_assert(_x0 < 4 && _x1 < 4 && _x2 < 4 && _x3 < 4, "Values _x0-_x3 must be in the interval [0, 3]");
    return _mmx_permute_p<PERMUTE_4_MASK(_x0, _x1, _x2, _x3)>(source);
}



template <U32 _index, typename _registerType>
inline _registerType Swizzle1(_registerType reg)
{
    static_assert(_index < numValuesPerLane<_registerType>, "_index must be in the interval [0, numValuesPerLane]");

    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mmx_permute_p<PERMUTE_4_MASK(_index, _index, _index, _index)>(reg);

    if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mmx_permute_p<PERMUTE_2_MASK(_index, _index)>(reg);

#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return _mm256_permute_ps(reg, PERMUTE_4_MASK(_index, _index, _index, _index));

    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_permute_pd(reg, PERMUTE256_PD_MASK(_index % 2, _index % 2, _index % 2, _index % 2));

#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
} // namespace GDL::sse



template <U32 _index, typename _registerType>
inline _registerType Swizzle1AcrossLanes(_registerType reg)
{
    static_assert(_index < numRegisterValues<_registerType>, "_index must be in the interval [0, numRegisterValues]");

    if constexpr (std::is_same<_registerType, __m128>::value)
        return _mmx_permute_p<PERMUTE_4_MASK(_index, _index, _index, _index)>(reg);

    if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mmx_permute_p<PERMUTE_2_MASK(_index, _index)>(reg);

#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
    {
        _registerType tmp = _mm256_permute_ps(reg, PERMUTE_4_MASK(_index % 4, _index % 4, _index % 4, _index % 4));
        return _mm256_permute2f128_ps(tmp, tmp, PERMUTE_2F128_MASK(0, _index / 4, 0, _index / 4));
    }

    else if constexpr (std::is_same<_registerType, __m256d>::value)
    {
        _registerType tmp = _mm256_permute_pd(reg, PERMUTE256_PD_MASK(_index % 2, _index % 2, _index % 2, _index % 2));
        return _mm256_permute2f128_pd(tmp, tmp, PERMUTE_2F128_MASK(0, _index / 2, 0, _index / 2));
    }
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



} // namespace GDL::sse
