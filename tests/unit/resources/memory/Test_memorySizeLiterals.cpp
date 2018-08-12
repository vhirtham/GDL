#include <boost/test/unit_test.hpp>


#include "gdl/resources/memory/memorySizeLiterals.h"


using namespace GDL;


BOOST_AUTO_TEST_CASE(Literals)
{
    BOOST_CHECK(1_B == 1);

    BOOST_CHECK(1_kB == (Pow<3, size_t>(10)));
    BOOST_CHECK(1_MB == (Pow<6, size_t>(10)));
    BOOST_CHECK(1_GB == (Pow<9, size_t>(10)));
    BOOST_CHECK(1_TB == (Pow<12, size_t>(10)));

    BOOST_CHECK(1_kiB == (Pow<10, size_t>(2)));
    BOOST_CHECK(1_MiB == (Pow<20, size_t>(2)));
    BOOST_CHECK(1_GiB == (Pow<30, size_t>(2)));
    BOOST_CHECK(1_TiB == (Pow<40, size_t>(2)));
}
