#pragma once
#include "gdl/base/simd/swizzle.h"

#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/utility.h"

namespace GDL::simd
{

// --------------------------------------------------------------------------------------------------------------------

template <U32 _shift, typename _registerType>
inline _registerType AlignRight([[maybe_unused]] _registerType src0, [[maybe_unused]] _registerType src1)
{
    constexpr U32 elementSize = sizeof(_registerType) / numRegisterValues<_registerType>;
    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;

    static_assert(_shift <= numLaneVals, "_shift must be in the range [0, numValuesPerLane]");

    if constexpr (_shift == 0)
        return src1;
    else if constexpr (_shift == numLaneVals)
        return src0;
    else if constexpr (numLanes<_registerType> == 1)
        return _mm_castIF<_registerType>(_mm_alignr_epi8(_mm_castFI(src0), _mm_castFI(src1), _shift * elementSize));
#ifdef __AVX2__
    else if constexpr (numLanes<_registerType> == 2)
        return _mm_castIF<_registerType>(_mm256_alignr_epi8(_mm_castFI(src0), _mm_castFI(src1), _shift * elementSize));
#endif //__AVX2
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1>
inline __m128d Blend(__m128d source0, __m128d source1)
{
    return _mm_blend<BlendMask<_src0, _src1>()>(source0, source1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, typename _registerType>
inline _registerType Blend(_registerType source0, _registerType source1)
{
    static_assert(Is__m128<_registerType> || Is__m256d<_registerType>,
                  "Function overload only compatble with __m128 and __m256d registers.");

    return _mm_blend<BlendMask<_src0, _src1, _src2, _src3>()>(source0, source1);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
inline __m256 Blend(__m256 source0, __m256 source1)
{
    return _mm_blend<BlendMask<_src0, _src1, _src2, _src3, _src4, _src5, _src6, _src7>()>(source0, source1);
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <U32 _index, typename _registerType>
inline _registerType BlendIndex(_registerType source0, _registerType source1)
{
    using namespace GDL::simd;

    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    static_assert(_index < numRegVals, "Index must be lower than the number of register values.");
    static_assert(numRegVals == 2 || numRegVals == 4 || numRegVals == 8,
                  "Only registers with 2, 4 or 8 values are supported.");

    if constexpr (numRegVals == 2)
    {
        constexpr U32 b0 = (_index == 0) ? 1 : 0;
        constexpr U32 b1 = (_index == 1) ? 1 : 0;

        return Blend<b0, b1>(source0, source1);
    }
    if constexpr (numRegVals == 4)
    {
        constexpr U32 b0 = (_index == 0) ? 1 : 0;
        constexpr U32 b1 = (_index == 1) ? 1 : 0;
        constexpr U32 b2 = (_index == 2) ? 1 : 0;
        constexpr U32 b3 = (_index == 3) ? 1 : 0;

        return Blend<b0, b1, b2, b3>(source0, source1);
    }
    if constexpr (numRegVals == 8)
    {
        constexpr U32 b0 = (_index == 0) ? 1 : 0;
        constexpr U32 b1 = (_index == 1) ? 1 : 0;
        constexpr U32 b2 = (_index == 2) ? 1 : 0;
        constexpr U32 b3 = (_index == 3) ? 1 : 0;
        constexpr U32 b4 = (_index == 4) ? 1 : 0;
        constexpr U32 b5 = (_index == 5) ? 1 : 0;
        constexpr U32 b6 = (_index == 6) ? 1 : 0;
        constexpr U32 b7 = (_index == 7) ? 1 : 0;

        return Blend<b0, b1, b2, b3, b4, b5, b6, b7>(source0, source1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _index, typename _registerType>
inline _registerType BlendAboveIndex(_registerType source0, [[maybe_unused]] _registerType source1)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    static_assert(_index < numRegVals, "Index must be lower than the number of register values.");
    static_assert(numRegVals == 2 || numRegVals == 4 || numRegVals == 8,
                  "Only registers with 2, 4 or 8 values are supported.");

    if constexpr (_index == 0)
        return source0;
    else if constexpr (numRegVals == 2)
    {
        constexpr U32 b0 = (_index > 0) ? 1 : 0;
        constexpr U32 b1 = (_index > 1) ? 1 : 0;

        return Blend<b0, b1>(source0, source1);
    }
    else if constexpr (numRegVals == 4)
    {
        constexpr U32 b0 = (_index > 0) ? 1 : 0;
        constexpr U32 b1 = (_index > 1) ? 1 : 0;
        constexpr U32 b2 = (_index > 2) ? 1 : 0;
        constexpr U32 b3 = (_index > 3) ? 1 : 0;

        return Blend<b0, b1, b2, b3>(source0, source1);
    }
    else if constexpr (numRegVals == 8)
    {
        constexpr U32 b0 = (_index > 0) ? 1 : 0;
        constexpr U32 b1 = (_index > 1) ? 1 : 0;
        constexpr U32 b2 = (_index > 2) ? 1 : 0;
        constexpr U32 b3 = (_index > 3) ? 1 : 0;
        constexpr U32 b4 = (_index > 4) ? 1 : 0;
        constexpr U32 b5 = (_index > 5) ? 1 : 0;
        constexpr U32 b6 = (_index > 6) ? 1 : 0;
        constexpr U32 b7 = (_index > 7) ? 1 : 0;

        return Blend<b0, b1, b2, b3, b4, b5, b6, b7>(source0, source1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idxFirst, U32 _idxLast, typename _registerType>
inline _registerType BlendInRange([[maybe_unused]] _registerType source0, _registerType source1)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    static_assert(_idxFirst < numRegVals && _idxLast < numRegVals,
                  "Indices must be lower than the number of register values.");
    static_assert(_idxFirst <= _idxLast, "First index must be lower or equal to the second index");
    static_assert(numRegVals == 2 || numRegVals == 4 || numRegVals == 8,
                  "Only registers with 2, 4 or 8 values are supported.");
    if constexpr (_idxFirst == 0 && _idxLast == numRegVals - 1)
        return source1;
    else if constexpr (numRegVals == 2)
    {
        constexpr U32 b0 = (_idxFirst <= 0 && _idxLast >= 0) ? 1 : 0;
        constexpr U32 b1 = (_idxFirst <= 1 && _idxLast >= 1) ? 1 : 0;

        return Blend<b0, b1>(source0, source1);
    }
    else if constexpr (numRegVals == 4)
    {
        constexpr U32 b0 = (_idxFirst <= 0 && _idxLast >= 0) ? 1 : 0;
        constexpr U32 b1 = (_idxFirst <= 1 && _idxLast >= 1) ? 1 : 0;
        constexpr U32 b2 = (_idxFirst <= 2 && _idxLast >= 2) ? 1 : 0;
        constexpr U32 b3 = (_idxFirst <= 3 && _idxLast >= 3) ? 1 : 0;

        return Blend<b0, b1, b2, b3>(source0, source1);
    }
    else if constexpr (numRegVals == 8)
    {
        constexpr U32 b0 = (_idxFirst <= 0 && _idxLast >= 0) ? 1 : 0;
        constexpr U32 b1 = (_idxFirst <= 1 && _idxLast >= 1) ? 1 : 0;
        constexpr U32 b2 = (_idxFirst <= 2 && _idxLast >= 2) ? 1 : 0;
        constexpr U32 b3 = (_idxFirst <= 3 && _idxLast >= 3) ? 1 : 0;
        constexpr U32 b4 = (_idxFirst <= 4 && _idxLast >= 4) ? 1 : 0;
        constexpr U32 b5 = (_idxFirst <= 5 && _idxLast >= 5) ? 1 : 0;
        constexpr U32 b6 = (_idxFirst <= 6 && _idxLast >= 6) ? 1 : 0;
        constexpr U32 b7 = (_idxFirst <= 7 && _idxLast >= 7) ? 1 : 0;

        return Blend<b0, b1, b2, b3, b4, b5, b6, b7>(source0, source1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _index, typename _registerType>
inline _registerType BlendBelowIndex(_registerType source0, [[maybe_unused]] _registerType source1)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    static_assert(_index < numRegVals, "Index must be lower than the number of register values.");
    static_assert(numRegVals == 2 || numRegVals == 4 || numRegVals == 8,
                  "Only registers with 2, 4 or 8 values are supported.");

    constexpr U32 b0 = 0;

    if constexpr (_index >= numRegVals)
        return source0;
    else if constexpr (numRegVals == 2)
    {
        constexpr U32 b1 = (_index < 1) ? 1 : 0;

        return Blend<b0, b1>(source0, source1);
    }
    else if constexpr (numRegVals == 4)
    {
        constexpr U32 b1 = (_index < 1) ? 1 : 0;
        constexpr U32 b2 = (_index < 2) ? 1 : 0;
        constexpr U32 b3 = (_index < 3) ? 1 : 0;

        return Blend<b0, b1, b2, b3>(source0, source1);
    }
    else if constexpr (numRegVals == 8)
    {
        constexpr U32 b1 = (_index < 1) ? 1 : 0;
        constexpr U32 b2 = (_index < 2) ? 1 : 0;
        constexpr U32 b3 = (_index < 3) ? 1 : 0;
        constexpr U32 b4 = (_index < 4) ? 1 : 0;
        constexpr U32 b5 = (_index < 5) ? 1 : 0;
        constexpr U32 b6 = (_index < 6) ? 1 : 0;
        constexpr U32 b7 = (_index < 7) ? 1 : 0;

        return Blend<b0, b1, b2, b3, b4, b5, b6, b7>(source0, source1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1>
constexpr U32 BlendMask()
{
    static_assert(_src0 < 2 && _src1 < 2, "Values _src0 ans _src1 must be in the interval [0, 1]");

    return (((_src1) << 1) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 BlendMask()
{
    static_assert(_src0 < 2 && _src1 < 2 && _src2 < 2 && _src3 < 2,
                  "Values _src0-_src3 must be in the interval [0, 1]");

    return (((_src3) << 3) | ((_src2) << 2) | ((_src1) << 1) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
constexpr U32 BlendMask()
{
    static_assert(_src0 < 2 && _src1 < 2 && _src2 < 2 && _src3 < 2 && _src4 < 2 && _src5 < 2 && _src6 < 2 && _src7 < 2,
                  "Values _src0-_src7 must be in the interval [0, 1]");

    return (((_src7) << 7) | ((_src6) << 6) | ((_src5) << 5) | ((_src4) << 4) | ((_src3) << 3) | ((_src2) << 2) |
            ((_src1) << 1) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _index, typename _registerType>
inline _registerType Broadcast(_registerType reg)
{
    static_assert(_index < numValuesPerLane<_registerType>, "_index must be in the interval [0, numValuesPerLane]");
    static_assert(IsRegisterType<_registerType>,
                  "Only __m128, __m128d, __m256 and __m256d registers are supported by this function.");

    // clang-format off
#ifdef __AVX2__
    if constexpr (_index == 0 && numLanes<_registerType> == 1)
        return _mm_broadcasts(reg);
    else
#endif // __AVX2__
    if constexpr (Is__m128d<_registerType>)
        return Permute<_index, _index>(reg);
    else
        return Permute<_index, _index, _index, _index>(reg);
    // clang-format on
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idx0, U32 _idx1, typename _registerType>
inline _registerType Broadcast(_registerType reg)
{
    static_assert(_idx0 < numValuesPerLane<_registerType> && _idx1 < numValuesPerLane<_registerType>,
                  "Indices must be in the interval [0, numValuesPerLane]");
    static_assert(numLanes<_registerType> == 2, "Function only supports registers with 2 lanes.");
    static_assert(Is__m256<_registerType> || Is__m256d<_registerType>,
                  "Only __m256 ans __m256d registers are supported.");

    if constexpr (Is__m256d<_registerType>)
        return Permute<_idx0, _idx0, _idx1, _idx1>(reg);
    else
        return Permute<_idx0, _idx0, _idx0, _idx0, _idx1, _idx1, _idx1, _idx1>(reg);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _index, typename _registerType>
inline _registerType BroadcastAcrossLanes(_registerType reg)
{
    static_assert(IsRegisterType<_registerType>,
                  "Only __m128, __m128d, __m256 and __m256d registers are supported by this function.");

#ifdef __AVX2__
    if constexpr (_index == 0)
        return _mm_broadcasts(reg);
    else if constexpr (numLanes<_registerType> == 1)
        return Broadcast<_index>(reg);
    else
    {

        constexpr U32 valueIndex = _index % numValuesPerLane<_registerType>;
        constexpr U32 laneIndex = _index / numValuesPerLane<_registerType>;
        return Permute2F128<laneIndex, laneIndex>(Broadcast<valueIndex>(reg));
    }

#else
    return Broadcast<_index>(reg);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idx0, U32 _idx1>
inline void Exchange(__m128& reg0, __m128& reg1)
{
    using RegisterType = std::remove_reference<decltype(reg0)>::type;
    constexpr U32 numRegVals = numRegisterValues<RegisterType>;
    static_assert(_idx0 < numRegVals && _idx1 < numRegVals, "Indices must be in the range [0, 3]");

    constexpr U32 b00 = (_idx0 == 0) ? 1 : 0;
    constexpr U32 b01 = (_idx0 == 1) ? 1 : 0;
    constexpr U32 b02 = (_idx0 == 2) ? 1 : 0;
    constexpr U32 b03 = (_idx0 == 3) ? 1 : 0;

    constexpr U32 b10 = (_idx1 == 0) ? 1 : 0;
    constexpr U32 b11 = (_idx1 == 1) ? 1 : 0;
    constexpr U32 b12 = (_idx1 == 2) ? 1 : 0;
    constexpr U32 b13 = (_idx1 == 3) ? 1 : 0;

    if constexpr (_idx0 == _idx1)
    {
        RegisterType tmp = reg0;
        reg0 = Blend<b00, b01, b02, b03>(reg0, reg1);
        reg1 = Blend<b10, b11, b12, b13>(reg1, tmp);
    }
    else
    {
        RegisterType tmp = Broadcast<_idx0>(reg0);
        reg0 = Blend<b00, b01, b02, b03>(reg0, Broadcast<_idx1>(reg1));
        reg1 = Blend<b10, b11, b12, b13>(reg1, tmp);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idx0, U32 _idx1>
inline void Exchange(__m128d& reg0, __m128d& reg1)
{
    using RegisterType = std::remove_reference<decltype(reg0)>::type;
    constexpr U32 numRegVals = numRegisterValues<RegisterType>;

    static_assert(_idx0 < numRegVals && _idx1 < numRegVals, "Indices must be in the range [0, 1]");

    constexpr U32 b00 = (_idx0 == 0) ? 1 : 0;
    constexpr U32 b01 = (_idx0 == 1) ? 1 : 0;

    constexpr U32 b10 = (_idx1 == 0) ? 1 : 0;
    constexpr U32 b11 = (_idx1 == 1) ? 1 : 0;

    if constexpr (_idx0 == _idx1)
    {
        RegisterType tmp = reg0;
        reg0 = Blend<b00, b01>(reg0, reg1);
        reg1 = Blend<b10, b11>(reg1, tmp);
    }
    else
    {
        RegisterType tmp = Broadcast<_idx0>(reg0);
        reg0 = Blend<b00, b01>(reg0, Broadcast<_idx1>(reg1));
        reg1 = Blend<b10, b11>(reg1, tmp);
    }
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <U32 _idx0, U32 _idx1>
inline void Exchange(__m256& reg0, __m256& reg1)
{
    using RegisterType = std::remove_reference<decltype(reg0)>::type;
    constexpr U32 numRegVals = numRegisterValues<RegisterType>;
    constexpr U32 numLaneVals = numValuesPerLane<RegisterType>;

    static_assert(_idx0 < numRegVals && _idx1 < numRegVals, "Indices must be in the range [0, 7]");


    constexpr U32 lane0 = _idx0 / numLaneVals;
    constexpr U32 lane1 = _idx1 / numLaneVals;

    constexpr U32 b00 = (_idx0 == 0) ? 1 : 0;
    constexpr U32 b01 = (_idx0 == 1) ? 1 : 0;
    constexpr U32 b02 = (_idx0 == 2) ? 1 : 0;
    constexpr U32 b03 = (_idx0 == 3) ? 1 : 0;
    constexpr U32 b04 = (_idx0 == 4) ? 1 : 0;
    constexpr U32 b05 = (_idx0 == 5) ? 1 : 0;
    constexpr U32 b06 = (_idx0 == 6) ? 1 : 0;
    constexpr U32 b07 = (_idx0 == 7) ? 1 : 0;

    constexpr U32 b10 = (_idx1 == 0) ? 1 : 0;
    constexpr U32 b11 = (_idx1 == 1) ? 1 : 0;
    constexpr U32 b12 = (_idx1 == 2) ? 1 : 0;
    constexpr U32 b13 = (_idx1 == 3) ? 1 : 0;
    constexpr U32 b14 = (_idx1 == 4) ? 1 : 0;
    constexpr U32 b15 = (_idx1 == 5) ? 1 : 0;
    constexpr U32 b16 = (_idx1 == 6) ? 1 : 0;
    constexpr U32 b17 = (_idx1 == 7) ? 1 : 0;

    if constexpr (_idx0 == _idx1)
    {
        RegisterType tmp = reg0;
        reg0 = Blend<b00, b01, b02, b03, b04, b05, b06, b07>(reg0, reg1);
        reg1 = Blend<b10, b11, b12, b13, b14, b15, b16, b17>(reg1, tmp);
    }
    else if constexpr (lane0 == lane1)
    {
        RegisterType tmp = Broadcast<_idx0 % numLaneVals>(reg0);
        reg0 = Blend<b00, b01, b02, b03, b04, b05, b06, b07>(reg0, Broadcast<_idx1 % numLaneVals>(reg1));
        reg1 = Blend<b10, b11, b12, b13, b14, b15, b16, b17>(reg1, tmp);
    }
    else
    {
        constexpr U32 regIdx0 = (lane0 == 0) ? 1 : 0;
        constexpr U32 regIdx1 = (lane1 == 0) ? 1 : 0;


        constexpr U32 cIdx0 = (lane0 == 0) ? _idx1 % numLaneVals : _idx0 % numLaneVals;
        constexpr U32 cIdx1 = (lane0 == 0) ? _idx0 % numLaneVals : _idx1 % numLaneVals;

        if constexpr (cIdx0 == cIdx1)
        {
            RegisterType tmp = Permute2F128<regIdx0, 1, regIdx1, 0>(reg0, reg1);
            reg0 = Blend<b00, b01, b02, b03, b04, b05, b06, b07>(reg0, tmp);
            reg1 = Blend<b10, b11, b12, b13, b14, b15, b16, b17>(reg1, tmp);
        }
        else
        {
            RegisterType tmp = Broadcast<cIdx0, cIdx1>(Permute2F128<regIdx0, 1, regIdx1, 0>(reg0, reg1));
            reg0 = Blend<b00, b01, b02, b03, b04, b05, b06, b07>(reg0, tmp);
            reg1 = Blend<b10, b11, b12, b13, b14, b15, b16, b17>(reg1, tmp);
        }
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idx0, U32 _idx1>
inline void Exchange(__m256d& reg0, __m256d& reg1)
{
    using RegisterType = std::remove_reference<decltype(reg0)>::type;
    constexpr U32 numRegVals = numRegisterValues<RegisterType>;
    constexpr U32 numLaneVals = numValuesPerLane<RegisterType>;

    static_assert(_idx0 < numRegVals && _idx1 < numRegVals, "Indices must be in the range [0, 7]");


    constexpr U32 lane0 = _idx0 / numLaneVals;
    constexpr U32 lane1 = _idx1 / numLaneVals;

    constexpr U32 b00 = (_idx0 == 0) ? 1 : 0;
    constexpr U32 b01 = (_idx0 == 1) ? 1 : 0;
    constexpr U32 b02 = (_idx0 == 2) ? 1 : 0;
    constexpr U32 b03 = (_idx0 == 3) ? 1 : 0;

    constexpr U32 b10 = (_idx1 == 0) ? 1 : 0;
    constexpr U32 b11 = (_idx1 == 1) ? 1 : 0;
    constexpr U32 b12 = (_idx1 == 2) ? 1 : 0;
    constexpr U32 b13 = (_idx1 == 3) ? 1 : 0;

    if constexpr (_idx0 == _idx1)
    {
        RegisterType tmp = reg0;
        reg0 = Blend<b00, b01, b02, b03>(reg0, reg1);
        reg1 = Blend<b10, b11, b12, b13>(reg1, tmp);
    }
    else if constexpr (lane0 == lane1)
    {
        RegisterType tmp = Broadcast<_idx0 % numLaneVals>(reg0);
        reg0 = Blend<b00, b01, b02, b03>(reg0, Broadcast<_idx1 % numLaneVals>(reg1));
        reg1 = Blend<b10, b11, b12, b13>(reg1, tmp);
    }
    else
    {
        constexpr U32 regIdx0 = (lane0 == 0) ? 1 : 0;
        constexpr U32 regIdx1 = (lane1 == 0) ? 1 : 0;


        constexpr U32 cIdx0 = (lane0 == 0) ? _idx1 % numLaneVals : _idx0 % numLaneVals;
        constexpr U32 cIdx1 = (lane0 == 0) ? _idx0 % numLaneVals : _idx1 % numLaneVals;

        if constexpr (cIdx0 == cIdx1)
        {
            RegisterType tmp = Permute2F128<regIdx0, 1, regIdx1, 0>(reg0, reg1);
            reg0 = Blend<b00, b01, b02, b03>(reg0, tmp);
            reg1 = Blend<b10, b11, b12, b13>(reg1, tmp);
        }
        else
        {
            RegisterType tmp = Broadcast<cIdx0, cIdx1>(Permute2F128<regIdx0, 1, regIdx1, 0>(reg0, reg1));
            reg0 = Blend<b00, b01, b02, b03>(reg0, tmp);
            reg1 = Blend<b10, b11, b12, b13>(reg1, tmp);
        }
    }
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idxSrc, U32 _idxDst, bool _setZeroIdx0, bool _setZeroIdx1, bool _setZeroIdx2, bool _setZeroIdx3>
inline __m128 Insert(__m128 src, __m128 dst)
{
    constexpr U32 mask = InsertMask<_idxSrc, _idxDst, _setZeroIdx0, _setZeroIdx1, _setZeroIdx2, _setZeroIdx3>();
    return _mm_insert_ps(dst, src, mask);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idxSrc, U32 _idxDst, bool _setZeroIdx0, bool _setZeroIdx1, bool _setZeroIdx2, bool _setZeroIdx3>
inline constexpr U32 InsertMask()
{
    constexpr U32 sz0 = (_setZeroIdx0) ? 1 : 0;
    constexpr U32 sz1 = (_setZeroIdx1) ? 1 : 0;
    constexpr U32 sz2 = (_setZeroIdx2) ? 1 : 0;
    constexpr U32 sz3 = (_setZeroIdx3) ? 1 : 0;

    return (((_idxSrc) << 6) | ((_idxDst) << 4) | ((sz3) << 3) | ((sz2) << 2) | ((sz1) << 1) | (sz0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1>
inline __m128d Permute(__m128d source)
{
    return _mm_permute<PermuteMask<_src0, _src1>()>(source);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m128 Permute(__m128 source)
{
    return _mm_permute<PermuteMask<_src0, _src1, _src2, _src3>()>(source);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <U32 _src0, U32 _src1>
inline __m256d Permute(__m256d source)
{
    return Permute<_src0, _src1, _src0, _src1>(source);
}



// --------------------------------------------------------------------------------------------------------------------



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256 Permute(__m256 source)
{
    return _mm_permute<PermuteMask<_src0, _src1, _src2, _src3>()>(source);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256d Permute(__m256d source)
{
    return _mm_permute<Permute256dMask<_src0, _src1, _src2, _src3>()>(source);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
inline __m256 Permute(__m256 source)
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4 && _src4 < 4 && _src5 < 4 && _src6 < 4 && _src7 < 4,
                  "Values _src0-_src7 must be in the interval [0, 3]");

    return _mm256_permutevar_ps(source, _mm256_setr_epi32(_src0, _src1, _src2, _src3, _src4, _src5, _src6, _src7));
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1>
constexpr U32 PermuteMask()
{
    static_assert(_src0 < 2 && _src1 < 2, "Values _src0 ans _src1 must be in the interval [0, 1]");

    return (((_src1) << 1) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 PermuteMask()
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4,
                  "Values _src0-_src3 must be in the interval [0, 3]");

    return (((_src3) << 6) | ((_src2) << 4) | ((_src1) << 2) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 Permute256dMask()
{
    static_assert(_src0 < 2 && _src1 < 2 && _src2 < 2 && _src3 < 2,
                  "Values _src0-_src3 must be in the interval [0, 1]");

    return (((_src3) << 3) | ((_src2) << 2) | ((_src1) << 1) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <U32 _lane0SrcLane, U32 _lane1SrcLane, typename _registerType>
inline _registerType Permute2F128(_registerType source)
{
    if constexpr (_lane0SrcLane == 0 && _lane1SrcLane == 1)
        return source;
    else
        return Permute2F128<0, _lane0SrcLane, 0, _lane1SrcLane>(source, source);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _lane0SrcReg, U32 _lane0SrcLane, U32 _lane1SrcReg, U32 _lane1SrcLane, typename _registerType>
inline _registerType Permute2F128(_registerType source0, _registerType source1)
{
    static_assert(Is__m256<_registerType> || Is__m256d<_registerType>,
                  "Function only compatible with __m256 and __m256d registers.");

    return _mm_permute2f128<Permute2F128Mask<_lane0SrcReg, _lane0SrcLane, _lane1SrcReg, _lane1SrcLane>()>(source0,
                                                                                                          source1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _lane0SrcReg, U32 _lane0SrcLane, U32 _lane1SrcReg, U32 _lane1SrcLane>
constexpr U32 Permute2F128Mask()
{
    static_assert(_lane0SrcReg < 2 && _lane0SrcLane < 2 && _lane1SrcReg < 2 && _lane1SrcLane < 2,
                  "Template values must be in the interval [0, 1]");

    return (((_lane1SrcReg) << 5) | ((_lane1SrcLane) << 4) | ((_lane0SrcReg) << 1) | (_lane0SrcLane));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256d Permute4F64(__m256d source)
{
    return _mm_permute4x64<Permute4F64Mask<_src0, _src1, _src2, _src3>()>(source);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr inline U32 Permute4F64Mask()
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4,
                  "Values _src0-_src3 must be in the interval [0, 3]");

    return (((_src3) << 6) | ((_src2) << 4) | ((_src1) << 2) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
inline __m256 Permute8F32(__m256 source)
{
    static_assert(_src0 < 8 && _src1 < 8 && _src2 < 8 && _src3 < 8 && _src4 < 8 && _src5 < 8 && _src6 < 8 && _src7 < 8,
                  "Values _src0-_src7 must be in the interval [0, 7]");

    const __m256i mask = _mm256_setr_epi32(_src0, _src1, _src2, _src3, _src4, _src5, _src6, _src7);

    return _mm256_permutevar8x32_ps(source, mask);
}



#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, typename _registerType>
inline _registerType Shuffle(_registerType source0, _registerType source1)
{
    static_assert(Is__m128d<_registerType> || Is__m256d<_registerType>,
                  "Function only compatible with __m128d and __m256d registers.");

    if constexpr (Is__m128d<_registerType>)
        return _mm_shuffle<ShuffleMask<_src0, _src1>()>(source0, source1);
#ifdef __AVX2__
    else
        return _mm_shuffle<ShuffleMask256d<_src0, _src1, _src0, _src1>()>(source0, source1);
#endif // __AVX2__
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256d Shuffle(__m256d source0, __m256d source1)
{
    return _mm_shuffle<ShuffleMask256d<_src0, _src1, _src2, _src3>()>(source0, source1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, typename _registerType>
inline _registerType Shuffle(_registerType source0, _registerType source1)
{
    static_assert(Is__m128<_registerType> || Is__m256<_registerType>,
                  "Function only compatible with __m128 and __m256 float registers.");

    return _mm_shuffle<ShuffleMask<_src0, _src1, _src2, _src3>()>(source0, source1);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1>
constexpr U32 ShuffleMask()
{
    static_assert(_src0 < 2 && _src1 < 2, "Values _src0 and _src1 must be in the interval [0, 1]");

    return (((_src1) << 1) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 ShuffleMask()
{
    static_assert(_src0 < 4 && _src1 < 4 && _src2 < 4 && _src3 < 4,
                  "Values _src0 to _src3 must be in the interval [0, 3]");

    return (((_src3) << 6) | ((_src2) << 4) | ((_src1) << 2) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 ShuffleMask256d()
{
    static_assert(_src0 < 2 && _src1 < 2 && _src2 < 2 && _src3 < 2,
                  "Values _src0 to _src3 must be in the interval [0, 1]");
    return (((_src3) << 3) | ((_src2) << 2) | ((_src1) << 1) | (_src0));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idx0, U32 _idx1>
inline __m128 Swap(__m128 source)
{
    constexpr U32 numRegVals = numRegisterValues<decltype(source)>;

    static_assert(_idx0 < numRegVals && _idx1 < numRegVals, "Indices must be in the range [0, 3]");

    if constexpr (_idx0 == _idx1)
        return source;

    constexpr U32 s0 = (_idx0 == 0) ? _idx1 : (_idx1 == 0) ? _idx0 : 0;
    constexpr U32 s1 = (_idx0 == 1) ? _idx1 : (_idx1 == 1) ? _idx0 : 1;
    constexpr U32 s2 = (_idx0 == 2) ? _idx1 : (_idx1 == 2) ? _idx0 : 2;
    constexpr U32 s3 = (_idx0 == 3) ? _idx1 : (_idx1 == 3) ? _idx0 : 3;

    return Permute<s0, s1, s2, s3>(source);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idx0, U32 _idx1>
inline __m128d Swap(__m128d source)
{
    constexpr U32 numRegVals = numRegisterValues<decltype(source)>;

    static_assert(_idx0 != _idx1, "Indices must differ");
    static_assert(_idx0 < numRegVals && _idx1 < numRegVals, "Indices must be in the range [0, 1]");

    return Permute<1, 0>(source);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <U32 _idx0, U32 _idx1>
inline __m256 Swap(__m256 source)
{
    using RegisterType = decltype(source);
    constexpr U32 numRegVals = numRegisterValues<RegisterType>;
    constexpr U32 numLaneValues = numValuesPerLane<RegisterType>;

    static_assert(_idx0 != _idx1, "Indices must differ");
    static_assert(_idx0 < numRegVals && _idx1 < numRegVals, "Indices must be in the range [0, 7]");

    constexpr U32 lane0 = _idx0 / numLaneValues;
    constexpr U32 lane1 = _idx1 / numLaneValues;

    if constexpr (lane0 == lane1)
    {
        constexpr U32 cIdx0 = _idx0 % numLaneValues;
        constexpr U32 cIdx1 = _idx1 % numLaneValues;

        constexpr U32 s0 = (_idx0 == 0) ? _idx1 : (_idx1 == 0) ? _idx0 : 0;
        constexpr U32 s1 = (_idx0 == 1) ? _idx1 : (_idx1 == 1) ? _idx0 : 1;
        constexpr U32 s2 = (_idx0 == 2) ? _idx1 : (_idx1 == 2) ? _idx0 : 2;
        constexpr U32 s3 = (_idx0 == 3) ? _idx1 : (_idx1 == 3) ? _idx0 : 3;
        constexpr U32 s4 = (_idx0 == 4) ? cIdx1 : (_idx1 == 4) ? cIdx0 : 0;
        constexpr U32 s5 = (_idx0 == 5) ? cIdx1 : (_idx1 == 5) ? cIdx0 : 1;
        constexpr U32 s6 = (_idx0 == 6) ? cIdx1 : (_idx1 == 6) ? cIdx0 : 2;
        constexpr U32 s7 = (_idx0 == 7) ? cIdx1 : (_idx1 == 7) ? cIdx0 : 3;

        return Permute<s0, s1, s2, s3, s4, s5, s6, s7>(source);
    }
    else
    {
        constexpr U32 idxL0 = (_idx0 < _idx1) ? _idx0 : _idx1;
        constexpr U32 idxL1 = (_idx0 > _idx1) ? _idx0 : _idx1;
        constexpr U32 cIdxL1 = idxL1 % numLaneValues;

        constexpr U32 b0 = (idxL0 == 0) ? 1 : 0;
        constexpr U32 b1 = (idxL0 == 1) ? 1 : 0;
        constexpr U32 b2 = (idxL0 == 2) ? 1 : 0;
        constexpr U32 b3 = (idxL0 == 3) ? 1 : 0;
        constexpr U32 b4 = (idxL1 == 4) ? 1 : 0;
        constexpr U32 b5 = (idxL1 == 5) ? 1 : 0;
        constexpr U32 b6 = (idxL1 == 6) ? 1 : 0;
        constexpr U32 b7 = (idxL1 == 7) ? 1 : 0;

        RegisterType bc = Broadcast<idxL0, cIdxL1>(source);
        RegisterType tmp = Permute2F128<1, 0>(bc);
        return Blend<b0, b1, b2, b3, b4, b5, b6, b7>(source, tmp);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idx0, U32 _idx1>
inline __m256d Swap(__m256d source)
{
    using RegisterType = decltype(source);
    constexpr U32 numRegVals = numRegisterValues<RegisterType>;
    constexpr U32 numLaneValues = numValuesPerLane<RegisterType>;

    static_assert(_idx0 != _idx1, "Indices must differ");
    static_assert(_idx0 < numRegVals && _idx1 < numRegVals, "Indices must be in the range [0, 3]");

    constexpr U32 lane0 = _idx0 / numLaneValues;
    constexpr U32 lane1 = _idx1 / numLaneValues;

    if constexpr (lane0 == lane1)
    {
        constexpr U32 cIdx0 = _idx0 % numLaneValues;
        constexpr U32 cIdx1 = _idx1 % numLaneValues;

        constexpr U32 s0 = (_idx0 == 0) ? _idx1 : (_idx1 == 0) ? _idx0 : 0;
        constexpr U32 s1 = (_idx0 == 1) ? _idx1 : (_idx1 == 1) ? _idx0 : 1;
        constexpr U32 s2 = (_idx0 == 2) ? cIdx1 : (_idx1 == 2) ? cIdx0 : 0;
        constexpr U32 s3 = (_idx0 == 3) ? cIdx1 : (_idx1 == 3) ? cIdx0 : 1;

        return Permute<s0, s1, s2, s3>(source);
    }
    else
    {
        constexpr U32 idxL0 = (_idx0 < _idx1) ? _idx0 : _idx1;
        constexpr U32 idxL1 = (_idx0 > _idx1) ? _idx0 : _idx1;
        constexpr U32 cIdxL1 = idxL1 % numLaneValues;

        constexpr U32 b0 = (idxL0 == 0) ? 1 : 0;
        constexpr U32 b1 = (idxL0 == 1) ? 1 : 0;
        constexpr U32 b2 = (idxL1 == 2) ? 1 : 0;
        constexpr U32 b3 = (idxL1 == 3) ? 1 : 0;

        RegisterType bc = Broadcast<idxL0, cIdxL1>(source);
        RegisterType tmp = Permute2F128<1, 0>(bc);
        return Blend<b0, b1, b2, b3>(source, tmp);
    }
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
[[nodiscard]] inline _registerType SwapLanes(_registerType in) noexcept
{
    static_assert(numLanes<_registerType> == 2, "Only registers with 2 lanes supported.");
    return Permute2F128<1, 0>(in);
}



// --------------------------------------------------------------------------------------------------------------------

template <bool _swapLanes, typename _registerType>
[[nodiscard]] inline _registerType SwapLanesIf(_registerType in) noexcept
{
    static_assert(numLanes<_registerType> == 2, "Only registers with 2 lanes supported.");
    if constexpr (_swapLanes)
        return SwapLanes(in);
    else
        return in;
}


} // namespace GDL::simd
