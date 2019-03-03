#pragma once

#include "gdl/math/sse/vecSSE.h"

#include "gdl/base/functions/alignment.h"

namespace GDL
{

template <typename _type, I32 _size, bool _isCol>
VecSSE<_type, _size, _isCol>::VecSSE()
    : mData{{0}}
{
    DEV_EXCEPTION(!IsInternalDataValid(), "Internal data is not valid. Alignment or size is not as expected.");
}



template <typename _type, I32 _size, bool _isCol>
template <typename... _args>
VecSSE<_type, _size, _isCol>::VecSSE(_args... args)
{
    static_assert(sizeof...(args) == _size, "Number of parameters must be equal to the vectors size");

    *this = VecSSE(std::array<_type, _size>{{static_cast<_type>(args)...}});
}



template <typename _type, I32 _size, bool _isCol>
VecSSE<_type, _size, _isCol>::VecSSE(const std::array<_type, _size>& data)
{
    DEV_EXCEPTION(!IsInternalDataValid(), "Internal data is not valid. Alignment or size is not as expected.");

    std::memcpy(&mData, &data, sizeof(data));
}



template <typename _type, I32 _size, bool _isCol>
inline std::array<_type, _size> VecSSE<_type, _size, _isCol>::Data() const
{
    std::array<_type, _size> data;

    std::memcpy(&data, &mData, sizeof(data));

    return data;
}



template <typename _type, I32 _size, bool _isCol>
bool VecSSE<_type, _size, _isCol>::IsInternalDataValid() const
{
    static_assert(sizeof(decltype(mData)) == sizeof(RegisterType) * mNumRegisters); // Array needs to be compact

    bool result = true;
    for (const auto& reg : mData)
        result = result && IsAligned(&reg, mAlignment);

    return result;
}

} // namespace GDL
