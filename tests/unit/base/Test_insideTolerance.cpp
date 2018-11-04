#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/insideTolerance.h"

using namespace GDL;


template <typename _type>
void TestInsideTolerance()
{

    InsideTolerance<_type> insideTolerance(static_cast<_type>(10), static_cast<_type>(2));

    BOOST_CHECK(insideTolerance == 10);
    BOOST_CHECK(insideTolerance == 12);
    BOOST_CHECK(!(insideTolerance == 13));
    BOOST_CHECK(!(insideTolerance != 12));
    BOOST_CHECK(insideTolerance != 13);
    BOOST_CHECK(insideTolerance == 8);
    BOOST_CHECK(!(insideTolerance == 7));
    BOOST_CHECK(!(insideTolerance != 8));
    BOOST_CHECK(insideTolerance != 7);


    BOOST_CHECK(10 == insideTolerance);
    BOOST_CHECK(12 == insideTolerance);
    BOOST_CHECK(!(13 == insideTolerance));
    BOOST_CHECK(!(12 != insideTolerance));
    BOOST_CHECK(13 != insideTolerance);
    BOOST_CHECK(8 == insideTolerance);
    BOOST_CHECK(!(7 == insideTolerance));
    BOOST_CHECK(!(8 != insideTolerance));
    BOOST_CHECK(7 != insideTolerance);
}

BOOST_AUTO_TEST_CASE(Inside_Tolerance)
{
    TestInsideTolerance<I32>();
    TestInsideTolerance<F32>();
    TestInsideTolerance<F64>();
}


template <typename _registerType>
void TestInsideToleranceSSE()
{
    constexpr U32 numRegisterEntries = SSEGetNumRegisterEntries<_registerType>();

    F32 toleranceValue = 1.5;
    alignas(alignmentBytes<_registerType>) _registerType ref = _mmx_setzero_p<_registerType>();
    alignas(alignmentBytes<_registerType>) _registerType tolerance = _mmx_set1_p<_registerType>(toleranceValue);

    for (U32 i = 0; i < numRegisterEntries; ++i)
        ref[i] = i;

    InsideTolerance<_registerType> insideTolerance(ref, tolerance);
    InsideTolerance<_registerType, numRegisterEntries - 1> insideToleranceMinus1(ref, tolerance);


    for (U32 run = 0; run < 2; ++run)
    {
        if (run == 1)
        {
            toleranceValue = 2.5;
            insideTolerance = InsideTolerance<_registerType>(ref, toleranceValue);
            insideToleranceMinus1 = InsideTolerance<_registerType, numRegisterEntries - 1>(ref, toleranceValue);
        }

        BOOST_CHECK(insideTolerance == ref);
        BOOST_CHECK(ref == insideTolerance);
        BOOST_CHECK(insideTolerance != _mmx_set1_p<_registerType>(-10));
        BOOST_CHECK(_mmx_set1_p<_registerType>(-10) != insideTolerance);

        alignas(alignmentBytes<_registerType>) _registerType cmpAll;
        alignas(alignmentBytes<_registerType>) _registerType cmpSingle;

        for (I32 i = -4; i < 4; ++i)
        {
            cmpAll = ref;
            cmpSingle = ref;

            cmpAll = _mmx_add_p(cmpAll, _mmx_set1_p<_registerType>(i));
            cmpSingle[1] += i;
            if (std::abs(i) <= toleranceValue)
            {
                BOOST_CHECK(cmpAll == insideTolerance);
                BOOST_CHECK(cmpSingle == insideTolerance);
                BOOST_CHECK(!(cmpAll != insideTolerance));
                BOOST_CHECK(!(cmpSingle != insideTolerance));
            }
            else
            {
                BOOST_CHECK(cmpAll != insideTolerance);
                BOOST_CHECK(cmpSingle != insideTolerance);
                BOOST_CHECK(!(cmpAll == insideTolerance));
                BOOST_CHECK(!(cmpSingle == insideTolerance));
            }
        }


        // Not all values should be in tolerance --- Not doing a complete check here, since the used
        // SSECompareAllLessEqual function is already tested.

        for (I32 i = -4; i < 4; ++i)
        {
            cmpAll = ref;
            cmpSingle = ref;

            cmpAll = _mmx_add_p(cmpAll, _mmx_set1_p<_registerType>(i));
            cmpSingle[1] += i;
            if (std::abs(i) <= toleranceValue)
            {
                BOOST_CHECK(cmpAll == insideTolerance);
                BOOST_CHECK(cmpSingle == insideTolerance);
                BOOST_CHECK(!(cmpAll != insideTolerance));
                BOOST_CHECK(!(cmpSingle != insideTolerance));
            }
            else
            {
                BOOST_CHECK(cmpAll != insideTolerance);
                BOOST_CHECK(cmpSingle != insideTolerance);
                BOOST_CHECK(!(cmpAll == insideTolerance));
                BOOST_CHECK(!(cmpSingle == insideTolerance));
            }



            cmpSingle = ref;
            cmpSingle[0] += i;
            cmpAll[numRegisterEntries - 1] = -1337;
            cmpSingle[numRegisterEntries - 1] = -1337;
            if (std::abs(i) <= toleranceValue)
            {
                BOOST_CHECK(cmpAll == insideToleranceMinus1);
                BOOST_CHECK(cmpSingle == insideToleranceMinus1);
                BOOST_CHECK(!(cmpAll != insideToleranceMinus1));
                BOOST_CHECK(!(cmpSingle != insideToleranceMinus1));
            }
            else
            {
                BOOST_CHECK(cmpAll != insideToleranceMinus1);
                BOOST_CHECK(cmpSingle != insideToleranceMinus1);
                BOOST_CHECK(!(cmpAll == insideToleranceMinus1));
                BOOST_CHECK(!(cmpSingle == insideToleranceMinus1));
            }
        }
    }
}


BOOST_AUTO_TEST_CASE(Inside_Tolerance_SSE)
{
    TestInsideToleranceSSE<__m128>();
    TestInsideToleranceSSE<__m128d>();
#ifdef __AVX2__
    TestInsideToleranceSSE<__m256>();
    TestInsideToleranceSSE<__m256d>();
#ifdef __AVX512F__
    TestInsideToleranceSSE<__m512>();
    TestInsideToleranceSSE<__m512d>();
#endif // __AVX512F__
#endif // __AVX2__
}
