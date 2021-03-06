#pragma once
#include "gdl/math/simd/mat4fSSE.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/base/simd/determinant.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/swizzle.h"
#include "gdl/base/simd/transpose.h"
#include "gdl/math/vec4.h"



#include <cassert>
#include <cstring>
#include <type_traits>



namespace GDL
{



Mat4fSSE::Mat4fSSE()
    : mData({{{_mm_setzero<__m128>()}, {_mm_setzero<__m128>()}, {_mm_setzero<__m128>()}, {_mm_setzero<__m128>()}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fSSE are not 16 byte aligned");
}


Mat4fSSE::Mat4fSSE(std::array<F32, 16> data)
    : mData({{{_mm_setr<__m128>(data[0], data[1], data[2], data[3])},
              {_mm_setr<__m128>(data[4], data[5], data[6], data[7])},
              {_mm_setr<__m128>(data[8], data[9], data[10], data[11])},
              {_mm_setr<__m128>(data[12], data[13], data[14], data[15])}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fSSE are not 16 byte aligned");
}



Mat4fSSE::Mat4fSSE(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10, F32 v11,
                   F32 v12, F32 v13, F32 v14, F32 v15)
    : mData({{{_mm_setr<__m128>(v0, v1, v2, v3)},
              {_mm_setr<__m128>(v4, v5, v6, v7)},
              {_mm_setr<__m128>(v8, v9, v10, v11)},
              {_mm_setr<__m128>(v12, v13, v14, v15)}}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fSSE are not 16 byte aligned");
}



Mat4fSSE::Mat4fSSE(__m128 col0, __m128 col1, __m128 col2, __m128 col3)
    : mData({{col0, col1, col2, col3}})
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fSSE are not 16 byte aligned");
}



Mat4fSSE::Mat4fSSE(const Mat4fSSE& other)
    : mData(other.mData)
{
    DEV_EXCEPTION(!IsDataAligned(), "One or more registers of Mat4fSSE are not 16 byte aligned");
}



F32 Mat4fSSE::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 3, "row - invalid value! [0..3]");
    DEV_EXCEPTION(col > 3, "col - invalid value! [0..3]");

    return simd::GetValue(mData[col], row);
}

bool Mat4fSSE::operator==(const Mat4fSSE& rhs) const
{
    bool result = true;
    for (U32 i = 0; i < 4; ++i)
        result = result && mData[i] == Approx(rhs.mData[i]);
    return result;
}



bool Mat4fSSE::operator!=(const Mat4fSSE& rhs) const
{
    return !(operator==(rhs));
}



Mat4fSSE& Mat4fSSE::operator+=(const Mat4fSSE& other)

{
    mData[0] = _mm_add(mData[0], other.mData[0]);
    mData[1] = _mm_add(mData[1], other.mData[1]);
    mData[2] = _mm_add(mData[2], other.mData[2]);
    mData[3] = _mm_add(mData[3], other.mData[3]);
    return *this;
}



Mat4fSSE Mat4fSSE::operator+(const Mat4fSSE& other)

{
    return Mat4fSSE(_mm_add(mData[0], other.mData[0]), _mm_add(mData[1], other.mData[1]),
                    _mm_add(mData[2], other.mData[2]), _mm_add(mData[3], other.mData[3]));
}



Mat4fSSE Mat4fSSE::operator*(const Mat4fSSE& rhs) const
{
    using namespace GDL::simd;
    return Mat4fSSE(_mm_fmadd(simd::Broadcast<0>(rhs.mData[0]), mData[0],
                              _mm_fmadd(Broadcast<1>(rhs.mData[0]), mData[1],
                                        _mm_fmadd(Broadcast<2>(rhs.mData[0]), mData[2],
                                                  _mm_mul(Broadcast<3>(rhs.mData[0]), mData[3])))),
                    _mm_fmadd(Broadcast<0>(rhs.mData[1]), mData[0],
                              _mm_fmadd(Broadcast<1>(rhs.mData[1]), mData[1],
                                        _mm_fmadd(Broadcast<2>(rhs.mData[1]), mData[2],
                                                  _mm_mul(Broadcast<3>(rhs.mData[1]), mData[3])))),
                    _mm_fmadd(Broadcast<0>(rhs.mData[2]), mData[0],
                              _mm_fmadd(Broadcast<1>(rhs.mData[2]), mData[1],
                                        _mm_fmadd(Broadcast<2>(rhs.mData[2]), mData[2],
                                                  _mm_mul(Broadcast<3>(rhs.mData[2]), mData[3])))),
                    _mm_fmadd(Broadcast<0>(rhs.mData[3]), mData[0],
                              _mm_fmadd(Broadcast<1>(rhs.mData[3]), mData[1],
                                        _mm_fmadd(Broadcast<2>(rhs.mData[3]), mData[2],
                                                  _mm_mul(Broadcast<3>(rhs.mData[3]), mData[3])))));
}

Vec4fSSE<true> Mat4fSSE::operator*(const Vec4fSSE<true>& rhs) const
{
    using namespace GDL::simd;
    return Vec4fSSE<true>(_mm_fmadd(
            Broadcast<0>(rhs.mData), mData[0],
            _mm_fmadd(Broadcast<1>(rhs.mData), mData[1],
                      _mm_fmadd(Broadcast<2>(rhs.mData), mData[2], _mm_mul(Broadcast<3>(rhs.mData), mData[3])))));
}



const std::array<F32, 16> Mat4fSSE::Data() const
{
    std::array<F32, 16> data;
    assert(sizeof(mData) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}

const std::array<__m128, 4>& Mat4fSSE::DataSSE() const
{
    return mData;
}


F32 Mat4fSSE::Det() const
{
    return simd::Determinant4x4(mData[0], mData[1], mData[2], mData[3]);
}



Mat4fSSE Mat4fSSE::Transpose() const
{
    Mat4fSSE t;
    simd::Transpose4x4(mData[0], mData[1], mData[2], mData[3], t.mData[0], t.mData[1], t.mData[2], t.mData[3]);
    return t;
}



bool Mat4fSSE::IsDataAligned() const
{
    return (IsAligned(&mData[0], simd::alignmentBytes<__m128>) && IsAligned(&mData[1], simd::alignmentBytes<__m128>) &&
            IsAligned(&mData[2], simd::alignmentBytes<__m128>) && IsAligned(&mData[3], simd::alignmentBytes<__m128>));
}



inline std::ostream& operator<<(std::ostream& os, const Mat4fSSE& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " " << mat(i, 2) << " " << mat(i, 3) << " |" << std::endl;
    return os;
}



} // namespace GDL
