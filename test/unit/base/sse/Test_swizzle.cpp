#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/approx.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/swizzle.h"

using namespace GDL;
using namespace GDL::sse;



// Blend --------------------------------------------------------------------------------------------------------------

template <U32 _i = 0, U32 _j = 0>
void TestBlend128d()
{
    const __m128d a = _mm_setr_pd(1., 2.);
    const __m128d b = _mm_setr_pd(3., 4.);

    const __m128d c = Blend<_i, _j>(a, b);

    BOOST_CHECK(sse::GetValue<0>(c) == Approx((_i == 0) ? 1.f : 3.f));
    BOOST_CHECK(sse::GetValue<1>(c) == Approx((_j == 0) ? 2.f : 4.f));

    constexpr U32 iNext = (_i < 1) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? _j + 1 : _j;
    if constexpr (jNext < 2)
    {
        TestBlend128d<iNext, jNext>();
    }
}



template <typename _registerType, U32 _i = 0, U32 _j = 0, U32 _k = 0, U32 _l = 0>
void TestBlend4()
{
    const _registerType a = _mmx_setr_p<_registerType>(1.f, 2.f, 3.f, 4.f);
    const _registerType b = _mmx_setr_p<_registerType>(5.f, 6.f, 7.f, 8.f);

    const _registerType c = Blend<_i, _j, _k, _l>(a, b);


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
        TestBlend4<_registerType, iNext, jNext, kNext, lNext>();
    }
}



template <U32 _i = 0, U32 _j = 0, U32 _k = 0, U32 _l = 0, U32 _m = 0, U32 _n = 0, U32 _o = 0, U32 _p = 0>
void TestBlend256()
{
    const __m256 a = _mm256_setr_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
    const __m256 b = _mm256_setr_ps(9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f);

    const __m256 c = Blend<_i, _j, _k, _l, _m, _n, _o, _p>(a, b);


    BOOST_CHECK(sse::GetValue<0>(c) == Approx((_i == 0) ? 1.f : 9.f));
    BOOST_CHECK(sse::GetValue<1>(c) == Approx((_j == 0) ? 2.f : 10.f));
    BOOST_CHECK(sse::GetValue<2>(c) == Approx((_k == 0) ? 3.f : 11.f));
    BOOST_CHECK(sse::GetValue<3>(c) == Approx((_l == 0) ? 4.f : 12.f));
    BOOST_CHECK(sse::GetValue<4>(c) == Approx((_m == 0) ? 5.f : 13.f));
    BOOST_CHECK(sse::GetValue<5>(c) == Approx((_n == 0) ? 6.f : 14.f));
    BOOST_CHECK(sse::GetValue<6>(c) == Approx((_o == 0) ? 7.f : 15.f));
    BOOST_CHECK(sse::GetValue<7>(c) == Approx((_p == 0) ? 8.f : 16.f));


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
    {
        TestBlend256<iNext, jNext, kNext, lNext, mNext, nNext, oNext, pNext>();
    }
}



BOOST_AUTO_TEST_CASE(Blend2)
{
    TestBlend128d();
    TestBlend4<__m128>();

#ifdef __AVX2__
    TestBlend4<__m256d>();
    TestBlend256();
#endif // __AVX2__
}



// Broadcast -----------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _index = 0>
void TestBroadcast()
{
    _registerType a = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
        SetValue(a, i, i);

    _registerType b = Broadcast<_index>(a);
    for (U32 i = 0; i < numValuesPerLane<_registerType>; ++i)
    {
        BOOST_CHECK(sse::GetValue(b, i) == Approx(sse::GetValue<_index>(a)));
        if constexpr (numRegisterValues<_registerType> / numValuesPerLane<_registerType> == 2)
            BOOST_CHECK(sse::GetValue(b, i + numValuesPerLane<_registerType>) ==
                        Approx(sse::GetValue<_index + numValuesPerLane<_registerType>>(a)));
    }

    if constexpr (_index + 1 < numValuesPerLane<_registerType>)
        TestBroadcast<_registerType, _index + 1>();
}


BOOST_AUTO_TEST_CASE(Test_Broadcast)
{
    TestBroadcast<__m128>();
    TestBroadcast<__m128d>();
#ifdef __AVX2__
    TestBroadcast<__m256>();
    TestBroadcast<__m256d>();
#endif // __AVX2__
}



// BroadcastAcrossLanes -----------------------------------------------------------------------------------------------

template <typename _registerType, U32 _index = 0>
void TestBroadcastAcrossLanes()
{
    _registerType a = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
        SetValue(a, i, i);

    _registerType b = BroadcastAcrossLanes<_index>(a);
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
        BOOST_CHECK(sse::GetValue(b, i) == Approx(sse::GetValue<_index>(a)));


    if constexpr (_index + 1 < numRegisterValues<_registerType>)
        TestBroadcastAcrossLanes<_registerType, _index + 1>();
}


BOOST_AUTO_TEST_CASE(Test_Broadcast_Across_Lanes)
{
    TestBroadcastAcrossLanes<__m128>();
    TestBroadcastAcrossLanes<__m128d>();
#ifdef __AVX2__
    TestBroadcastAcrossLanes<__m256>();
    TestBroadcastAcrossLanes<__m256d>();
#endif // __AVX2__
}



// Permute ------------------------------------------------------------------------------------------------------------

template <U32 _i, U32 _j>
void TestPermute128d()
{
    const __m128d a = _mmx_setr_p<__m128d>(1.f, 2.f);

    const __m128d b = Permute<_i, _j>(a);

    BOOST_CHECK(sse::GetValue<0>(b) == Approx(sse::GetValue<_i>(a)));
    BOOST_CHECK(sse::GetValue<1>(b) == Approx(sse::GetValue<_j>(a)));
}



template <U32 _i, U32 _j, U32 _k, U32 _l>
void TestPermute128()
{
    const __m128 a = _mmx_setr_p<__m128>(1.f, 2.f, 3.f, 4.f);

    const __m128 b = Permute<_i, _j, _k, _l>(a);

    BOOST_CHECK(sse::GetValue<0>(b) == Approx(sse::GetValue<_i>(a)));
    BOOST_CHECK(sse::GetValue<1>(b) == Approx(sse::GetValue<_j>(a)));
    BOOST_CHECK(sse::GetValue<2>(b) == Approx(sse::GetValue<_k>(a)));
    BOOST_CHECK(sse::GetValue<3>(b) == Approx(sse::GetValue<_l>(a)));
}



#ifdef __AVX2__

template <U32 _i, U32 _j, U32 _k, U32 _l>
void TestPermute256d()
{
    const __m256d a = _mmx_setr_p<__m256d>(1.f, 2.f, 3.f, 4.f);

    const __m256d b = Permute<_i, _j, _k, _l>(a);

    BOOST_CHECK(sse::GetValue<0>(b) == Approx(sse::GetValue<_i>(a)));
    BOOST_CHECK(sse::GetValue<1>(b) == Approx(sse::GetValue<_j>(a)));
    BOOST_CHECK(sse::GetValue<2>(b) == Approx(sse::GetValue<_k + 2>(a)));
    BOOST_CHECK(sse::GetValue<3>(b) == Approx(sse::GetValue<_l + 2>(a)));
}



template <U32 _i, U32 _j, U32 _k, U32 _l, U32 _m, U32 _n, U32 _o, U32 _p>
void TestPermute256()
{
    const __m256 a = _mmx_setr_p<__m256>(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);

    const __m256 b = Permute<_i, _j, _k, _l, _m, _n, _o, _p>(a);

    BOOST_CHECK(sse::GetValue<0>(b) == Approx(sse::GetValue<_i>(a)));
    BOOST_CHECK(sse::GetValue<1>(b) == Approx(sse::GetValue<_j>(a)));
    BOOST_CHECK(sse::GetValue<2>(b) == Approx(sse::GetValue<_k>(a)));
    BOOST_CHECK(sse::GetValue<3>(b) == Approx(sse::GetValue<_l>(a)));
    BOOST_CHECK(sse::GetValue<4>(b) == Approx(sse::GetValue<_m + 4>(a)));
    BOOST_CHECK(sse::GetValue<5>(b) == Approx(sse::GetValue<_n + 4>(a)));
    BOOST_CHECK(sse::GetValue<6>(b) == Approx(sse::GetValue<_o + 4>(a)));
    BOOST_CHECK(sse::GetValue<7>(b) == Approx(sse::GetValue<_p + 4>(a)));


    const __m256 c = Permute<_i, _j, _k, _l>(a);

    BOOST_CHECK(sse::GetValue<0>(c) == Approx(sse::GetValue<_i>(a)));
    BOOST_CHECK(sse::GetValue<1>(c) == Approx(sse::GetValue<_j>(a)));
    BOOST_CHECK(sse::GetValue<2>(c) == Approx(sse::GetValue<_k>(a)));
    BOOST_CHECK(sse::GetValue<3>(c) == Approx(sse::GetValue<_l>(a)));
    BOOST_CHECK(sse::GetValue<4>(c) == Approx(sse::GetValue<_i + 4>(a)));
    BOOST_CHECK(sse::GetValue<5>(c) == Approx(sse::GetValue<_j + 4>(a)));
    BOOST_CHECK(sse::GetValue<6>(c) == Approx(sse::GetValue<_k + 4>(a)));
    BOOST_CHECK(sse::GetValue<7>(c) == Approx(sse::GetValue<_l + 4>(a)));
}
#endif // __AVX2__



BOOST_AUTO_TEST_CASE(Test_Permute)
{
    TestPermute128<1, 3, 2, 2>();
    TestPermute128<2, 3, 1, 0>();
    TestPermute128<0, 0, 3, 1>();
    TestPermute128<1, 1, 1, 3>();
    TestPermute128d<0, 0>();
    TestPermute128d<0, 1>();
    TestPermute128d<1, 0>();
    TestPermute128d<1, 1>();
#ifdef __AVX2__
    TestPermute256d<0, 0, 0, 0>();
    TestPermute256d<0, 1, 0, 0>();
    TestPermute256d<1, 0, 0, 0>();
    TestPermute256d<1, 1, 0, 0>();
    TestPermute256d<0, 0, 1, 0>();
    TestPermute256d<0, 1, 1, 0>();
    TestPermute256d<1, 0, 1, 0>();
    TestPermute256d<1, 1, 1, 0>();
    TestPermute256d<0, 0, 0, 1>();
    TestPermute256d<0, 1, 0, 1>();
    TestPermute256d<1, 0, 0, 1>();
    TestPermute256d<1, 1, 0, 1>();
    TestPermute256d<0, 0, 1, 1>();
    TestPermute256d<0, 1, 1, 1>();
    TestPermute256d<1, 0, 1, 1>();
    TestPermute256d<1, 1, 1, 1>();
    TestPermute256<1, 3, 2, 2, 1, 3, 2, 2>();
    TestPermute256<2, 3, 1, 0, 1, 3, 2, 2>();
    TestPermute256<2, 3, 1, 0, 2, 3, 1, 0>();
    TestPermute256<0, 0, 3, 1, 1, 1, 1, 3>();
#endif // __AVX2__
}



// Permute2F128 -------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <typename _registerType, U32 _lane0 = 0, U32 _lane1 = 0>
void TestPermute2F128SingleRegister()
{
    _registerType a = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
        SetValue(a, i + 1, i);

    _registerType b = Permute2F128<_lane0, _lane1>(a);

    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;

    for (U32 i = 0; i < numLaneVals; ++i)
    {
        BOOST_CHECK(sse::GetValue(b, i) == Approx(sse::GetValue(a, i + _lane0 * numLaneVals)));
        BOOST_CHECK(sse::GetValue(b, i + numLaneVals) == Approx(sse::GetValue(a, i + _lane1 * numLaneVals)));
    }


    constexpr U32 lane0Next = (_lane0 < 1) ? _lane0 + 1 : 0;
    constexpr U32 lane1Next = (lane0Next == 0) ? _lane1 + 1 : _lane1;
    if constexpr (lane1Next < 2)
        TestPermute2F128SingleRegister<_registerType, lane0Next, lane1Next>();
}


template <typename _registerType, U32 _src0 = 0, U32 _lane0 = 0, U32 _src1 = 0, U32 _lane1 = 0>
void TestPermute2F128TwoRegisters()
{
    _registerType a = _mmx_setzero_p<_registerType>();
    _registerType b = _mmx_setzero_p<_registerType>();

    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
    {
        SetValue(a, i + 1, i);
        SetValue(a, i + 1 + numRegisterValues<_registerType>, i);
    }

    _registerType c = Permute2F128<_src0, _lane0, _src1, _lane1>(a, b);

    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;

    for (U32 i = 0; i < numLaneVals; ++i)
    {
        if constexpr (_src0 == 0)
            BOOST_CHECK(sse::GetValue(c, i) == Approx(sse::GetValue(a, i + _lane0 * numLaneVals)));
        else
            BOOST_CHECK(sse::GetValue(c, i) == Approx(sse::GetValue(b, i + _lane0 * numLaneVals)));
        if constexpr (_src1 == 0)
            BOOST_CHECK(sse::GetValue(c, i + numLaneVals) == Approx(sse::GetValue(a, i + _lane1 * numLaneVals)));
        else
            BOOST_CHECK(sse::GetValue(c, i + numLaneVals) == Approx(sse::GetValue(b, i + _lane1 * numLaneVals)));
    }


    constexpr U32 src0Next = (_src0 < 1) ? _src0 + 1 : 0;
    constexpr U32 lane0Next = (src0Next == 0) ? (_lane0 < 1) ? _lane0 + 1 : 0 : _lane0;
    constexpr U32 src1Next = (src0Next == 0 && lane0Next == 0) ? (_src1 < 1) ? _src1 + 1 : 0 : _src1;
    constexpr U32 lane1Next = (src0Next == 0 && lane0Next == 0 && src1Next == 0) ? _lane1 + 1 : _lane1;
    if constexpr (lane1Next < 2)
        TestPermute2F128TwoRegisters<_registerType, src0Next, lane0Next, src1Next, lane1Next>();
}



template <typename _registerType>
void TestPermute2F128()
{
    TestPermute2F128SingleRegister<_registerType>();
    TestPermute2F128TwoRegisters<_registerType>();
}


BOOST_AUTO_TEST_CASE(Test_Permute2F128)
{
    TestPermute2F128<__m256>();
    TestPermute2F128<__m256d>();
}
#endif // __AVX2__



// Shuffle ------------------------------------------------------------------------------------------------------------

#include <iostream>


template <typename _registerType, U32 _i, U32 _j>
void TestShuffle2()
{


    _registerType a = _mmx_setzero_p<_registerType>();
    _registerType b = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
    {
        SetValue(a, i + 1 + numRegisterValues<_registerType>, i);
        SetValue(b, i + 1 + numRegisterValues<_registerType>, i);
    }

    const _registerType c = Shuffle<_i, _j>(a, b);

    for (U32 i = 0; i < numLanes<_registerType>; ++i)
    {
        U32 offset = +i * numValuesPerLane<_registerType>;
        BOOST_CHECK(sse::GetValue(c, 0 + offset) == Approx(sse::GetValue(a, _i + offset)));
        BOOST_CHECK(sse::GetValue(c, 1 + offset) == Approx(sse::GetValue(a, _j + offset)));
    }
}



template <typename _registerType>
void TestShuffle2()
{
    TestShuffle2<_registerType, 0, 0>();
    TestShuffle2<_registerType, 0, 1>();
    TestShuffle2<_registerType, 1, 0>();
    TestShuffle2<_registerType, 1, 1>();
}


#ifdef __AVX2__

template <U32 _i, U32 _j, U32 _k, U32 _l>
void TestShuffle256d()
{

    __m256d a = _mmx_setr_p<__m256d>(1., 2., 3., 4.);
    __m256d b = _mmx_setr_p<__m256d>(5., 6., 7., 8.);

    const __m256d c = Shuffle<_i, _j, _k, _l>(a, b);



    BOOST_CHECK(sse::GetValue(c, 0) == Approx(sse::GetValue(a, _i)));
    BOOST_CHECK(sse::GetValue(c, 1) == Approx(sse::GetValue(b, _j)));
    BOOST_CHECK(sse::GetValue(c, 2) == Approx(sse::GetValue(a, _k + 2)));
    BOOST_CHECK(sse::GetValue(c, 3) == Approx(sse::GetValue(b, _l + 2)));
}



void TestShuffle256d()
{
    TestShuffle256d<0, 0, 0, 0>();
    TestShuffle256d<0, 1, 0, 0>();
    TestShuffle256d<1, 0, 0, 0>();
    TestShuffle256d<1, 1, 0, 0>();

    TestShuffle256d<0, 0, 0, 1>();
    TestShuffle256d<0, 1, 0, 1>();
    TestShuffle256d<1, 0, 0, 1>();
    TestShuffle256d<1, 1, 0, 1>();

    TestShuffle256d<0, 0, 1, 0>();
    TestShuffle256d<0, 1, 1, 0>();
    TestShuffle256d<1, 0, 1, 0>();
    TestShuffle256d<1, 1, 1, 0>();

    TestShuffle256d<0, 0, 1, 1>();
    TestShuffle256d<0, 1, 1, 1>();
    TestShuffle256d<1, 0, 1, 1>();
    TestShuffle256d<1, 1, 1, 1>();
}

#endif // __AVX2__

template <typename _registerType, U32 _i, U32 _j, U32 _k, U32 _l>
void TestShuffle4()
{


    _registerType a = _mmx_setzero_p<_registerType>();
    _registerType b = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
    {
        SetValue(a, i + 1 + numRegisterValues<_registerType>, i);
        SetValue(b, i + 1 + numRegisterValues<_registerType>, i);
    }

    const _registerType c = Shuffle<_i, _j, _k, _l>(a, b);

    for (U32 i = 0; i < numLanes<_registerType>; ++i)
    {
        U32 offset = +i * numValuesPerLane<_registerType>;
        BOOST_CHECK(sse::GetValue(c, 0 + offset) == Approx(sse::GetValue(a, _i + offset)));
        BOOST_CHECK(sse::GetValue(c, 1 + offset) == Approx(sse::GetValue(a, _j + offset)));
        BOOST_CHECK(sse::GetValue(c, 2 + offset) == Approx(sse::GetValue(b, _k + offset)));
        BOOST_CHECK(sse::GetValue(c, 3 + offset) == Approx(sse::GetValue(b, _l + offset)));
    }
}



template <typename _registerType>
void TestShuffle4()
{
    TestShuffle4<_registerType, 1, 3, 2, 2>();
    TestShuffle4<_registerType, 2, 3, 1, 0>();
    TestShuffle4<_registerType, 0, 0, 3, 1>();
    TestShuffle4<_registerType, 1, 1, 1, 3>();
}



BOOST_AUTO_TEST_CASE(Test_Shuffle)
{
    TestShuffle4<__m128>();
    TestShuffle2<__m128d>();
#ifdef __AVX2__
    TestShuffle256d();
    TestShuffle4<__m256>();
    TestShuffle2<__m256d>();
#endif // __AVX2__
}
