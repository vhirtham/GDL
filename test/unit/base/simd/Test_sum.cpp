#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/sum.h"
#include "gdl/base/simd/utility.h"

using namespace GDL;
using namespace GDL::simd;



// Test register sum --------------------------------------------------------------------------------------------------

template <typename _registerType>
void TestRegisterSum()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    _registerType a = _mm_setzero<_registerType>();
    Type sum = 0;

    for (U32 i = 0; i < numRegVals; ++i)
    {
        sum += (i + 1);
        SetValue(a, i, static_cast<Type>(i + 1));
    }

    _registerType b = RegisterSum(a);

    for (U32 i = 0; i < numRegVals; ++i)
        BOOST_CHECK(GetValue(b, i) == Approx(sum));
}



BOOST_AUTO_TEST_CASE(Register_sum_m128)
{
    TestRegisterSum<__m128>();
}



BOOST_AUTO_TEST_CASE(Register_sum_m128d)
{
    TestRegisterSum<__m128d>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(Register_sum_m256)
{
    TestRegisterSum<__m256>();
}



BOOST_AUTO_TEST_CASE(Register_sum_m256d)
{
    TestRegisterSum<__m256d>();
}

#endif // __AVX2__



// Test register array sum  -------------------------------------------------------------------------------------------

#include <iostream>


template <U32 _regStartIdx, typename _registerType, std::size_t _numRegistersInArray>
void TestRegisterArraySumTestcase(const std::array<_registerType, _numRegistersInArray>& arr, U32 idxStart = 0)
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;
    constexpr U32 numArrayVals = numRegVals * _numRegistersInArray;

    Type sum = 0;

    U32 firstIdx = idxStart * numRegVals + _regStartIdx;
    for (U32 i = firstIdx; i < numArrayVals; ++i)
        sum += static_cast<Type>(i + 1);

    _registerType b = RegisterArraySum<_regStartIdx>(arr, idxStart);

    for (U32 i = 0; i < numRegVals; ++i)
        BOOST_CHECK(GetValue(b, i) == Approx(sum));

    if (idxStart < _numRegistersInArray - 1)
        TestRegisterArraySumTestcase<_regStartIdx>(arr, idxStart + 1);
}



template <typename _registerType, std::size_t _numRegistersInArray = 6, U32 _regStartIdx = 0>
void TestRegisterArraySum()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    std::array<_registerType, _numRegistersInArray> arr;

    for (U32 i = 0; i < _numRegistersInArray; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
        {
            Type count = static_cast<Type>(1 + j + i * numRegVals);
            SetValue(arr[i], j, count);
        }

    TestRegisterArraySumTestcase<_regStartIdx>(arr);

    if constexpr (_regStartIdx < numRegVals - 1)
        TestRegisterArraySum<_registerType, _numRegistersInArray, _regStartIdx + 1>();
}



BOOST_AUTO_TEST_CASE(Register_Array_Sum_m128)
{
    TestRegisterArraySum<__m128>();
}



BOOST_AUTO_TEST_CASE(Register_Array_Sum_m128d)
{
    TestRegisterArraySum<__m128d>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(Register_Array_Sum_m256)
{
    TestRegisterArraySum<__m256>();
}



BOOST_AUTO_TEST_CASE(Register_Array_Sum_m256d)
{
    TestRegisterArraySum<__m256d>();
}


#endif //__AVX2__



// Test register array square sum  ------------------------------------------------------------------------------------

#include <iostream>


template <U32 _regStartIdx, typename _registerType, std::size_t _numRegistersInArray>
void TestRegisterArraySquareSumTestcase(const std::array<_registerType, _numRegistersInArray>& arr, U32 idxStart = 0)
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;
    constexpr U32 numArrayVals = numRegVals * _numRegistersInArray;

    Type sum = 0;

    U32 firstIdx = idxStart * numRegVals + _regStartIdx;
    for (U32 i = firstIdx; i < numArrayVals; ++i)
        sum += static_cast<Type>((i + 1) * (i + 1));

    _registerType b = RegisterArraySquareSum<_regStartIdx>(arr, idxStart);

    for (U32 i = 0; i < numRegVals; ++i)
        BOOST_CHECK(GetValue(b, i) == Approx(sum));

    if (idxStart < _numRegistersInArray - 1)
        TestRegisterArraySquareSumTestcase<_regStartIdx>(arr, idxStart + 1);
}



template <typename _registerType, std::size_t _numRegistersInArray = 6, U32 _regStartIdx = 0>
void TestRegisterArraySquareSum()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    std::array<_registerType, _numRegistersInArray> arr;

    for (U32 i = 0; i < _numRegistersInArray; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
        {
            Type count = static_cast<Type>(1 + j + i * numRegVals);
            SetValue(arr[i], j, count);
        }

    TestRegisterArraySquareSumTestcase<_regStartIdx>(arr);

    if constexpr (_regStartIdx < numRegVals - 1)
        TestRegisterArraySquareSum<_registerType, _numRegistersInArray, _regStartIdx + 1>();
}



BOOST_AUTO_TEST_CASE(Register_Array_SquareSum_m128)
{
    TestRegisterArraySquareSum<__m128>();
}



BOOST_AUTO_TEST_CASE(Register_Array_SquareSum_m128d)
{
    TestRegisterArraySquareSum<__m128d>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(Register_Array_SquareSum_m256)
{
    TestRegisterArraySquareSum<__m256>();
}



BOOST_AUTO_TEST_CASE(Register_Array_SquareSum_m256d)
{
    TestRegisterArraySquareSum<__m256d>();
}

#endif //__AVX2__



// Test register multi sum --------------------------------------------------------------------------------------------

template <typename _registerType>
void TestRegisterMultiSum()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    std::array<Type, numRegVals> exp = {{0}};

    std::array<_registerType, numRegVals> a;
    for (U32 i = 0; i < numRegVals; ++i)
        for (U32 j = 0; j < numRegVals; ++j)
        {
            SetValue(a[i], j, static_cast<Type>(j * numRegVals + i + 1));
            exp[i] += static_cast<Type>(j * numRegVals + i + 1);
        }

    _registerType result = RegisterMultiSum(a);
    for (U32 i = 0; i < numRegVals; ++i)
        BOOST_CHECK(GetValue(result, i) == Approx(exp[i]));
}



BOOST_AUTO_TEST_CASE(Register_Multi_Sum_m128d)
{
    TestRegisterMultiSum<__m128d>();
}



BOOST_AUTO_TEST_CASE(Register_Multi_Sum_m128)
{
    TestRegisterMultiSum<__m128>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(Register_Multi_Sum_m256d)
{
    TestRegisterMultiSum<__m256d>();
}



BOOST_AUTO_TEST_CASE(Register_Multi_Sum_m256)
{
    TestRegisterMultiSum<__m256>();
}

#endif
