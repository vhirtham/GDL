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
          U32 _colStartOut, bool _overwriteUnused, bool _unusedSetZero, std::size_t _arrSizeIn, std::size_t _arrSizeOut,
          typename _registerType>
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
        std::array<_registerType, _rows + (_rows - 1) * (_colStrideOut - 1)> ref2 = {{_mm_setzero<_registerType>()}};
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

    std::array<_registerType, _arrSizeOut> ref = {{_mm_setzero<_registerType>()}};
    for (U32 i = 0; i < _arrSizeOut; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            SetValue(ref[i], j, static_cast<Type>(1. / (j * _arrSizeOut + i + 10.)));

    //    std::cout << _arrSizeIn - 1 << "/" << _arrSizeOut - 1 << "/" << _firstRowIn << "/" << _colStartIn << "/"
    //              << _firstRowOut << "/" << _colStartOut << std::endl;


    TestTransposeTestcase<_rows, _cols, _colStrideIn, _colStrideOut, _firstRowIn, _colStartIn, _firstRowOut,
                          _colStartOut, true, true>(in, ref);
    TestTransposeTestcase<_rows, _cols, _colStrideIn, _colStrideOut, _firstRowIn, _colStartIn, _firstRowOut,
                          _colStartOut, true, false>(in, ref);
    TestTransposeTestcase<_rows, _cols, _colStrideIn, _colStrideOut, _firstRowIn, _colStartIn, _firstRowOut,
                          _colStartOut, false, false>(in, ref);



    if constexpr (_stepColOut > 0 && _colStartOut + _colStrideOut * (_rows - 1) + 1 + _stepColOut <= _arrSizeOut)
        TestTransposeTestcasesColStartOut<_rows, _cols, _colStrideIn, _colStrideOut, _stepColOut, _arrSizeIn,
                                          _arrSizeOut, _firstRowIn, _colStartIn, _firstRowOut,
                                          _colStartOut + _stepColOut>(in);
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

    if constexpr (_stepRowOut > 0 && _firstRowOut + _cols + _stepRowOut <= numRegVals)
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

    std::array<_registerType, _arrSizeIn> in = {{_mm_setzero<_registerType>()}};
    for (U32 i = 0; i < _arrSizeIn; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
            SetValue(in[i], j, static_cast<Type>(j * _arrSizeIn + i + 1));

    TestTransposeStepArrSizeOut<_rows, _cols, _colStrideIn, _colStrideOut, _stepRowIn, _stepRowOut, _stepColIn,
                                _stepColOut, _stepArrSizeOut, _arrSizeIn, _arrSizeOut>(in);

    if constexpr (_stepArrSizeIn > 0 && _arrSizeIn + _stepArrSizeIn <= numRegVals)
        TestTranspose<_registerType, _rows, _cols, _stepRowIn, _stepRowOut, _stepColIn, _stepColOut, _colStrideIn,
                      _colStrideOut, _stepArrSizeIn, _stepArrSizeOut, _arrSizeIn + _stepArrSizeIn, _arrSizeOut>();
}
