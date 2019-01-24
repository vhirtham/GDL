#pragma once

#include <x86intrin.h>
#include "gdl/base/fundamentalTypes.h"


namespace GDL
{

//! @brief Creates a new register with arbitrary value combination from the source register
//! @tparam _x0: Index of the source register value that should be stored as first element of the new register
//! @tparam _x1: Index of the source register value that should be stored as second element of the new register
//! @tparam _x2: Index of the source register value that should be stored as third element of the new register
//! @tparam _x3: Index of the source register value that should be stored as fourth element of the new register
//! @param source: Source register
//! @return Register with arbitrary value combination from the source register
template <U32 _x0, U32 _x1, U32 _x2, U32 _x3>
inline __m128 Swizzle(__m128 source);

//! @brief Creates a new register with a single arbitrary value from the source register
//! @tparam _index: Index of the value from the source register that should be stored in all elements of the new
//! register
//! @param source: Source register
//! @return Register with a single arbitrary value from the source register
template <U32 _index>
inline __m128 Swizzle1(__m128 reg);


} // namespace GDL


#include "gdl/base/sse/swizzle.inl"
