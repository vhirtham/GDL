#pragma once

#include "test/unit/base/simd/transpose_test_template_general.h"


#include "gdl/base/approx.h"
#include "gdl/base/string.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/transpose.h"



using namespace GDL::simd;



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols, U32 _maxOffset>
inline void TestArrayOffset()
{
    constexpr UST input_size = _cols + _maxOffset;

    std::array<_registerType, input_size> input = GetDefaultInputArray<_registerType, input_size>();

    TestArrayOffsetIterateInputOffset<_rows, _cols, _maxOffset>(input);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _maxOffset, U32 _columnOffsetIn, typename _registerType, UST _inputArraySize>
inline void TestArrayOffsetIterateInputOffset(std::array<_registerType, _inputArraySize> input)
{
    TestArrayOffsetIterateOutputOffset<_rows, _cols, _maxOffset, _columnOffsetIn>(input);

    if constexpr (_columnOffsetIn < _maxOffset)
        TestArrayOffsetIterateInputOffset<_rows, _cols, _maxOffset, _columnOffsetIn + 1>(input);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _maxOffset, U32 _columnOffsetIn, U32 _columnOffsetOut, typename _registerType,
          UST _inputArraySize>
inline void TestArrayOffsetIterateOutputOffset(std::array<_registerType, _inputArraySize> input)
{

    TestArrayOffsetTestCase<_rows, _cols, _columnOffsetIn, _columnOffsetOut>(input);

    if constexpr (_columnOffsetOut < _maxOffset)
        TestArrayOffsetIterateOutputOffset<_rows, _cols, _maxOffset, _columnOffsetIn, _columnOffsetOut + 1>(input);
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

template <typename _registerType, U32 _rows, U32 _cols, U32 _numStrides>
inline void TestArrayStride()
{
    constexpr UST input_size = _cols * _numStrides;

    std::array<_registerType, input_size> input = GetDefaultInputArray<_registerType, input_size>();

    TestArrayStrideIterateInputStride<_rows, _cols, _numStrides>(input);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _numStrides, U32 _columnStrideIn, typename _registerType, UST _inputArraySize>
inline void TestArrayStrideIterateInputStride(std::array<_registerType, _inputArraySize> input)
{
    TestArrayStrideIterateOutputStride<_rows, _cols, _numStrides, _columnStrideIn>(input);

    if constexpr (_columnStrideIn < _numStrides)
        TestArrayStrideIterateInputStride<_rows, _cols, _numStrides, _columnStrideIn + 1>(input);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _numStrides, U32 _columnStrideIn, U32 _columnStrideOut, typename _registerType,
          UST _inputArraySize>
inline void TestArrayStrideIterateOutputStride(std::array<_registerType, _inputArraySize> input)
{

    TestArrayStrideTestCase<_rows, _cols, _columnStrideIn, _columnStrideOut>(input);

    if constexpr (_columnStrideOut < _numStrides)
        TestArrayStrideIterateOutputStride<_rows, _cols, _numStrides, _columnStrideIn, _columnStrideOut + 1>(input);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _columnStrideIn, U32 _columnStrideOut, typename _registerType, UST _inputArraySize>
inline void TestArrayStrideTestCase(std::array<_registerType, _inputArraySize> input)
{
    constexpr UST output_array_size = _inputArraySize / _cols * _rows;

    std::array<_registerType, output_array_size> reference =
            GetDefaultReferenceArray<_registerType, output_array_size>();
    std::array<_registerType, output_array_size> output = reference;

    Transpose<_rows, _cols, 0, 0, 0, 0, _columnStrideIn, _columnStrideOut>(input, output);

    CheckResults<_rows, _cols, 0, 0, 0, 0, _columnStrideIn, _columnStrideOut, true, false>(input, output, reference);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols>
inline void TestCombinedArrayStrideAndOffset()
{
    constexpr UST input_size = _cols * 2 + 3;
    constexpr UST output_array_size = _rows * 3 + 1;

    std::array<_registerType, input_size> input = GetDefaultInputArray<_registerType, input_size>();
    std::array<_registerType, output_array_size> reference =
            GetDefaultReferenceArray<_registerType, output_array_size>();
    std::array<_registerType, output_array_size> output = reference;

    Transpose<_rows, _cols, 0, 0, 3, 1, 2, 3>(input, output);

    CheckResults<_rows, _cols, 0, 0, 3, 1, 2, 3, true, false>(input, output, reference);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType>
inline void TestAllMatrixSizes()
{
    TestAllMatrixSizesIterateRows<_registerType>();
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows>
inline void TestAllMatrixSizesIterateRows()
{
    TestAllMatrixSizesIterateColumns<_registerType, _rows>();

    if constexpr (_rows < numRegisterValues<_registerType>)
        TestAllMatrixSizesIterateRows<_registerType, _rows + 1>();
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols>
inline void TestAllMatrixSizesIterateColumns()
{
    TestAllMatrixSizesTestcasse<_registerType, _rows, _cols>();

    if constexpr (_cols < numRegisterValues<_registerType>)
        TestAllMatrixSizesIterateColumns<_registerType, _rows, _cols + 1>();
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _rows, U32 _cols>
inline void TestAllMatrixSizesTestcasse()
{

    constexpr U32 firstRowInMax = numRegisterValues<_registerType> - _rows;
    constexpr U32 firstRowOutMax = numRegisterValues<_registerType> - _cols;

    std::array<_registerType, _cols> input = GetDefaultInputArray<_registerType, _cols>();
    std::array<_registerType, _rows> reference = GetDefaultReferenceArray<_registerType, _rows>();

    std::array<_registerType, _rows> output = reference;
    Transpose<_rows, _cols, firstRowInMax, firstRowOutMax, 0, 0, 1, 1, true, false>(input, output);
    CheckResults<_rows, _cols, firstRowInMax, firstRowOutMax, 0, 0, 1, 1, true, false>(input, output, reference);

    Transpose<_rows, _cols, firstRowInMax / 3, firstRowOutMax / 2, 0, 0, 1, 1, true, true>(input, output);
    CheckResults<_rows, _cols, firstRowInMax / 3, firstRowOutMax / 2, 0, 0, 1, 1, true, true>(input, output, reference);

    output = reference;
    Transpose<_rows, _cols, firstRowInMax / 2, firstRowOutMax, 0, 0, 1, 1, false, false>(input, output);
    CheckResults<_rows, _cols, firstRowInMax / 2, firstRowOutMax, 0, 0, 1, 1, false, false>(input, output, reference);
}


// --------------------------------------------------------------------------------------------------------------------

template <U32 _rows, U32 _cols, U32 _firstRowIn, U32 _firstRowOut, U32 _colStartIn, U32 _colStartOut, U32 _colStrideIn,
          U32 _colStrideOut, bool _overwriteUnused, bool _unusedSetZero, bool _checkUnusedCols, UST _inputArraySize,
          UST _outputArraySize, typename _registerType>
inline void CheckResults(const std::array<_registerType, _inputArraySize>& in,
                         const std::array<_registerType, _outputArraySize>& out,
                         const std::array<_registerType, _outputArraySize>& ref)
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

    for (U32 i = 0; i < _outputArraySize; ++i)
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
