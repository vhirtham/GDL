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

    _registerType a;
    Type sum = 0;

    for (U32 i = 0; i < numRegVals; ++i)
    {
        sum += (i + 1);
        SetValue(a, i, static_cast<Type>(i + 1));
    }

    _registerType b = Sum(a);

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

    _registerType b = Sum<_regStartIdx>(arr, idxStart);

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


BOOST_AUTO_TEST_CASE(Register_array_sum_m128)
{
    TestRegisterArraySum<__m128>();
}
