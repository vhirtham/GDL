#pragma once
#include "gdl/base/sse/swizzle.h"

#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/maskMacros.h"
#include "gdl/base/sse/utility.h"

namespace GDL::sse
{



template <U32 _index, typename _registerType>
inline _registerType Broadcast(_registerType reg)
{
    static_assert(_index < numValuesPerLane<_registerType>, "_index must be in the interval [0, numValuesPerLane]");

    if constexpr (numValuesPerLane<_registerType> == 4)
        return Permute<_index, _index, _index, _index>(reg);

    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return _mmx_permute_p<PERMUTE_2_MASK(_index, _index)>(reg);

#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return _mm256_permute_pd(reg, PERMUTE256_PD_MASK(_index, _index, _index, _index));

#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
} // namespace GDL::sse



template <U32 _index, typename _registerType>
inline _registerType BroadcastAcrossLanes(_registerType reg)
{
    static_assert(_index < numRegisterValues<_registerType>, "_index must be in the interval [0, numRegisterValues]");

    if constexpr (std::is_same<_registerType, __m128>::value || std::is_same<_registerType, __m128d>::value)
        return Broadcast<_index>(reg);

#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value || std::is_same<_registerType, __m256d>::value)
    {
        constexpr U32 valueIndex = _index % numValuesPerLane<_registerType>;
        constexpr U32 laneIndex = _index / numValuesPerLane<_registerType>;
        return Permute2F128<laneIndex, laneIndex>(Broadcast<valueIndex>(reg));
    }
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m128 Permute(__m128 source)
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4,
                  "Values _src0-_src3 must be in the interval [0, 3]");
    return _mmx_permute_p<PERMUTE_4_MASK(_src0, _src1, _src2, _src3)>(source);
}


#ifdef __AVX2__


template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256 Permute(__m256 source)
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4,
                  "Values _src0-_src3 must be in the interval [0, 3]");
    return _mmx_permute_p<PERMUTE_4_MASK(_src0, _src1, _src2, _src3)>(source);
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
inline __m256 Permute(__m256 source)
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4 && _src4 < 4 && _src5 < 4 && _src6 < 4 && _src7 < 4,
                  "Values _src0-_src7 must be in the interval [0, 3]");
    return _mm256_permutevar_ps(source, _mm256_setr_epi32(_src0, _src1, _src2, _src3, _src4, _src5, _src6, _src7));
}

#endif // __AVX2__


#ifdef __AVX2__

template <U32 _lane0SrcLane, U32 _lane1SrcLane, typename _registerType>
inline _registerType Permute2F128(_registerType source)
{
    return Permute2F128<0, _lane0SrcLane, 0, _lane1SrcLane>(source, source);
}



template <U32 _lane0SrcReg, U32 _lane0SrcLane, U32 _lane1SrcReg, U32 _lane1SrcLane, typename _registerType>
inline _registerType Permute2F128(_registerType source0, _registerType source1)
{
    if constexpr (std::is_same<_registerType, __m256>::value || std::is_same<_registerType, __m256d>::value)
        return _mmx_permute2f128_p<PERMUTE_2F128_MASK(_lane0SrcReg, _lane0SrcLane, _lane1SrcReg, _lane1SrcLane)>(
                source0, source1);
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}

#endif // __AVX2__



} // namespace GDL::sse
