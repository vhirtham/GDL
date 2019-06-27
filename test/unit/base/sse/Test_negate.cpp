#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/negate.h"

using namespace GDL;
using namespace GDL::simd;

#include <iostream>

template <U32 _i = 0, U32 _j = 0>
void TestNegate128d()
{
    const __m128d a = _mm_setr_pd(1., 2.);

    const __m128d b = Negate<_i, _j>(a);

    BOOST_CHECK(simd::GetValue<0>(b) == Approx((_i == 0) ? 1. : -1.));
    BOOST_CHECK(simd::GetValue<1>(b) == Approx((_j == 0) ? 2. : -2.));


    constexpr U32 iNext = (_i < 1) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? _j + 1 : _j;
    if constexpr (jNext < 2)
        TestNegate128d<iNext, jNext>();
}



template <typename _registerType, U32 _i = 0, U32 _j = 0, U32 _k = 0, U32 _l = 0>
void TestNegate4()
{
    const _registerType a = _mmx_setr_p<_registerType>(1.f, 2.f, 3.f, 4.f);

    const _registerType b = Negate<_i, _j, _k, _l>(a);


    BOOST_CHECK(simd::GetValue<0>(b) == Approx((_i == 0) ? 1. : -1.));
    BOOST_CHECK(simd::GetValue<1>(b) == Approx((_j == 0) ? 2. : -2.));
    BOOST_CHECK(simd::GetValue<2>(b) == Approx((_k == 0) ? 3. : -3.));
    BOOST_CHECK(simd::GetValue<3>(b) == Approx((_l == 0) ? 4. : -4.));


    constexpr U32 iNext = (_i < 1) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? (_j < 1) ? _j + 1 : 0 : _j;
    constexpr U32 kNext = (iNext == 0 && jNext == 0) ? (_k < 1) ? _k + 1 : 0 : _k;
    constexpr U32 lNext = (iNext == 0 && jNext == 0 && kNext == 0) ? _l + 1 : _l;
    if constexpr (lNext < 2)
    {
        TestNegate4<_registerType, iNext, jNext, kNext, lNext>();
    }
}


#ifdef __AVX2__

template <U32 _i = 0, U32 _j = 0, U32 _k = 0, U32 _l = 0, U32 _m = 0, U32 _n = 0, U32 _o = 0, U32 _p = 0>
void TestNegate256()
{
    const __m256 a = _mm256_setr_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);

    const __m256 b = Negate<_i, _j, _k, _l, _m, _n, _o, _p>(a);


    BOOST_CHECK(simd::GetValue<0>(b) == Approx((_i == 0) ? 1.f : -1.f));
    BOOST_CHECK(simd::GetValue<1>(b) == Approx((_j == 0) ? 2.f : -2.f));
    BOOST_CHECK(simd::GetValue<2>(b) == Approx((_k == 0) ? 3.f : -3.f));
    BOOST_CHECK(simd::GetValue<3>(b) == Approx((_l == 0) ? 4.f : -4.f));
    BOOST_CHECK(simd::GetValue<4>(b) == Approx((_m == 0) ? 5.f : -5.f));
    BOOST_CHECK(simd::GetValue<5>(b) == Approx((_n == 0) ? 6.f : -6.f));
    BOOST_CHECK(simd::GetValue<6>(b) == Approx((_o == 0) ? 7.f : -7.f));
    BOOST_CHECK(simd::GetValue<7>(b) == Approx((_p == 0) ? 8.f : -8.f));


    constexpr U32 iNext = (_i < 1) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? (_j < 1) ? _j + 1 : 0 : _j;
    constexpr U32 kNext = (iNext == 0 && jNext == 0) ? (_k < 1) ? _k + 1 : 0 : _k;
    constexpr U32 lNext = (iNext == 0 && jNext == 0 && kNext == 0) ? (_l < 1) ? _l + 1 : 0 : _l;
    constexpr U32 mNext = (iNext == 0 && jNext == 0 && kNext == 0 && lNext == 0) ? (_m < 1) ? _m + 1 : 0 : _m;
    constexpr U32 nNext =
            (iNext == 0 && jNext == 0 && kNext == 0 && lNext == 0 && mNext == 0) ? (_n < 1) ? _n + 1 : 0 : _n;
    constexpr U32 oNext = (iNext == 0 && jNext == 0 && kNext == 0 && lNext == 0 && mNext == 0 && nNext == 0)
                                  ? (_o < 1) ? _o + 1 : 0
                                  : _o;
    constexpr U32 pNext =
            (iNext == 0 && jNext == 0 && kNext == 0 && lNext == 0 && mNext == 0 && nNext == 0 && oNext == 0) ? _p + 1
                                                                                                             : _p;
    if constexpr (pNext < 2)
        TestNegate256<iNext, jNext, kNext, lNext, mNext, nNext, oNext, pNext>();
}
#endif // __AVX2__


BOOST_AUTO_TEST_CASE(Test_Negate)
{
    TestNegate128d();
    TestNegate4<__m128>();
#ifdef __AVX2__
    TestNegate4<__m256d>();
    TestNegate256();
#endif // __AVX2__
}



// Negate all ---------------------------------------------------------------------------------------------------------

template <typename _registerType>
void TestNegateAll()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    _registerType a = _mmx_setzero_p<_registerType>();

    for (U32 i = 0; i < numRegVals; ++i)
        SetValue(a, i, static_cast<Type>(i));

    _registerType b = Negate(a);
    _registerType c = Negate(b);

    for (I32 i = 0; i < static_cast<I32>(numRegVals); ++i)
    {
        BOOST_CHECK(b[i] == Approx(-a[i]));
        BOOST_CHECK(c[i] == Approx(-b[i]));
        BOOST_CHECK(c[i] == Approx(a[i]));
    }
}



BOOST_AUTO_TEST_CASE(Test_Negate_All)
{
    TestNegateAll<__m128>();
    TestNegateAll<__m128d>();
#ifdef __AVX2__
    TestNegateAll<__m256>();
    TestNegateAll<__m256d>();
#endif // __AVX2__
}
