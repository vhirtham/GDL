#pragma once

#include "gdl/base/simd/compareAll.h"

#include "gdl/base/functions/pow.h"
#include "gdl/base/simd/intrinsics.h"



#include <limits>

namespace GDL::simd
{

template <typename _registerType, U32 _numComparedValues>
constexpr auto SSECalculateComparisonValueAllTrue()
{
    using ReturnType = decltype(_mm_movemaskEpi8(_mm_castFI(_mm_setzero<_registerType>())));
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

    auto cmpResult = _mm_movemaskEpi8(_mm_castFI(compFunction(lhs, rhs)));
    constexpr auto refResult = SSECalculateComparisonValueAllTrue<_registerType, _numComparedValues>();

    static_assert(std::is_same_v<const decltype(cmpResult), decltype(refResult)>, "Mismatching types for comparison");

    if constexpr (_numComparedValues != numRegisterEntries)
        cmpResult &= refResult; // Set bits of elemts that should not be compared to zero

    return cmpResult >= refResult;
}



template <typename _registerType, U32 _numComparedValues>
inline bool CompareAllEqual(_registerType lhs, _registerType rhs)
{
    return CompareAllTrue<_registerType, _numComparedValues, decltype(_mm_cmpeq<_registerType>)>(
            lhs, rhs, &_mm_cmpeq<_registerType>);
}



template <typename _registerType, U32 _numComparedValues>
inline bool CompareAllLessEqual(_registerType lhs, _registerType rhs)
{
    return CompareAllTrue<_registerType, _numComparedValues, decltype(_mm_cmple<_registerType>)>(
            lhs, rhs, &_mm_cmple<_registerType>);
}



template <typename _registerType, U32 _numComparedValues>
inline bool CompareAllLessThan(_registerType lhs, _registerType rhs)
{
    return CompareAllTrue<_registerType, _numComparedValues, decltype(_mm_cmplt<_registerType>)>(
            lhs, rhs, &_mm_cmplt<_registerType>);
}



template <typename _registerType, U32 _numComparedValues>
inline bool CompareAllGreaterThan(_registerType lhs, _registerType rhs)
{
    return CompareAllTrue<_registerType, _numComparedValues, decltype(_mm_cmpgt<_registerType>)>(
            lhs, rhs, &_mm_cmpgt<_registerType>);
}



template <typename _registerType>
inline bool CompareMemoryZero(const _registerType& reg)
{
    const _registerType zero = _mm_set1<_registerType>(0.);
    return !static_cast<bool>(std::memcmp(&zero, &reg, sizeof(_registerType)));
}
} // namespace GDL::simd
