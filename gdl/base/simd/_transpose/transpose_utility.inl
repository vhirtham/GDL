#pragma once

#include "gdl/base/simd/_transpose/transpose_utility.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"

#include <array>

namespace GDL::simd
{


// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline _registerType SwapLanes(_registerType in)
{
    static_assert(numLanes<_registerType> == 2, "Only registers with 2 lanes supported.");
    return Permute2F128<1, 0>(in);
}



// --------------------------------------------------------------------------------------------------------------------

template <bool _condition, typename _registerType>
inline _registerType SwapLanesIf(_registerType in)
{
    static_assert(numLanes<_registerType> == 2, "Only registers with 2 lanes supported.");
    if constexpr (_condition)
        return SwapLanes(in);
    else
        return in;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void TransposeSetOutput(_registerType& out0, _registerType in0)
{
    [[maybe_unused]] constexpr U32 lastRowOut = _firstRowOut + _numValues - 1;

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_firstRowOut, lastRowOut>(_mm_setzero<_registerType>(), in0);
        else
            out0 = in0;
    }
    else
        out0 = BlendInRange<_firstRowOut, lastRowOut>(out0, in0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void TransposeSetOutput(_registerType& out0, _registerType& out1, _registerType in0, _registerType in1)
{
    [[maybe_unused]] constexpr U32 lastRowOut = _firstRowOut + _numValues - 1;

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            _registerType zero = _mm_setzero<_registerType>();
            out0 = BlendInRange<_firstRowOut, lastRowOut>(zero, in0);
            out1 = BlendInRange<_firstRowOut, lastRowOut>(zero, in1);
        }
        else
        {
            out0 = in0;
            out1 = in1;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, lastRowOut>(out0, in0);
        out1 = BlendInRange<_firstRowOut, lastRowOut>(out1, in1);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void TransposeSetOutput(_registerType& out0, _registerType& out1, _registerType& out2, _registerType in0,
                               _registerType in1, _registerType in2)
{
    [[maybe_unused]] constexpr U32 lastRowOut = _firstRowOut + _numValues - 1;

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            _registerType zero = _mm_setzero<_registerType>();
            out0 = BlendInRange<_firstRowOut, lastRowOut>(zero, in0);
            out1 = BlendInRange<_firstRowOut, lastRowOut>(zero, in1);
            out2 = BlendInRange<_firstRowOut, lastRowOut>(zero, in2);
        }
        else
        {
            out0 = in0;
            out1 = in1;
            out2 = in2;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, lastRowOut>(out0, in0);
        out1 = BlendInRange<_firstRowOut, lastRowOut>(out1, in1);
        out2 = BlendInRange<_firstRowOut, lastRowOut>(out2, in2);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void TransposeSetOutput(_registerType& out0, _registerType& out1, _registerType& out2, _registerType& out3,
                               _registerType in0, _registerType in1, _registerType in2, _registerType in3)
{
    [[maybe_unused]] constexpr U32 lastRowOut = _firstRowOut + _numValues - 1;

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            _registerType zero = _mm_setzero<_registerType>();
            out0 = BlendInRange<_firstRowOut, lastRowOut>(zero, in0);
            out1 = BlendInRange<_firstRowOut, lastRowOut>(zero, in1);
            out2 = BlendInRange<_firstRowOut, lastRowOut>(zero, in2);
            out3 = BlendInRange<_firstRowOut, lastRowOut>(zero, in3);
        }
        else
        {
            out0 = in0;
            out1 = in1;
            out2 = in2;
            out3 = in3;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, lastRowOut>(out0, in0);
        out1 = BlendInRange<_firstRowOut, lastRowOut>(out1, in1);
        out2 = BlendInRange<_firstRowOut, lastRowOut>(out2, in2);
        out3 = BlendInRange<_firstRowOut, lastRowOut>(out3, in3);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void TransposeSetOutput(_registerType& out0, _registerType& out1, _registerType& out2, _registerType& out3,
                               _registerType& out4, _registerType in0, _registerType in1, _registerType in2,
                               _registerType in3, _registerType in4)
{
    [[maybe_unused]] constexpr U32 lastRowOut = _firstRowOut + _numValues - 1;

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            _registerType zero = _mm_setzero<_registerType>();
            out0 = BlendInRange<_firstRowOut, lastRowOut>(zero, in0);
            out1 = BlendInRange<_firstRowOut, lastRowOut>(zero, in1);
            out2 = BlendInRange<_firstRowOut, lastRowOut>(zero, in2);
            out3 = BlendInRange<_firstRowOut, lastRowOut>(zero, in3);
            out4 = BlendInRange<_firstRowOut, lastRowOut>(zero, in4);
        }
        else
        {
            out0 = in0;
            out1 = in1;
            out2 = in2;
            out3 = in3;
            out4 = in4;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, lastRowOut>(out0, in0);
        out1 = BlendInRange<_firstRowOut, lastRowOut>(out1, in1);
        out2 = BlendInRange<_firstRowOut, lastRowOut>(out2, in2);
        out3 = BlendInRange<_firstRowOut, lastRowOut>(out3, in3);
        out4 = BlendInRange<_firstRowOut, lastRowOut>(out4, in4);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void TransposeSetOutput(_registerType& out0, _registerType& out1, _registerType& out2, _registerType& out3,
                               _registerType& out4, _registerType& out5, _registerType in0, _registerType in1,
                               _registerType in2, _registerType in3, _registerType in4, _registerType in5)
{
    [[maybe_unused]] constexpr U32 lastRowOut = _firstRowOut + _numValues - 1;

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            _registerType zero = _mm_setzero<_registerType>();
            out0 = BlendInRange<_firstRowOut, lastRowOut>(zero, in0);
            out1 = BlendInRange<_firstRowOut, lastRowOut>(zero, in1);
            out2 = BlendInRange<_firstRowOut, lastRowOut>(zero, in2);
            out3 = BlendInRange<_firstRowOut, lastRowOut>(zero, in3);
            out4 = BlendInRange<_firstRowOut, lastRowOut>(zero, in4);
            out5 = BlendInRange<_firstRowOut, lastRowOut>(zero, in5);
        }
        else
        {
            out0 = in0;
            out1 = in1;
            out2 = in2;
            out3 = in3;
            out4 = in4;
            out5 = in5;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, lastRowOut>(out0, in0);
        out1 = BlendInRange<_firstRowOut, lastRowOut>(out1, in1);
        out2 = BlendInRange<_firstRowOut, lastRowOut>(out2, in2);
        out3 = BlendInRange<_firstRowOut, lastRowOut>(out3, in3);
        out4 = BlendInRange<_firstRowOut, lastRowOut>(out4, in4);
        out5 = BlendInRange<_firstRowOut, lastRowOut>(out5, in5);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void TransposeSetOutput(_registerType& out0, _registerType& out1, _registerType& out2, _registerType& out3,
                               _registerType& out4, _registerType& out5, _registerType& out6, _registerType in0,
                               _registerType in1, _registerType in2, _registerType in3, _registerType in4,
                               _registerType in5, _registerType in6)
{
    [[maybe_unused]] constexpr U32 lastRowOut = _firstRowOut + _numValues - 1;

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            _registerType zero = _mm_setzero<_registerType>();
            out0 = BlendInRange<_firstRowOut, lastRowOut>(zero, in0);
            out1 = BlendInRange<_firstRowOut, lastRowOut>(zero, in1);
            out2 = BlendInRange<_firstRowOut, lastRowOut>(zero, in2);
            out3 = BlendInRange<_firstRowOut, lastRowOut>(zero, in3);
            out4 = BlendInRange<_firstRowOut, lastRowOut>(zero, in4);
            out5 = BlendInRange<_firstRowOut, lastRowOut>(zero, in5);
            out6 = BlendInRange<_firstRowOut, lastRowOut>(zero, in6);
        }
        else
        {
            out0 = in0;
            out1 = in1;
            out2 = in2;
            out3 = in3;
            out4 = in4;
            out5 = in5;
            out6 = in6;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, lastRowOut>(out0, in0);
        out1 = BlendInRange<_firstRowOut, lastRowOut>(out1, in1);
        out2 = BlendInRange<_firstRowOut, lastRowOut>(out2, in2);
        out3 = BlendInRange<_firstRowOut, lastRowOut>(out3, in3);
        out4 = BlendInRange<_firstRowOut, lastRowOut>(out4, in4);
        out5 = BlendInRange<_firstRowOut, lastRowOut>(out5, in5);
        out6 = BlendInRange<_firstRowOut, lastRowOut>(out6, in6);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _firstRowOut, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void TransposeSetOutput(_registerType& out0, _registerType& out1, _registerType& out2, _registerType& out3,
                               _registerType& out4, _registerType& out5, _registerType& out6, _registerType& out7,
                               _registerType in0, _registerType in1, _registerType in2, _registerType in3,
                               _registerType in4, _registerType in5, _registerType in6, _registerType in7)
{
    [[maybe_unused]] constexpr U32 lastRowOut = _firstRowOut + _numValues - 1;

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
        {
            _registerType zero = _mm_setzero<_registerType>();
            out0 = BlendInRange<_firstRowOut, lastRowOut>(zero, in0);
            out1 = BlendInRange<_firstRowOut, lastRowOut>(zero, in1);
            out2 = BlendInRange<_firstRowOut, lastRowOut>(zero, in2);
            out3 = BlendInRange<_firstRowOut, lastRowOut>(zero, in3);
            out4 = BlendInRange<_firstRowOut, lastRowOut>(zero, in4);
            out5 = BlendInRange<_firstRowOut, lastRowOut>(zero, in5);
            out6 = BlendInRange<_firstRowOut, lastRowOut>(zero, in6);
            out7 = BlendInRange<_firstRowOut, lastRowOut>(zero, in7);
        }
        else
        {
            out0 = in0;
            out1 = in1;
            out2 = in2;
            out3 = in3;
            out4 = in4;
            out5 = in5;
            out6 = in6;
            out7 = in7;
        }
    }
    else
    {
        out0 = BlendInRange<_firstRowOut, lastRowOut>(out0, in0);
        out1 = BlendInRange<_firstRowOut, lastRowOut>(out1, in1);
        out2 = BlendInRange<_firstRowOut, lastRowOut>(out2, in2);
        out3 = BlendInRange<_firstRowOut, lastRowOut>(out3, in3);
        out4 = BlendInRange<_firstRowOut, lastRowOut>(out4, in4);
        out5 = BlendInRange<_firstRowOut, lastRowOut>(out5, in5);
        out6 = BlendInRange<_firstRowOut, lastRowOut>(out6, in6);
        out7 = BlendInRange<_firstRowOut, lastRowOut>(out7, in7);
    }
}



} // namespace GDL::simd
