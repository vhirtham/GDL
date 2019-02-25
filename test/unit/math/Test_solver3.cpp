#include <boost/test/unit_test.hpp>


#include "gdl/math/solver/solver3.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;

BOOST_AUTO_TEST_CASE(TestSolver)
{
    Mat3fSSE A1(1, 0, 0, 0, 1, 0, 0, 0, 1);
    Vec3fSSE<> b1(1, 2, 3);
    Vec3fSSE<> exp1 = b1;
    BOOST_CHECK(Solver::Cramer(A1, b1) == exp1);

    Mat3fSSE A2(2, 1, 3, 4, 1, 4, 8, 1, 5);
    Vec3fSSE<> b2(2, 1, 2);
    Vec3fSSE<> exp2(3, -3, 1);
    BOOST_CHECK(Solver::Cramer(A2, b2) == exp2);

    Mat3fSSE AThrow(2, 1, 3, 2, 1, 3, 2, 1, 3);
    GDL_CHECK_THROW_DEV(Solver::Cramer(AThrow, b2), Exception);
}
