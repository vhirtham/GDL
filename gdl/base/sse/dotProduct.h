#pragma once

#include <x86intrin.h>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/utility.h"

namespace GDL::sse
{

template <typename _registerType>
constexpr U32 GetDotProductMask(U32 numValues, U32 dstIndex);


template <typename _registerType, U32 _numValues = numRegisterValues<_registerType>>
inline auto DotProduct(_registerType lhs, _registerType rhs);

} // namespace GDL::sse



#include "gdl/base/sse/dotProduct.inl"
