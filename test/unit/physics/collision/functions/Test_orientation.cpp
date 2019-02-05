#include <boost/test/unit_test.hpp>

#include "gdl/math/vec2.h"
#include "gdl/physics/collision/functions/orientation.h"

#include "test/tools/ExceptionChecks.h"

using namespace GDL;



BOOST_AUTO_TEST_CASE(Orientation_2D)
{
    Vec2fSSE a(1, 1);
    Vec2fSSE b{2, 3};

    Vec2fSSE c0{1, 4};
    BOOST_CHECK(Orientation(a, b, c0) > 0);
    Vec2fSSE c1{0, -1};
    BOOST_CHECK(Orientation(a, b, c1) == ApproxZero<F32>());
    Vec2fSSE c2{3, 4};
    BOOST_CHECK(Orientation(a, b, c2) < 0);
}
