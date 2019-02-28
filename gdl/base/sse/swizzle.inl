#pragma once
#include "gdl/base/sse/swizzle.h"

#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/utility.h"

namespace GDL::sse
{


template <U32 _src0, U32 _src1>
inline __m128d Blend(__m128d source0, __m128d source1)
{
    return _mmx_blend_p<BlendMask<_src0, _src1>()>(source0, source1);
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, typename _registerType>
inline _registerType Blend(_registerType source0, _registerType source1)
{
    static_assert(Is__m128<_registerType> || Is__m256d<_registerType>,
                  "Function overload only compatble with __m128 and __m256d registers.");

    return _mmx_blend_p<BlendMask<_src0, _src1, _src2, _src3>()>(source0, source1);
}



#ifdef __AVX2__

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
inline __m256 Blend(__m256 source0, __m256 source1)
{
    return _mmx_blend_p<BlendMask<_src0, _src1, _src2, _src3, _src4, _src5, _src6, _src7>()>(source0, source1);
}

#endif // __AVX2__



template <U32 _src0, U32 _src1>
constexpr U32 BlendMask()
{
    static_assert(_src0 < 2 && _src1 < 2, "Values _src0 ans _src1 must be in the interval [0, 1]");

    return (((_src1) << 1) | (_src0));
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 BlendMask()
{
    static_assert(_src0 < 2 && _src1 < 2 && _src2 < 2 && _src3 < 2,
                  "Values _src0-_src3 must be in the interval [0, 1]");

    return (((_src3) << 3) | ((_src2) << 2) | ((_src1) << 1) | (_src0));
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
constexpr U32 BlendMask()
{
    static_assert(_src0 < 2 && _src1 < 2 && _src2 < 2 && _src3 < 2 && _src4 < 2 && _src5 < 2 && _src6 < 2 && _src7 < 2,
                  "Values _src0-_src7 must be in the interval [0, 1]");

    return (((_src7) << 7) | ((_src6) << 6) | ((_src5) << 5) | ((_src4) << 4) | ((_src3) << 3) | ((_src2) << 2) |
            ((_src1) << 1) | (_src0));
}



template <U32 _index, typename _registerType>
inline _registerType Broadcast(_registerType reg)
{
    static_assert(_index < numValuesPerLane<_registerType>, "_index must be in the interval [0, numValuesPerLane]");
    static_assert(IsRegisterType<_registerType>,
                  "Only __m128, __m128d, __m256 and __m256d registers are supported by this function.");

    if constexpr (Is__m128d<_registerType>)
        return Permute<_index, _index>(reg);
    else
        return Permute<_index, _index, _index, _index>(reg);

} // namespace GDL::sse



template <U32 _index, typename _registerType>
inline _registerType BroadcastAcrossLanes(_registerType reg)
{
    static_assert(IsRegisterType<_registerType>,
                  "Only __m128, __m128d, __m256 and __m256d registers are supported by this function.");

    if constexpr (numLanes<_registerType> == 1)
        return Broadcast<_index>(reg);
#ifdef __AVX2__
    else
    {

        constexpr U32 valueIndex = _index % numValuesPerLane<_registerType>;
        constexpr U32 laneIndex = _index / numValuesPerLane<_registerType>;
        return Permute2F128<laneIndex, laneIndex>(Broadcast<valueIndex>(reg));
    }


#endif // __AVX2__
}



template <U32 _src0, U32 _src1>
inline __m128d Permute(__m128d source)
{
    return _mmx_permute_p<PermuteMask<_src0, _src1>()>(source);
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m128 Permute(__m128 source)
{
    return _mmx_permute_p<PermuteMask<_src0, _src1, _src2, _src3>()>(source);
}



#ifdef __AVX2__

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256 Permute(__m256 source)
{
    return _mmx_permute_p<PermuteMask<_src0, _src1, _src2, _src3>()>(source);
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256d Permute(__m256d source)
{
    return _mmx_permute_p<Permute256dMask<_src0, _src1, _src2, _src3>()>(source);
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
inline __m256 Permute(__m256 source)
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4 && _src4 < 4 && _src5 < 4 && _src6 < 4 && _src7 < 4,
                  "Values _src0-_src7 must be in the interval [0, 3]");

    return _mm256_permutevar_ps(source, _mm256_setr_epi32(_src0, _src1, _src2, _src3, _src4, _src5, _src6, _src7));
}

#endif // __AVX2__



template <U32 _src0, U32 _src1>
constexpr U32 PermuteMask()
{
    static_assert(_src0 < 2 && _src1 < 2, "Values _src0 ans _src1 must be in the interval [0, 1]");

    return (((_src1) << 1) | (_src0));
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 PermuteMask()
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4,
                  "Values _src0-_src3 must be in the interval [0, 3]");

    return (((_src3) << 6) | ((_src2) << 4) | ((_src1) << 2) | (_src0));
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 Permute256dMask()
{
    static_assert(_src0 < 2 && _src1 < 2 && _src2 < 2 && _src3 < 2,
                  "Values _src0-_src3 must be in the interval [0, 1]");

    return (((_src3) << 3) | ((_src2) << 2) | ((_src1) << 1) | (_src0));
}



#ifdef __AVX2__

template <U32 _lane0SrcLane, U32 _lane1SrcLane, typename _registerType>
inline _registerType Permute2F128(_registerType source)
{
    return Permute2F128<0, _lane0SrcLane, 0, _lane1SrcLane>(source, source);
}



template <U32 _lane0SrcReg, U32 _lane0SrcLane, U32 _lane1SrcReg, U32 _lane1SrcLane, typename _registerType>
inline _registerType Permute2F128(_registerType source0, _registerType source1)
{
    static_assert(Is__m256<_registerType> || Is__m256d<_registerType>,
                  "Function only compatible with __m256 and __m256d registers.");

    return _mmx_permute2f128_p<Permute2F128Mask<_lane0SrcReg, _lane0SrcLane, _lane1SrcReg, _lane1SrcLane>()>(source0,
                                                                                                             source1);
}



template <U32 _lane0SrcReg, U32 _lane0SrcLane, U32 _lane1SrcReg, U32 _lane1SrcLane>
constexpr U32 Permute2F128Mask()
{
    static_assert(_lane0SrcReg < 2 && _lane0SrcLane < 2 && _lane1SrcReg < 2 && _lane1SrcLane < 2,
                  "Template values must be in the interval [0, 1]");

    return (((_lane1SrcReg) << 5) | ((_lane1SrcLane) << 4) | ((_lane0SrcReg) << 1) | (_lane0SrcLane));
}



#endif // __AVX2__



template <U32 _src0, U32 _src1, typename _registerType>
inline _registerType Shuffle(_registerType source0, _registerType source1)
{
    static_assert(Is__m128d<_registerType> || Is__m256d<_registerType>,
                  "Function only compatible with __m128d and __m256d registers.");

    if constexpr (Is__m128d<_registerType>)
        return _mmx_shuffle_p<ShuffleMask<_src0, _src1>()>(source0, source1);
#ifdef __AVX2__
    else
        return _mmx_shuffle_p<ShuffleMask256d<_src0, _src1, _src0, _src1>()>(source0, source1);
#endif // __AVX2__
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256d Shuffle(__m256d source0, __m256d source1)
{
    return _mmx_shuffle_p<ShuffleMask256d<_src0, _src1, _src2, _src3>()>(source0, source1);
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, typename _registerType>
inline _registerType Shuffle(_registerType source0, _registerType source1)
{
    static_assert(Is__m128<_registerType> || Is__m256<_registerType>,
                  "Function only compatible with __m128 and __m256 float registers.");

    return _mmx_shuffle_p<ShuffleMask<_src0, _src1, _src2, _src3>()>(source0, source1);
}



template <U32 _src0, U32 _src1>
constexpr U32 ShuffleMask()
{
    static_assert(_src0 < 2 && _src1 < 2, "Values _src0 and _src1 must be in the interval [0, 1]");

    return (((_src1) << 1) | (_src0));
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 ShuffleMask()
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4,
                  "Values _src0 to _src3 must be in the interval [0, 3]");

    return (((_src3) << 6) | ((_src2) << 4) | ((_src1) << 2) | (_src0));
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 ShuffleMask256d()
{
    static_assert(_src0 < 2 && _src1 < 2 && _src2 < 2 && _src3 < 2,
                  "Values _src0 to _src3 must be in the interval [0, 1]");
    return (((_src3) << 3) | ((_src2) << 2) | ((_src1) << 1) | (_src0));
}



} // namespace GDL::sse