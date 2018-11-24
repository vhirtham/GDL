#pragma once

#include "gdl/base/sse/directAccess.h"

#include "gdl/base/exception.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/utility.h"


namespace GDL::sse
{

template <U32 _index, typename _registerType>
inline auto GetValue(const _registerType reg)
{
    using ValueType = decltype(sse::GetDataType<_registerType>());

    static_assert(_index < sse::numRegisterValues<_registerType>, "Index >= numRegisterValues");

    alignas(sse::alignmentBytes<_registerType>) ValueType array[sse::numRegisterValues<_registerType>];
    _mmx_store_p(array, reg);
    return array[_index];
}



template <typename _registerType>
inline auto GetValue(const _registerType reg, const U32 index)
{
    using ValueType = decltype(sse::GetDataType<_registerType>());

    DEV_EXCEPTION(index >= sse::numRegisterValues<_registerType>, "Index >= numRegisterValues");

    alignas(sse::alignmentBytes<_registerType>) ValueType array[sse::numRegisterValues<_registerType>];
    _mmx_store_p(array, reg);
    return array[index];
}



template <U32 _index, typename _registerType, typename _type>
inline auto SetValue(_registerType& reg, const _type value)
{
    using ValueType = decltype(sse::GetDataType<_registerType>());

    static_assert(_index < sse::numRegisterValues<_registerType>, "Index >= numRegisterValues");
    static_assert(std::is_convertible<_type, ValueType>::value,
                  "The values type can't be converted to the registers value type");

    alignas(sse::alignmentBytes<_registerType>) ValueType array[sse::numRegisterValues<_registerType>];
    _mmx_store_p(array, reg);
    array[_index] = static_cast<ValueType>(value);
    reg = _mmx_load_p<_registerType>(array);
}



template <typename _registerType, typename _type>
inline auto SetValue(_registerType& reg, const _type value, const U32 index)
{
    using ValueType = decltype(sse::GetDataType<_registerType>());

    DEV_EXCEPTION(index >= sse::numRegisterValues<_registerType>, "Index >= numRegisterValues");
    static_assert(std::is_convertible<_type, ValueType>::value,
                  "The values type can't be converted to the registers value type");

    alignas(sse::alignmentBytes<_registerType>) ValueType array[sse::numRegisterValues<_registerType>];
    _mmx_store_p(array, reg);
    array[index] = static_cast<ValueType>(value);
    reg = _mmx_load_p<_registerType>(array);
}

} // namespace GDL::sse
