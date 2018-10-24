#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/pow.h"

#include "tests/tools/ExceptionChecks.h"

using namespace GDL;


template <typename _type>
void TestApprox()
{
    for (U32 i = 1; i < 20; ++i)
    {
        for (U32 j = 1; j < 100; ++j)
        {
            _type value = Pow(static_cast<_type>(10.), i) * static_cast<_type>(1.e-10);

            _type scaledEps = std::numeric_limits<_type>::epsilon() * value; // Epsilon of the current value

            I32 factor = static_cast<I32>(j); // Scales the tolerance (1 = epsilon of value)


            // Compare values that are one epsilon above and below the adjusted tolerance
            BOOST_CHECK(value + scaledEps * (factor - 1) == Approx<_type>(value, factor));
            BOOST_CHECK(value + scaledEps * (factor + 1) != Approx<_type>(value, factor));
            BOOST_CHECK(-value + scaledEps * (factor - 1) == Approx<_type>(-value, factor));
            BOOST_CHECK(-value + scaledEps * (factor + 1) != Approx<_type>(-value, factor));
        }
    }

    GDL_CHECK_THROW_DEV(Approx<_type>(0), Exception);
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
            _type base = Pow(static_cast<_type>(10.), i) * static_cast<_type>(1.e-10);

            _type scaledEps = std::numeric_limits<_type>::epsilon() * base; // Epsilon of the current value

            I32 factor = static_cast<I32>(j); // Scales the tolerance (1 = epsilon of value)


            // Compare values that are one epsilon above and below the adjusted tolerance
            BOOST_CHECK(scaledEps * (factor - 1) == ApproxZero<_type>(base, factor));
            BOOST_CHECK(scaledEps * (factor + 1) != ApproxZero<_type>(base, factor));
            BOOST_CHECK(scaledEps * (factor - 1) == ApproxZero<_type>(-base, factor));
            BOOST_CHECK(scaledEps * (factor + 1) != ApproxZero<_type>(-base, factor));
        }

        GDL_CHECK_THROW_DEV_DISABLE(ApproxZero<_type>(0, 1), Exception);
        GDL_CHECK_THROW_DEV_DISABLE(ApproxZero<_type>(1, 0), Exception);
        GDL_CHECK_THROW_DEV_DISABLE(ApproxZero<_type>(1, -1), Exception);
    }
}

BOOST_AUTO_TEST_CASE(Zero)
{
    TestApproxZero<F32>();
    TestApproxZero<F64>();
}
