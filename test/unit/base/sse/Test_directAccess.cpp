#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/sse/directAccess.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;

BOOST_AUTO_TEST_CASE(Get_Value)
{
    __m128d d128 = _mm_setr_pd(1., 2.);
    BOOST_CHECK(simd::GetValue<0>(d128) == 1.);
    BOOST_CHECK(simd::GetValue<1>(d128) == 2.);
    BOOST_CHECK(simd::GetValue(d128, 0) == 1.);
    BOOST_CHECK(simd::GetValue(d128, 1) == 2.);
    GDL_CHECK_THROW_DEV(simd::GetValue(d128, 2), Exception);

    __m128 f128 = _mm_setr_ps(1.f, 2.f, 3.f, 4.f);
    BOOST_CHECK(simd::GetValue<0>(f128) == 1.f);
    BOOST_CHECK(simd::GetValue<1>(f128) == 2.f);
    BOOST_CHECK(simd::GetValue<2>(f128) == 3.f);
    BOOST_CHECK(simd::GetValue<3>(f128) == 4.f);
    BOOST_CHECK(simd::GetValue(f128, 0) == 1.f);
    BOOST_CHECK(simd::GetValue(f128, 1) == 2.f);
    BOOST_CHECK(simd::GetValue(f128, 2) == 3.f);
    BOOST_CHECK(simd::GetValue(f128, 3) == 4.f);
    GDL_CHECK_THROW_DEV(simd::GetValue(f128, 4), Exception);


#ifdef __AVX2__
    __m256d d256 = _mm256_setr_pd(1., 2., 3., 4.);
    BOOST_CHECK(simd::GetValue<0>(d256) == 1.);
    BOOST_CHECK(simd::GetValue<1>(d256) == 2.);
    BOOST_CHECK(simd::GetValue<2>(d256) == 3.);
    BOOST_CHECK(simd::GetValue<3>(d256) == 4.);
    BOOST_CHECK(simd::GetValue(d256, 0) == 1.);
    BOOST_CHECK(simd::GetValue(d256, 1) == 2.);
    BOOST_CHECK(simd::GetValue(d256, 2) == 3.);
    BOOST_CHECK(simd::GetValue(d256, 3) == 4.);
    GDL_CHECK_THROW_DEV(simd::GetValue(d256, 4), Exception);

    __m256 f256 = _mm256_setr_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
    BOOST_CHECK(simd::GetValue<0>(f256) == 1.f);
    BOOST_CHECK(simd::GetValue<1>(f256) == 2.f);
    BOOST_CHECK(simd::GetValue<2>(f256) == 3.f);
    BOOST_CHECK(simd::GetValue<3>(f256) == 4.f);
    BOOST_CHECK(simd::GetValue<4>(f256) == 5.f);
    BOOST_CHECK(simd::GetValue<5>(f256) == 6.f);
    BOOST_CHECK(simd::GetValue<6>(f256) == 7.f);
    BOOST_CHECK(simd::GetValue<7>(f256) == 8.f);
    BOOST_CHECK(simd::GetValue(f256, 0) == 1.f);
    BOOST_CHECK(simd::GetValue(f256, 1) == 2.f);
    BOOST_CHECK(simd::GetValue(f256, 2) == 3.f);
    BOOST_CHECK(simd::GetValue(f256, 3) == 4.f);
    BOOST_CHECK(simd::GetValue(f256, 4) == 5.f);
    BOOST_CHECK(simd::GetValue(f256, 5) == 6.f);
    BOOST_CHECK(simd::GetValue(f256, 6) == 7.f);
    BOOST_CHECK(simd::GetValue(f256, 7) == 8.f);
    GDL_CHECK_THROW_DEV(simd::GetValue(f256, 8), Exception);
#endif // __AVX2__
}



template <typename _registerType>
void TestSetValue()
{
    using _valueType = decltype(simd::GetDataType<_registerType>());

    _registerType reg1 = _mmx_setzero_p<_registerType>();
    BOOST_CHECK(reg1 == ApproxZero<_registerType>());

    simd::SetValue<0>(reg1, 1);
    simd::SetValue<1>(reg1, 2);
    if constexpr (simd::numRegisterValues<_registerType>> 2)
    {
        simd::SetValue<2>(reg1, 3);
        simd::SetValue<3>(reg1, 4);
    }
    if constexpr (simd::numRegisterValues<_registerType>> 4)
    {
        simd::SetValue<4>(reg1, 5);
        simd::SetValue<5>(reg1, 6);
        simd::SetValue<6>(reg1, 7);
        simd::SetValue<7>(reg1, 8);
    }

    BOOST_CHECK(reg1 != ApproxZero<_registerType>());

    alignas(simd::alignmentBytes<_registerType>) std::array<_valueType, simd::numRegisterValues<_registerType>> values;
    _mmx_store_p(values.data(), reg1);
    for (U32 i = 0; i < values.size(); ++i)
        BOOST_CHECK(values[i] == Approx(static_cast<_valueType>(i + 1)));



    _registerType reg2 = _mmx_setzero_p<_registerType>();
    BOOST_CHECK(reg2 == ApproxZero<_registerType>());
    for (U32 i = 0; i < values.size(); ++i)
        simd::SetValue(reg2, i, i + 1);
    GDL_CHECK_THROW_DEV(simd::SetValue(reg2, simd::numRegisterValues<_registerType>, 10), Exception);

    BOOST_CHECK(reg2 != ApproxZero<_registerType>());

    alignas(simd::alignmentBytes<_registerType>) std::array<_valueType, simd::numRegisterValues<_registerType>> values2;
    _mmx_store_p(values2.data(), reg2);
    for (U32 i = 0; i < values2.size(); ++i)
        BOOST_CHECK(values2[i] == Approx(static_cast<_valueType>(i + 1)));
}

BOOST_AUTO_TEST_CASE(Set_Value)
{
    TestSetValue<__m128>();
    TestSetValue<__m128d>();
#ifdef __AVX2__
    TestSetValue<__m256>();
    TestSetValue<__m256d>();
#endif // __AVX2__
}
