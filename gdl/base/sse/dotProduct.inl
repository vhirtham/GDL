#pragma once

#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/intrinsics.h"


namespace GDL::sse
{

template <I32 _dstIndex>
constexpr U32 GetDotProductDestinationMask()
{
    static_assert(_dstIndex < 4, "Invalid destination index. Valid values: [0..3]");
    if constexpr (_dstIndex < 0)
        return ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));
    else
        return (1 << _dstIndex);
}



template <typename _registerType, U32 _numValues, I32 _dstIndex>
constexpr U32 GetDotProductMask()
{
    U32 mask = GetDotProductDestinationMask<_dstIndex>();
    for (U32 i = 0; i < _numValues; ++i)
        mask |= 1 << (i + 4);
    return mask;
}



template <typename _registerType, U32 _numValues, bool _returnRegister, I32 _dstIndex>
auto DotProduct(const _registerType& lhs, const _registerType& rhs)
{
    static_assert(_numValues <= sse::numRegisterValues<_registerType>, "Invalid number of values");

    if constexpr (_returnRegister)
        return _mmx_dp_p<GetDotProductMask<_registerType, _numValues, _dstIndex>()>(lhs, rhs);
    else
        return _mmx_cvtsx_fx<_registerType>(DotProduct<_registerType, _numValues, true, 0>(lhs, rhs));
}



} // namespace GDL::sse
