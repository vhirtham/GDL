#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/tolerance.h"
#include "gdl/base/sse/directAccess.h"

#include "test/tools/ExceptionChecks.h"

using namespace GDL;


template <typename _type>
void TestTolerance()
{
    const _type refValue = 10;

    Tolerance<_type> tolerance(refValue, 2);
    _type cmp = 0;

    for (I32 tol = 1; tol < 6; ++tol)
    {
        tolerance = Tolerance<_type>(refValue, tol);
        for (I32 i = -10; i < 10; ++i)
        {
            cmp = refValue + static_cast<_type>(i);

            if (std::abs(i) <= tol)
            {
                BOOST_CHECK(tolerance == cmp);
                BOOST_CHECK(!(tolerance != cmp));
            }
            else
            {
                BOOST_CHECK(tolerance != cmp);
                BOOST_CHECK(!(tolerance == cmp));
            }
        }
    }

    GDL_CHECK_THROW_DEV(Tolerance<_type>(1, 0), Exception);
    GDL_CHECK_THROW_DEV(Tolerance<_type>(1, -1), Exception);
}

BOOST_AUTO_TEST_CASE(Inside_Tolerance)
{
    TestTolerance<I32>();
    TestTolerance<F32>();
    TestTolerance<F64>();
}


template <typename _registerType, U32 _count = 1>
void TestToleranceSSE()
{
    constexpr U32 numRegisterEntries = sse::numRegisterValues<_registerType>;
    using ToleranceType = Tolerance<_registerType, _count>;

    F32 toleranceValue = 1.5;
    alignas(sse::alignmentBytes<_registerType>) _registerType ref = _mmx_setzero_p<_registerType>();
    alignas(sse::alignmentBytes<_registerType>) _registerType toleranceValueSSE =
            _mmx_set1_p<_registerType>(toleranceValue);

    for (U32 i = 0; i < numRegisterEntries; ++i)
        sse::SetValue(ref, i, i);

    ToleranceType tolerance(ref, toleranceValueSSE);


    for (U32 run = 0; run < 2; ++run)
    {
        // Use alternative ctor
        if (run == 1)
        {
            toleranceValue = 2.5;
            tolerance = ToleranceType(ref, toleranceValue);
        }

        BOOST_CHECK(tolerance == ref);
        BOOST_CHECK(ref == tolerance);
        BOOST_CHECK(tolerance != _mmx_set1_p<_registerType>(-10));
        BOOST_CHECK(_mmx_set1_p<_registerType>(-10) != tolerance);

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
                BOOST_CHECK(cmpAll == tolerance);
                BOOST_CHECK(!(cmpAll != tolerance));
            }
            else
            {
                BOOST_CHECK(cmpAll != tolerance);
                BOOST_CHECK(!(cmpAll == tolerance));
            }

            // Up to all compared values might be out of tolerance
            for (U32 j = _count; j < numRegisterEntries; ++j)
            {
                sse::SetValue(cmpAll, -1337, j);
                if (std::abs(i) <= toleranceValue)
                {
                    BOOST_CHECK(cmpAll == tolerance);
                    BOOST_CHECK(!(cmpAll != tolerance));
                }
                else
                {
                    BOOST_CHECK(cmpAll != tolerance);
                    BOOST_CHECK(!(cmpAll == tolerance));
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
                    BOOST_CHECK(cmpSingle == tolerance);
                    BOOST_CHECK(!(cmpSingle != tolerance));
                }
                else
                {
                    if (j < _count)
                    {
                        BOOST_CHECK(cmpSingle != tolerance);
                        BOOST_CHECK(!(cmpSingle == tolerance));
                    }
                    else
                    {
                        BOOST_CHECK(cmpSingle == tolerance);
                        BOOST_CHECK(!(cmpSingle != tolerance));
                    }
                }
            }
        }
    }



    GDL_CHECK_THROW_DEV(ToleranceType(ref, 0), Exception);
    GDL_CHECK_THROW_DEV(ToleranceType(ref, -1), Exception);
    GDL_CHECK_THROW_DEV(ToleranceType(ref, _mmx_setzero_p<_registerType>()), Exception);
    GDL_CHECK_THROW_DEV(ToleranceType(ref, _mmx_set1_p<_registerType>(-1)), Exception);
    // Only one invalid tolerance
    for (U32 i = 0; i < numRegisterEntries; ++i)
    {
        toleranceValueSSE = _mmx_set1_p<_registerType>(toleranceValue);
        sse::SetValue(toleranceValueSSE, -1, i);
        GDL_CHECK_THROW_DEV(ToleranceType(ref, toleranceValueSSE), Exception);
    }

    if constexpr (_count < numRegisterEntries)
        TestToleranceSSE<_registerType, _count + 1>();
}


BOOST_AUTO_TEST_CASE(Inside_Tolerance_SSE)
{
    TestToleranceSSE<__m128>();
    TestToleranceSSE<__m128d>();
#ifdef __AVX2__
    TestToleranceSSE<__m256>();
    TestToleranceSSE<__m256d>();
#endif // __AVX2__
}
