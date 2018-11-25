#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/approx.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/maskMacros.h"

using namespace GDL;

#include <iostream>



// Shuffle 2 ----------------------------------------------------------------------------------------------------------

template <U32 _i = 0, U32 _j = 0>
void TestShuffle2()
{
    const __m128d a = _mm_setr_pd(1., 2.);
    const __m128d b = _mm_setr_pd(3., 4.);

    const __m128d c = _mm_shuffle_pd(a, b, SHUFFLE_MASK_2(_i, _j));

    BOOST_CHECK(sse::GetValue<0>(c) == Approx(sse::GetValue<_i>(a)));
    BOOST_CHECK(sse::GetValue<1>(c) == Approx(sse::GetValue<_j>(b)));

    constexpr U32 iNext = (_i < 1) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? _j + 1 : _j;
    if constexpr (jNext < 2)
    {
        TestShuffle2<iNext, jNext>();
    }
}



BOOST_AUTO_TEST_CASE(Shuffle2)
{
    TestShuffle2();
}



// Shuffle 4 ----------------------------------------------------------------------------------------------------------

template <U32 _i = 0, U32 _j = 0, U32 _k = 0, U32 _l = 0>
void TestShuffle4()
{
    const __m128 a = _mm_setr_ps(1.f, 2.f, 3.f, 4.f);
    const __m128 b = _mm_setr_ps(5.f, 6.f, 7.f, 8.f);

    const __m128 c = _mm_shuffle_ps(a, b, SHUFFLE_MASK_4(_i, _j, _k, _l));



    BOOST_CHECK(sse::GetValue<0>(c) == Approx(sse::GetValue<_i>(a)));
    BOOST_CHECK(sse::GetValue<1>(c) == Approx(sse::GetValue<_j>(a)));
    BOOST_CHECK(sse::GetValue<2>(c) == Approx(sse::GetValue<_k>(b)));
    BOOST_CHECK(sse::GetValue<3>(c) == Approx(sse::GetValue<_l>(b)));


    constexpr U32 iNext = (_i < 3) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? (_j < 3) ? _j + 1 : 0 : _j;
    constexpr U32 kNext = (iNext == 0 && jNext == 0) ? (_k < 3) ? _k + 1 : 0 : _k;
    constexpr U32 lNext = (iNext == 0 && jNext == 0 && kNext == 0) ? _l + 1 : _l;
    if constexpr (lNext < 4)
    {
        TestShuffle4<iNext, jNext, kNext, lNext>();
    }
}



BOOST_AUTO_TEST_CASE(Shuffle4)
{
    TestShuffle4();
}



// Blend 2 ------------------------------------------------------------------------------------------------------------

template <U32 _i = 0, U32 _j = 0>
void TestBlend2()
{
    const __m128d a = _mm_setr_pd(1., 2.);
    const __m128d b = _mm_setr_pd(3., 4.);

    const __m128d c = _mm_blend_pd(a, b, BLEND_MASK_2(_i, _j));

    BOOST_CHECK(sse::GetValue<0>(c) == Approx((_i == 0) ? 1.f : 3.f));
    BOOST_CHECK(sse::GetValue<1>(c) == Approx((_j == 0) ? 2.f : 4.f));

    constexpr U32 iNext = (_i < 1) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? _j + 1 : _j;
    if constexpr (jNext < 2)
    {
        TestBlend2<iNext, jNext>();
    }
}


BOOST_AUTO_TEST_CASE(Blend2)
{
    TestBlend2();
}



// Blend 4 ------------------------------------------------------------------------------------------------------------

template <U32 _i = 0, U32 _j = 0, U32 _k = 0, U32 _l = 0>
void TestBlend4()
{
    const __m128 a = _mm_setr_ps(1.f, 2.f, 3.f, 4.f);
    const __m128 b = _mm_setr_ps(5.f, 6.f, 7.f, 8.f);

    const __m128 c = _mm_blend_ps(a, b, BLEND_MASK_4(_i, _j, _k, _l));


    BOOST_CHECK(sse::GetValue<0>(c) == Approx((_i == 0) ? 1.f : 5.f));
    BOOST_CHECK(sse::GetValue<1>(c) == Approx((_j == 0) ? 2.f : 6.f));
    BOOST_CHECK(sse::GetValue<2>(c) == Approx((_k == 0) ? 3.f : 7.f));
    BOOST_CHECK(sse::GetValue<3>(c) == Approx((_l == 0) ? 4.f : 8.f));


    constexpr U32 iNext = (_i < 1) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? (_j < 1) ? _j + 1 : 0 : _j;
    constexpr U32 kNext = (iNext == 0 && jNext == 0) ? (_k < 1) ? _k + 1 : 0 : _k;
    constexpr U32 lNext = (iNext == 0 && jNext == 0 && kNext == 0) ? _l + 1 : _l;
    if constexpr (lNext < 2)
    {
        TestBlend4<iNext, jNext, kNext, lNext>();
    }
}



BOOST_AUTO_TEST_CASE(Blend4)
{
    TestBlend4();
}



// Permute ------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <U32 _i = 0, U32 _j = 0, U32 _k = 0, U32 _l = 0>
void TestPermute2f128()
{
    __m256 a = _mm256_setr_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
    __m256 b = _mm256_setr_ps(9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f);

    __m256 c = _mm256_permute2f128_ps(a, b, PERMUTE_MASK_2F128(_i, _j, _k, _l));

    for (U32 i = 0; i < 4; ++i)
    {
        BOOST_CHECK(sse::GetValue(c, i) == Approx(1.f + i + _i * 8 + _j * 4));
        BOOST_CHECK(sse::GetValue(c, i + 4) == Approx(1.f + i + _k * 8 + _l * 4));
    }


    constexpr U32 iNext = (_i < 1) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? (_j < 1) ? _j + 1 : 0 : _j;
    constexpr U32 kNext = (iNext == 0 && jNext == 0) ? (_k < 1) ? _k + 1 : 0 : _k;
    constexpr U32 lNext = (iNext == 0 && jNext == 0 && kNext == 0) ? _l + 1 : _l;
    if constexpr (lNext < 2)
    {
        TestPermute2f128<iNext, jNext, kNext, lNext>();
    }
}


BOOST_AUTO_TEST_CASE(Permute2f128)
{
    TestPermute2f128();
}

#endif // __AVX2__
