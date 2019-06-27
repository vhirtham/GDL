#pragma once

#include "gdl/math/sse/quatfSSE.h"

#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/dotProduct.h"

#include <cassert>



namespace GDL
{


QuatfSSE::QuatfSSE()
    : mData{_mm_setzero<__m128>()}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of QuatfSSE is not aligned");
}



QuatfSSE::QuatfSSE(const QuatfSSE& other)
    : mData{other.mData}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of QuatfSSE is not aligned");
}



QuatfSSE::QuatfSSE(std::array<F32, 4> data)
    : mData{_mm_setr<__m128>(data[0], data[1], data[2], data[3])}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of QuatfSSE is not aligned");
}



QuatfSSE::QuatfSSE(F32 x, F32 y, F32 z, F32 w)
    : mData{_mm_setr<__m128>(x, y, z, w)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of QuatfSSE is not aligned");
}



QuatfSSE::QuatfSSE(__m128 data)
    : mData{data}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of QuatfSSE is not aligned");
}



F32 QuatfSSE::operator[](const U32 index) const
{
    DEV_EXCEPTION(index > 3, "Invalid index value! [0..3]");

    return simd::GetValue(mData, index);
}



bool QuatfSSE::operator==(const QuatfSSE& rhs) const
{
    return mData == Approx(rhs.mData);
}



bool QuatfSSE::operator!=(const QuatfSSE& rhs) const
{
    return !(operator==(rhs));
}



const std::array<F32, 4> QuatfSSE::Data() const
{
    std::array<F32, 4> data;
    assert(sizeof(mData) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}



bool QuatfSSE::IsDataAligned() const
{
    return IsAligned(&mData, IsAligned(&mData, simd::alignmentBytes<__m128>));
}



// LCOV_EXCL_START

inline std::ostream& operator<<(std::ostream& os, const QuatfSSE& quat)
{
    os << "| " << quat[0] << " |\n| " << quat[1] << " |\n| " << quat[2] << " |\n| " << quat[3] << " |" << std::endl;
    return os;
}

// LCOV_EXCL_STOP

} // namespace GDL
