#include <boost/test/unit_test.hpp>

#include "gdl/GDLTypedefs.h"

#include "gdl/base/functions/isPowerOf2.h"
#include "gdl/base/functions/pow.h"

using namespace GDL;

#include <iostream>

BOOST_AUTO_TEST_CASE(Power_Functions)
{
    BOOST_CHECK(Pow<3>(3) == 27);
    BOOST_CHECK(Pow<3>(3.f) == 27.f);

    BOOST_CHECK(Pow(3, 3) == 27);
    BOOST_CHECK(Pow(3.f, 3) == 27.f);

    BOOST_CHECK(Square(5) == 25);
    BOOST_CHECK(Square(5.f) == 25.f);
}

BOOST_AUTO_TEST_CASE(IsPowerOf2_Function)
{
    // maximum number of different powers of 2 that can be stored in a size_t
    constexpr U32 numChecks = sizeof(size_t) * 8;

    for (U32 i = 0; i < numChecks; ++i)
    {
        size_t value = Pow(static_cast<size_t>(2), i);
        BOOST_CHECK(IsPowerOf2(value));
        if (i > 0)
            BOOST_CHECK(IsPowerOf2(value + 1) == false);
    }
}
