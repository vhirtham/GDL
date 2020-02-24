#pragma once

#include "test/unit/base/simd/transpose_test_template_register.h"

#include "gdl/base/approx.h"
#include "gdl/base/string.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/intrinsics.h"


using namespace GDL::simd;


// forward declarations -----------------------------------------------------------------------------------------------

namespace GDL::simd
{


template <U32, U32, bool, bool>
void Transpose1x1(__m128d, __m128d&);
template <U32, U32, bool, bool>
void Transpose1x2(__m128d, __m128d, __m128d&);
template <U32, U32, bool, bool>
void Transpose2x1(__m128d, __m128d&, __m128d&);
template <U32, U32, bool, bool>
void Transpose2x2(__m128d, __m128d, __m128d&, __m128d&);

template <U32, U32, bool, bool>
void Transpose1x1(__m128, __m128&);
template <U32, U32, bool, bool>
void Transpose1x2(__m128, __m128, __m128&);
template <U32, U32, bool, bool>
void Transpose1x3(__m128, __m128, __m128, __m128&);
template <U32, U32, bool, bool>
void Transpose1x4(__m128, __m128, __m128, __m128, __m128&);
template <U32, U32, bool, bool>
void Transpose2x1(__m128, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose2x2(__m128, __m128, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose2x3(__m128, __m128, __m128, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose2x4(__m128, __m128, __m128, __m128, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose3x1(__m128, __m128&, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose3x2(__m128, __m128, __m128&, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose3x3(__m128, __m128, __m128, __m128&, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose3x4(__m128, __m128, __m128, __m128, __m128&, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose4x1(__m128, __m128&, __m128&, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose4x2(__m128, __m128, __m128&, __m128&, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose4x3(__m128, __m128, __m128, __m128&, __m128&, __m128&, __m128&);
template <U32, U32, bool, bool>
void Transpose4x4(__m128, __m128, __m128, __m128, __m128&, __m128&, __m128&, __m128&);

#ifdef __AVX2__

template <U32, U32, bool, bool>
void Transpose1x1(__m256d, __m256d&);
template <U32, U32, bool, bool>
void Transpose1x2(__m256d, __m256d, __m256d&);
template <U32, U32, bool, bool>
void Transpose1x3(__m256d, __m256d, __m256d, __m256d&);
template <U32, U32, bool, bool>
void Transpose1x4(__m256d, __m256d, __m256d, __m256d, __m256d&);
template <U32, U32, bool, bool>
void Transpose2x1(__m256d, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose2x2(__m256d, __m256d, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose2x3(__m256d, __m256d, __m256d, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose2x4(__m256d, __m256d, __m256d, __m256d, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose3x1(__m256d, __m256d&, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose3x2(__m256d, __m256d, __m256d&, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose3x3(__m256d, __m256d, __m256d, __m256d&, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose3x4(__m256d, __m256d, __m256d, __m256d, __m256d&, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose4x1(__m256d, __m256d&, __m256d&, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose4x2(__m256d, __m256d, __m256d&, __m256d&, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose4x3(__m256d, __m256d, __m256d, __m256d&, __m256d&, __m256d&, __m256d&);
template <U32, U32, bool, bool>
void Transpose4x4(__m256d, __m256d, __m256d, __m256d, __m256d&, __m256d&, __m256d&, __m256d&);


template <U32, U32, bool, bool>
void Transpose1x1(__m256, __m256&);
template <U32, U32, bool, bool>
void Transpose1x2(__m256, __m256, __m256&);
template <U32, U32, bool, bool>
void Transpose1x3(__m256, __m256, __m256, __m256&);
template <U32, U32, bool, bool>
void Transpose1x4(__m256, __m256, __m256, __m256, __m256&);
template <U32, U32, bool, bool>
void Transpose1x5(__m256, __m256, __m256, __m256, __m256, __m256&);
template <U32, U32, bool, bool>
void Transpose1x6(__m256, __m256, __m256, __m256, __m256, __m256, __m256&);
template <U32, U32, bool, bool>
void Transpose1x7(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&);
template <U32, U32, bool, bool>
void Transpose1x8(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&);
template <U32, U32, bool, bool>
void Transpose2x1(__m256, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose2x2(__m256, __m256, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose2x3(__m256, __m256, __m256, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose2x4(__m256, __m256, __m256, __m256, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose2x5(__m256, __m256, __m256, __m256, __m256, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose2x6(__m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose2x7(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose2x8(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose3x1(__m256, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose3x2(__m256, __m256, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose3x3(__m256, __m256, __m256, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose3x4(__m256, __m256, __m256, __m256, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose3x5(__m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose3x6(__m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose3x7(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose3x8(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose4x1(__m256, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose4x2(__m256, __m256, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose4x3(__m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose4x4(__m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose4x5(__m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose4x6(__m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose4x7(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose4x8(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose5x1(__m256, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose5x2(__m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose5x3(__m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose5x4(__m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose5x5(__m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose5x6(__m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose5x7(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose5x8(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&,
                  __m256&);
template <U32, U32, bool, bool>
void Transpose6x1(__m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose6x2(__m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose6x3(__m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose6x4(__m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose6x5(__m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose6x6(__m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose6x7(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&,
                  __m256&);
template <U32, U32, bool, bool>
void Transpose6x8(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&,
                  __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose7x1(__m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose7x2(__m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose7x3(__m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose7x4(__m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose7x5(__m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&,
                  __m256&);
template <U32, U32, bool, bool>
void Transpose7x6(__m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&,
                  __m256&);
template <U32, U32, bool, bool>
void Transpose7x7(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&,
                  __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose7x8(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&,
                  __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose8x1(__m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose8x2(__m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose8x3(__m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose8x4(__m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&,
                  __m256&);
template <U32, U32, bool, bool>
void Transpose8x5(__m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&,
                  __m256&);
template <U32, U32, bool, bool>
void Transpose8x6(__m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&, __m256&,
                  __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose8x7(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&, __m256&,
                  __m256&, __m256&, __m256&);
template <U32, U32, bool, bool>
void Transpose8x8(__m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256, __m256&, __m256&, __m256&, __m256&,
                  __m256&, __m256&, __m256&, __m256&);
#endif // __AVX2__

} // namespace GDL::simd



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols>
void TestTranspose()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    std::array<_registerType, _cols> in = {{_mm_setzero<_registerType>()}};
    for (U32 i = 0; i < _cols; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            SetValue(in[i], j, static_cast<Type>(j * _cols + i + 1));

    TestTransposeSelectFirstRowIn<_registerType, _rows, _cols>(in);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn>
void TestTransposeSelectFirstRowIn(std::array<_registerType, _cols>& in)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    TestTransposeSelectFirstRowOut<_registerType, _rows, _cols, _firstRowIn>(in);

    if constexpr (_firstRowIn + _rows + 1 <= numRegVals)
        TestTransposeSelectFirstRowIn<_registerType, _rows, _cols, _firstRowIn + 1>(in);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut>
void TestTransposeSelectFirstRowOut(std::array<_registerType, _cols>& in)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    TestTransposeTestAllOutputs<_registerType, _rows, _cols, _firstRowIn, _firstRowOut>(in);

    if constexpr (_firstRowOut + _cols + 1 <= numRegVals)
        TestTransposeSelectFirstRowOut<_registerType, _rows, _cols, _firstRowIn, _firstRowOut + 1>(in);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut>
void TestTransposeTestAllOutputs(std::array<_registerType, _cols>& in)
{
    TestTransposeTestCase<_registerType, _rows, _cols, _firstRowIn, _firstRowOut, false, false>(in);
    TestTransposeTestCase<_registerType, _rows, _cols, _firstRowIn, _firstRowOut, true, false>(in);
    TestTransposeTestCase<_registerType, _rows, _cols, _firstRowIn, _firstRowOut, true, true>(in);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused,
          bool _unusedSetZero>
void TestTransposeTestCase(std::array<_registerType, _cols>& in)
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    std::array<_registerType, _rows> ref = {{_mm_setzero<_registerType>()}};
    for (U32 i = 0; i < _rows; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            SetValue(ref[i], j, static_cast<Type>(1. / (j * _rows + i + 10.)));

    std::array<_registerType, _rows> out = ref;

    TransposeData<_registerType, _rows, _cols, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in, out);
    CheckResults<_registerType, _rows, _cols, _firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in, out,
                                                                                                           ref);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused,
          bool _unusedSetZero>
void TransposeData(std::array<_registerType, _cols>& in, std::array<_registerType, _rows>& out)
{
    if constexpr (_rows == 1)
    {
        if constexpr (_cols == 1)
            Transpose1x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], out[0]);
        else if constexpr (_cols == 2)
            Transpose1x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], out[0]);
        else if constexpr (_cols == 3)
            Transpose1x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], out[0]);
        else if constexpr (_cols == 4)
            Transpose1x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3],
                                                                                      out[0]);
        else if constexpr (_cols == 5)
            Transpose1x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      out[0]);
        else if constexpr (_cols == 6)
            Transpose1x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      in[5], out[0]);
        else if constexpr (_cols == 7)
            Transpose1x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      in[5], in[6], out[0]);
        else if constexpr (_cols == 8)
            Transpose1x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      in[5], in[6], in[7], out[0]);
    }
    else if constexpr (_rows == 2)
    {
        if constexpr (_cols == 1)
            Transpose2x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], out[0], out[1]);
        else if constexpr (_cols == 2)
            Transpose2x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], out[0], out[1]);
        else if constexpr (_cols == 3)
            Transpose2x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], out[0],
                                                                                      out[1]);
        else if constexpr (_cols == 4)
            Transpose2x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3],
                                                                                      out[0], out[1]);
        else if constexpr (_cols == 5)
            Transpose2x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      out[0], out[1]);
        else if constexpr (_cols == 6)
            Transpose2x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      in[5], out[0], out[1]);
        else if constexpr (_cols == 7)
            Transpose2x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      in[5], in[6], out[0], out[1]);
        else if constexpr (_cols == 8)
            Transpose2x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], out[0], out[1]);
    }
    else if constexpr (_rows == 3)
    {
        if constexpr (_cols == 1)
            Transpose3x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], out[0], out[1], out[2]);
        else if constexpr (_cols == 2)
            Transpose3x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], out[0], out[1],
                                                                                      out[2]);
        else if constexpr (_cols == 3)
            Transpose3x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], out[0],
                                                                                      out[1], out[2]);
        else if constexpr (_cols == 4)
            Transpose3x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3],
                                                                                      out[0], out[1], out[2]);
        else if constexpr (_cols == 5)
            Transpose3x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      out[0], out[1], out[2]);
        else if constexpr (_cols == 6)
            Transpose3x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      in[5], out[0], out[1], out[2]);
        else if constexpr (_cols == 7)
            Transpose3x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], out[0], out[1], out[2]);
        else if constexpr (_cols == 8)
            Transpose3x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], out[0], out[1], out[2]);
    }
    else if constexpr (_rows == 4)
    {
        if constexpr (_cols == 1)
            Transpose4x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], out[0], out[1], out[2],
                                                                                      out[3]);
        else if constexpr (_cols == 2)
            Transpose4x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], out[0], out[1],
                                                                                      out[2], out[3]);
        else if constexpr (_cols == 3)
            Transpose4x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], out[0],
                                                                                      out[1], out[2], out[3]);
        else if constexpr (_cols == 4)
            Transpose4x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3],
                                                                                      out[0], out[1], out[2], out[3]);
        else if constexpr (_cols == 5)
            Transpose4x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], in[3], in[4],
                                                                                      out[0], out[1], out[2], out[3]);
        else if constexpr (_cols == 6)
            Transpose4x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], out[0], out[1], out[2], out[3]);
        else if constexpr (_cols == 7)
            Transpose4x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], out[0], out[1], out[2], out[3]);
        else if constexpr (_cols == 8)
            Transpose4x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], out[0], out[1], out[2], out[3]);
    }
    else if constexpr (_rows == 5)
    {
        if constexpr (_cols == 1)
            Transpose5x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], out[0], out[1], out[2],
                                                                                      out[3], out[4]);
        else if constexpr (_cols == 2)
            Transpose5x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], out[0], out[1],
                                                                                      out[2], out[3], out[4]);
        else if constexpr (_cols == 3)
            Transpose5x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], in[2], out[0],
                                                                                      out[1], out[2], out[3], out[4]);
        else if constexpr (_cols == 4)
            Transpose5x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], out[0], out[1], out[2], out[3], out[4]);
        else if constexpr (_cols == 5)
            Transpose5x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], out[0], out[1], out[2], out[3], out[4]);
        else if constexpr (_cols == 6)
            Transpose5x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], out[0], out[1], out[2], out[3], out[4]);
        else if constexpr (_cols == 7)
            Transpose5x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], out[0], out[1], out[2], out[3], out[4]);
        else if constexpr (_cols == 8)
            Transpose5x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], out[0], out[1], out[2], out[3], out[4]);
    }
    else if constexpr (_rows == 6)
    {
        if constexpr (_cols == 1)
            Transpose6x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], out[0], out[1], out[2],
                                                                                      out[3], out[4], out[5]);
        else if constexpr (_cols == 2)
            Transpose6x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], in[1], out[0], out[1],
                                                                                      out[2], out[3], out[4], out[5]);
        else if constexpr (_cols == 3)
            Transpose6x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], out[0], out[1], out[2], out[3], out[4], out[5]);
        else if constexpr (_cols == 4)
            Transpose6x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], out[0], out[1], out[2], out[3], out[4], out[5]);
        else if constexpr (_cols == 5)
            Transpose6x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], out[0], out[1], out[2], out[3], out[4], out[5]);
        else if constexpr (_cols == 6)
            Transpose6x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], out[0], out[1], out[2], out[3], out[4], out[5]);
        else if constexpr (_cols == 7)
            Transpose6x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], out[0], out[1], out[2], out[3], out[4], out[5]);
        else if constexpr (_cols == 8)
            Transpose6x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], out[0], out[1], out[2], out[3], out[4],
                    out[5]);
    }
    else if constexpr (_rows == 7)
    {
        if constexpr (_cols == 1)
            Transpose7x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(in[0], out[0], out[1], out[2],
                                                                                      out[3], out[4], out[5], out[6]);
        else if constexpr (_cols == 2)
            Transpose7x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], out[0], out[1], out[2], out[3], out[4], out[5], out[6]);
        else if constexpr (_cols == 3)
            Transpose7x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], out[0], out[1], out[2], out[3], out[4], out[5], out[6]);
        else if constexpr (_cols == 4)
            Transpose7x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], out[0], out[1], out[2], out[3], out[4], out[5], out[6]);
        else if constexpr (_cols == 5)
            Transpose7x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], out[0], out[1], out[2], out[3], out[4], out[5], out[6]);
        else if constexpr (_cols == 6)
            Transpose7x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], out[0], out[1], out[2], out[3], out[4], out[5], out[6]);
        else if constexpr (_cols == 7)
            Transpose7x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], out[0], out[1], out[2], out[3], out[4], out[5],
                    out[6]);
        else if constexpr (_cols == 8)
            Transpose7x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], out[0], out[1], out[2], out[3], out[4],
                    out[5], out[6]);
    }
    else if constexpr (_rows == 8)
    {
        if constexpr (_cols == 1)
            Transpose8x1<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
        else if constexpr (_cols == 2)
            Transpose8x2<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
        else if constexpr (_cols == 3)
            Transpose8x3<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
        else if constexpr (_cols == 4)
            Transpose8x4<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
        else if constexpr (_cols == 5)
            Transpose8x5<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
        else if constexpr (_cols == 6)
            Transpose8x6<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], out[0], out[1], out[2], out[3], out[4], out[5], out[6],
                    out[7]);
        else if constexpr (_cols == 7)
            Transpose8x7<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], out[0], out[1], out[2], out[3], out[4], out[5],
                    out[6], out[7]);
        else if constexpr (_cols == 8)
            Transpose8x8<_firstRowIn, _firstRowOut, _overwriteUnused, _unusedSetZero>(
                    in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], out[0], out[1], out[2], out[3], out[4],
                    out[5], out[6], out[7]);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, bool _overwriteUnused,
          bool _unusedSetZero>
void CheckResults(const std::array<_registerType, _cols>& in, const std::array<_registerType, _rows>& out,
                  const std::array<_registerType, _rows>& ref)
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;


    // Generate test setup sting which is part of the error message if a test fails
    std::string setupString = MakeString<std::string>(
            "\nSetup --- rows:", std::to_string(_rows), ", cols:", std::to_string(_cols),
            ", first row in:", std::to_string(_firstRowIn), ", first row out:", std::to_string(_firstRowOut),
            ", overwrite:", std::to_string(_overwriteUnused), ", setzero:", std::to_string(_unusedSetZero),
            "\nError --- ");


    for (U32 i = 0; i < _rows; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            if (j >= _firstRowOut && j <= _firstRowOut + _cols - 1)
            {
                BOOST_CHECK_MESSAGE(GetValue(out[i], j) == Approx(GetValue(in[(j - _firstRowOut)], i + _firstRowIn)),
                                    setupString + "Transposed matrix is incorrect.\n");
            }
            else
            {
                if constexpr (_overwriteUnused)
                {
                    if constexpr (_unusedSetZero)
                        BOOST_CHECK_MESSAGE(
                                GetValue(out[i], j) == ApproxZero<Type>(),
                                setupString + "Unused register value is not set to zero when option is turned on.\n");
                }
                else
                    BOOST_CHECK_MESSAGE(
                            GetValue(out[i], j) == Approx(GetValue(ref[i], j)),
                            setupString + "Unused register value is overwritten even though option is set to false.\n");
            }
}
