#pragma once
#include "gdl/math/mat4SSE.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/transpose.h"
#include "gdl/math/vec4.h"

#include "gdl/base/sse/maskMacros.h"
#include "gdl/base/sse/swizzle.h"

#include <cassert>
#include <cstring>
#include <type_traits>



namespace GDL
{



Mat4SSE::Mat4SSE()
    : mData({{{_mmx_setzero_p<__m128>()},
              {_mmx_setzero_p<__m128>()},
              {_mmx_setzero_p<__m128>()},
              {_mmx_setzero_p<__m128>()}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4SSE are not 16 byte aligned");
}


Mat4SSE::Mat4SSE(std::array<F32, 16> data)
    : mData({{{_mmx_setr_p<__m128>(data[0], data[1], data[2], data[3])},
              {_mmx_setr_p<__m128>(data[4], data[5], data[6], data[7])},
              {_mmx_setr_p<__m128>(data[8], data[9], data[10], data[11])},
              {_mmx_setr_p<__m128>(data[12], data[13], data[14], data[15])}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4SSE are not 16 byte aligned");
}



Mat4SSE::Mat4SSE(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10, F32 v11,
                 F32 v12, F32 v13, F32 v14, F32 v15)
    : mData({{{_mmx_setr_p<__m128>(v0, v1, v2, v3)},
              {_mmx_setr_p<__m128>(v4, v5, v6, v7)},
              {_mmx_setr_p<__m128>(v8, v9, v10, v11)},
              {_mmx_setr_p<__m128>(v12, v13, v14, v15)}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4SSE are not 16 byte aligned");
}



Mat4SSE::Mat4SSE(__m128 col0, __m128 col1, __m128 col2, __m128 col3)
    : mData({{col0, col1, col2, col3}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4SSE are not 16 byte aligned");
}



Mat4SSE::Mat4SSE(const Mat4SSE& other)
    : mData(other.mData)
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4SSE are not 16 byte aligned");
}



F32 Mat4SSE::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 3, "row - invalid value! [0..3]");
    DEV_EXCEPTION(col > 3, "col - invalid value! [0..3]");

    return sse::GetValue(mData[col], row);
}

bool Mat4SSE::operator==(const Mat4SSE& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 4; ++i)
        result = result && mData[i] == Approx(rhs.mData[i]);
    return result;
}



bool Mat4SSE::operator!=(const Mat4SSE& rhs) const
{
    return !(operator==(rhs));
}



Mat4SSE& Mat4SSE::operator+=(const Mat4SSE& other)

{
    mData[0] = _mmx_add_p(mData[0], other.mData[0]);
    mData[1] = _mmx_add_p(mData[1], other.mData[1]);
    mData[2] = _mmx_add_p(mData[2], other.mData[2]);
    mData[3] = _mmx_add_p(mData[3], other.mData[3]);
    return *this;
}



Mat4SSE Mat4SSE::operator+(const Mat4SSE& other)

{
    return Mat4SSE(_mmx_add_p(mData[0], other.mData[0]), _mmx_add_p(mData[1], other.mData[1]),
                   _mmx_add_p(mData[2], other.mData[2]), _mmx_add_p(mData[3], other.mData[3]));
}



Mat4SSE Mat4SSE::operator*(const Mat4SSE& rhs) const
{

    return Mat4SSE(_mmx_fmadd_p(Swizzle1<0>(rhs.mData[0]), mData[0],
                                _mmx_fmadd_p(Swizzle1<1>(rhs.mData[0]), mData[1],
                                             _mmx_fmadd_p(Swizzle1<2>(rhs.mData[0]), mData[2],
                                                          _mm_mul_ps(Swizzle1<3>(rhs.mData[0]), mData[3])))),
                   _mmx_fmadd_p(Swizzle1<0>(rhs.mData[1]), mData[0],
                                _mmx_fmadd_p(Swizzle1<1>(rhs.mData[1]), mData[1],
                                             _mmx_fmadd_p(Swizzle1<2>(rhs.mData[1]), mData[2],
                                                          _mm_mul_ps(Swizzle1<3>(rhs.mData[1]), mData[3])))),
                   _mmx_fmadd_p(Swizzle1<0>(rhs.mData[2]), mData[0],
                                _mmx_fmadd_p(Swizzle1<1>(rhs.mData[2]), mData[1],
                                             _mmx_fmadd_p(Swizzle1<2>(rhs.mData[2]), mData[2],
                                                          _mm_mul_ps(Swizzle1<3>(rhs.mData[2]), mData[3])))),
                   _mmx_fmadd_p(Swizzle1<0>(rhs.mData[3]), mData[0],
                                _mmx_fmadd_p(Swizzle1<1>(rhs.mData[3]), mData[1],
                                             _mmx_fmadd_p(Swizzle1<2>(rhs.mData[3]), mData[2],
                                                          _mm_mul_ps(Swizzle1<3>(rhs.mData[3]), mData[3])))));
}

Vec4SSE<true> Mat4SSE::operator*(const Vec4SSE<true>& rhs) const
{
    return Vec4fSSE<true>(_mmx_fmadd_p(Swizzle1<0>(rhs.mData), mData[0],
                                       _mmx_fmadd_p(Swizzle1<1>(rhs.mData), mData[1],
                                                    _mmx_fmadd_p(Swizzle1<2>(rhs.mData), mData[2],
                                                                 _mm_mul_ps(Swizzle1<3>(rhs.mData), mData[3])))));
}



Mat4SSE Mat4SSE::Transpose() const
{
    Mat4SSE t;
    sse::Transpose(mData[0], mData[1], mData[2], mData[3], t.mData[0], t.mData[1], t.mData[2], t.mData[3]);
    return t;
}



const std::array<F32, 16> Mat4SSE::Data() const
{
    std::array<F32, 16> data;
    assert(sizeof(mData) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}



bool Mat4SSE::IsDataAligned() const
{
    return (IsAligned(&mData[0], 16) && IsAligned(&mData[1], 16) && IsAligned(&mData[2], 16) &&
            IsAligned(&mData[3], 16));
}



inline std::ostream& operator<<(std::ostream& os, const Mat4SSE& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " " << mat(i, 2) << " " << mat(i, 3) << " |" << std::endl;
    return os;
}



} // namespace GDL
