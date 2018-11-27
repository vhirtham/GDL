#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/insideTolerance.h"
#include "gdl/base/sse/directAccess.h"

#include "test/tools/ExceptionChecks.h"

using namespace GDL;


template <typename _type>
void TestInsideTolerance()
{
    const _type refValue = 10;

    InsideTolerance<_type> insideTolerance(refValue, 2);
    _type cmp = 0;

    for (I32 tol = 1; tol < 6; ++tol)
    {
        insideTolerance = InsideTolerance<_type>(refValue, tol);
        for (I32 i = -10; i < 10; ++i)
        {
            cmp = refValue + static_cast<_type>(i);

            if (std::abs(i) <= tol)
            {
                BOOST_CHECK(insideTolerance == cmp);
                BOOST_CHECK(!(insideTolerance != cmp));
            }
            else
            {
                BOOST_CHECK(insideTolerance != cmp);
                BOOST_CHECK(!(insideTolerance == cmp));
            }
        }
    }

    GDL_CHECK_THROW_DEV(InsideTolerance<_type>(1, 0), Exception);
    GDL_CHECK_THROW_DEV(InsideTolerance<_type>(1, -1), Exception);
}

BOOST_AUTO_TEST_CASE(Inside_Tolerance)
{
    TestInsideTolerance<I32>();
    TestInsideTolerance<F32>();
    TestInsideTolerance<F64>();
}


template <typename _registerType, U32 _count = 1>
void TestInsideToleranceSSE()
{
    constexpr U32 numRegisterEntries = sse::numRegisterValues<_registerType>;
    using InsideToleranceType = InsideTolerance<_registerType, _count>;

    F32 toleranceValue = 1.5;
    alignas(sse::alignmentBytes<_registerType>) _registerType ref = _mmx_setzero_p<_registerType>();
    alignas(sse::alignmentBytes<_registerType>) _registerType tolerance = _mmx_set1_p<_registerType>(toleranceValue);

    for (U32 i = 0; i < numRegisterEntries; ++i)
        sse::SetValue(ref, i, i);

    InsideToleranceType insideTolerance(ref, tolerance);


    for (U32 run = 0; run < 2; ++run)
    {
        // Use alternative ctor
        if (run == 1)
        {
            toleranceValue = 2.5;
            insideTolerance = InsideToleranceType(ref, toleranceValue);
        }

        BOOST_CHECK(insideTolerance == ref);
        BOOST_CHECK(ref == insideTolerance);
        BOOST_CHECK(insideTolerance != _mmx_set1_p<_registerType>(-10));
        BOOST_CHECK(_mmx_set1_p<_registerType>(-10) != insideTolerance);

        alignas(sse::alignmentBytes<_registerType>) _registerType cmpAll;
        alignas(sse::alignmentBytes<_registerType>) _registerType cmpSingle;

        for (I32 i = -4; i < 4; ++i)
        {
            // All values modified
            // -------------------
            cmpAll = _mmx_add_p(ref, _mmx_set1_p<_registerType>(i));

            // Not compared values are equal to ref
            for (U32 j = _count; j < numRegisterEntries; ++j)
                sse::SetValue(cmpAll, sse::GetValue(ref, j), j);

            if (std::abs(i) <= toleranceValue)
            {
                BOOST_CHECK(cmpAll == insideTolerance);
                BOOST_CHECK(!(cmpAll != insideTolerance));
            }
            else
            {
                BOOST_CHECK(cmpAll != insideTolerance);
                BOOST_CHECK(!(cmpAll == insideTolerance));
            }

            // Up to all compared values might be out of tolerance
            for (U32 j = _count; j < numRegisterEntries; ++j)
            {
                sse::SetValue(cmpAll, -1337, j);
                if (std::abs(i) <= toleranceValue)
                {
                    BOOST_CHECK(cmpAll == insideTolerance);
                    BOOST_CHECK(!(cmpAll != insideTolerance));
                }
                else
                {
                    BOOST_CHECK(cmpAll != insideTolerance);
                    BOOST_CHECK(!(cmpAll == insideTolerance));
                }
            }


            // Only single value modified
            // --------------------------
            for (U32 j = 0; j < numRegisterEntries; ++j)
            {
                cmpSingle = ref;

                sse::SetValue(cmpSingle, sse::GetValue(cmpSingle, j) + i, j);
                if (std::abs(i) <= toleranceValue)
                {
                    BOOST_CHECK(cmpSingle == insideTolerance);
                    BOOST_CHECK(!(cmpSingle != insideTolerance));
                }
                else
                {
                    if (j < _count)
                    {
                        BOOST_CHECK(cmpSingle != insideTolerance);
                        BOOST_CHECK(!(cmpSingle == insideTolerance));
                    }
                    else
                    {
                        BOOST_CHECK(cmpSingle == insideTolerance);
                        BOOST_CHECK(!(cmpSingle != insideTolerance));
                    }
                }
            }
        }
    }



    GDL_CHECK_THROW_DEV(InsideToleranceType(ref, 0), Exception);
    GDL_CHECK_THROW_DEV(InsideToleranceType(ref, -1), Exception);
    GDL_CHECK_THROW_DEV(InsideToleranceType(ref, _mmx_setzero_p<_registerType>()), Exception);
    GDL_CHECK_THROW_DEV(InsideToleranceType(ref, _mmx_set1_p<_registerType>(-1)), Exception);
    // Only one invalid tolerance
    for (U32 i = 0; i < numRegisterEntries; ++i)
    {
        tolerance = _mmx_set1_p<_registerType>(toleranceValue);
        sse::SetValue(tolerance, -1, i);
        GDL_CHECK_THROW_DEV(InsideToleranceType(ref, tolerance), Exception);
    }

    if constexpr (_count < numRegisterEntries)
        TestInsideToleranceSSE<_registerType, _count + 1>();
}


BOOST_AUTO_TEST_CASE(Inside_Tolerance_SSE)
{
    TestInsideToleranceSSE<__m128>();
    TestInsideToleranceSSE<__m128d>();
#ifdef __AVX2__
    TestInsideToleranceSSE<__m256>();
    TestInsideToleranceSSE<__m256d>();
#endif // __AVX2__
}
