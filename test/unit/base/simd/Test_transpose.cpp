#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/string.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/transpose.h"


#include <array>

using namespace GDL;
using namespace GDL::simd;

#include <iostream>

// Test Template ------------------------------------------------------------------------------------------------------


template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _colStartIn, U32 _colStrideIn, U32 _firstRowOut, U32 _colStartOut,
          U32 _colStrideOut, bool _overwriteUnused, bool _unusedSetZero, bool _checkUnusedCols = true,
          std::size_t _arrSizeIn, std::size_t _arrSizeOut, typename _registerType>
inline void TestTransposeCheckResults(const std::array<_registerType, _arrSizeIn>& in,
                                      const std::array<_registerType, _arrSizeOut>& out,
                                      const std::array<_registerType, _arrSizeOut>& ref)
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;


    // Generate test setup sting which is part of the error message if a test fails
    std::string setupString =
            MakeString<std::string>("\nSetup --- r:", std::to_string(_rows), ", c:", std::to_string(_cols),
                                    ", fri:", std::to_string(_firstRowIn), ", fro:", std::to_string(_firstRowOut),
                                    ", fci:", std::to_string(_colStartIn), ", fco:", std::to_string(_colStartIn),
                                    ", csi:", std::to_string(_colStrideIn), ", cso:", std::to_string(_colStrideOut),
                                    ", overwrite:", std::to_string(_overwriteUnused),
                                    ", setzero:", std::to_string(_unusedSetZero), "\nError --- ");

    // std::cout << setupString << std::endl;

    for (U32 i = 0; i < _arrSizeOut; ++i)
        if (i >= _colStartOut && i <= _colStartOut + _rows * _colStrideOut - 1 &&
            (i - _colStartOut) % _colStrideOut == 0)
        {
            for (U32 j = 0; j < numRegVals; ++j)
                if (j >= _firstRowOut && j <= _firstRowOut + _cols - 1)
                {
                    BOOST_CHECK_MESSAGE(GetValue(out[i], j) ==
                                                Approx(GetValue(in[(j - _firstRowOut) * _colStrideIn + _colStartIn],
                                                                (i - _colStartOut) / _colStrideOut + _firstRowIn)),
                                        setupString + "Transposed matrix is incorrect.\n");
                }
                else
                {
                    if constexpr (_overwriteUnused)
                    {
                        if constexpr (_unusedSetZero)
                            BOOST_CHECK_MESSAGE(
                                    GetValue(out[i], j) == ApproxZero<Type>(),
                                    setupString +
                                            "Unused register value is not set to zero when option is turned on.\n");
                    }
                    else
                        BOOST_CHECK_MESSAGE(
                                GetValue(out[i], j) == Approx(GetValue(ref[i], j)),
                                setupString +
                                        "Unused register value is overwritten even though option is set to false.\n");
                }
        }
        else if (_checkUnusedCols)
            for (U32 j = 0; j < numRegVals; ++j)
                BOOST_CHECK_MESSAGE(GetValue(out[i], j) == Approx(GetValue(ref[i], j)),
                                    setupString + "Unused column register was modified.\n");
}



template <U32 _rows, U32 _cols, U32 _colStrideIn, U32 _colStrideOut, U32 _firstRowIn, U32 _colStartIn, U32 _firstRowOut,
          U32 _colStartOut, bool _overwriteUnused = true, bool _unusedSetZero = true, std::size_t _arrSizeIn,
          std::size_t _arrSizeOut, typename _registerType>
inline void TestTransposeTestcase(const std::array<_registerType, _arrSizeIn>& in,
                                  const std::array<_registerType, _arrSizeOut>& ref)
{
    std::array<_registerType, _arrSizeOut> out = ref;

    Transpose<_rows, _cols, _firstRowIn, _colStartIn, _overwriteUnused, _unusedSetZero, _colStrideIn, _firstRowOut,
              _colStartOut, _colStrideOut>(in, out);

    TestTransposeCheckResults<_rows, _cols, _firstRowIn, _colStartIn, _colStrideIn, _firstRowOut, _colStartOut,
                              _colStrideOut, _overwriteUnused, _unusedSetZero>(in, out, ref);


    if constexpr (_colStartOut == 0 && _overwriteUnused)
    {
        std::array<_registerType, _rows + (_rows - 1) * (_colStrideOut - 1)> ref2 = {0};
        std::array<_registerType, _rows + (_rows - 1) * (_colStrideOut - 1)> out2 =
                Transpose<_rows, _cols, _firstRowIn, _colStartIn, _firstRowOut, _unusedSetZero, _colStrideIn,
                          _colStrideOut>(in);
        TestTransposeCheckResults<_rows, _cols, _firstRowIn, _colStartIn, _colStrideIn, _firstRowOut, _colStartOut,

                                  _colStrideOut, _overwriteUnused, _unusedSetZero, false>(in, out2, ref2);
    }
}



template <U32 _rows, U32 _cols, U32 _colStrideIn, U32 _colStrideOut, U32 _stepColOut, std::size_t _arrSizeIn,
          std::size_t _arrSizeOut, U32 _firstRowIn, U32 _colStartIn, U32 _firstRowOut, U32 _colStartOut = 0,
          typename _registerType>
inline void TestTransposeTestcasesColStartOut(const std::array<_registerType, _arrSizeIn>& in)
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    std::array<_registerType, _arrSizeOut> ref;
    for (U32 i = 0; i < _arrSizeOut; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            SetValue(ref[i], j, static_cast<Type>(1. / (j * _arrSizeOut + i + 10.)));

    //    std::cout << _arrSizeIn - 1 << "/" << _arrSizeOut - 1 << "/" << _firstRowIn << "/" << _colStartIn << "/"
    //              << _firstRowIn << "/" << _colStartOut << std::endl;


    TestTransposeTestcase<_rows, _cols, _colStrideIn, _colStrideOut, _firstRowIn, _colStartIn, _firstRowOut,
                          _colStartOut>(in, ref);
    TestTransposeTestcase<_rows, _cols, _colStrideIn, _colStrideOut, _firstRowIn, _colStartIn, _firstRowOut,
                          _colStartOut, true, false>(in, ref);
    TestTransposeTestcase<_rows, _cols, _colStrideIn, _colStrideOut, _firstRowIn, _colStartIn, _firstRowOut,
                          _colStartOut, false, false>(in, ref);



    if constexpr (_stepColOut > 0 && _colStartOut + _colStrideOut * (_rows - 1) + 1 + _stepColOut <= _arrSizeOut)
        TestTransposeTestcasesColStartOut<_rows, _cols, _colStrideIn, _colStrideOut, _stepColOut, _arrSizeIn,
                                          _arrSizeOut, _firstRowIn, _colStartIn, _colStartOut + _stepColOut>(in);
}



template <U32 _rows, U32 _cols, U32 _colStrideIn, U32 _colStrideOut, U32 _stepColIn, U32 _stepColOut,
          std::size_t _arrSizeIn, std::size_t _arrSizeOut, U32 _firstRowIn, U32 _firstRowOut, U32 _colStartIn = 0,
          typename _registerType>
inline void TestTransposeTestcasesColStartIn(const std::array<_registerType, _arrSizeIn>& in)
{
    TestTransposeTestcasesColStartOut<_rows, _cols, _colStrideIn, _colStrideOut, _stepColOut, _arrSizeIn, _arrSizeOut,
                                      _firstRowIn, _colStartIn, _firstRowOut>(in);

    if constexpr (_stepColIn > 0 && _colStartIn + _colStrideIn * (_cols - 1) + 1 + _stepColIn <= _arrSizeIn)
        TestTransposeTestcasesColStartIn<_rows, _cols, _colStrideIn, _colStrideOut, _stepColIn, _stepColOut, _arrSizeIn,
                                         _arrSizeOut, _firstRowIn, _firstRowOut, _colStartIn + _stepColIn>(in);
}



template <U32 _rows, U32 _cols, U32 _colStrideIn, U32 _colStrideOut, U32 _stepRowOut, U32 _stepColIn, U32 _stepColOut,
          std::size_t _arrSizeIn, std::size_t _arrSizeOut, U32 _firstRowIn, U32 _firstRowOut = 0,
          typename _registerType>
inline void TestTransposeTestcasesRowStartOut(const std::array<_registerType, _arrSizeIn>& in)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;


    TestTransposeTestcasesColStartIn<_rows, _cols, _colStrideIn, _colStrideOut, _stepColIn, _stepColOut, _arrSizeIn,
                                     _arrSizeOut, _firstRowIn, _firstRowOut>(in);

    if constexpr (_stepRowOut > 0 && _firstRowOut + _cols + _stepRowOut <= numRegVals &&
                  _firstRowOut + _cols + _stepRowOut <= numRegVals)
        TestTransposeTestcasesRowStartOut<_rows, _cols, _colStrideIn, _colStrideOut, _stepRowOut, _stepColIn,
                                          _stepColOut, _arrSizeIn, _arrSizeOut, _firstRowIn,
                                          _firstRowOut + _stepRowOut>(in);
}



template <U32 _rows, U32 _cols, U32 _colStrideIn, U32 _colStrideOut, U32 _stepRowIn, U32 _stepRowOut, U32 _stepColIn,
          U32 _stepColOut, std::size_t _arrSizeIn, std::size_t _arrSizeOut, U32 _firstRowIn = 0, typename _registerType>
inline void TestTransposeTestcasesRowStartIn(const std::array<_registerType, _arrSizeIn>& in)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;


    TestTransposeTestcasesRowStartOut<_rows, _cols, _colStrideIn, _colStrideOut, _stepRowOut, _stepColIn, _stepColOut,
                                      _arrSizeIn, _arrSizeOut, _firstRowIn>(in);

    if constexpr (_stepRowIn > 0 && _firstRowIn + _rows + _stepRowIn <= numRegVals)
        TestTransposeTestcasesRowStartIn<_rows, _cols, _colStrideIn, _colStrideOut, _stepRowIn, _stepRowOut, _stepColIn,
                                         _stepColOut, _arrSizeIn, _arrSizeOut, _firstRowIn + _stepRowIn>(in);
}



template <U32 _rows, U32 _cols, U32 _colStrideIn, U32 _colStrideOut, U32 _stepRowIn, U32 _stepRowOut, U32 _stepColIn,
          U32 _stepColOut, U32 _stepArrSizeOut, std::size_t _arrSizeIn, std::size_t _arrSizeOut, typename _registerType>
void TestTransposeStepArrSizeOut(const std::array<_registerType, _arrSizeIn>& in)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    TestTransposeTestcasesRowStartIn<_rows, _cols, _colStrideIn, _colStrideOut, _stepRowIn, _stepRowOut, _stepColIn,
                                     _stepColOut, _arrSizeIn, _arrSizeOut>(in);

    if constexpr (_stepArrSizeOut > 0 && _arrSizeOut + _stepArrSizeOut <= numRegVals)
        TestTransposeStepArrSizeOut<_rows, _cols, _colStrideIn, _colStrideOut, _stepRowIn, _stepRowOut, _stepColIn,
                                    _stepColOut, _stepArrSizeOut, _arrSizeIn, _arrSizeOut + _stepArrSizeOut>(in);
}



template <typename _registerType, U32 _rows, U32 _cols, U32 _stepRowIn = 1, U32 _stepRowOut = 1, U32 _stepColIn = 0,
          U32 _stepColOut = 0, U32 _colStrideIn = 1, U32 _colStrideOut = 1, U32 _stepArrSizeIn = 0,
          U32 _stepArrSizeOut = 0, std::size_t _arrSizeIn = _cols, std::size_t _arrSizeOut = _rows>
void TestTranspose()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    std::array<_registerType, _arrSizeIn> in;
    for (U32 i = 0; i < _arrSizeIn; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            SetValue(in[i], j, static_cast<Type>(j * _arrSizeIn + i + 1));

    TestTransposeStepArrSizeOut<_rows, _cols, _colStrideIn, _colStrideOut, _stepRowIn, _stepRowOut, _stepColIn,
                                _stepColOut, _stepArrSizeOut, _arrSizeIn, _arrSizeOut>(in);

    if constexpr (_stepArrSizeIn > 0 && _arrSizeIn + _stepArrSizeIn <= numRegVals)
        TestTranspose<_registerType, _rows, _cols, _stepRowIn, _stepRowOut, _stepColIn, _stepColOut, _colStrideIn,
                      _colStrideOut, _stepArrSizeIn, _stepArrSizeOut, _arrSizeIn + _stepArrSizeIn, _arrSizeOut>();
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



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(Transpose1x1_256d)
{
    TestTranspose<__m256d, 1, 1>();
}



BOOST_AUTO_TEST_CASE(Transpose1x1_256)
{
    TestTranspose<__m256, 1, 1>();
}

#endif //__AVX2__



// Transpose 1x2 ------------------------------------------------------------------------------------------------------

// BOOST_AUTO_TEST_CASE(Transpose1x2_128d)
//{
//    TestTranspose<__m128d, 1, 2>();
//}



// BOOST_AUTO_TEST_CASE(Transpose1x2_128)
//{
//    TestTranspose<__m128, 1, 2>();
//}



//#ifdef __AVX2__

// BOOST_AUTO_TEST_CASE(Transpose1x2_256d)
//{
//    TestTranspose<__m256d, 1, 2>();
//}



// BOOST_AUTO_TEST_CASE(Transpose1x2_256)
//{
//    TestTranspose<__m256, 1, 2>();
//}

//#endif //__AVX2__



// Transpose 2x1 ------------------------------------------------------------------------------------------------------


// BOOST_AUTO_TEST_CASE(Transpose2x1_128d)
//{
//    TestTranspose<__m128d, 2, 1>();
//}



// BOOST_AUTO_TEST_CASE(Transpose2x1_128)
//{
//    TestTranspose<__m128, 2, 1>();
//}



//#ifdef __AVX2__

// BOOST_AUTO_TEST_CASE(Transpose2x1_256d)
//{
//    TestTranspose<__m256d, 2, 1>();
//}



// BOOST_AUTO_TEST_CASE(Transpose2x1_256)
//{
//    TestTranspose<__m256, 2, 1>();
//}

//#endif //__AVX2__



// Transpose 2x2 ------------------------------------------------------------------------------------------------------

// BOOST_AUTO_TEST_CASE(Transpose2x2_128d)
//{
//    TestTranspose<__m128d, 2, 2>();
//}



// BOOST_AUTO_TEST_CASE(Transpose2x2_128)
//{
//    TestTranspose<__m128, 2, 2>();
//}



//#ifdef __AVX2__

// BOOST_AUTO_TEST_CASE(Transpose2x2_256d)
//{
//    TestTranspose<__m256d, 2, 2>();
//}



// BOOST_AUTO_TEST_CASE(Transpose2x2_256)
//{
//    TestTranspose<__m256, 2, 2>();
//}

//#endif //__AVX2__



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

#endif //__AVX2__



// Array sizes --------------------------------------------------------------------------------------------------------

// BOOST_AUTO_TEST_CASE(ArraySizes_128d)
//{
//    TestTranspose<__m128d, 1, 1, 0, 0, 0, 1, 1, 1, 1>();
//}



// BOOST_AUTO_TEST_CASE(ArraySizes_128)
//{
//    TestTranspose<__m128, 1, 1, 0, 0, 0, 1, 1, 1, 1>();
//}



//#ifdef __AVX2__

// BOOST_AUTO_TEST_CASE(ArraySizes_256d)
//{
//    TestTranspose<__m256d, 1, 1, 0, 0, 0, 1, 1, 1, 1>();
//}



// BOOST_AUTO_TEST_CASE(ArraySizes_256)
//{
//    TestTranspose<__m256, 1, 1, 0, 0, 0, 1, 1, 1, 1>();
//}

//#endif //__AVX2__



// Column offsets -----------------------------------------------------------------------------------------------------

// BOOST_AUTO_TEST_CASE(ColumnOffset_128d)
//{
//    TestTranspose<__m128d, 1, 1, 0, 3, 3, 1, 1, 0, 0, 4, 4>();
//}



// BOOST_AUTO_TEST_CASE(ColumnOffset_128)
//{
//    TestTranspose<__m128, 1, 1, 0, 5, 5, 1, 1, 0, 0, 16, 16>();
//}



//#ifdef __AVX2__

// BOOST_AUTO_TEST_CASE(ColumnOffset_256d)
//{
//    TestTranspose<__m256d, 1, 1, 0, 5, 5, 1, 1, 0, 0, 16, 16>();
//}



// BOOST_AUTO_TEST_CASE(ColumnOffset_256)
//{
//    TestTranspose<__m256, 1, 1, 0, 9, 9, 1, 1, 0, 0, 64, 64>();
//}

//#endif //__AVX2__



// Column strides -----------------------------------------------------------------------------------------------------

// BOOST_AUTO_TEST_CASE(ColumnStrides_128d)
//{
//    TestTranspose<__m128d, 2, 2, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
//}



// BOOST_AUTO_TEST_CASE(ColumnStrides_128)
//{
//    TestTranspose<__m128, 2, 2, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
//}



//#ifdef __AVX2__

// BOOST_AUTO_TEST_CASE(ColumnStrides_256d)
//{
//    TestTranspose<__m256d, 2, 2, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
//}



// BOOST_AUTO_TEST_CASE(ColumnStrides_256)
//{
//    TestTranspose<__m256, 2, 2, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
//}

//#endif //__AVX2__
