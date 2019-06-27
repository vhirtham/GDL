#pragma once
#include "gdl/math/sse/mat2fSSE.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/swizzle.h"



#include <cassert>
#include <cstring>
#include <type_traits>



namespace GDL
{



Mat2fSSE::Mat2fSSE()
    : mData{_mm_setzero<__m128>()}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2fSSE is not 16 byte aligned");
}



Mat2fSSE::Mat2fSSE(std::array<F32, 4> data)
    : mData{_mm_setr<__m128>(data[0], data[1], data[2], data[3])}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2fSSE is not 16 byte aligned");
}



Mat2fSSE::Mat2fSSE(F32 v0, F32 v1, F32 v2, F32 v3)
    : mData{_mm_setr<__m128>(v0, v1, v2, v3)}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2fSSE is not 16 byte aligned");
}



Mat2fSSE::Mat2fSSE(__m128 reg)
    : mData{reg}
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2fSSE is not 16 byte aligned");
}



Mat2fSSE::Mat2fSSE(const Mat2fSSE& other)
    : mData(other.mData)
{
    DEV_EXCEPTION(!IsDataAligned(), "Register of Mat2fSSE is not 16 byte aligned");
}



F32 Mat2fSSE::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 1, "row - invalid value! [0..1]");
    DEV_EXCEPTION(col > 1, "col - invalid value! [0..1]");

    return simd::GetValue(mData, row + 2 * col);
}



bool Mat2fSSE::operator==(const Mat2fSSE& rhs) const
{
    return mData == Approx(rhs.mData);
}



bool Mat2fSSE::operator!=(const Mat2fSSE& rhs) const
{
    return !(operator==(rhs));
}



Mat2fSSE& Mat2fSSE::operator+=(const Mat2fSSE& other)
{
    mData = _mm_add(mData, other.mData);
    return *this;
}



Mat2fSSE Mat2fSSE::operator+(const Mat2fSSE& other)

{
    return Mat2fSSE(_mm_add(mData, other.mData));
}



Mat2fSSE Mat2fSSE::operator*(const Mat2fSSE& rhs) const
{
    using namespace GDL::simd;
    return Mat2fSSE(_mm_fmadd(mData, Permute<0, 0, 3, 3>(rhs.mData),
                                 _mm_mul(Permute<2, 3, 0, 1>(mData), Permute<1, 1, 2, 2>(rhs.mData))));
}



Vec2fSSE<true> Mat2fSSE::operator*(const Vec2fSSE<true>& rhs) const
{
    using namespace GDL::simd;
    return Vec2fSSE<true>(_mm_fmadd(mData, Permute<0, 0, 3, 3>(rhs.mData),
                                       _mm_mul(Permute<2, 3, 0, 1>(mData), Permute<1, 1, 2, 2>(rhs.mData))));
}



const std::array<F32, 4> Mat2fSSE::Data() const
{
    alignas(simd::alignmentBytes<__m128>) std::array<F32, 4> data;
    assert(sizeof(mData) == sizeof(data));

    _mm_store(&data[0], mData);
    return data;
}



inline F32 Mat2fSSE::Det() const
{
    return simd::Determinant2x2(mData);
}



Mat2fSSE Mat2fSSE::Transpose() const
{
    return Mat2fSSE(simd::Permute<0, 2, 1, 3>(mData));
}



bool Mat2fSSE::IsDataAligned() const
{
    return IsAligned(&mData, simd::alignmentBytes<__m128>);
}



inline std::ostream& operator<<(std::ostream& os, const Mat2fSSE& mat)
{
    for (U32 i = 0; i < 2; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " |" << std::endl;
    return os;
}



} // namespace GDL
