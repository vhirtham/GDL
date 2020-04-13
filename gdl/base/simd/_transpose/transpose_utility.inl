#pragma once

#include "gdl/base/simd/_transpose/transpose_utility.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"



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



namespace intern
{



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idxFirst, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, bool _initialCall,
          typename _registerType, UST _arraySize, typename... _args>
inline void TransposeSetOutput(std::array<_registerType, _arraySize> in, _registerType& out, _args&... args)
{
    constexpr UST packSize = sizeof...(_args);

    static_assert(not(_initialCall && _arraySize - 1 != packSize),
                  "Number of output registers must be equal to the array size.");

    constexpr UST idx = _arraySize - 1 - packSize;
    constexpr U32 lastRowOut = _idxFirst + _numValues - 1;

    static_assert(lastRowOut < simd::numRegisterValues<_registerType>,
                  "Index of last modified element exceeds register size.");

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
            out = BlendInRange<_idxFirst, lastRowOut>(_mm_setzero<_registerType>(), in[idx]);
        else
            out = in[idx];
    }
    else
        out = BlendInRange<_idxFirst, lastRowOut>(out, in[idx]);

    if constexpr (packSize > 0)
        TransposeSetOutput<_idxFirst, _numValues, _overwriteUnused, _unusedSetZero, false>(in, args...);
}

} // namespace intern

} // namespace GDL::simd
