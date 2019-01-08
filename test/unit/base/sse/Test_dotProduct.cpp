#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/utility.h"

#include <array>
#include <iostream>



using namespace GDL;


template <typename _registerType, U32 _numValues = 1>
void TestDotProduct()
{
    using ValueType = decltype(sse::GetDataType<_registerType>());

    std::array<ValueType, 8> testValues1 = {{1, 2, 3, 4, 5, 6, 7, 8}};
    std::array<ValueType, 8> testValues2 = {{4, 11, 5, 1, 8, 6, 9, 2}};

    _registerType reg1 = _mmx_setzero_p<_registerType>();
    _registerType reg2 = _mmx_setzero_p<_registerType>();

    for (U32 i = 0; i < sse::numRegisterValues<_registerType>; ++i)
    {
        sse::SetValue(reg1, testValues1[i], i);
        sse::SetValue(reg2, testValues2[i], i);
    }


    ValueType expectedResult = 0;
    for (U32 i = 0; i < _numValues; ++i)
        expectedResult += testValues1[i] * testValues2[i];


    ValueType result = sse::DotProduct<_registerType, _numValues>(reg1, reg2);
    BOOST_CHECK(result == Approx(expectedResult));

    if constexpr (_numValues < sse::numRegisterValues<_registerType>)
        TestDotProduct<_registerType, _numValues + 1>();
}

BOOST_AUTO_TEST_CASE(Dot_Product)
{
    TestDotProduct<__m128d>();
    TestDotProduct<__m128>();
}
