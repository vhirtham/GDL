#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/approx.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/swizzle.h"

using namespace GDL;
using namespace GDL::simd;



// AlignRight ---------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _shift = 0>
void TestAlignRight()
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;
    constexpr U32 nLanes = numLanes<_registerType>;
    constexpr U32 numLaneVals = numRegVals / nLanes;

    using Type = decltype(GetDataType<_registerType>());

    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_setzero<_registerType>();
    _registerType exp = _mm_setzero<_registerType>();

    for (U32 i = 0; i < numRegVals; ++i)
    {
        SetValue(a, i, static_cast<Type>(i + 1.25));
        SetValue(b, i, static_cast<Type>(i + numRegVals + 1.33));
    }

    for (U32 i = 0; i < numLaneVals; ++i)
        for (U32 j = 0; j < nLanes; ++j)
            if (i < numLaneVals - _shift)
                exp[i + j * numLaneVals] = b[i + _shift + j * numLaneVals];
            else
                exp[i + j * numLaneVals] = a[i - numLaneVals + _shift + j * numLaneVals];

    _registerType c = AlignRight<_shift>(a, b);

    for (U32 i = 0; i < numRegVals; ++i)
        BOOST_CHECK(GetValue(c, i) == Approx(GetValue(exp, i)));

    if constexpr (_shift < numLaneVals)
        TestAlignRight<_registerType, _shift + 1>();
}



BOOST_AUTO_TEST_CASE(AlignRight_m128)
{
    TestAlignRight<__m128>();
}



BOOST_AUTO_TEST_CASE(AlignRight_m128d)
{
    TestAlignRight<__m128d>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(AlignRight_m256)
{
    TestAlignRight<__m256>();
}



BOOST_AUTO_TEST_CASE(AlignRight_m256d)
{
    TestAlignRight<__m256d>();
}

#endif // __AVX2__



// Blend --------------------------------------------------------------------------------------------------------------

template <U32 _i = 0, U32 _j = 0>
void TestBlend128d()
{
    const __m128d a = _mm_setr_pd(1., 2.);
    const __m128d b = _mm_setr_pd(3., 4.);

    const __m128d c = Blend<_i, _j>(a, b);

    BOOST_CHECK(simd::GetValue<0>(c) == Approx((_i == 0) ? 1.f : 3.f));
    BOOST_CHECK(simd::GetValue<1>(c) == Approx((_j == 0) ? 2.f : 4.f));

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
    const _registerType a = _mm_setr<_registerType>(1.f, 2.f, 3.f, 4.f);
    const _registerType b = _mm_setr<_registerType>(5.f, 6.f, 7.f, 8.f);

    const _registerType c = Blend<_i, _j, _k, _l>(a, b);


    BOOST_CHECK(simd::GetValue<0>(c) == Approx((_i == 0) ? 1.f : 5.f));
    BOOST_CHECK(simd::GetValue<1>(c) == Approx((_j == 0) ? 2.f : 6.f));
    BOOST_CHECK(simd::GetValue<2>(c) == Approx((_k == 0) ? 3.f : 7.f));
    BOOST_CHECK(simd::GetValue<3>(c) == Approx((_l == 0) ? 4.f : 8.f));


    constexpr U32 iNext = (_i < 1) ? _i + 1 : 0;
    constexpr U32 jNext = (iNext == 0) ? (_j < 1) ? _j + 1 : 0 : _j;
    constexpr U32 kNext = (iNext == 0 && jNext == 0) ? (_k < 1) ? _k + 1 : 0 : _k;
    constexpr U32 lNext = (iNext == 0 && jNext == 0 && kNext == 0) ? _l + 1 : _l;
    if constexpr (lNext < 2)
    {
        TestBlend4<_registerType, iNext, jNext, kNext, lNext>();
    }
}


#ifdef __AVX2__

template <U32 _i = 0, U32 _j = 0, U32 _k = 0, U32 _l = 0, U32 _m = 0, U32 _n = 0, U32 _o = 0, U32 _p = 0>
void TestBlend256()
{
    const __m256 a = _mm256_setr_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
    const __m256 b = _mm256_setr_ps(9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f);

    const __m256 c = Blend<_i, _j, _k, _l, _m, _n, _o, _p>(a, b);


    BOOST_CHECK(simd::GetValue<0>(c) == Approx((_i == 0) ? 1.f : 9.f));
    BOOST_CHECK(simd::GetValue<1>(c) == Approx((_j == 0) ? 2.f : 10.f));
    BOOST_CHECK(simd::GetValue<2>(c) == Approx((_k == 0) ? 3.f : 11.f));
    BOOST_CHECK(simd::GetValue<3>(c) == Approx((_l == 0) ? 4.f : 12.f));
    BOOST_CHECK(simd::GetValue<4>(c) == Approx((_m == 0) ? 5.f : 13.f));
    BOOST_CHECK(simd::GetValue<5>(c) == Approx((_n == 0) ? 6.f : 14.f));
    BOOST_CHECK(simd::GetValue<6>(c) == Approx((_o == 0) ? 7.f : 15.f));
    BOOST_CHECK(simd::GetValue<7>(c) == Approx((_p == 0) ? 8.f : 16.f));


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

#endif // __AVX2__



BOOST_AUTO_TEST_CASE(Blend2)
{
    TestBlend128d();
    TestBlend4<__m128>();

#ifdef __AVX2__
    TestBlend4<__m256d>();
    TestBlend256();
#endif // __AVX2__
}


// Blend Index --------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _index = 0>
void TestBlendIndex()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_set1<_registerType>(1);

    _registerType c = BlendIndex<_index>(a, b);
    for (U32 i = 0; i < numRegVals; ++i)
        if (i == _index)
            BOOST_CHECK(GetValue(c, i) == Approx<Type>(1));
        else
            BOOST_CHECK(GetValue(c, i) == ApproxZero<Type>());

    if constexpr (_index + 1 < numRegVals)
        TestBlendIndex<_registerType, _index + 1>();
}

BOOST_AUTO_TEST_CASE(Test_Blend_Index)
{
    TestBlendIndex<__m128>();
    TestBlendIndex<__m128d>();

#ifdef __AVX2__
    TestBlendIndex<__m256>();
    TestBlendIndex<__m256d>();
#endif // __AVX2__
}



// Blend Above Index --------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _index = 0>
void TestBlendAboveIndex()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_set1<_registerType>(1);

    _registerType c = BlendAboveIndex<_index>(a, b);
    for (U32 i = 0; i < numRegVals; ++i)
        if (i < _index)
            BOOST_CHECK(GetValue(c, i) == Approx<Type>(1));
        else
            BOOST_CHECK(GetValue(c, i) == ApproxZero<Type>());

    if constexpr (_index + 1 < numRegVals)
        TestBlendAboveIndex<_registerType, _index + 1>();
}

BOOST_AUTO_TEST_CASE(Test_Blend_Above_Index)
{
    TestBlendAboveIndex<__m128>();
    TestBlendAboveIndex<__m128d>();

#ifdef __AVX2__
    TestBlendAboveIndex<__m256>();
    TestBlendAboveIndex<__m256d>();
#endif // __AVX2__
}



// Blend Below Index --------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _index = 0>
void TestBlendBelowIndex()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_set1<_registerType>(1);

    _registerType c = BlendBelowIndex<_index>(a, b);
    for (U32 i = 0; i < numRegVals; ++i)
        if (i > _index)
            BOOST_CHECK(GetValue(c, i) == Approx<Type>(1));
        else
            BOOST_CHECK(GetValue(c, i) == ApproxZero<Type>());

    if constexpr (_index + 1 < numRegVals)
        TestBlendAboveIndex<_registerType, _index + 1>();
}

BOOST_AUTO_TEST_CASE(Test_Blend_Below_Index)
{
    TestBlendBelowIndex<__m128>();
    TestBlendBelowIndex<__m128d>();

#ifdef __AVX2__
    TestBlendBelowIndex<__m256>();
    TestBlendBelowIndex<__m256d>();
#endif // __AVX2__
}



// Blend in range -----------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _idxFirst = 0, U32 _idxLast = 0>
void TestBlendInRange()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_set1<_registerType>(1);

    _registerType c = BlendInRange<_idxFirst, _idxLast>(a, b);
    for (U32 i = 0; i < numRegVals; ++i)
        if (i >= _idxFirst && i <= _idxLast)
            BOOST_CHECK(GetValue(c, i) == Approx<Type>(1));
        else
            BOOST_CHECK(GetValue(c, i) == ApproxZero<Type>());

    if constexpr (_idxLast + 1 < numRegVals)
        TestBlendInRange<_registerType, _idxFirst, _idxLast + 1>();
    else if constexpr (_idxFirst + 1 < numRegVals)
        TestBlendInRange<_registerType, _idxFirst + 1, _idxFirst + 1>();
}

BOOST_AUTO_TEST_CASE(Test_Blend_In_Range)
{
    TestBlendInRange<__m128>();
    TestBlendInRange<__m128d>();

#ifdef __AVX2__
    TestBlendInRange<__m256>();
    TestBlendInRange<__m256d>();
#endif // __AVX2__
}



// Broadcast -----------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _index = 0>
void TestBroadcast()
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;
    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;

    _registerType a = _mm_setzero<_registerType>();

    for (U32 i = 0; i < numRegVals; ++i)
        SetValue(a, i, i);

    _registerType b = Broadcast<_index>(a);
    for (U32 i = 0; i < numLaneVals; ++i)
    {
        BOOST_CHECK(simd::GetValue(b, i) == Approx(simd::GetValue<_index>(a)));
        if constexpr (numLanes<_registerType> == 2)
            BOOST_CHECK(simd::GetValue(b, i + numLaneVals) == Approx(simd::GetValue<_index + numLaneVals>(a)));
    }

    if constexpr (_index + 1 < numValuesPerLane<_registerType>)
        TestBroadcast<_registerType, _index + 1>();
}



#ifdef __AVX2__

template <typename _registerType, U32 _idx0 = 0, U32 _idx1 = 0>
void TestBroadcast2Lanes()
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;
    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;

    _registerType a = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegVals; ++i)
        SetValue(a, i, i);

    _registerType b = Broadcast<_idx0, _idx1>(a);
    for (U32 i = 0; i < numLaneVals; ++i)
    {
        BOOST_CHECK(simd::GetValue(b, i) == Approx(simd::GetValue<_idx0>(a)));
        BOOST_CHECK(simd::GetValue(b, i + numLaneVals) == Approx(simd::GetValue<_idx1 + numLaneVals>(a)));
    }

    if constexpr (_idx0 + 1 < numValuesPerLane<_registerType>)
        TestBroadcast2Lanes<_registerType, _idx0 + 1, _idx1>();
    else if constexpr (_idx1 + 1 < numValuesPerLane<_registerType>)
        TestBroadcast2Lanes<_registerType, 0, _idx1 + 1>();
}
#endif // __AVX2__

BOOST_AUTO_TEST_CASE(Test_Broadcast)
{
    TestBroadcast<__m128>();
    TestBroadcast<__m128d>();
#ifdef __AVX2__
    TestBroadcast<__m256>();
    TestBroadcast<__m256d>();
    TestBroadcast2Lanes<__m256>();
    TestBroadcast2Lanes<__m256d>();
#endif // __AVX2__
}



// BroadcastAcrossLanes -----------------------------------------------------------------------------------------------

template <typename _registerType, U32 _index = 0>
void TestBroadcastAcrossLanes()
{
    _registerType a = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
        SetValue(a, i, i);

    _registerType b = BroadcastAcrossLanes<_index>(a);
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
        BOOST_CHECK(simd::GetValue(b, i) == Approx(simd::GetValue<_index>(a)));


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



// Exchange -----------------------------------------------------------------------------------------------------------

#include <iostream>

template <typename _registerType, U32 _idx0, U32 _idx1>
void TestExchangeTest()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_setzero<_registerType>();

    for (U32 i = 0; i < numRegVals; ++i)
    {
        SetValue(a, i, i);
        SetValue(b, i, i + numRegVals);
    }

    _registerType aRef = a;
    _registerType bRef = b;

    Exchange<_idx0, _idx1>(a, b);

    for (U32 i = 0; i < numRegVals; ++i)
    {
        Type aV = GetValue(a, i);
        if (i == _idx0)
            BOOST_CHECK(aV == Approx(GetValue<_idx1>(bRef)));
        else
            BOOST_CHECK(aV == Approx(GetValue(a, i)));


        Type bV = GetValue(b, i);
        if (i == _idx1)
            BOOST_CHECK(bV == Approx(GetValue<_idx0>(aRef)));
        else
            BOOST_CHECK(bV == Approx(GetValue(b, i)));
    }
}



template <typename _registerType, U32 _idx0 = 0>
void TestExchange()
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    TestExchangeTest<_registerType, _idx0, 0>();
    TestExchangeTest<_registerType, _idx0, 1>();

    if constexpr (numRegVals > 2)
    {
        TestExchangeTest<_registerType, _idx0, 2>();
        TestExchangeTest<_registerType, _idx0, 3>();
    }

    if constexpr (numRegVals > 4)
    {
        TestExchangeTest<_registerType, _idx0, 4>();
        TestExchangeTest<_registerType, _idx0, 5>();
        TestExchangeTest<_registerType, _idx0, 6>();
        TestExchangeTest<_registerType, _idx0, 7>();
    }

    if constexpr (_idx0 + 1 < numRegVals)
        TestExchange<_registerType, _idx0 + 1>();
}



BOOST_AUTO_TEST_CASE(Test_Exchange)
{
    TestExchange<__m128>();
    TestExchange<__m128d>();
#ifdef __AVX2__
    TestExchange<__m256>();
    TestExchange<__m256d>();
#endif // __AVX2__
}



// Insert -------------------------------------------------------------------------------------------------------------

template <U32 _idxSrc = 0, U32 _idxDst = 0>
void TestInsert()
{
    __m128 a = _mm_setr<__m128>(1, 2, 3, 4);
    __m128 b = _mm_setr<__m128>(5, 6, 7, 8);

    __m128 c = Insert<_idxSrc, _idxDst>(a, b);

    for (U32 i = 0; i < 4; ++i)
        if (i == _idxDst)
            BOOST_CHECK_MESSAGE(GetValue<_idxSrc>(a) == Approx(GetValue(c, i)),
                                "Source value is not copied to destination");
        else
            BOOST_CHECK_MESSAGE(GetValue(b, i) == Approx(GetValue(c, i)),
                                "Wrong value of destination register modified");

    if constexpr (_idxSrc + 1 < 4)
        TestInsert<_idxSrc + 1, _idxDst>();
    else if constexpr (_idxDst + 1 < 4)
        TestInsert<0, _idxDst + 1>();
}



template <bool _setZeroIdx0 = false, bool _setZeroIdx1 = false, bool _setZeroIdx2 = false, bool _setZeroIdx3 = false>
void TestInsertSetZero()
{
    __m128 a = _mm_setr<__m128>(1, 2, 3, 4);
    __m128 b = _mm_setr<__m128>(5, 6, 7, 8);

    __m128 c = Insert<3, 1, _setZeroIdx0, _setZeroIdx1, _setZeroIdx2, _setZeroIdx3>(a, b);
    __m128 d = Insert<1, 3, _setZeroIdx0, _setZeroIdx1, _setZeroIdx2, _setZeroIdx3>(a, b);

    if constexpr (_setZeroIdx0)
    {
        BOOST_CHECK(GetValue<0>(c) == ApproxZero<F32>());
        BOOST_CHECK(GetValue<0>(d) == ApproxZero<F32>());
    }
    else
    {
        BOOST_CHECK(GetValue<0>(c) >= 1.f);
        BOOST_CHECK(GetValue<0>(d) >= 1.f);
    }

    if constexpr (_setZeroIdx1)
    {
        BOOST_CHECK(GetValue<1>(c) == ApproxZero<F32>());
        BOOST_CHECK(GetValue<1>(d) == ApproxZero<F32>());
    }
    else
    {
        BOOST_CHECK(GetValue<1>(c) >= 1.f);
        BOOST_CHECK(GetValue<1>(d) >= 1.f);
    }

    if constexpr (_setZeroIdx2)
    {
        BOOST_CHECK(GetValue<2>(c) == ApproxZero<F32>());
        BOOST_CHECK(GetValue<2>(d) == ApproxZero<F32>());
    }
    else
    {
        BOOST_CHECK(GetValue<2>(c) >= 1.f);
        BOOST_CHECK(GetValue<2>(d) >= 1.f);
    }

    if constexpr (_setZeroIdx3)
    {
        BOOST_CHECK(GetValue<3>(c) == ApproxZero<F32>());
        BOOST_CHECK(GetValue<3>(d) == ApproxZero<F32>());
    }
    else
    {
        BOOST_CHECK(GetValue<3>(c) >= 1.f);
        BOOST_CHECK(GetValue<3>(d) >= 1.f);
    }


    if constexpr (not _setZeroIdx0)
        TestInsertSetZero<true, _setZeroIdx1, _setZeroIdx2, _setZeroIdx3>();
    else if constexpr (not _setZeroIdx1)
        TestInsertSetZero<false, true, _setZeroIdx2, _setZeroIdx3>();
    else if constexpr (not _setZeroIdx2)
        TestInsertSetZero<false, false, true, _setZeroIdx3>();
    else if constexpr (not _setZeroIdx3)
        TestInsertSetZero<false, false, false, true>();
}



BOOST_AUTO_TEST_CASE(Test_Insert)
{
    TestInsert();
}



BOOST_AUTO_TEST_CASE(Test_Insert_SetZero)
{
    TestInsertSetZero();
}



// Permute ------------------------------------------------------------------------------------------------------------

template <U32 _i, U32 _j>
void TestPermute128d()
{
    const __m128d a = _mm_setr<__m128d>(1.f, 2.f);

    const __m128d b = Permute<_i, _j>(a);

    BOOST_CHECK(simd::GetValue<0>(b) == Approx(simd::GetValue<_i>(a)));
    BOOST_CHECK(simd::GetValue<1>(b) == Approx(simd::GetValue<_j>(a)));
}



template <U32 _i, U32 _j, U32 _k, U32 _l>
void TestPermute128()
{
    const __m128 a = _mm_setr<__m128>(1.f, 2.f, 3.f, 4.f);

    const __m128 b = Permute<_i, _j, _k, _l>(a);

    BOOST_CHECK(simd::GetValue<0>(b) == Approx(simd::GetValue<_i>(a)));
    BOOST_CHECK(simd::GetValue<1>(b) == Approx(simd::GetValue<_j>(a)));
    BOOST_CHECK(simd::GetValue<2>(b) == Approx(simd::GetValue<_k>(a)));
    BOOST_CHECK(simd::GetValue<3>(b) == Approx(simd::GetValue<_l>(a)));
}



#ifdef __AVX2__

template <U32 _i, U32 _j, U32 _k, U32 _l>
void TestPermute256d()
{
    const __m256d a = _mm_setr<__m256d>(1.f, 2.f, 3.f, 4.f);

    const __m256d b = Permute<_i, _j, _k, _l>(a);

    BOOST_CHECK(simd::GetValue<0>(b) == Approx(simd::GetValue<_i>(a)));
    BOOST_CHECK(simd::GetValue<1>(b) == Approx(simd::GetValue<_j>(a)));
    BOOST_CHECK(simd::GetValue<2>(b) == Approx(simd::GetValue<_k + 2>(a)));
    BOOST_CHECK(simd::GetValue<3>(b) == Approx(simd::GetValue<_l + 2>(a)));

    const __m256d c = Permute<_i, _j>(a);

    BOOST_CHECK(simd::GetValue<0>(c) == Approx(simd::GetValue<_i>(a)));
    BOOST_CHECK(simd::GetValue<1>(c) == Approx(simd::GetValue<_j>(a)));
    BOOST_CHECK(simd::GetValue<2>(c) == Approx(simd::GetValue<_i + 2>(a)));
    BOOST_CHECK(simd::GetValue<3>(c) == Approx(simd::GetValue<_j + 2>(a)));
}



template <U32 _i, U32 _j, U32 _k, U32 _l, U32 _m, U32 _n, U32 _o, U32 _p>
void TestPermute256()
{
    const __m256 a = _mm_setr<__m256>(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);

    const __m256 b = Permute<_i, _j, _k, _l, _m, _n, _o, _p>(a);

    BOOST_CHECK(simd::GetValue<0>(b) == Approx(simd::GetValue<_i>(a)));
    BOOST_CHECK(simd::GetValue<1>(b) == Approx(simd::GetValue<_j>(a)));
    BOOST_CHECK(simd::GetValue<2>(b) == Approx(simd::GetValue<_k>(a)));
    BOOST_CHECK(simd::GetValue<3>(b) == Approx(simd::GetValue<_l>(a)));
    BOOST_CHECK(simd::GetValue<4>(b) == Approx(simd::GetValue<_m + 4>(a)));
    BOOST_CHECK(simd::GetValue<5>(b) == Approx(simd::GetValue<_n + 4>(a)));
    BOOST_CHECK(simd::GetValue<6>(b) == Approx(simd::GetValue<_o + 4>(a)));
    BOOST_CHECK(simd::GetValue<7>(b) == Approx(simd::GetValue<_p + 4>(a)));


    const __m256 c = Permute<_i, _j, _k, _l>(a);

    BOOST_CHECK(simd::GetValue<0>(c) == Approx(simd::GetValue<_i>(a)));
    BOOST_CHECK(simd::GetValue<1>(c) == Approx(simd::GetValue<_j>(a)));
    BOOST_CHECK(simd::GetValue<2>(c) == Approx(simd::GetValue<_k>(a)));
    BOOST_CHECK(simd::GetValue<3>(c) == Approx(simd::GetValue<_l>(a)));
    BOOST_CHECK(simd::GetValue<4>(c) == Approx(simd::GetValue<_i + 4>(a)));
    BOOST_CHECK(simd::GetValue<5>(c) == Approx(simd::GetValue<_j + 4>(a)));
    BOOST_CHECK(simd::GetValue<6>(c) == Approx(simd::GetValue<_k + 4>(a)));
    BOOST_CHECK(simd::GetValue<7>(c) == Approx(simd::GetValue<_l + 4>(a)));
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
    _registerType a = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
        SetValue(a, i, i + 1);

    _registerType b = Permute2F128<_lane0, _lane1>(a);

    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;

    for (U32 i = 0; i < numLaneVals; ++i)
    {
        BOOST_CHECK(simd::GetValue(b, i) == Approx(simd::GetValue(a, i + _lane0 * numLaneVals)));
        BOOST_CHECK(simd::GetValue(b, i + numLaneVals) == Approx(simd::GetValue(a, i + _lane1 * numLaneVals)));
    }


    constexpr U32 lane0Next = (_lane0 < 1) ? _lane0 + 1 : 0;
    constexpr U32 lane1Next = (lane0Next == 0) ? _lane1 + 1 : _lane1;
    if constexpr (lane1Next < 2)
        TestPermute2F128SingleRegister<_registerType, lane0Next, lane1Next>();
}


template <typename _registerType, U32 _src0 = 0, U32 _lane0 = 0, U32 _src1 = 0, U32 _lane1 = 0>
void TestPermute2F128TwoRegisters()
{
    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_setzero<_registerType>();

    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
    {
        SetValue(a, i, i + 1);
        SetValue(a, i, i + 1 + numRegisterValues<_registerType>);
    }

    _registerType c = Permute2F128<_src0, _lane0, _src1, _lane1>(a, b);

    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;

    for (U32 i = 0; i < numLaneVals; ++i)
    {
        if constexpr (_src0 == 0)
            BOOST_CHECK(simd::GetValue(c, i) == Approx(simd::GetValue(a, i + _lane0 * numLaneVals)));
        else
            BOOST_CHECK(simd::GetValue(c, i) == Approx(simd::GetValue(b, i + _lane0 * numLaneVals)));
        if constexpr (_src1 == 0)
            BOOST_CHECK(simd::GetValue(c, i + numLaneVals) == Approx(simd::GetValue(a, i + _lane1 * numLaneVals)));
        else
            BOOST_CHECK(simd::GetValue(c, i + numLaneVals) == Approx(simd::GetValue(b, i + _lane1 * numLaneVals)));
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



// Permute4F64 --------------------------------------------------------------------------------------------------------

template <U32 _i0, U32 _i1, U32 _i2, U32 _i3>
void TestPermute4F64()
{
    __m256d a = _mm_setr<__m256d>(1, 2, 3, 4);
    __m256d b = Permute4F64<_i0, _i1, _i2, _i3>(a);

    BOOST_CHECK(GetValue<0>(b) == Approx(GetValue<_i0>(a)));
    BOOST_CHECK(GetValue<1>(b) == Approx(GetValue<_i1>(a)));
    BOOST_CHECK(GetValue<2>(b) == Approx(GetValue<_i2>(a)));
    BOOST_CHECK(GetValue<3>(b) == Approx(GetValue<_i3>(a)));
}



BOOST_AUTO_TEST_CASE(Test_Permute4F64)
{
    TestPermute4F64<0, 0, 0, 0>();
    TestPermute4F64<1, 1, 1, 1>();
    TestPermute4F64<2, 2, 2, 2>();
    TestPermute4F64<3, 3, 3, 3>();
    TestPermute4F64<3, 2, 1, 0>();
    TestPermute4F64<0, 1, 0, 1>();
    TestPermute4F64<2, 3, 2, 3>();
    TestPermute4F64<3, 1, 0, 2>();
    TestPermute4F64<1, 3, 2, 3>();
}



// Permute8F32 --------------------------------------------------------------------------------------------------------

template <U32 _i0, U32 _i1, U32 _i2, U32 _i3, U32 _i4, U32 _i5, U32 _i6, U32 _i7>
void TestPermute8F32()
{
    __m256 a = _mm_setr<__m256>(1, 2, 3, 4, 5, 6, 7, 8);
    __m256 b = Permute8F32<_i0, _i1, _i2, _i3, _i4, _i5, _i6, _i7>(a);

    BOOST_CHECK(GetValue<0>(b) == Approx(GetValue<_i0>(a)));
    BOOST_CHECK(GetValue<1>(b) == Approx(GetValue<_i1>(a)));
    BOOST_CHECK(GetValue<2>(b) == Approx(GetValue<_i2>(a)));
    BOOST_CHECK(GetValue<3>(b) == Approx(GetValue<_i3>(a)));
    BOOST_CHECK(GetValue<4>(b) == Approx(GetValue<_i4>(a)));
    BOOST_CHECK(GetValue<5>(b) == Approx(GetValue<_i5>(a)));
    BOOST_CHECK(GetValue<6>(b) == Approx(GetValue<_i6>(a)));
    BOOST_CHECK(GetValue<7>(b) == Approx(GetValue<_i7>(a)));
}



BOOST_AUTO_TEST_CASE(Test_Permute8F32)
{
    TestPermute8F32<0, 0, 0, 0, 0, 0, 0, 0>();
    TestPermute8F32<1, 1, 1, 1, 1, 1, 1, 1>();
    TestPermute8F32<2, 2, 2, 2, 2, 2, 2, 2>();
    TestPermute8F32<3, 3, 3, 3, 3, 3, 3, 3>();
    TestPermute8F32<4, 4, 4, 4, 4, 4, 4, 4>();
    TestPermute8F32<5, 5, 5, 5, 5, 5, 5, 5>();
    TestPermute8F32<6, 6, 6, 6, 6, 6, 6, 6>();
    TestPermute8F32<7, 7, 7, 7, 7, 7, 7, 7>();
    TestPermute8F32<0, 1, 2, 3, 4, 5, 6, 7>();
    TestPermute8F32<1, 2, 3, 0, 1, 2, 3, 0>();
    TestPermute8F32<5, 6, 7, 4, 5, 6, 7, 4>();
    TestPermute8F32<1, 6, 4, 3, 2, 0, 7, 5>();
    TestPermute8F32<5, 7, 6, 4, 1, 0, 3, 2>();
}

#endif // __AVX2__



// Shuffle ------------------------------------------------------------------------------------------------------------

template <typename _registerType, U32 _i, U32 _j>
void TestShuffle2()
{


    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
    {
        SetValue(a, i, i + 1 + numRegisterValues<_registerType>);
        SetValue(b, i, i + 1 + numRegisterValues<_registerType>);
    }

    const _registerType c = Shuffle<_i, _j>(a, b);

    for (U32 i = 0; i < numLanes<_registerType>; ++i)
    {
        U32 offset = +i * numValuesPerLane<_registerType>;
        BOOST_CHECK(simd::GetValue(c, 0 + offset) == Approx(simd::GetValue(a, _i + offset)));
        BOOST_CHECK(simd::GetValue(c, 1 + offset) == Approx(simd::GetValue(a, _j + offset)));
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

    __m256d a = _mm_setr<__m256d>(1., 2., 3., 4.);
    __m256d b = _mm_setr<__m256d>(5., 6., 7., 8.);

    const __m256d c = Shuffle<_i, _j, _k, _l>(a, b);



    BOOST_CHECK(simd::GetValue(c, 0) == Approx(simd::GetValue(a, _i)));
    BOOST_CHECK(simd::GetValue(c, 1) == Approx(simd::GetValue(b, _j)));
    BOOST_CHECK(simd::GetValue(c, 2) == Approx(simd::GetValue(a, _k + 2)));
    BOOST_CHECK(simd::GetValue(c, 3) == Approx(simd::GetValue(b, _l + 2)));
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


    _registerType a = _mm_setzero<_registerType>();
    _registerType b = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
    {
        SetValue(a, i, i + 1 + numRegisterValues<_registerType>);
        SetValue(b, i, i + 1 + numRegisterValues<_registerType>);
    }

    const _registerType c = Shuffle<_i, _j, _k, _l>(a, b);

    for (U32 i = 0; i < numLanes<_registerType>; ++i)
    {
        U32 offset = +i * numValuesPerLane<_registerType>;
        BOOST_CHECK(simd::GetValue(c, 0 + offset) == Approx(simd::GetValue(a, _i + offset)));
        BOOST_CHECK(simd::GetValue(c, 1 + offset) == Approx(simd::GetValue(a, _j + offset)));
        BOOST_CHECK(simd::GetValue(c, 2 + offset) == Approx(simd::GetValue(b, _k + offset)));
        BOOST_CHECK(simd::GetValue(c, 3 + offset) == Approx(simd::GetValue(b, _l + offset)));
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



// Swap ---------------------------------------------------------------------------------------------------------------


template <typename _registerType, U32 _idx0, U32 _idx1>
void TestSwapTest()
{
    using Type = decltype(GetDataType<_registerType>());
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    _registerType a = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegVals; ++i)
        SetValue(a, i, i);

    _registerType b = Swap<_idx0, _idx1>(a);

    for (U32 i = 0; i < numRegVals; ++i)
    {
        Type bV = GetValue(b, i);

        switch (i)
        {
        case _idx0:
            BOOST_CHECK(bV == Approx(GetValue<_idx1>(a)));
            break;
        case _idx1:
            BOOST_CHECK(bV == Approx(GetValue<_idx0>(a)));
            break;
        default:
            BOOST_CHECK(bV == Approx(GetValue(a, i)));
        }
    }
}



template <typename _registerType, U32 _idx0 = 0>
void TestSwap()
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    if constexpr (_idx0 != 0)
        TestSwapTest<_registerType, _idx0, 0>();
    if constexpr (_idx0 != 1)
        TestSwapTest<_registerType, _idx0, 1>();

    if constexpr (numRegVals > 2)
    {
        if constexpr (_idx0 != 2)
            TestSwapTest<_registerType, _idx0, 2>();
        if constexpr (_idx0 != 3)
            TestSwapTest<_registerType, _idx0, 3>();
    }

    if constexpr (numRegVals > 4)
    {
        if constexpr (_idx0 != 4)
            TestSwapTest<_registerType, _idx0, 4>();
        if constexpr (_idx0 != 5)
            TestSwapTest<_registerType, _idx0, 5>();
        if constexpr (_idx0 != 6)
            TestSwapTest<_registerType, _idx0, 6>();
        if constexpr (_idx0 != 7)
            TestSwapTest<_registerType, _idx0, 7>();
    }


    if constexpr (_idx0 + 1 < numRegVals)
        TestSwap<_registerType, _idx0 + 1>();
}


BOOST_AUTO_TEST_CASE(Test_Swap)
{
    TestSwap<__m128>();
    TestSwap<__m128d>();
#ifdef __AVX2__
    TestSwap<__m256>();
    TestSwap<__m256d>();
#endif // __AVX2__
}



// SwapLanes and SwapLanesIf ------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <typename _registerType>
void TestSwapLanes()
{
    using namespace simd;

    _registerType a = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegisterValues<_registerType>; ++i)
        SetValue(a, i, i + 1);

    _registerType b = SwapLanes(a);
    _registerType c = SwapLanesIf<true>(a);
    _registerType d = SwapLanesIf<false>(a);

    constexpr U32 numLaneVals = numValuesPerLane<_registerType>;

    for (U32 i = 0; i < numLaneVals; ++i)
    {
        BOOST_CHECK_MESSAGE(GetValue(b, i) == Approx(GetValue(a, i + numLaneVals)), "SwapLanes didn't swap lanes");
        BOOST_CHECK_MESSAGE(GetValue(b, i + numLaneVals) == Approx(GetValue(a, i)), "SwapLanes didn't swap lanes");

        BOOST_CHECK_MESSAGE(GetValue(c, i) == Approx(GetValue(a, i + numLaneVals)),
                            "SwapLanesIf<true> didn't swap lanes");
        BOOST_CHECK_MESSAGE(GetValue(c, i + numLaneVals) == Approx(GetValue(a, i)),
                            "SwapLanesIf<true> didn't swap lanes");

        BOOST_CHECK_MESSAGE(GetValue(d, i) == Approx(GetValue(a, i)), "SwapLanesIf<false> swapped lanes");
        BOOST_CHECK_MESSAGE(GetValue(d, i + numLaneVals) == Approx(GetValue(a, i + numLaneVals)),
                            "SwapLanesIf<false> swapped lanes");
    }
}


BOOST_AUTO_TEST_CASE(Test_Swap_Lanes)
{
    TestSwapLanes<__m256d>();
    TestSwapLanes<__m256>();
}


#endif // __AVX2__
