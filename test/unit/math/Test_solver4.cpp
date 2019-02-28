#include <boost/test/unit_test.hpp>


#include "gdl/math/single/mat4Single.h"
#include "gdl/math/single/vec4Single.h"
#include "gdl/math/solver/solver4.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;

template <typename _solver>
bool SolverIsSSE = false;

template <typename _solver, typename Matrix, typename Vector>
void TestSolverTestcase(_solver solver, const Matrix& A, const Vector& b, const Vector& exp)
{
    Vector res = solver(A, b);
    std::cout << res << std::endl;
    for (U32 i = 0; i < 4; ++i)
        BOOST_CHECK(res[i] == Approx(exp[i], 10));
}



template <bool IsSSE, typename _solver>
void TestSolver(_solver solver)
{
    using Matrix = typename std::conditional<IsSSE, Mat4fSSE, Mat4fSingle>::type;
    using Vector = typename std::conditional<IsSSE, Vec4fSSE<true>, Vec4fSingle<true>>::type;

    TestSolverTestcase(solver, Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), Vector(1, 2, 3, 4),
                       Vector(1, 2, 3, 4));
    TestSolverTestcase(solver, Matrix(2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5), Vector(-6, 0, -21, 18),
                       Vector(-1.5, 3, 1, -6));

    //    TestSolverTestcase(solver, Matrix(0, 2, 4, 6, 2, 2, -3, 1, 0, 3, 0, -6, 1, 2, 1, -5), Vector(0, -6, -21, 18),
    //                       Vector(-1.5, 3, 1, -6));

    Matrix AThrow(2, 1, 3, 4, 2, 1, 3, 4, 2, 1, 3, 4, 2, 1, 3, 4);
    GDL_CHECK_THROW_DEV(solver(AThrow, Vector()), Exception);
}



BOOST_AUTO_TEST_CASE(TestCramerSSE)
{
    TestSolver<true>(Solver::Cramer);
}



BOOST_AUTO_TEST_CASE(TestGaussNoPivotSSE)
{
    TestSolver<true>(Solver::GaussNoPivot);
}



BOOST_AUTO_TEST_CASE(TestGaussPartialPivotSSE)
{
    TestSolver<true>(Solver::GaussPartialPivot);
}
