#include <boost/test/unit_test.hpp>

#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/utility.h"

using namespace GDL;

BOOST_AUTO_TEST_CASE(Get_Value)
{
    __m128d d128 = _mm_setr_pd(1., 2.);
    BOOST_CHECK(sse::GetValue<0>(d128) == 1.);
    BOOST_CHECK(sse::GetValue<1>(d128) == 2.);
    BOOST_CHECK(sse::GetValue(d128, 0) == 1.);
    BOOST_CHECK(sse::GetValue(d128, 1) == 2.);


    __m128 f128 = _mm_setr_ps(1.f, 2.f, 3.f, 4.f);
    BOOST_CHECK(sse::GetValue<0>(f128) == 1.f);
    BOOST_CHECK(sse::GetValue<1>(f128) == 2.f);
    BOOST_CHECK(sse::GetValue<2>(f128) == 3.f);
    BOOST_CHECK(sse::GetValue<3>(f128) == 4.f);
    BOOST_CHECK(sse::GetValue(f128, 0) == 1.f);
    BOOST_CHECK(sse::GetValue(f128, 1) == 2.f);
    BOOST_CHECK(sse::GetValue(f128, 2) == 3.f);
    BOOST_CHECK(sse::GetValue(f128, 3) == 4.f);

#ifdef __AVX2__
    __m256d d256 = _mm256_setr_pd(1., 2., 3., 4.);
    BOOST_CHECK(sse::GetValue<0>(d256) == 1.);
    BOOST_CHECK(sse::GetValue<1>(d256) == 2.);
    BOOST_CHECK(sse::GetValue<2>(d256) == 3.);
    BOOST_CHECK(sse::GetValue<3>(d256) == 4.);
    BOOST_CHECK(sse::GetValue(d256, 0) == 1.);
    BOOST_CHECK(sse::GetValue(d256, 1) == 2.);
    BOOST_CHECK(sse::GetValue(d256, 2) == 3.);
    BOOST_CHECK(sse::GetValue(d256, 3) == 4.);


    __m256 f256 = _mm256_setr_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
    BOOST_CHECK(sse::GetValue<0>(f256) == 1.f);
    BOOST_CHECK(sse::GetValue<1>(f256) == 2.f);
    BOOST_CHECK(sse::GetValue<2>(f256) == 3.f);
    BOOST_CHECK(sse::GetValue<3>(f256) == 4.f);
    BOOST_CHECK(sse::GetValue<4>(f256) == 5.f);
    BOOST_CHECK(sse::GetValue<5>(f256) == 6.f);
    BOOST_CHECK(sse::GetValue<6>(f256) == 7.f);
    BOOST_CHECK(sse::GetValue<7>(f256) == 8.f);
    BOOST_CHECK(sse::GetValue(f256, 0) == 1.f);
    BOOST_CHECK(sse::GetValue(f256, 1) == 2.f);
    BOOST_CHECK(sse::GetValue(f256, 2) == 3.f);
    BOOST_CHECK(sse::GetValue(f256, 3) == 4.f);
    BOOST_CHECK(sse::GetValue(f256, 4) == 5.f);
    BOOST_CHECK(sse::GetValue(f256, 5) == 6.f);
    BOOST_CHECK(sse::GetValue(f256, 6) == 7.f);
    BOOST_CHECK(sse::GetValue(f256, 7) == 8.f);
#endif // __AVX2__
}
