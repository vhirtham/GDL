#pragma once
#include "gdl/math/sse/mat2SSE.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/swizzle.h"



#include <cassert>
#include <cstring>
#include <type_traits>



namespace GDL
{



Mat2SSE::Mat2SSE()
    : mData{_mmx_setzero_p<__m128>()}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2SSE is not 16 byte aligned");
}



Mat2SSE::Mat2SSE(std::array<F32, 4> data)
    : mData{_mmx_setr_p<__m128>(data[0], data[1], data[2], data[3])}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2SSE is not 16 byte aligned");
}



Mat2SSE::Mat2SSE(F32 v0, F32 v1, F32 v2, F32 v3)
    : mData{_mmx_setr_p<__m128>(v0, v1, v2, v3)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2SSE is not 16 byte aligned");
}



Mat2SSE::Mat2SSE(__m128 reg)
    : mData{reg}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2SSE is not 16 byte aligned");
}



Mat2SSE::Mat2SSE(const Mat2SSE& other)
    : mData(other.mData)
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2SSE is not 16 byte aligned");
}



F32 Mat2SSE::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 1, "row - invalid value! [0..1]");
    DEV_EXCEPTION(col > 1, "col - invalid value! [0..1]");

    return sse::GetValue(mData, row + 2 * col);
}



bool Mat2SSE::operator==(const Mat2SSE& rhs) const
{
    return mData == Approx(rhs.mData);
}



bool Mat2SSE::operator!=(const Mat2SSE& rhs) const
{
    return !(operator==(rhs));
}



Mat2SSE& Mat2SSE::operator+=(const Mat2SSE& other)
{
    mData = _mmx_add_p(mData, other.mData);
    return *this;
}



Mat2SSE Mat2SSE::operator+(const Mat2SSE& other)

{
    return Mat2SSE(_mmx_add_p(mData, other.mData));
}



Mat2SSE Mat2SSE::operator*(const Mat2SSE& rhs) const
{
    using namespace GDL::sse;
    return Mat2SSE(_mmx_fmadd_p(mData, Swizzle<0, 0, 3, 3>(rhs.mData),
                                _mmx_mul_p(Swizzle<2, 3, 0, 1>(mData), Swizzle<1, 1, 2, 2>(rhs.mData))));
}



Vec2SSE<true> Mat2SSE::operator*(const Vec2SSE<true>& rhs) const
{
    using namespace GDL::sse;
    return Vec2fSSE<true>(_mmx_fmadd_p(mData, Swizzle<0, 0, 3, 3>(rhs.mData),
                                       _mmx_mul_p(Swizzle<2, 3, 0, 1>(mData), Swizzle<1, 1, 2, 2>(rhs.mData))));
}



const std::array<F32, 4> Mat2SSE::Data() const
{
    alignas(sse::alignmentBytes<__m128>) std::array<F32, 4> data;
    assert(sizeof(mData) == sizeof(data));

    _mmx_store_p(&data[0], mData);
    return data;
}



inline F32 Mat2SSE::Det() const
{
    __m128 tmp = _mmx_mul_p(mData, sse::Swizzle<3, 2, 1, 0>(mData));
    return _mm_cvtss_f32(_mm_sub_ps(tmp, sse::Swizzle<1, 0, 3, 1>(tmp)));
}



Mat2SSE Mat2SSE::Transpose() const
{
    return Mat2SSE(sse::Swizzle<0, 2, 1, 3>(mData));
}



bool Mat2SSE::IsDataAligned() const
{
    return IsAligned(&mData, sse::alignmentBytes<__m128>);
}



inline std::ostream& operator<<(std::ostream& os, const Mat2SSE& mat)
{
    for (U32 i = 0; i < 2; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " |" << std::endl;
    return os;
}



} // namespace GDL
