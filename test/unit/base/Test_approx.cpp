#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/pow.h"
#include "gdl/base/simd/directAccess.h"

#include "test/tools/ExceptionChecks.h"

using namespace GDL;

template <typename _type>
void TestApprox()
{
    for (U32 i = 1; i < 20; ++i)
    {
        for (U32 j = 1; j < 100; ++j)
        {
            _type value = Pow(static_cast<_type>(10.), static_cast<I32>(i - 10));

            _type scaledEps = std::numeric_limits<_type>::epsilon() * value; // Epsilon of the current value

            I32 factor = static_cast<I32>(j); // Scales the tolerance (1 = epsilon of value)


            // Compare values that are one epsilon above and below the adjusted tolerance
            BOOST_CHECK(value + scaledEps * (factor - 1) == Approx<_type>(value, factor, value));
            BOOST_CHECK(value + scaledEps * (factor + 1) != Approx<_type>(value, factor, value));
            BOOST_CHECK(-value + scaledEps * (factor - 1) == Approx<_type>(-value, factor, value));
            BOOST_CHECK(-value + scaledEps * (factor + 1) != Approx<_type>(-value, factor, value));
            BOOST_CHECK(Approx<_type>(value, factor, value) == value + scaledEps * (factor - 1));
            BOOST_CHECK(Approx<_type>(value, factor, value) != value + scaledEps * (factor + 1));
        }
    }

    GDL_CHECK_THROW_DEV(Approx<_type>(1, 0), Exception);
    GDL_CHECK_THROW_DEV(Approx<_type>(1, -1), Exception);
}


BOOST_AUTO_TEST_CASE(Approx_Non_Zero)
{
    TestApprox<F32>();
    TestApprox<F64>();
}

template <typename _type>
void TestApproxZero()
{
    // Base and scaling ctor
    for (U32 i = 1; i < 20; ++i)
    {
        for (U32 j = 1; j < 100; ++j)
        {
            _type base = Pow(static_cast<_type>(10.), static_cast<I32>(i - 10));

            _type scaledEps = std::numeric_limits<_type>::epsilon() * base; // Epsilon of the current value

            I32 factor = static_cast<I32>(j); // Scales the tolerance (1 = epsilon of value)


            // Compare values that are one epsilon above and below the adjusted tolerance
            BOOST_CHECK(scaledEps * (factor - 1) == Approx<_type>(0, factor, base));
            BOOST_CHECK(scaledEps * (factor + 1) != Approx<_type>(0, factor, base));
            BOOST_CHECK(0 == Approx<_type>(scaledEps * (factor - 1), factor, base));
            BOOST_CHECK(0 != Approx<_type>(scaledEps * (factor + 1), factor, base));

            BOOST_CHECK(scaledEps * (factor - 1) == ApproxZero<_type>(base, factor));
            BOOST_CHECK(scaledEps * (factor + 1) != ApproxZero<_type>(base, factor));
            BOOST_CHECK(scaledEps * (factor - 1) == ApproxZero<_type>(-base, factor));
            BOOST_CHECK(scaledEps * (factor + 1) != ApproxZero<_type>(-base, factor));
        }

        GDL_CHECK_THROW_DEV(ApproxZero<_type>(0, 1), Exception);
        GDL_CHECK_THROW_DEV(ApproxZero<_type>(1, 0), Exception);
        GDL_CHECK_THROW_DEV(ApproxZero<_type>(1, -1), Exception);
    }
}

BOOST_AUTO_TEST_CASE(Zero)
{
    TestApproxZero<F32>();
    TestApproxZero<F64>();
}



template <typename _registerType>
void TestApproxSSE()
{
    constexpr U32 numRegisterEntries = simd::numRegisterValues<_registerType>;

    using ElementType = decltype(simd::GetDataType<_registerType>());



    for (U32 i = 1; i < 20; ++i)
    {
        for (U32 j = 1; j < 100; ++j)
        {
            ElementType valueBase = Pow(static_cast<ElementType>(10.), static_cast<I32>(i - 10));
            ElementType epsilonBase = std::numeric_limits<ElementType>::epsilon(); // Epsilon of the current value

            _registerType values = _mm_setzero<_registerType>();
            _registerType cmp = values;


            for (U32 k = 0; k < numRegisterEntries; ++k)
                simd::SetValue(values, k, valueBase * static_cast<ElementType>(k));

            _registerType negValues = _mm_mul(values, _mm_set1<_registerType>(-1));

            _registerType scaledEps = _mm_mul(values, _mm_set1<_registerType>(epsilonBase));



            I32 factor = static_cast<I32>(j); // Scales the tolerance (1 = epsilon of value)


            Approx<_registerType> approx = Approx<_registerType>(values, factor, valueBase);

            // All comparisons true or false
            cmp = _mm_add(values, _mm_mul(scaledEps, _mm_set1<_registerType>(factor - 1)));
            BOOST_CHECK(cmp == approx);
            BOOST_CHECK(approx == cmp);
            BOOST_CHECK(!(cmp != approx));
            BOOST_CHECK(!(approx != cmp));

            cmp = _mm_add(values, _mm_mul(scaledEps, _mm_set1<_registerType>(factor + 1)));
            BOOST_CHECK(cmp != approx);
            BOOST_CHECK(approx != cmp);
            BOOST_CHECK(!(cmp == approx));
            BOOST_CHECK(!(approx == cmp));


            approx = Approx<_registerType>(negValues, factor, valueBase);
            cmp = _mm_add(negValues, _mm_mul(scaledEps, _mm_set1<_registerType>(factor - 1)));
            BOOST_CHECK(cmp == approx);
            BOOST_CHECK(approx == cmp);

            cmp = _mm_add(negValues, _mm_mul(scaledEps, _mm_set1<_registerType>(factor + 1)));
            BOOST_CHECK(cmp != approx);
            BOOST_CHECK(approx != cmp);



            approx = Approx<_registerType>(values, factor, valueBase);

            // Mixed comparison results
            for (U32 k = 0; k < numRegisterEntries; ++k)
            {
                cmp = values;
                // first value is 0! ---> comparison will use valueBase
                if (k == 0)
                    simd::SetValue(cmp, k, valueBase * epsilonBase * (factor - 1));
                else
                    simd::SetValue(cmp, k,
                                   simd::GetValue(values, k) + simd::GetValue(values, k) * epsilonBase * (factor - 1));
                BOOST_CHECK(cmp == approx);
                BOOST_CHECK(approx == cmp);

                // first value is 0! ---> comparison will use valueBase
                if (k == 0)
                    simd::SetValue(cmp, k, valueBase * epsilonBase * (factor + 1));
                else
                    simd::SetValue(cmp, k,
                                   simd::GetValue(values, k) + simd::GetValue(values, k) * epsilonBase * (factor + 1));
                BOOST_CHECK(cmp != approx);
                BOOST_CHECK(approx != cmp);
            }


            // Not all values should be compared
            Approx<_registerType, numRegisterEntries - 1> approxMinus1 =
                    Approx<_registerType, numRegisterEntries - 1>(values, factor, valueBase);

            for (U32 k = 0; k < numRegisterEntries; ++k)
            {
                cmp = values;
                // first value is 0! ---> comparison will use valueBase
                if (k == 0)
                    simd::SetValue(cmp, k, valueBase * epsilonBase * (factor - 1));
                else
                    simd::SetValue(cmp, k,
                                   simd::GetValue(values, k) + simd::GetValue(values, k) * epsilonBase * (factor - 1));
                BOOST_CHECK(cmp == approxMinus1);
                BOOST_CHECK(approxMinus1 == cmp);

                // first value is 0! ---> comparison will use valueBase
                if (k == 0)
                    simd::SetValue(cmp, k, valueBase * epsilonBase * (factor + 1));
                else
                    simd::SetValue(cmp, k,
                                   simd::GetValue(values, k) + simd::GetValue(values, k) * epsilonBase * (factor + 1));
                if (k < numRegisterEntries - 1)
                {
                    BOOST_CHECK(cmp != approxMinus1);
                    BOOST_CHECK(approxMinus1 != cmp);
                }
                else
                {
                    BOOST_CHECK(cmp == approxMinus1);
                    BOOST_CHECK(approxMinus1 == cmp);
                }
            }
        }
    }

    _registerType allEntriesOne = _mm_set1<_registerType>(1.);
    GDL_CHECK_THROW_DEV(Approx<_registerType>(allEntriesOne, 0), Exception);
    GDL_CHECK_THROW_DEV(Approx<_registerType>(allEntriesOne, -1), Exception);
}


BOOST_AUTO_TEST_CASE(Approx_Non_Zero_SSE)
{
    TestApproxSSE<__m128>();
    TestApproxSSE<__m128d>();
#ifdef __AVX2__
    TestApproxSSE<__m256>();
    TestApproxSSE<__m256d>();
#endif // __AVX2__
}



template <typename _registerType>
void TestApproxZeroSSE()
{
    constexpr U32 numRegisterEntries = simd::numRegisterValues<_registerType>;

    using ElementType = decltype(simd::GetDataType<_registerType>());

    // Base and scaling ctor
    for (U32 i = 1; i < 20; ++i)
    {
        for (U32 j = 1; j < 100; ++j)
        {
            ElementType base = Pow(static_cast<ElementType>(10.), static_cast<I32>(i - 10));

            _registerType scaledEps = _mm_set1<_registerType>(std::numeric_limits<ElementType>::epsilon() *
                                                              base); // Epsilon of the current value

            I32 factor = static_cast<I32>(j); // Scales the tolerance (1 = epsilon of value)

            _registerType zeroReg = _mm_setzero<_registerType>();
            _registerType cmp = _mm_mul(scaledEps, _mm_set1<_registerType>(factor - 1));

            // Compare values that are one epsilon above and below the adjusted tolerance
            BOOST_CHECK(cmp == Approx<_registerType>(zeroReg, factor, base));
            BOOST_CHECK(cmp == ApproxZero<_registerType>(base, factor));
            BOOST_CHECK(cmp == ApproxZero<_registerType>(-base, factor));

            cmp = _mm_mul(scaledEps, _mm_set1<_registerType>(factor + 1));
            BOOST_CHECK(cmp != Approx<_registerType>(zeroReg, factor, base));
            BOOST_CHECK(cmp != ApproxZero<_registerType>(base, factor));
            BOOST_CHECK(cmp != ApproxZero<_registerType>(-base, factor));


            BOOST_CHECK(zeroReg ==
                        Approx<_registerType>(_mm_mul(scaledEps, _mm_set1<_registerType>(factor - 1)), factor, base));
            BOOST_CHECK(zeroReg !=
                        Approx<_registerType>(_mm_mul(scaledEps, _mm_set1<_registerType>(factor + 1)), factor, base));


            // Only one comparison fails
            for (U32 k = 0; k < numRegisterEntries; ++k)
            {
                cmp = zeroReg;
                simd::SetValue(cmp, k, simd::GetValue(scaledEps, 0) * (factor + 1));

                BOOST_CHECK(cmp != ApproxZero<_registerType>(base, factor));
            }



            // Not all values compared
            for (U32 k = 0; k < numRegisterEntries; ++k)
            {
                cmp = zeroReg;
                simd::SetValue(cmp, k, simd::GetValue(scaledEps, 0) * (factor + 1));

                auto approxZero = ApproxZero<_registerType, numRegisterEntries - 1>(base, factor);
                if (k < numRegisterEntries - 1)
                    BOOST_CHECK(cmp != approxZero);
                else
                    BOOST_CHECK(cmp == approxZero);
            }
        }

        GDL_CHECK_THROW_DEV(ApproxZero<_registerType>(0, 1), Exception);
        GDL_CHECK_THROW_DEV(ApproxZero<_registerType>(1, 0), Exception);
        GDL_CHECK_THROW_DEV(ApproxZero<_registerType>(1, -1), Exception);
    }
}


BOOST_AUTO_TEST_CASE(Approx_Zero_SSE)
{
    TestApproxZeroSSE<__m128>();
    TestApproxZeroSSE<__m128d>();
#ifdef __AVX2__
    TestApproxZeroSSE<__m256>();
    TestApproxZeroSSE<__m256d>();
#endif // __AVX2__
}
