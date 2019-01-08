#pragma once

#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/intrinsics.h"


namespace GDL::sse
{

template <typename _registerType>
constexpr U32 GetDotProductMask(U32 numValues, U32 dstIndex)
{
    U32 mask = 1 << dstIndex;
    for (U32 i = 0; i < numValues; ++i)
        mask |= 1 << (i + 4);
    return mask;
}



template <typename _registerType, U32 _numValues>
auto DotProduct(_registerType lhs, _registerType rhs)
{
    return _mmx_cvtsx_fx<_registerType>(_mmx_dp_p<GetDotProductMask<_registerType>(_numValues, 0)>(lhs, rhs));
}



} // namespace GDL::sse
