#include <boost/test/unit_test.hpp>

#include "gdl/base/sse/compareAll.h"
#include "gdl/base/sse/directAccess.h"

using namespace GDL;



// Test CompareAllEqual ----------------------------------------------------------------------------------------------

//! @brief Tests the simd::CompareAllEqual function with varying number of compared values.
template <typename _registerType, U32 _count = 1>
void TestCompareAllEqual()
{
    using DataType = decltype(simd::GetDataType<_registerType>());
    constexpr U32 numRegisterEntries = simd::numRegisterValues<_registerType>;

    auto compEQ = [](_registerType lhs, _registerType rhs) {
        return simd::CompareAllEqual<_registerType, _count>(lhs, rhs);
    };


    alignas(simd::alignmentBytes<_registerType>) _registerType lhs = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegisterEntries; ++i)
        simd::SetValue(lhs, i, static_cast<DataType>(i + 1));
    alignas(simd::alignmentBytes<_registerType>) _registerType rhs;


    // all values return true
    BOOST_CHECK(compEQ(lhs, lhs));

    // all values return false
    BOOST_CHECK(!compEQ(lhs, _mm_setzero<_registerType>()));


    // Maximal 1 used value returns false
    for (U32 i = 0; i < _count; ++i)
    {
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            // all unused values return true
            rhs = lhs;
            BOOST_CHECK(compEQ(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) + std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(!compEQ(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) - std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(!compEQ(lhs, rhs));

            // one unused value returns false
            rhs = lhs;
            simd::SetValue(rhs, j, 0);
            BOOST_CHECK(compEQ(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) + std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(!compEQ(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) - std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(!compEQ(lhs, rhs));
        }

        // Up to all unused values return false
        rhs = lhs;
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            simd::SetValue(rhs, j, 0);

            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) + std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(!compEQ(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) - std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(!compEQ(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i));
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
            simd::SetValue(rhs, i, 0);
            BOOST_CHECK(!compEQ(lhs, rhs));

            // one unused value returns false
            simd::SetValue(rhs, j, 0);
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

//! @brief Tests the simd::CompareAllLessEqual function with varying number of compared values.
template <typename _registerType, U32 _count = 1>
void TestCompareAllLessEqual()
{
    using DataType = decltype(simd::GetDataType<_registerType>());
    constexpr U32 numRegisterEntries = simd::numRegisterValues<_registerType>;

    auto compLE = [](_registerType lhs, _registerType rhs) {
        return simd::CompareAllLessEqual<_registerType, _count>(lhs, rhs);
    };


    alignas(simd::alignmentBytes<_registerType>) _registerType lhs = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegisterEntries; ++i)
        simd::SetValue(lhs, i, static_cast<DataType>(i + 1));
    alignas(simd::alignmentBytes<_registerType>) _registerType rhs;


    // all values return true
    BOOST_CHECK(compLE(lhs, lhs));

    // all values return false
    BOOST_CHECK(!compLE(lhs, _mm_setzero<_registerType>()));


    // Maximal 1 used value returns false
    for (U32 i = 0; i < _count; ++i)
    {
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            // all unused values return true
            rhs = lhs;
            BOOST_CHECK(compLE(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) + std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(compLE(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) - std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(!compLE(lhs, rhs));

            // one unused value returns false
            rhs = lhs;
            simd::SetValue(rhs, 0, j);
            BOOST_CHECK(compLE(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) + std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(compLE(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) - std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(!compLE(lhs, rhs));
        }

        // Up to all unused values return false
        rhs = lhs;
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            simd::SetValue(rhs, j, 0);

            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) + std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(compLE(lhs, rhs));
            simd::SetValue(rhs, i,
                          simd::GetValue(lhs, i) - std::numeric_limits<DataType>::epsilon() * numRegisterEntries);
            BOOST_CHECK(!compLE(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i));
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
            simd::SetValue(rhs, i, 0);
            BOOST_CHECK(!compLE(lhs, rhs));

            // one unused value returns false
            simd::SetValue(rhs, j, 0);
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

//! @brief Tests the simd::CompareAllLessEqual function with varying number of compared values.
template <typename _registerType, U32 _count = 1>
void TestCompareAllGreaterThan()
{
    using DataType = decltype(simd::GetDataType<_registerType>());
    constexpr U32 numRegisterEntries = simd::numRegisterValues<_registerType>;

    auto compGT = [](_registerType lhs, _registerType rhs) {
        return simd::CompareAllGreaterThan<_registerType, _count>(lhs, rhs);
    };


    alignas(simd::alignmentBytes<_registerType>) _registerType lhs = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegisterEntries; ++i)
        simd::SetValue(lhs, i, static_cast<DataType>(i + 1));
    alignas(simd::alignmentBytes<_registerType>) _registerType rhs;


    // all values return true
    BOOST_CHECK(!compGT(lhs, lhs));

    // all values return false
    BOOST_CHECK(compGT(lhs, _mm_set1<_registerType>(-100)));


    // Maximal 1 used value returns false
    for (U32 i = 0; i < _count; ++i)
    {
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            // all unused values return true
            rhs = _mm_sub(lhs, _mm_set1<_registerType>(1));
            BOOST_CHECK(compGT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) + 1);
            BOOST_CHECK(!compGT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) + 10);
            BOOST_CHECK(!compGT(lhs, rhs));

            // one unused value returns false
            rhs = _mm_sub(lhs, _mm_set1<_registerType>(1));
            simd::SetValue(rhs, j, simd::GetValue(lhs, i) + 10);
            BOOST_CHECK(compGT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) + 1);
            BOOST_CHECK(!compGT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) + 10);
            BOOST_CHECK(!compGT(lhs, rhs));
        }

        // Up to all unused values return false
        rhs = _mm_sub(lhs, _mm_set1<_registerType>(1));
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            simd::SetValue(rhs, j, simd::GetValue(lhs, i) + 10);

            simd::SetValue(rhs, i, simd::GetValue(lhs, i) + 1);
            BOOST_CHECK(!compGT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) + 10);
            BOOST_CHECK(!compGT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) - 1);
            BOOST_CHECK(compGT(lhs, rhs));
        }
    }

    // maximal all used values return false
    for (U32 j = _count; j < numRegisterEntries; ++j)
    {
        rhs = _mm_sub(lhs, _mm_set1<_registerType>(1));
        for (U32 i = 0; i < _count; ++i)
        {
            // all unused values return true
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) + 10);
            BOOST_CHECK(!compGT(lhs, rhs));

            // one unused value returns false
            simd::SetValue(rhs, j, simd::GetValue(lhs, i) + 10);
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



// Test CompareAllLessThan -----------------------------------------------------------------------------------------

//! @brief Tests the simd::CompareAllLessEqual function with varying number of compared values.
template <typename _registerType, U32 _count = 1>
void TestCompareAllLessThan()
{
    using DataType = decltype(simd::GetDataType<_registerType>());
    constexpr U32 numRegisterEntries = simd::numRegisterValues<_registerType>;

    auto compLT = [](_registerType lhs, _registerType rhs) {
        return simd::CompareAllLessThan<_registerType, _count>(lhs, rhs);
    };


    alignas(simd::alignmentBytes<_registerType>) _registerType lhs = _mm_setzero<_registerType>();
    for (U32 i = 0; i < numRegisterEntries; ++i)
        simd::SetValue(lhs, i, static_cast<DataType>(i + 1));
    alignas(simd::alignmentBytes<_registerType>) _registerType rhs;


    // all values return false
    BOOST_CHECK(!compLT(lhs, lhs));

    // all values return true
    BOOST_CHECK(compLT(lhs, _mm_set1<_registerType>(100)));


    // Maximal 1 used value returns false
    for (U32 i = 0; i < _count; ++i)
    {
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            // all unused values return true
            rhs = _mm_add(lhs, _mm_set1<_registerType>(1));
            BOOST_CHECK(compLT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) - 1);
            BOOST_CHECK(!compLT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) - 10);
            BOOST_CHECK(!compLT(lhs, rhs));

            // one unused value returns false
            rhs = _mm_add(lhs, _mm_set1<_registerType>(1));
            simd::SetValue(rhs, j, simd::GetValue(lhs, i) - 10);
            BOOST_CHECK(compLT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) - 1);
            BOOST_CHECK(!compLT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) - 10);
            BOOST_CHECK(!compLT(lhs, rhs));
        }

        // Up to all unused values return false
        rhs = _mm_add(lhs, _mm_set1<_registerType>(1));
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            simd::SetValue(rhs, j, simd::GetValue(lhs, i) - 10);

            simd::SetValue(rhs, i, simd::GetValue(lhs, i) - 1);
            BOOST_CHECK(!compLT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) - 10);
            BOOST_CHECK(!compLT(lhs, rhs));
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) + 1);
            BOOST_CHECK(compLT(lhs, rhs));
        }
    }

    // maximal all used values return false
    for (U32 j = _count; j < numRegisterEntries; ++j)
    {
        rhs = _mm_add(lhs, _mm_set1<_registerType>(1));
        for (U32 i = 0; i < _count; ++i)
        {
            // all unused values return true
            simd::SetValue(rhs, i, simd::GetValue(lhs, i) - 10);
            BOOST_CHECK(!compLT(lhs, rhs));

            // one unused value returns false
            simd::SetValue(rhs, j, simd::GetValue(lhs, i) - 10);
            BOOST_CHECK(!compLT(lhs, rhs));
        }
    }


    if constexpr (_count < numRegisterEntries)
        TestCompareAllLessThan<_registerType, _count + 1>();
}



BOOST_AUTO_TEST_CASE(All_Less_Than)
{
    TestCompareAllLessThan<__m128>();
    TestCompareAllLessThan<__m128d>();
#ifdef __AVX2__
    TestCompareAllLessThan<__m256>();
    TestCompareAllLessThan<__m256d>();
#endif // __AVX2__
}
