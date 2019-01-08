#pragma once

#include "gdl/math/sse/vec3SSE.h"

#include "gdl/base/approx.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/dotProduct.h"

#include <cassert>



namespace GDL
{

template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE()
    : mData{_mmx_setzero_p<__m128>()}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE(const Vec3SSE& other)
    : mData{other.mData}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE(std::array<F32, 3> data)
    : mData{_mmx_setr_p<__m128>(data[0], data[1], data[2], 0)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE(F32 v0, F32 v1, F32 v2)
    : mData{_mmx_setr_p<__m128>(v0, v1, v2, 0)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
Vec3SSE<_isCol>::Vec3SSE(__m128 data)
    : mData{data}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Vec3SSE is not aligned");
}



template <bool _isCol>
bool Vec3SSE<_isCol>::operator==(const Vec3SSE& rhs) const
{
    return mData == Approx<__m128, 3>(rhs.mData);
}



template <bool _isCol>
bool Vec3SSE<_isCol>::operator!=(const Vec3SSE& rhs) const
{
    return !(operator==(rhs));
}



template <bool _isCol>
F32 Vec3SSE<_isCol>::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 2, "Invalid index value! [0..2]");

    return sse::GetValue(mData, index);
}



template <bool _isCol>
const std::array<F32, 3> Vec3SSE<_isCol>::Data() const
{
    std::array<F32, 3> data;
    assert(sizeof(mData) - sizeof(F32) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}



template <bool _isCol>
template<bool _isColRhs>
F32 Vec3SSE<_isCol>::Dot(Vec3SSE<_isColRhs> rhs) const
{
    return sse::DotProduct<__m128,3>(mData,rhs.mData);
}






template <bool _isCol>
bool Vec3SSE<_isCol>::IsDataAligned() const
{
    return IsAligned(&mData, 16);
}




// LCOV_EXCL_START

template <>
std::ostream& operator<<(std::ostream& os, const Vec3SSE<true>& vec)
{
    os << "| " << vec[0] << " |\n| " << vec[1] << " |\n| " << vec[2] << " |" << std::endl;
    return os;
}



template <>
std::ostream& operator<<(std::ostream& os, const Vec3SSE<false>& vec)
{
    os << "| " << vec[0] << " " << vec[1] << " " << vec[2] << " |" << std::endl;
    return os;
}


// LCOV_EXCL_STOP


} // namespace GDL
