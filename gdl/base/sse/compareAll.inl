#pragma once

#include "gdl/base/sse/compareAll.h"

#include "gdl/base/functions/pow.h"
#include "gdl/base/sse/intrinsics.h"



#include <limits>

namespace GDL::simd
{

template <typename _registerType, U32 _numComparedValues>
constexpr auto SSECalculateComparisonValueAllTrue()
{
    using ReturnType = decltype(_mmx_movemask_epi8(ReinterpretAsIntRegister(_mmx_setzero_p<_registerType>())));
    constexpr U32 numRegisterEntries = numRegisterValues<_registerType>;

    if constexpr (_numComparedValues == numRegisterEntries)
        return std::numeric_limits<ReturnType>::max();
    else
    {
        constexpr U32 numUnusedBits = (sizeof(ReturnType) * 8 * _numComparedValues) / numRegisterEntries;
        return static_cast<ReturnType>(Pow<numUnusedBits>(2) - 1);
    }
}



template <typename _registerType, U32 _numComparedValues, typename _compFunction>
inline bool CompareAllTrue(_registerType lhs, _registerType rhs, _compFunction compFunction)
{
    constexpr U32 numRegisterEntries = numRegisterValues<_registerType>;

    static_assert(_numComparedValues > 0 && _numComparedValues <= numRegisterEntries,
                  "Invalid number of compared values ---> [1 ... numRegisterEntries]");

    auto cmpResult = _mmx_movemask_epi8(ReinterpretAsIntRegister(compFunction(lhs, rhs)));
    constexpr auto refResult = SSECalculateComparisonValueAllTrue<_registerType, _numComparedValues>();

    static_assert(std::is_same_v<const decltype(cmpResult), decltype(refResult)>, "Mismatching types for comparison");

    if constexpr (_numComparedValues != numRegisterEntries)
        cmpResult &= refResult; // Set bits of elemts that should not be compared to zero

    return cmpResult >= refResult;
}



template <typename _registerType, U32 _numComparedValues>
inline bool CompareAllEqual(_registerType lhs, _registerType rhs)
{
    return CompareAllTrue<_registerType, _numComparedValues, decltype(_mmx_cmpeq_p<_registerType>)>(
            lhs, rhs, &_mmx_cmpeq_p<_registerType>);
}



template <typename _registerType, U32 _numComparedValues>
inline bool CompareAllLessEqual(_registerType lhs, _registerType rhs)
{
    return CompareAllTrue<_registerType, _numComparedValues, decltype(_mmx_cmple_p<_registerType>)>(
            lhs, rhs, &_mmx_cmple_p<_registerType>);
}



template <typename _registerType, U32 _numComparedValues>
inline bool CompareAllLessThan(_registerType lhs, _registerType rhs)
{
    return CompareAllTrue<_registerType, _numComparedValues, decltype(_mmx_cmplt_p<_registerType>)>(
            lhs, rhs, &_mmx_cmplt_p<_registerType>);
}



template <typename _registerType, U32 _numComparedValues>
inline bool CompareAllGreaterThan(_registerType lhs, _registerType rhs)
{
    return CompareAllTrue<_registerType, _numComparedValues, decltype(_mmx_cmpgt_p<_registerType>)>(
            lhs, rhs, &_mmx_cmpgt_p<_registerType>);
}
} // namespace GDL::simd
