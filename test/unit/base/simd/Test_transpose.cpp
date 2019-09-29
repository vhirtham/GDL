#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/transpose.h"


#include <array>


using namespace GDL;
using namespace GDL::simd;



// Test Template ------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _stepSize, std::size_t _arrSizeIn, std::size_t _arrSizeOut, U32 _rowStart,
          U32 _colStartIn, U32 _colStartOut = 0, typename _registerType>
inline void TestTransposeTestcase(const std::array<_registerType, _arrSizeIn>& in)
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    std::array<_registerType, _arrSizeOut> out, ref;
    for (U32 i = 0; i < _arrSizeOut; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            SetValue(out[i], j, static_cast<Type>(1. / (j * _arrSizeOut + i + 10.)));

    for (U32 i = 0; i < _arrSizeOut; ++i)
        ref[i] = out[i];

    Transpose<_rows, _cols, _rowStart, _colStartIn, _colStartOut>(in, out);

    for (U32 i = 0; i < _arrSizeOut; ++i)
        if (i >= _colStartOut && i <= _colStartOut + _rows - 1)
            for (U32 j = 0; j < numRegVals; ++j)
                if (j >= _rowStart && j <= _rowStart + _cols - 1)
                    BOOST_CHECK(GetValue(out[i], j) ==
                                Approx(GetValue(in[j - _rowStart + _colStartIn], i - _colStartOut + _rowStart)));
                else
                    BOOST_CHECK(GetValue(out[i], j) == ApproxZero<Type>());
        else
            for (U32 j = 0; j < numRegVals; ++j)
                BOOST_CHECK(GetValue(out[i], j) == Approx(GetValue(ref[i], j)));

    if constexpr (_colStartOut == 0)
    {
        std::array<_registerType, _rows> out2 = Transpose<_rows, _cols, _rowStart, _colStartIn>(in);
        for (U32 i = 0; i < _rows; ++i)
            for (U32 j = 0; j < numRegVals; ++j)
                if (j >= _rowStart && j <= _rowStart + _cols - 1)
                    BOOST_CHECK(GetValue(out2[i], j) ==
                                Approx(GetValue(in[j - _rowStart + _colStartIn], i + _rowStart)));
                else
                    BOOST_CHECK(GetValue(out2[i], j) == ApproxZero<Type>());
    }

    if constexpr (_colStartOut + _cols + _stepSize <= _arrSizeOut)
        TestTransposeTestcase<_rows, _cols, _stepSize, _arrSizeIn, _arrSizeOut, _rowStart, _colStartIn,
                              _colStartOut + _stepSize>(in);
}



template <U32 _rows, U32 _cols, U32 _stepSize, std::size_t _arrSizeIn, std::size_t _arrSizeOut, U32 _rowStart,
          U32 _colStartIn = 0, typename _registerType>
inline void TestTransposeTestcasesColStartIn(const std::array<_registerType, _arrSizeIn>& in)
{
    TestTransposeTestcase<_rows, _cols, _stepSize, _arrSizeIn, _arrSizeOut, _rowStart, _colStartIn>(in);

    if constexpr (_colStartIn + _cols + _stepSize <= _arrSizeIn)
        TestTransposeTestcasesColStartIn<_rows, _cols, _stepSize, _arrSizeIn, _arrSizeOut, _rowStart,
                                         _colStartIn + _stepSize>(in);
}



template <U32 _rows, U32 _cols, U32 _stepSize, std::size_t _arrSizeIn, std::size_t _arrSizeOut, U32 _rowStart = 0,
          typename _registerType>
inline void TestTransposeTestcases(const std::array<_registerType, _arrSizeIn>& in)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;


    TestTransposeTestcasesColStartIn<_rows, _cols, _stepSize, _arrSizeIn, _arrSizeOut, _rowStart>(in);

    if constexpr (_rowStart + _rows + _stepSize <= numRegVals)
        TestTransposeTestcases<_rows, _cols, _stepSize, _arrSizeIn, _arrSizeOut, _rowStart + _stepSize>(in);
}



template <typename _registerType, U32 _rows, U32 _cols, U32 _stepSize = 1, std::size_t _arrSizeIn = _cols,
          std::size_t _arrSizeOut = _rows>
void TestTranspose()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    std::array<_registerType, _arrSizeIn> in;
    for (U32 i = 0; i < _arrSizeIn; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            SetValue(in[i], j, static_cast<Type>(j * _arrSizeIn + i + 1));

    TestTransposeTestcases<_rows, _cols, _stepSize, _arrSizeIn, _arrSizeOut>(in);

    if constexpr (_arrSizeIn + _stepSize <= numRegVals)
        TestTranspose<_registerType, _rows, _cols, _stepSize, _arrSizeIn + _stepSize, _arrSizeOut>();
    else if constexpr (_arrSizeOut + _stepSize <= numRegVals)
        TestTranspose<_registerType, _rows, _cols, _stepSize, _cols, _arrSizeOut + _stepSize>();
}



// Transpose 1x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Transpose1x1_128d)
{
    TestTranspose<__m128d, 1, 1>();
}



BOOST_AUTO_TEST_CASE(Transpose1x1_128)
{
    TestTranspose<__m128, 1, 1>();
}



// Transpose 2x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Transpose2x2_128d)
{
    alignas(alignmentBytes<__m128d>) std::array<__m128d, 2> a, b, c, d, expected;
    a[0] = _mm_setr_pd(1, 2);
    a[1] = _mm_setr_pd(3, 4);

    expected[0] = _mm_setr_pd(1, 3);
    expected[1] = _mm_setr_pd(2, 4);


    Transpose2x2(a[0], a[1], b[0], b[1]);
    Transpose(a, c);
    d = Transpose(a);

    for (U32 i = 0; i < 2; ++i)
        for (U32 j = 0; j < 2; ++j)
        {
            BOOST_CHECK(GetValue(b[i], j) == Approx(GetValue(expected[i], j)));
            BOOST_CHECK(GetValue(c[i], j) == Approx(GetValue(expected[i], j)));
            BOOST_CHECK(GetValue(d[i], j) == Approx(GetValue(expected[i], j)));
        }
}



// Transpose 2x1 ------------------------------------------------------------------------------------------------------


BOOST_AUTO_TEST_CASE(Transpose2x1_128d)
{
    alignas(alignmentBytes<__m128d>) std::array<__m128d, 2> a, b0, b1, c0, c1, c0_1, d0, d1, d0_1, exp0, exp1;
    alignas(alignmentBytes<__m128d>) std::array<__m128d, 1> a_1;

    a[0] = _mm_setr_pd(1, 2);
    a[1] = _mm_setr_pd(3, 4);
    a_1[0] = a[0];

    exp0[0] = _mm_setr_pd(1, 0);
    exp0[1] = _mm_setr_pd(2, 0);
    exp1[0] = _mm_setr_pd(3, 0);
    exp1[1] = _mm_setr_pd(4, 0);


    Transpose2x1(a[0], b0[0], b0[1]);
    Transpose2x1(a[1], b1[0], b1[1]);

    Transpose<2, 1>(a, c0);
    Transpose<2, 1, 0, 1>(a, c1);
    Transpose<2, 1>(a_1, c0_1);

    d0 = Transpose<2, 1>(a);
    d1 = Transpose<2, 1, 0, 1>(a);
    d0_1 = Transpose<2, 1>(a_1);

    for (U32 i = 0; i < 2; ++i)
        for (U32 j = 0; j < 2; ++j)
        {
            BOOST_CHECK(GetValue(b0[i], j) == Approx(GetValue(exp0[i], j)));
            BOOST_CHECK(GetValue(b1[i], j) == Approx(GetValue(exp1[i], j)));

            BOOST_CHECK(GetValue(c0[i], j) == Approx(GetValue(exp0[i], j)));
            BOOST_CHECK(GetValue(c1[i], j) == Approx(GetValue(exp1[i], j)));
            BOOST_CHECK(GetValue(c0_1[i], j) == Approx(GetValue(exp0[i], j)));

            BOOST_CHECK(GetValue(d0[i], j) == Approx(GetValue(exp0[i], j)));
            BOOST_CHECK(GetValue(d1[i], j) == Approx(GetValue(exp1[i], j)));
            BOOST_CHECK(GetValue(d0_1[i], j) == Approx(GetValue(exp0[i], j)));
        }
}



// Transpose 1x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Transpose1x2_128d)
{
    alignas(alignmentBytes<__m128d>) std::array<__m128d, 2> a, c0, c1;
    alignas(alignmentBytes<__m128d>) std::array<__m128d, 1> c0_1, d0_1;
    __m128d b, exp;
    a[0] = _mm_setr_pd(1, 2);
    a[1] = _mm_setr_pd(3, 4);

    for (U32 i = 0; i < 2; ++i)
    {
        c0[i] = a[i];
        c1[i] = a[i];
    }
    exp = _mm_setr_pd(1, 3);

    Transpose1x2(a[0], a[1], b);
    Transpose<1, 2>(a, c0);
    Transpose<1, 2, 0, 0, 1>(a, c1);
    Transpose<1, 2>(a, c0_1);
    d0_1 = Transpose<1, 2>(a);

    for (U32 i = 0; i < 2; ++i)
    {
        BOOST_CHECK(GetValue(b, i) == Approx(GetValue(exp, i)));
        BOOST_CHECK(GetValue(c0[0], i) == Approx(GetValue(exp, i)));
        BOOST_CHECK(GetValue(c0[1], i) == Approx(GetValue(a[1], i)));
        BOOST_CHECK(GetValue(c1[0], i) == Approx(GetValue(a[0], i)));
        BOOST_CHECK(GetValue(c1[1], i) == Approx(GetValue(exp, i)));
        BOOST_CHECK(GetValue(c0_1[0], i) == Approx(GetValue(exp, i)));
        BOOST_CHECK(GetValue(d0_1[0], i) == Approx(GetValue(exp, i)));
    }
}



// Transpose 4x4 ------------------------------------------------------------------------------------------------------


template <typename _registerType>
void TestTranspose4x4()
{
    alignas(alignmentBytes<_registerType>) std::array<_registerType, 4> a, b, expected;

    a[0] = _mm_setr<_registerType>(1, 2, 3, 4);
    a[1] = _mm_setr<_registerType>(5, 6, 7, 8);
    a[2] = _mm_setr<_registerType>(9, 10, 11, 12);
    a[3] = _mm_setr<_registerType>(13, 14, 15, 16);

    expected[0] = _mm_setr<_registerType>(1, 5, 9, 13);
    expected[1] = _mm_setr<_registerType>(2, 6, 10, 14);
    expected[2] = _mm_setr<_registerType>(3, 7, 11, 15);
    expected[3] = _mm_setr<_registerType>(4, 8, 12, 16);


    Transpose4x4(a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3]);

    for (U32 i = 0; i < 4; ++i)
        for (U32 j = 0; j < 4; ++j)
            BOOST_CHECK(GetValue(b[i], j) == Approx(GetValue(expected[i], j)));
}



BOOST_AUTO_TEST_CASE(Transpose4x4_128)
{
    TestTranspose4x4<__m128>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(Transpose4x4_256d)
{
    TestTranspose4x4<__m256d>();
}

#endif // __AVX2__



// Transpose 8x8 ------------------------------------------------------------------------------------------------------

template <typename _registerType>
void TestTranspose8x8()
{
    alignas(alignmentBytes<_registerType>) std::array<_registerType, 8> a, b, expected;
    a[0] = _mm_setr<_registerType>(1, 2, 3, 4, 5, 6, 7, 8);
    a[1] = _mm_setr<_registerType>(9, 10, 11, 12, 13, 14, 15, 16);
    a[2] = _mm_setr<_registerType>(17, 18, 19, 20, 21, 22, 23, 24);
    a[3] = _mm_setr<_registerType>(25, 26, 27, 28, 29, 30, 31, 32);
    a[4] = _mm_setr<_registerType>(33, 34, 35, 36, 37, 38, 39, 40);
    a[5] = _mm_setr<_registerType>(41, 42, 43, 44, 45, 46, 47, 48);
    a[6] = _mm_setr<_registerType>(49, 50, 51, 52, 53, 54, 55, 56);
    a[7] = _mm_setr<_registerType>(57, 58, 59, 60, 61, 62, 63, 64);


    expected[0] = _mm_setr<_registerType>(1, 9, 17, 25, 33, 41, 49, 57);
    expected[1] = _mm_setr<_registerType>(2, 10, 18, 26, 34, 42, 50, 58);
    expected[2] = _mm_setr<_registerType>(3, 11, 19, 27, 35, 43, 51, 59);
    expected[3] = _mm_setr<_registerType>(4, 12, 20, 28, 36, 44, 52, 60);
    expected[4] = _mm_setr<_registerType>(5, 13, 21, 29, 37, 45, 53, 61);
    expected[5] = _mm_setr<_registerType>(6, 14, 22, 30, 38, 46, 54, 62);
    expected[6] = _mm_setr<_registerType>(7, 15, 23, 31, 39, 47, 55, 63);
    expected[7] = _mm_setr<_registerType>(8, 16, 24, 32, 40, 48, 56, 64);


    Transpose8x8(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);

    for (U32 i = 0; i < 8; ++i)
        for (U32 j = 0; j < 8; ++j)
            BOOST_CHECK(GetValue(b[i], j) == Approx(GetValue(expected[i], j)));
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(Transpose8x8_256)
{
    TestTranspose8x8<__m256>();
}



#endif // __AVX2__
