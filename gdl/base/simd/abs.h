#pragma once

#include "gdl/base/simd/intrinsics.h"

namespace GDL::simd
{

//! @brief Calculates the absolute value of each element
//! @tparam _registerType: Register type
//! @param reg: Input register
//! @return Register with absolute values
//! @remark source:
//! https://stackoverflow.com/questions/23847377/how-does-this-function-compute-the-absolute-value-of-a-float-through-a-not-and-a
template <typename _registerType>
inline _registerType Abs(_registerType reg)
{
    const _registerType mask = _mm_set1<_registerType>(-0.0);
    return _mm_andnot(mask, reg);
}

} // namespace GDL::simd
