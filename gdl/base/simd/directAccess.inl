#pragma once

#include "gdl/base/simd/directAccess.h"

#include "gdl/base/exception.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/utility.h"


namespace GDL::simd
{

template <U32 _index, typename _registerType>
inline auto GetValue(const _registerType reg)
{
    using ValueType = decltype(simd::GetDataType<_registerType>());

    static_assert(_index < simd::numRegisterValues<_registerType>, "Index >= numRegisterValues");

    alignas(simd::alignmentBytes<_registerType>) ValueType array[simd::numRegisterValues<_registerType>];
    _mm_store(array, reg);
    return array[_index];
}



template <typename _registerType>
inline auto GetValue(const _registerType reg, const U32 index)
{
    using ValueType = decltype(simd::GetDataType<_registerType>());

    DEV_EXCEPTION(index >= simd::numRegisterValues<_registerType>, "Index >= numRegisterValues");

    alignas(simd::alignmentBytes<_registerType>) ValueType array[simd::numRegisterValues<_registerType>];
    _mm_store(array, reg);
    return array[index];
}



template <U32 _index, typename _registerType, typename _type>
inline auto SetValue(_registerType& reg, const _type value)
{
    using ValueType = decltype(simd::GetDataType<_registerType>());

    static_assert(_index < simd::numRegisterValues<_registerType>, "Index >= numRegisterValues");
    static_assert(std::is_convertible<_type, ValueType>::value,
                  "The values type can't be converted to the registers value type");

    alignas(simd::alignmentBytes<_registerType>) ValueType array[simd::numRegisterValues<_registerType>];
    _mm_store(array, reg);
    array[_index] = static_cast<ValueType>(value);
    reg = _mmx_load_p<_registerType>(array);
}



template <typename _registerType, typename _type>
inline auto SetValue(_registerType& reg, const U32 index, const _type value)
{
    using ValueType = decltype(simd::GetDataType<_registerType>());

    DEV_EXCEPTION(index >= simd::numRegisterValues<_registerType>, "Index >= numRegisterValues");
    static_assert(std::is_convertible<_type, ValueType>::value,
                  "The values type can't be converted to the registers value type");

    alignas(simd::alignmentBytes<_registerType>) ValueType array[simd::numRegisterValues<_registerType>];
    _mm_store(array, reg);
    array[index] = static_cast<ValueType>(value);
    reg = _mmx_load_p<_registerType>(array);
}

} // namespace GDL::simd
