#include <boost/test/unit_test.hpp>


#include "gdl/resources/memory/memorySize.h"


using namespace GDL;


BOOST_AUTO_TEST_CASE(Construction)
{
    BOOST_CHECK((1_B).GetNumBytes() == 1);

    BOOST_CHECK((1_kB).GetNumBytes() == (Pow<3, size_t>(10)));
    BOOST_CHECK((1_MB).GetNumBytes() == (Pow<6, size_t>(10)));
    BOOST_CHECK((1_GB).GetNumBytes() == (Pow<9, size_t>(10)));
    BOOST_CHECK((1_TB).GetNumBytes() == (Pow<12, size_t>(10)));

    BOOST_CHECK((1_KiB).GetNumBytes() == (Pow<10, size_t>(2)));
    BOOST_CHECK((1_MiB).GetNumBytes() == (Pow<20, size_t>(2)));
    BOOST_CHECK((1_GiB).GetNumBytes() == (Pow<30, size_t>(2)));
    BOOST_CHECK((1_TiB).GetNumBytes() == (Pow<40, size_t>(2)));
}

BOOST_AUTO_TEST_CASE(Operators)
{
    MemorySize a = 1_MB;
    MemorySize b = 2_MB;

    BOOST_CHECK(a != b);
    BOOST_CHECK(4 * a == b * 2); // Test both multiplication operator versions

    BOOST_CHECK(a < b);
    BOOST_CHECK(3 * a > b);
    BOOST_CHECK(a <= b);
    BOOST_CHECK(3 * a >= b);

    BOOST_CHECK(4 * a <= b * 2);
    BOOST_CHECK(4 * a >= b * 2);



    // size_t comparisons
    BOOST_CHECK(a == 1000000);
    BOOST_CHECK(b != 1000000);
    BOOST_CHECK(1000000 == a);
    BOOST_CHECK(1000000 != b);

    BOOST_CHECK(1000000 < b);
    BOOST_CHECK(3000000 > b);
    BOOST_CHECK(b > 1000000);
    BOOST_CHECK(b < 3000000);

    BOOST_CHECK(1000000 <= b);
    BOOST_CHECK(3000000 >= b);
    BOOST_CHECK(b >= 1000000);
    BOOST_CHECK(b <= 3000000);

    BOOST_CHECK(2000000 <= b);
    BOOST_CHECK(2000000 >= b);
    BOOST_CHECK(b <= 2000000);
    BOOST_CHECK(b >= 2000000);
}
