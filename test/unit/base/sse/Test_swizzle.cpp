#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/approx.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/maskMacros.h"
#include "gdl/base/sse/swizzle.h"

using namespace GDL;
using namespace GDL::sse;

// Swizzle ------------------------------------------------------------------------------------------------------------

template <U32 _i = 0, U32 _j = 0, U32 _k = 0, U32 _l = 0>
void TestSwizzle()
{
    const __m128 a = _mm_setr_ps(1.f, 2.f, 3.f, 4.f);

    const __m128 b = Swizzle<_i, _j, _k, _l>(a);



    BOOST_CHECK(sse::GetValue<0>(b) == Approx(sse::GetValue<_i>(a)));
    BOOST_CHECK(sse::GetValue<1>(b) == Approx(sse::GetValue<_j>(a)));
    BOOST_CHECK(sse::GetValue<2>(b) == Approx(sse::GetValue<_k>(a)));
    BOOST_CHECK(sse::GetValue<3>(b) == Approx(sse::GetValue<_l>(a)));


    constexpr U32 iNext = (_i < 3) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? (_j < 3) ? _j + 1 : 0 : _j;
    constexpr U32 kNext = (iNext == 0 && jNext == 0) ? (_k < 3) ? _k + 1 : 0 : _k;
    constexpr U32 lNext = (iNext == 0 && jNext == 0 && kNext == 0) ? _l + 1 : _l;
    if constexpr (lNext < 4)
    {
        TestSwizzle<iNext, jNext, kNext, lNext>();
    }
}



BOOST_AUTO_TEST_CASE(Test_Swizzle)
{
    TestSwizzle();
}


// Swizzle1 -----------------------------------------------------------------------------------------------------------
#include <iostream>

template <typename _registerType, U32 _index = 0>
void TestSwizzle1()
{
    _registerType a = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
        SetValue(a, i, i);

    _registerType b = Swizzle1<_index>(a);
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
    {
        BOOST_CHECK(sse::GetValue(b, i) == Approx(sse::GetValue<_index>(a)));
        std::cout << sse::GetValue(b, i) << std::endl;
    }
    std::cout << std::endl;
    if constexpr (_index + 1 < numRegisterValues<_registerType>)
        TestSwizzle1<_registerType, _index + 1>();
}


BOOST_AUTO_TEST_CASE(Test_Swizzle1)
{
    TestSwizzle1<__m128>();
    TestSwizzle1<__m128d>();
    TestSwizzle1<__m256>();
    TestSwizzle1<__m256d>();
}
