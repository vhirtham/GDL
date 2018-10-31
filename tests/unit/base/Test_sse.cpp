#include <boost/test/unit_test.hpp>


#include "gdl/base/functions/sse.h"

#include <iostream>
using namespace GDL;


BOOST_AUTO_TEST_CASE(Calc_Min_Num_Array_Registers)
{
    BOOST_CHECK(CalcMinNumArrayRegisters<__m128>(13) == 4);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m128>(4) == 1);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m128d>(13) == 7);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m128d>(4) == 2);
#ifdef __AVX2__
    BOOST_CHECK(CalcMinNumArrayRegisters<__m256>(13) == 2);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m256>(4) == 1);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m256d>(13) == 4);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m256d>(4) == 1);
#ifdef __AVX512F__
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512>(21) == 2);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512>(13) == 1);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512>(4) == 1);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512d>(21) == 3);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512d>(13) == 2);
    BOOST_CHECK(CalcMinNumArrayRegisters<__m512d>(4) == 1);
#endif // __AVX512F__
#endif // __AVX2__
}


// -------------------------------------------------------------------------------------------------------------------
// Test comparison operations where all values need to return true
// We test only two operations (== and <=), since they use the same mechanics just with different comparison operators
// applied.
// -------------------------------------------------------------------------------------------------------------------


// Test CompareAllEqual ----------------------------------------------------------------------------------------------

//! @brief Tests the SSECompareAllEqual function with varying number of compared values.
template <typename _registerType>
void TestCompareAllEqual()
{
    using dataType = decltype(SSEGetDataType<_registerType>());
    constexpr U32 numRegisterEntries = GetNumRegisterEntries<_registerType>();


    alignas(AlignmentBytes<_registerType>) _registerType lhs = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterEntries; ++i)
        lhs[i] = static_cast<dataType>(i + 1);
    alignas(AlignmentBytes<_registerType>) _registerType rhs = lhs;

    BOOST_CHECK(SSECompareAllEqual(lhs, lhs));
    BOOST_CHECK(!SSECompareAllEqual(lhs, _mmx_setzero_p<_registerType>()));
    for (U32 i = 0; i < numRegisterEntries; ++i)
    {
        rhs = lhs;
        rhs[i] = rhs[i] + std::numeric_limits<dataType>::epsilon() * numRegisterEntries;
        BOOST_CHECK(!SSECompareAllEqual(lhs, rhs));
    }



    // Only one register value should be compared
    // ------------------------------------------

    // All uncompared elements equal
    rhs = lhs;

    bool resultComparison = SSECompareAllEqual<_registerType, 1>(lhs, rhs);
    BOOST_CHECK(resultComparison);
    rhs[0] = -1;
    resultComparison = SSECompareAllEqual<_registerType, 1>(lhs, rhs);
    BOOST_CHECK(!resultComparison);

    // All uncompared elements not equal
    rhs = lhs;
    for (U32 i = 1; i < numRegisterEntries; ++i)
        rhs[i] = static_cast<dataType>(0);

    resultComparison = SSECompareAllEqual<_registerType, 1>(lhs, rhs);
    BOOST_CHECK(resultComparison);
    rhs[0] = -1;
    resultComparison = SSECompareAllEqual<_registerType, 1>(lhs, rhs);
    BOOST_CHECK(!resultComparison);

    // One uncompared element differs
    for (U32 i = 1; i < numRegisterEntries; ++i)
    {
        rhs = lhs;
        rhs[i] = static_cast<dataType>(0);
        resultComparison = SSECompareAllEqual<_registerType, 1>(lhs, rhs);
        BOOST_CHECK(resultComparison);
        rhs[0] = -1;
        resultComparison = SSECompareAllEqual<_registerType, 1>(lhs, rhs);
        BOOST_CHECK(!resultComparison);
    }


    // All except one register values should be compared
    // -------------------------------------------------

    constexpr U32 indexLast = numRegisterEntries - 1;

    rhs = lhs;
    resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 1>(lhs, rhs);
    BOOST_CHECK(resultComparison);
    rhs[indexLast] = static_cast<dataType>(0);
    resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 1>(lhs, rhs);
    BOOST_CHECK(resultComparison);

    rhs = lhs;
    for (U32 i = 0; i < numRegisterEntries - 1; ++i)
    {
        rhs[i] = static_cast<dataType>(0);
    }
    resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 1>(lhs, rhs);
    BOOST_CHECK(!resultComparison);
    rhs[indexLast] = static_cast<dataType>(0);
    resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 1>(lhs, rhs);
    BOOST_CHECK(!resultComparison);

    for (U32 i = 0; i < numRegisterEntries - 1; ++i)
    {
        rhs = lhs;
        rhs[i] = rhs[i] + std::numeric_limits<dataType>::epsilon() * numRegisterEntries;
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 1>(lhs, rhs);
        BOOST_CHECK(!resultComparison);
        rhs[indexLast] = static_cast<dataType>(0);
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 1>(lhs, rhs);
        BOOST_CHECK(!resultComparison);
    }

    // All except two register values should be compared
    // -------------------------------------------------

    // clang-format off
    if constexpr(numRegisterEntries > 2)
    {
        constexpr U32 indexLast = numRegisterEntries - 1;

        rhs = lhs;
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
        BOOST_CHECK(resultComparison);
        rhs[indexLast] = static_cast<dataType>(0);
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
        BOOST_CHECK(resultComparison);
        rhs[indexLast-1] = static_cast<dataType>(0);
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
        BOOST_CHECK(resultComparison);
        rhs[indexLast] = lhs[indexLast];
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
        BOOST_CHECK(resultComparison);

        rhs = lhs;
        for (U32 i = 0; i < numRegisterEntries - 2; ++i)
        {
            rhs[i] = static_cast<dataType>(0);
        }
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
        BOOST_CHECK(!resultComparison);
        rhs[indexLast] = static_cast<dataType>(0);
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
        BOOST_CHECK(!resultComparison);
        rhs[indexLast-1] = static_cast<dataType>(0);
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
        BOOST_CHECK(!resultComparison);
        rhs[indexLast] = lhs[indexLast];
        resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
        BOOST_CHECK(!resultComparison);

        for (U32 i = 0; i < numRegisterEntries - 2; ++i)
        {
            rhs = lhs;
            rhs[i] = rhs[i] + std::numeric_limits<dataType>::epsilon() * numRegisterEntries;
            resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
            BOOST_CHECK(!resultComparison);
            rhs[indexLast] = static_cast<dataType>(0);
            resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
            BOOST_CHECK(!resultComparison);
            rhs[indexLast-1] = static_cast<dataType>(0);
            resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
            BOOST_CHECK(!resultComparison);
            rhs[indexLast] = lhs[indexLast];
            resultComparison = SSECompareAllEqual<_registerType, numRegisterEntries - 2>(lhs, rhs);
            BOOST_CHECK(!resultComparison);
        }
    }
    // clang-format on
}



BOOST_AUTO_TEST_CASE(All_Equal)
{
    TestCompareAllEqual<__m128>();
    TestCompareAllEqual<__m128d>();
#ifdef __AVX2__
    TestCompareAllEqual<__m256>();
    TestCompareAllEqual<__m256d>();
#ifdef __AVX512F__
    TestCompareAllEqual<__m512>();
    TestCompareAllEqual<__m512d>();
#endif // __AVX512F__
#endif // __AVX2__
}



// Test CompareAllEqual ----------------------------------------------------------------------------------------------

//! @brief Tests the SSECompareAllLessEqual function with varying number of compared values.
template <typename _registerType, U32 _count = 1>
void TestCompareAllLessEqual()
{
    using dataType = decltype(SSEGetDataType<_registerType>());
    constexpr U32 numRegisterEntries = GetNumRegisterEntries<_registerType>();
    constexpr U32 numUnusedElements = numRegisterEntries - _count;

    auto compLE = [](_registerType lhs, _registerType rhs) {
        return SSECompareAllLessEqual<_registerType, _count>(lhs, rhs);
    };


    alignas(AlignmentBytes<_registerType>) _registerType lhs = _mmx_setzero_p<_registerType>();
    for (U32 i = 0; i < numRegisterEntries; ++i)
        lhs[i] = static_cast<dataType>(i + 1);
    alignas(AlignmentBytes<_registerType>) _registerType rhs = lhs;


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
            rhs[i] = lhs[i] + std::numeric_limits<dataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(compLE(lhs, rhs));
            rhs[i] = lhs[i] - std::numeric_limits<dataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compLE(lhs, rhs));

            // one unused value returns false
            rhs = lhs;
            rhs[j] = 0;
            BOOST_CHECK(compLE(lhs, rhs));
            rhs[i] = lhs[i] + std::numeric_limits<dataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(compLE(lhs, rhs));
            rhs[i] = lhs[i] - std::numeric_limits<dataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(!compLE(lhs, rhs));
        }

        // Up to all unused values return false
        rhs = lhs;
        for (U32 j = _count; j < numRegisterEntries; ++j)
        {
            rhs[j] = 0;

            rhs[i] = lhs[i] + std::numeric_limits<dataType>::epsilon() * numRegisterEntries;
            BOOST_CHECK(compLE(lhs, rhs));
            rhs[i] = lhs[i] - std::numeric_limits<dataType>::epsilon() * numRegisterEntries;
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



    // clang-format off
    if constexpr(_count < numRegisterEntries)
        TestCompareAllLessEqual<_registerType, _count + 1>();
    // clang-format on
}



BOOST_AUTO_TEST_CASE(All_Less_Equal)
{
    TestCompareAllLessEqual<__m128>();
    TestCompareAllLessEqual<__m128d>();
#ifdef __AVX2__
    TestCompareAllLessEqual<__m256>();
    TestCompareAllLessEqual<__m256d>();
#ifdef __AVX512F__
    TestCompareAllLessEqual<__m512>();
    TestCompareAllLessEqual<__m512d>();
#endif // __AVX512F__
#endif // __AVX2__
}
