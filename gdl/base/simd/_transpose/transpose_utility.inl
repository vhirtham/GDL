#pragma once

#include "gdl/base/simd/_transpose/transpose_utility.h"

#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/intrinsics.h"



namespace GDL::simd::intern::transpose
{



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idxFirst, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, typename _registerType>
inline void SetOutput(_registerType in0, _registerType& out0) noexcept
{
    constexpr U32 lastRowOut = _idxFirst + _numValues - 1;
    static_assert(lastRowOut < simd::numRegisterValues<_registerType>,
                  "Index of last modified element exceeds register size.");

    if constexpr (_overwriteUnused)
    {
        if constexpr (_unusedSetZero)
            out0 = BlendInRange<_idxFirst, lastRowOut>(_mm_setzero<_registerType>(), in0);
        else
            out0 = in0;
    }
    else
        out0 = BlendInRange<_idxFirst, lastRowOut>(out0, in0);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idxFirst, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, bool _initialCall,
          typename _registerType, UST _arraySize, typename... _args>
inline void SetOutput(std::array<_registerType, _arraySize> in, _registerType& out, _args&... args) noexcept
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
        SetOutput<_idxFirst, _numValues, _overwriteUnused, _unusedSetZero, false>(in, args...);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused, bool _unusedSetZero,
          typename _registerType>
inline void StaticAssertions() noexcept
{
    static_assert(not(_overwriteUnused == false && _unusedSetZero == true), "Option _unusedSetZero has no effect.");

    constexpr U32 numRegVals = numRegisterValues<_registerType>;
    static_assert(_firstRowIn + _rows <= numRegVals, "Input submatrix exceeds register size.");
    static_assert(_firstRowOut + _cols <= numRegVals, "Output submatrix exceeds register size.");
}



} // namespace GDL::simd::intern::transpose
