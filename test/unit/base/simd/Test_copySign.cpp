#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/copySign.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/utility.h"

#include <array>
#include <cmath>

using namespace GDL;
using namespace GDL::simd;

#include <iostream>

// Test general version -----------------------------------------------------------------------------------------------

template <typename _registerType, typename _dataType, U32 _numRegVals>
void TestCopySignTestcase(std::array<_dataType, _numRegVals> srcSign, std::array<_dataType, _numRegVals> srcVal)
{
    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_setzero<_registerType>();
    _registerType bPos = _mm_setzero<_registerType>();


    for (U32 i = 0; i < _numRegVals; ++i)
    {
        SetValue(a, i, srcSign[i]);
        SetValue(b, i, srcVal[i]);
        SetValue(bPos, i, std::abs(srcVal[i]));
    }

    _registerType c = CopySign(a, b);
    _registerType cPos = CopySign<true>(a, bPos);

    for (U32 i = 0; i < _numRegVals; ++i)
    {
        _dataType val = GetValue(c, i);

        BOOST_CHECK(std::signbit(val) == std::signbit(srcSign[i]));
        BOOST_CHECK(std::abs(val) == Approx(std::abs(srcVal[i])));


        _dataType valPos = GetValue(cPos, i);

        BOOST_CHECK(std::signbit(valPos) == std::signbit(srcSign[i]));
        BOOST_CHECK(std::abs(valPos) == Approx(std::abs(srcVal[i])));
    }

    // std::cout << (reinterpret_cast<U64>(-0.0) == 0.0) << std::endl;
}



template <typename _registerType>
void TestCopySign()
{
    using DataType = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;


    std::array<DataType, numRegVals> a;
    std::array<DataType, numRegVals> b;
    std::array<DataType, numRegVals> c;
    for (U32 i = 0; i < numRegVals; ++i)
    {
        a[i] = static_cast<DataType>(3 * i * std::pow(-1, i));
        b[i] = static_cast<DataType>(-1. * (i + 1));
        c[i] = static_cast<DataType>((i + 5) * 12);
    }

    TestCopySignTestcase<_registerType, DataType, numRegVals>(a, b);
    TestCopySignTestcase<_registerType, DataType, numRegVals>(b, a);
    TestCopySignTestcase<_registerType, DataType, numRegVals>(a, c);
    TestCopySignTestcase<_registerType, DataType, numRegVals>(c, a);
    TestCopySignTestcase<_registerType, DataType, numRegVals>(b, c);
    TestCopySignTestcase<_registerType, DataType, numRegVals>(c, b);

#ifndef NDEVEXCEPTION
    for (U32 i = 0; i < numRegVals; ++i)
    {
        _registerType d = _mm_setzero<_registerType>();
        BOOST_CHECK_NO_THROW(CopySign<true>(d, d));
        SetValue(d, i, -0.);
        BOOST_CHECK_THROW(CopySign<true>(d, d), Exception);
        SetValue(d, i, static_cast<DataType>(i + 4));
        BOOST_CHECK_NO_THROW(CopySign<true>(d, d));
        SetValue(d, i, -1 * static_cast<DataType>(i + 4));
        BOOST_CHECK_THROW(CopySign<true>(d, d), Exception);
    }
#endif // NDEVEXCEPTION
}



BOOST_AUTO_TEST_CASE(CopySign_m128)
{
    TestCopySign<__m128>();
}



BOOST_AUTO_TEST_CASE(CopySign_m128d)
{
    TestCopySign<__m128d>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(CopySign_m256)
{
    TestCopySign<__m256>();
}



BOOST_AUTO_TEST_CASE(CopySign_m256d)
{
    TestCopySign<__m256d>();
}

#endif //__AVX2__



// Test version where all values of the second input are positive -----------------------------------------------------
