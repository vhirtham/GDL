#pragma once

#include "gdl/base/sse/intrinsics.h"

namespace GDL::sse
{

//! @brief Calculates the absolute value of each element
//! @tparam _registerType: Register type
//! @param reg: Input register
//! @return Register with absolute values
//! @remark source:
//! https://stackoverflow.com/questions/23847377/how-does-this-function-compute-the-absolute-value-of-a-float-through-a-not-and-a
template <typename _registerType>
_registerType Abs(_registerType reg)
{
    const _registerType mask = _mmx_set1_p<_registerType>(-0.0);
    return _mmx_andnot_p(mask, reg);
}

} // namespace GDL::sse
