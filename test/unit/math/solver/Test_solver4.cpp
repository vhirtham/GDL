#include <boost/test/unit_test.hpp>


#include "gdl/math/serial/mat4Serial.h"
#include "gdl/math/serial/vec4Serial.h"
#include "gdl/math/solver/solver4.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;


// --------------------------------------------------------------------------------------------------------------------

using SerialSolverPtr = Vec4Serial<F32, true> (*)(const Mat4Serial<F32>&, const Vec4Serial<F32, true>&);
using SSESolverPtr = Vec4fSSE<true> (*)(const Mat4fSSE&, const Vec4fSSE<true>&);
using AVXSolverPtr = Vec4fSSE<true> (*)(const Mat4fAVX&, const Vec4fSSE<true>&);



// --------------------------------------------------------------------------------------------------------------------

template <typename _solver, typename Matrix, typename Vector>
void TestSolverTestcase(_solver solver, const Matrix& A, const Vector& b, const Vector& exp)
{
    Vector res = solver(A, b);
    Vector b2 = A * res;
    for (U32 i = 0; i < 4; ++i)
    {
        BOOST_CHECK(res[i] == Approx(exp[i], 50));
        BOOST_CHECK(b2[i] == Approx(b[i], 50));
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _solver>
void TestSolver(_solver solver, bool pivot = true)
{

    using Vector = decltype(solver({}, {}));
    using Matrix = typename std::conditional<
            std::is_same<_solver, SSESolverPtr>::value, Mat4fSSE,
            typename std::conditional<std::is_same<_solver, AVXSolverPtr>::value, Mat4fAVX, Mat4fSerial>::type>::type;

    TestSolverTestcase(solver, Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), Vector(1, 2, 3, 4),
                       Vector(1, 2, 3, 4));
    TestSolverTestcase(solver, Matrix(2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5), Vector(-6, 0, -21, 18),
                       Vector(-1.5, 3, 1, -6));

    if (pivot)
    {
        // 1234
        TestSolverTestcase(solver, Matrix(0, 2, 4, 6, 2, 2, -3, 1, 0, 3, 0, -6, 1, 2, 1, -5), Vector(0, -6, -21, 18),
                           Vector(-1.5, 3, 1, -6));
        // 1243
        TestSolverTestcase(solver, Matrix(0, 2, 6, 4, 2, 2, 1, -3, 0, 3, -6, 0, 1, 2, -5, 1), Vector(0, -6, 18, -21),
                           Vector(-1.5, 3, 1, -6));
        // 1324
        TestSolverTestcase(solver, Matrix(0, 4, 2, 6, 2, -3, 2, 1, 0, 0, 3, -6, 1, 1, 2, -5), Vector(0, -21, -6, 18),
                           Vector(-1.5, 3, 1, -6));
        // 1342
        TestSolverTestcase(solver, Matrix(0, 4, 6, 2, 2, -3, 1, 2, 0, 0, -6, 3, 1, 1, -5, 2), Vector(0, -21, 18, -6),
                           Vector(-1.5, 3, 1, -6));
        // 1423
        TestSolverTestcase(solver, Matrix(0, 6, 2, 4, 2, 1, 2, -3, 0, -6, 3, 0, 1, -5, 2, 1), Vector(0, 18, -6, -21),
                           Vector(-1.5, 3, 1, -6));
        // 1432
        TestSolverTestcase(solver, Matrix(0, 6, 4, 2, 2, 1, -3, 2, 0, -6, 0, 3, 1, -5, 1, 2), Vector(0, 18, -21, -6),
                           Vector(-1.5, 3, 1, -6));
        // 2134
        TestSolverTestcase(solver, Matrix(2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5), Vector(-6, 0, -21, 18),
                           Vector(-1.5, 3, 1, -6));
        // 2143
        TestSolverTestcase(solver, Matrix(2, 0, 6, 4, 2, 2, 1, -3, 3, 0, -6, 0, 2, 1, -5, 1), Vector(-6, 0, 18, -21),
                           Vector(-1.5, 3, 1, -6));
    }

    Matrix AThrow(2, 1, 3, 4, 2, 1, 3, 4, 2, 1, 3, 4, 2, 1, 3, 4);
    GDL_CHECK_THROW_DEV(solver(AThrow, Vector()), Exception);
}



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

#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(TestCramerAVX)
{
    AVXSolverPtr solver = Solver::Cramer;
    TestSolver(solver);
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussNoPivotSSE)
{
    SSESolverPtr solver = Solver::GaussNoPivot;
    TestSolver(solver, false);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussNoPivotSerial)
{
    SerialSolverPtr solver = Solver::GaussNoPivot;
    TestSolver(solver, false);
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
