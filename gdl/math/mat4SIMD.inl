#pragma once
#include "gdl/math/mat4SIMD.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/alignment.h"

#include <cassert>
#include <cstring>
#include <type_traits>



namespace GDL
{



Mat4SIMD::Mat4SIMD()
    : mData({{{_mmx_setzero_p<__m128>()},
              {_mmx_setzero_p<__m128>()},
              {_mmx_setzero_p<__m128>()},
              {_mmx_setzero_p<__m128>()}}})
{
    ConstructionChecks();
}


Mat4SIMD::Mat4SIMD(std::array<F32, 16> data)
    : mData({{{_mmx_setr_p<__m128>(data[0], data[1], data[2], data[3])},
              {_mmx_setr_p<__m128>(data[4], data[5], data[6], data[7])},
              {_mmx_setr_p<__m128>(data[8], data[9], data[10], data[11])},
              {_mmx_setr_p<__m128>(data[12], data[13], data[14], data[15])}}})
{
    ConstructionChecks();
}



Mat4SIMD::Mat4SIMD(F32 v0, F32 v1, F32 v2, F32 v3, F32 v4, F32 v5, F32 v6, F32 v7, F32 v8, F32 v9, F32 v10, F32 v11,
                   F32 v12, F32 v13, F32 v14, F32 v15)
    : mData({{{_mmx_setr_p<__m128>(v0, v1, v2, v3)},
              {_mmx_setr_p<__m128>(v4, v5, v6, v7)},
              {_mmx_setr_p<__m128>(v8, v9, v10, v11)},
              {_mmx_setr_p<__m128>(v12, v13, v14, v15)}}})
{
    ConstructionChecks();
}



Mat4SIMD::Mat4SIMD(__m128 col0, __m128 col1, __m128 col2, __m128 col3)
    : mData({{col0, col1, col2, col3}})
{
    ConstructionChecks();
}



Mat4SIMD::Mat4SIMD(const Mat4SIMD& other)
    : mData(other.mData)
{
    ConstructionChecks();
}



F32 Mat4SIMD::operator()(const U32 row, const U32 col) const
{
    DEV_EXCEPTION(row > 3, "row - invalid value! [0..3]");
    DEV_EXCEPTION(col > 3, "col - invalid value! [0..3]");

    return mData[col][row];
}

bool Mat4SIMD::operator==(const Mat4SIMD& rhs) const
{
    // TODO: Replace this version as soon as Approx supports registers!
    bool result = true;
    for (U32 i = 0; i < 4; ++i)
        for (U32 j = 0; j < 4; ++j)
                result = result && mData[i][j] == Approx(rhs.mData[i][j]);
    return result;
}



bool Mat4SIMD::operator!=(const Mat4SIMD& rhs) const
{
    return !(operator==(rhs));
}



Mat4SIMD& Mat4SIMD::operator+=(const Mat4SIMD& other)

{
    mData[0] = _mmx_add_p(mData[0], other.mData[0]);
    mData[1] = _mmx_add_p(mData[1], other.mData[1]);
    mData[2] = _mmx_add_p(mData[2], other.mData[2]);
    mData[3] = _mmx_add_p(mData[3], other.mData[3]);
    return *this;
}



Mat4SIMD Mat4SIMD::operator+(const Mat4SIMD& other)

{
    return Mat4SIMD(_mmx_add_p(mData[0], other.mData[0]), _mmx_add_p(mData[1], other.mData[1]),
                    _mmx_add_p(mData[2], other.mData[2]), _mmx_add_p(mData[3], other.mData[3]));
}



Mat4SIMD Mat4SIMD::operator*(const Mat4SIMD& other) const
{
    alignas(16) __m128 Col0 =
            _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[0][0]), mData[0],
                         _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[0][1]), mData[1],
                                      _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[0][2]), mData[2],
                                                   _mm_mul_ps(_mmx_set1_p<__m128>(other.mData[0][3]), mData[3]))));

    alignas(16) __m128 Col1 =
            _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[1][0]), mData[0],
                         _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[1][1]), mData[1],
                                      _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[1][2]), mData[2],
                                                   _mm_mul_ps(_mmx_set1_p<__m128>(other.mData[1][3]), mData[3]))));

    alignas(16) __m128 Col2 =
            _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[2][0]), mData[0],
                         _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[2][1]), mData[1],
                                      _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[2][2]), mData[2],
                                                   _mm_mul_ps(_mmx_set1_p<__m128>(other.mData[2][3]), mData[3]))));

    alignas(16) __m128 Col3 =
            _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[3][0]), mData[0],
                         _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[3][1]), mData[1],
                                      _mmx_fmadd_p(_mmx_set1_p<__m128>(other.mData[3][2]), mData[2],
                                                   _mm_mul_ps(_mmx_set1_p<__m128>(other.mData[3][3]), mData[3]))));

    return Mat4SIMD(Col0, Col1, Col2, Col3);
}



const std::array<F32, 16> Mat4SIMD::Data() const
{
    std::array<F32, 16> data;
    assert(sizeof(mData) == sizeof(data));

    std::memcpy(&data, &mData, sizeof(data));
    return data;
}



void Mat4SIMD::ConstructionChecks() const
{
    DEV_EXCEPTION(!(IsAligned(&mData[0], 16) && IsAligned(&mData[1], 16) && IsAligned(&mData[2], 16) &&
                    IsAligned(&mData[3], 16)),
                  "One or more registers of Mat4SIMD are not 16 byte aligned");
}



inline std::ostream& operator<<(std::ostream& os, const Mat4SIMD& mat)
{
    for (U32 i = 0; i < 4; ++i)
        os << "| " << mat(i, 0) << " " << mat(i, 1) << " " << mat(i, 2) << " " << mat(i, 3) << " |" << std::endl;
    return os;
}



} // namespace GDL
