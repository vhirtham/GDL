#include <boost/test/unit_test.hpp>


#include "gdl/math/serial/mat3Serial.h"
#include "gdl/math/solver/solver3.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;


// --------------------------------------------------------------------------------------------------------------------

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



// --------------------------------------------------------------------------------------------------------------------

template <typename _solver>
void TestSolver(_solver solver)
{
    using Vector = decltype(solver({}, {}));
    using Matrix = typename std::conditional<std::is_same<Vector, Vec3fSSE<true>>::value, Mat3fSSE, Mat3fSerial>::type;

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

using SerialSolverPtr = Vec3Serial<F32, true> (*)(const Mat3Serial<F32>&, const Vec3Serial<F32, true>&);
using SSESolverPtr = Vec3fSSE<true> (*)(const Mat3fSSE&, const Vec3fSSE<true>&);



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestCramerSerial)
{
    SerialSolverPtr solver = Solver::Cramer;
    TestSolver(solver);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestCramerSSE)
{
    SSESolverPtr solver = Solver::Cramer;
    TestSolver(solver);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussPartialPivotSerial)
{
    SerialSolverPtr solver = Solver::GaussPartialPivot;
    TestSolver(solver);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussPartialPivotSSE)
{
    SSESolverPtr solver = Solver::GaussPartialPivot;
    TestSolver(solver);
}
