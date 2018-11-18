#include <boost/test/unit_test.hpp>

#include "gdl/base/sse/compareAll.h"

using namespace GDL;



// Test CompareAllEqual ----------------------------------------------------------------------------------------------

//! @brief Tests the sse::CompareAllEqual function with varying number of compared values.
template <typename _registerType, U32 _count = 1>
void TestCompareAllEqual()
{
    using DataType = decltype(sse::GetDataType<_registerType>());
    constexpr U32 numRegisterEntries = sse::numRegisterValues<_registerType>;

    auto compEQ = [](_registerType lhs, _registerType rhs) {
        return sse::CompareAllEqual<_registerType, _count>(lhs, rhs);
    };


    alignas(sse::alignmentBytes<_registerType>) _registerType lhs = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterEntries; ++i)
        lhs[i] = static_cast<DataType>(i + 1);
    alignas(sse::alignmentBytes<_registerType>) _registerType rhs;


    // all values return true
    BOOST_CHECK(compEQ(lhs, lhs));

    // all values return false
    BOOST_CHECK(!compEQ(lhs, _mmx_setzero_p<_registerType>()));


    // Maximal 1 used value returns false
    for (U32 i = 0; i < _count; ++i)
    {
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            // all unused values return true
            rhs = lhs;
            BOOST_CHECK(compEQ(lhs, rhs));
            rhs[i] = lhs[i] + std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compEQ(lhs, rhs));
            rhs[i] = lhs[i] - std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compEQ(lhs, rhs));

            // one unused value returns false
            rhs = lhs;
            rhs[j] = 0;
            BOOST_CHECK(compEQ(lhs, rhs));
            rhs[i] = lhs[i] + std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compEQ(lhs, rhs));
            rhs[i] = lhs[i] - std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compEQ(lhs, rhs));
        }

        // Up to all unused values return false
        rhs = lhs;
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            rhs[j] = 0;

            rhs[i] = lhs[i] + std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compEQ(lhs, rhs));
            rhs[i] = lhs[i] - std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compEQ(lhs, rhs));
            rhs[i] = lhs[i];
            BOOST_CHECK(compEQ(lhs, rhs));
        }
    }

    // maximal all used values return false
    for (U32 j = _count; j < numRegisterEntries; ++j)
    {
        rhs = lhs;
        for (U32 i = 0; i < _count; ++i)
        {
            // all unused values return true
            rhs[i] = 0;
            BOOST_CHECK(!compEQ(lhs, rhs));

            // one unused value returns false
            rhs[j] = 0;
            BOOST_CHECK(!compEQ(lhs, rhs));
        }
    }


    if constexpr (_count < numRegisterEntries)
        TestCompareAllEqual<_registerType, _count + 1>();
}



BOOST_AUTO_TEST_CASE(All_Equal)
{
    TestCompareAllEqual<__m128>();
    TestCompareAllEqual<__m128d>();
#ifdef __AVX2__
    TestCompareAllEqual<__m256>();
    TestCompareAllEqual<__m256d>();
#endif // __AVX2__
}



// Test CompareAllLessEqual -------------------------------------------------------------------------------------------

//! @brief Tests the sse::CompareAllLessEqual function with varying number of compared values.
template <typename _registerType, U32 _count = 1>
void TestCompareAllLessEqual()
{
    using DataType = decltype(sse::GetDataType<_registerType>());
    constexpr U32 numRegisterEntries = sse::numRegisterValues<_registerType>;

    auto compLE = [](_registerType lhs, _registerType rhs) {
        return sse::CompareAllLessEqual<_registerType, _count>(lhs, rhs);
    };


    alignas(sse::alignmentBytes<_registerType>) _registerType lhs = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterEntries; ++i)
        lhs[i] = static_cast<DataType>(i + 1);
    alignas(sse::alignmentBytes<_registerType>) _registerType rhs;


    // all values return true
    BOOST_CHECK(compLE(lhs, lhs));

    // all values return false
    BOOST_CHECK(!compLE(lhs, _mmx_setzero_p<_registerType>()));


    // Maximal 1 used value returns false
    for (U32 i = 0; i < _count; ++i)
    {
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            // all unused values return true
            rhs = lhs;
            BOOST_CHECK(compLE(lhs, rhs));
            rhs[i] = lhs[i] + std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(compLE(lhs, rhs));
            rhs[i] = lhs[i] - std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compLE(lhs, rhs));

            // one unused value returns false
            rhs = lhs;
            rhs[j] = 0;
            BOOST_CHECK(compLE(lhs, rhs));
            rhs[i] = lhs[i] + std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(compLE(lhs, rhs));
            rhs[i] = lhs[i] - std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compLE(lhs, rhs));
        }

        // Up to all unused values return false
        rhs = lhs;
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            rhs[j] = 0;

            rhs[i] = lhs[i] + std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(compLE(lhs, rhs));
            rhs[i] = lhs[i] - std::numeric_limits<DataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compLE(lhs, rhs));
            rhs[i] = lhs[i];
            BOOST_CHECK(compLE(lhs, rhs));
        }
    }

    // maximal all used values return false
    for (U32 j = _count; j < numRegisterEntries; ++j)
    {
        rhs = lhs;
        for (U32 i = 0; i < _count; ++i)
        {
            // all unused values return true
            rhs[i] = 0;
            BOOST_CHECK(!compLE(lhs, rhs));

            // one unused value returns false
            rhs[j] = 0;
            BOOST_CHECK(!compLE(lhs, rhs));
        }
    }


    if constexpr (_count < numRegisterEntries)
        TestCompareAllLessEqual<_registerType, _count + 1>();
}



BOOST_AUTO_TEST_CASE(All_Less_Equal)
{
    TestCompareAllLessEqual<__m128>();
    TestCompareAllLessEqual<__m128d>();
#ifdef __AVX2__
    TestCompareAllLessEqual<__m256>();
    TestCompareAllLessEqual<__m256d>();
#endif // __AVX2__
}



// Test CompareAllGreaterThan -----------------------------------------------------------------------------------------

//! @brief Tests the sse::CompareAllLessEqual function with varying number of compared values.
template <typename _registerType, U32 _count = 1>
void TestCompareAllGreaterThan()
{
    using DataType = decltype(sse::GetDataType<_registerType>());
    constexpr U32 numRegisterEntries = sse::numRegisterValues<_registerType>;

    auto compGT = [](_registerType lhs, _registerType rhs) {
        return sse::CompareAllGreaterThan<_registerType, _count>(lhs, rhs);
    };


    alignas(sse::alignmentBytes<_registerType>) _registerType lhs = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterEntries; ++i)
        lhs[i] = static_cast<DataType>(i + 1);
    alignas(sse::alignmentBytes<_registerType>) _registerType rhs;


    // all values return true
    BOOST_CHECK(!compGT(lhs, lhs));

    // all values return false
    BOOST_CHECK(compGT(lhs, _mmx_set1_p<_registerType>(-100)));


    // Maximal 1 used value returns false
    for (U32 i = 0; i < _count; ++i)
    {
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            // all unused values return true
            rhs = _mmx_sub_p(lhs, _mmx_set1_p<_registerType>(1));
            BOOST_CHECK(compGT(lhs, rhs));
            rhs[i] = lhs[i] + 1;
            BOOST_CHECK(!compGT(lhs, rhs));
            rhs[i] = lhs[i] + 10;
            BOOST_CHECK(!compGT(lhs, rhs));

            // one unused value returns false
            rhs = _mmx_sub_p(lhs, _mmx_set1_p<_registerType>(1));
            rhs[j] = lhs[j] + 10;
            BOOST_CHECK(compGT(lhs, rhs));
            rhs[i] = lhs[i] + 1;
            BOOST_CHECK(!compGT(lhs, rhs));
            rhs[i] = lhs[i] + 10;
            BOOST_CHECK(!compGT(lhs, rhs));
        }

        // Up to all unused values return false
        rhs = _mmx_sub_p(lhs, _mmx_set1_p<_registerType>(1));
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            rhs[j] = lhs[j] + 10;

            rhs[i] = lhs[i] + 1;
            BOOST_CHECK(!compGT(lhs, rhs));
            rhs[i] = lhs[i] + 10;
            BOOST_CHECK(!compGT(lhs, rhs));
            rhs[i] = lhs[i] - 1;
            BOOST_CHECK(compGT(lhs, rhs));
        }
    }

    // maximal all used values return false
    for (U32 j = _count; j < numRegisterEntries; ++j)
    {
        rhs = _mmx_sub_p(lhs, _mmx_set1_p<_registerType>(1));
        for (U32 i = 0; i < _count; ++i)
        {
            // all unused values return true
            rhs[i] = lhs[i] + 10;
            BOOST_CHECK(!compGT(lhs, rhs));

            // one unused value returns false
            rhs[j] = lhs[j] + 10;
            BOOST_CHECK(!compGT(lhs, rhs));
        }
    }


    if constexpr (_count < numRegisterEntries)
        TestCompareAllGreaterThan<_registerType, _count + 1>();
}



BOOST_AUTO_TEST_CASE(All_Greater_Than)
{
    TestCompareAllGreaterThan<__m128>();
    TestCompareAllGreaterThan<__m128d>();
#ifdef __AVX2__
    TestCompareAllGreaterThan<__m256>();
    TestCompareAllGreaterThan<__m256d>();
#endif // __AVX2__
}
