#pragma once

#include "test/unit/base/simd/transposeTest.h"


#include "gdl/base/approx.h"
#include "gdl/base/string.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/transpose.h"



using namespace GDL::simd;



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _numOffsets>
inline void TestArrayOffset()
{
    constexpr UST input_array_size = _cols + _numOffsets;

    std::array<_registerType, input_array_size> input_array = GetDefaultInputArray<_registerType, input_array_size>();

    TestArrayOffsetIterateInputOffset<_rows, _cols, _numOffsets>(input_array);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _numOffsets, U32 _columnOffsetIn, typename _registerType, UST _inputArraySize>
inline void TestArrayOffsetIterateInputOffset(std::array<_registerType, _inputArraySize> input_array)
{
    TestArrayOffsetIterateOutputOffset<_rows, _cols, _numOffsets, _columnOffsetIn>(input_array);

    if constexpr (_columnOffsetIn < _numOffsets)
        TestArrayOffsetIterateInputOffset<_rows, _cols, _numOffsets, _columnOffsetIn + 1>(input_array);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _numOffsets, U32 _columnOffsetIn, U32 _columnOffsetOut, typename _registerType,
          UST _inputArraySize>
inline void TestArrayOffsetIterateOutputOffset(std::array<_registerType, _inputArraySize> input_array)
{

    TestArrayOffsetTestCase<_rows, _cols, _columnOffsetIn, _columnOffsetOut>(input_array);

    if constexpr (_columnOffsetOut < _numOffsets)
        TestArrayOffsetIterateOutputOffset<_rows, _cols, _numOffsets, _columnOffsetIn, _columnOffsetOut + 1>(
                input_array);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _columnOffsetIn, U32 _columnOffsetOut, typename _registerType, UST _inputArraySize>
inline void TestArrayOffsetTestCase(std::array<_registerType, _inputArraySize> input)
{
    constexpr UST output_array_size = _inputArraySize - _cols + _rows;

    std::array<_registerType, output_array_size> reference =
            GetDefaultReferenceArray<_registerType, output_array_size>();
    std::array<_registerType, output_array_size> output = reference;

    Transpose<_rows, _cols, 0, 0, _columnOffsetIn, _columnOffsetOut>(input, output);

    CheckResults<_rows, _cols, 0, 0, _columnOffsetIn, _columnOffsetOut, 1, 1, true, false>(input, output, reference);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, U32 _colStartIn, U32 _colStartOut, U32 _colStrideIn,
          U32 _colStrideOut, bool _overwriteUnused, bool _unusedSetZero, bool _checkUnusedCols, UST _arraySizeIn,
          UST _arraySizeOut, typename _registerType>
inline void CheckResults(const std::array<_registerType, _arraySizeIn>& in,
                         const std::array<_registerType, _arraySizeOut>& out,
                         const std::array<_registerType, _arraySizeOut>& ref)
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;


    // Generate test setup sting which is part of the error message if a test fails
    std::string setupString = MakeString<std::string>(
            "\nSetup --- rows:", std::to_string(_rows), ", columns:", std::to_string(_cols),
            ", first row in:", std::to_string(_firstRowIn), ", first row out:", std::to_string(_firstRowOut),
            ", first column in:", std::to_string(_colStartIn), ", first column out:", std::to_string(_colStartIn),
            ", column stride in:", std::to_string(_colStrideIn), ", column stride out:", std::to_string(_colStrideOut),
            ", overwrite unused:", std::to_string(_overwriteUnused), ", set zero:", std::to_string(_unusedSetZero),
            "\nError --- ");

    // std::cout << setupString << std::endl;

    for (U32 i = 0; i < _arraySizeOut; ++i)
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, UST _arraySize>
[[nodiscard]] std::array<_registerType, _arraySize> GetDefaultInputArray()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 num_register_values = numRegisterValues<_registerType>;

    std::array<_registerType, _arraySize> test_array = {{_mm_setzero<_registerType>()}};
    for (U32 i = 0; i < _arraySize; ++i)
        for (U32 j = 0; j < num_register_values; ++j)
            SetValue(test_array[i], j, static_cast<Type>(j * _arraySize + i + 1));

    return test_array;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, UST _arraySize>
[[nodiscard]] std::array<_registerType, _arraySize> GetDefaultReferenceArray()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 num_register_values = numRegisterValues<_registerType>;

    std::array<_registerType, _arraySize> reference_array = {{_mm_setzero<_registerType>()}};
    for (U32 i = 0; i < _arraySize; ++i)
        for (U32 j = 0; j < num_register_values; ++j)
            SetValue(reference_array[i], j, static_cast<Type>(1. / (j * _arraySize + i + 10.)));

    return reference_array;
}
