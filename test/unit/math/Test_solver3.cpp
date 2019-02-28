#include <boost/test/unit_test.hpp>


#include "gdl/math/solver/solver3.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;

template <typename _solver>
void TestSolver(_solver solver)
{
    Mat3fSSE A1(1, 0, 0, 0, 1, 0, 0, 0, 1);
    Vec3fSSE<> b1(1, 2, 3);
    Vec3fSSE<> exp1 = b1;
    BOOST_CHECK(solver(A1, b1) == exp1);

    Mat3fSSE A2(2, 1, 3, 4, 1, 4, 8, 1, 5);
    Vec3fSSE<> b2(2, 1, 2);
    Vec3fSSE<> exp2(3, -3, 1);
    Vec3fSSE<> res2 = solver(A2, b2);
    for (U32 i = 0; i < 3; ++i)
        BOOST_CHECK(res2[i] == Approx(exp2[i], 10));

    Mat3fSSE A3(-2, -1, -3, -4, -1, -4, -8, -1, -5);
    Vec3fSSE<> b3(-2, -1, -2);
    Vec3fSSE<> exp3(3, -3, 1);
    Vec3fSSE<> res3 = solver(A3, b3);
    for (U32 i = 0; i < 3; ++i)
        BOOST_CHECK(res3[i] == Approx(exp3[i], 10));

    // Mat3fSSE AThrow(2, 1, 3, 2, 1, 3, 2, 1, 3);
    // GDL_CHECK_THROW_DEV(solver(AThrow, b2), Exception);
}

BOOST_AUTO_TEST_CASE(TestCramer)
{
    TestSolver(Solver::Cramer);
}

BOOST_AUTO_TEST_CASE(TestGaussPartialPivot)
{
    TestSolver(Solver::GaussPartialPivot);
}
