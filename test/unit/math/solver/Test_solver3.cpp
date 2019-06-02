#include <boost/test/unit_test.hpp>


#include "gdl/math/serial/mat3Serial.h"
#include "gdl/math/solver/solver3.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;

template <typename _solver>
bool SolverIsSSE = false;

template <typename _solver, typename Matrix, typename Vector>
void TestSolverTestcase(_solver solver, const Matrix& A, const Vector& b, const Vector& exp)
{
    Vector res = solver(A, b);
    Vector b2 = A * res;
    for (U32 i = 0; i < 3; ++i)
    {
        BOOST_CHECK(res[i] == Approx(exp[i], 10));
        BOOST_CHECK(b2[i] == Approx(b[i], 10));
    }
}



template <bool IsSSE, typename _solver>
void TestSolver(_solver solver)
{
    using Matrix = typename std::conditional<IsSSE, Mat3fSSE, Mat3fSerial>::type;
    using Vector = typename std::conditional<IsSSE, Vec3fSSE<true>, Vec3fSerial<true>>::type;

    TestSolverTestcase(solver, Matrix(1, 0, 0, 0, 1, 0, 0, 0, 1), Vector(1, 2, 3), Vector(1, 2, 3));
    TestSolverTestcase(solver, Matrix(-2, -1, -3, -4, -1, -4, -8, -1, -5), Vector(-2, -1, -2), Vector(3, -3, 1));
    TestSolverTestcase(solver, Matrix(2, 1, 3, 4, 1, 4, 8, 1, 5), Vector(2, 1, 2), Vector(3, -3, 1));
    TestSolverTestcase(solver, Matrix(2, 3, 1, 4, 4, 1, 8, 5, 1), Vector(2, 2, 1), Vector(3, -3, 1));
    TestSolverTestcase(solver, Matrix(1, 2, 3, 1, 4, 4, 1, 8, 5), Vector(1, 2, 2), Vector(3, -3, 1));
    TestSolverTestcase(solver, Matrix(1, 3, 2, 1, 4, 4, 1, 5, 8), Vector(1, 2, 2), Vector(3, -3, 1));
    TestSolverTestcase(solver, Matrix(3, 2, 1, 4, 4, 1, 5, 8, 1), Vector(2, 2, 1), Vector(3, -3, 1));
    TestSolverTestcase(solver, Matrix(3, 1, 2, 4, 1, 4, 5, 1, 8), Vector(2, 1, 2), Vector(3, -3, 1));

    Matrix AThrow(2, 1, 3, 2, 1, 3, 2, 1, 3);
    GDL_CHECK_THROW_DEV(solver(AThrow, Vector()), Exception);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestCramerSSE)
{
    TestSolver<true>(Solver::Cramer);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussPartialPivotSerial)
{
    Vec3Serial<F32, true> (&solver)(const Mat3Serial<F32>&, const Vec3Serial<F32, true>&) = Solver::GaussPartialPivot;
    TestSolver<false>(solver);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussPartialPivotSSE)
{
    Vec3fSSE<true> (&solver)(const Mat3fSSE&, const Vec3fSSE<true>&) = Solver::GaussPartialPivot;
    TestSolver<true>(solver);
}
