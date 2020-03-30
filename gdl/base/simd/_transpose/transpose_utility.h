#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/constants.h"
#include "gdl/base/simd/x86intrin.h"


namespace GDL::simd
{

template <typename _registerType, U32 _firstRowIn, U32 _firstRowOut>
struct TranspositionLaneData
{
    static constexpr U32 In = _firstRowIn / numValuesPerLane<_registerType>;
    static constexpr U32 Out = _firstRowOut / numValuesPerLane<_registerType>;
    static constexpr U32 OffsetIn = _firstRowIn % numValuesPerLane<_registerType>;
    static constexpr U32 OffsetOut = _firstRowOut % numValuesPerLane<_registerType>;
};

} // namespace GDL::simd

#include "gdl/base/simd/_transpose/transpose_utility.inl"
