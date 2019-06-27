#pragma once

#include "gdl/math/simd/vecSIMD.h"
#include "gdl/base/simd/directAccess.h"

#include "gdl/base/functions/alignment.h"

namespace GDL
{

template <typename _type, I32 _size, bool _isCol>
VecSIMD<_type, _size, _isCol>::VecSIMD()
    : mData{{0}}
{
    DEV_EXCEPTION(!IsInternalDataValid(), "Internal data is not valid. Alignment or size is not as expected.");
}



template <typename _type, I32 _size, bool _isCol>
template <typename... _args>
VecSIMD<_type, _size, _isCol>::VecSIMD(_args... args)
{
    static_assert(sizeof...(args) == _size, "Number of parameters must be equal to the vectors size");

    *this = VecSIMD(std::array<_type, _size>{{static_cast<_type>(args)...}});
}



template <typename _type, I32 _size, bool _isCol>
VecSIMD<_type, _size, _isCol>::VecSIMD(const std::array<_type, _size>& data)
{
    DEV_EXCEPTION(!IsInternalDataValid(), "Internal data is not valid. Alignment or size is not as expected.");

    std::memcpy(&mData, &data, sizeof(data));
}



template <typename _type, I32 _size, bool _isCol>
VecSIMD<_type, _size, _isCol>::VecSIMD(const DataArray& data)
    : mData{data}
{
    DEV_EXCEPTION(!IsInternalDataValid(), "Internal data is not valid. Alignment or size is not as expected.");
}



template <typename _type, I32 _size, bool _isCol>
inline _type VecSIMD<_type, _size, _isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index >= _size, "Invalid index");
    return simd::GetValue(mData[index / mNumRegisterEntries], index % mNumRegisterEntries);
}


template <typename _type, I32 _size, bool _isCol>
inline std::array<_type, _size> VecSIMD<_type, _size, _isCol>::Data() const
{
    std::array<_type, _size> data;

    std::memcpy(&data, &mData, sizeof(data));

    return data;
}



template <typename _type, I32 _size, bool _isCol>
inline const typename VecSIMD<_type, _size, _isCol>::DataArray& VecSIMD<_type, _size, _isCol>::DataSSE() const
{
    return mData;
}



template <typename _type, I32 _size, bool _isCol>
bool VecSIMD<_type, _size, _isCol>::IsInternalDataValid() const
{
    static_assert(sizeof(decltype(mData)) == sizeof(RegisterType) * mNumRegisters); // Array needs to be compact

    bool result = true;
    for (const auto& reg : mData)
        result = result && IsAligned(&reg, mAlignment);

    return result;
}



template <typename _type, I32 _size, bool _isCol>
inline std::ostream& operator<<(std::ostream& os, const VecSIMD<_type, _size, _isCol>& vec)
{
    if constexpr (_isCol)
    {
        for (U32 i = 0; i < _size; ++i)
            os << "| " << vec[i] << " |" << std::endl;
    }
    else
    {
        os << "| ";
        for (U32 i = 0; i < _size; ++i)
            os << vec[i] << " ";
        os << "|" << std::endl;
    }
    return os;
}

} // namespace GDL
