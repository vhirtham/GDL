#include <boost/test/unit_test.hpp>


#include "gdl/math/serial/mat3Serial.h"
#include "gdl/math/serial/vec3Serial.h"
#include "gdl/math/simd/mat3fSSE.h"
#include "gdl/math/simd/vec3fSSE.h"
#include "gdl/math/solver/solver3.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;

struct LinearSystem3x3
{
    std::array<F32, 9> A;
    std::array<F32, 3> b;
    std::array<F32, 3> x;

    LinearSystem3x3(std::array<F32, 9> pA, std::array<F32, 3> pb, std::array<F32, 3> px)
        : A{pA}
        , b{pb}
        , x{px}
    {
    }
};



// --------------------------------------------------------------------------------------------------------------------

class Mat3TestData
{
    std::vector<LinearSystem3x3> mTests;

    Mat3TestData()
    {
        std::array<F32, 9> A0 = {{1, 0, 0, 0, 1, 0, 0, 0, 1}};
        std::array<F32, 3> b0 = {{1, 2, 3}};
        std::array<F32, 3> x0 = {{1, 2, 3}};
        CreatePermutations(A0, b0, x0);

        std::array<F32, 9> A1 = {{-2, -1, -3, -4, -1, -4, -8, -1, -5}};
        std::array<F32, 3> b1 = {{-2, -1, -2}};
        std::array<F32, 3> x1 = {{3, -3, 1}};
        CreatePermutations(A1, b1, x1);

        std::array<F32, 9> A2 = {{2, 1, 3, 4, 1, 4, 8, 1, 5}};
        std::array<F32, 3> b2 = {{2, 1, 2}};
        std::array<F32, 3> x2 = {{3, -3, 1}};
        CreatePermutations(A2, b2, x2);
    }

    void CreatePermutations(const std::array<F32, 9>& A, const std::array<F32, 3>& b, const std::array<F32, 3>& x)
    {
        for (U32 i = 0; i < 3; ++i)
            for (U32 j = 0; j < 3; ++j)
                for (U32 k = 0; k < 3; ++k)
                    if (i != j && i != k && j != k)
                        AddPermutation(A, b, x, {{i, j, k}});
    }

    void AddPermutation(const std::array<F32, 9>& A, const std::array<F32, 3>& b, const std::array<F32, 3>& x,
                        const std::array<U32, 3>& permutation)
    {
        std::array<F32, 9> AP;
        std::array<F32, 3> bP;
        for (U32 i = 0; i < 3; ++i)
        {
            bP[i] = b[permutation[i]];
            for (U32 j = 0; j < 3; ++j)
                AP[i + j * 3] = A[permutation[i] + j * 3];
        }
        mTests.emplace_back(AP, bP, x);
    }

public:
    static const std::vector<LinearSystem3x3>& GetTestData()
    {
        static Mat3TestData testData;
        return testData.mTests;
    }
};



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
void TestSolver(_solver solver, bool pivot = true)
{
    using Vector = decltype(solver({}, {}));
    using Matrix = typename std::conditional<std::is_same<Vector, Vec3fSSE<true>>::value, Mat3fSSE, Mat3fSerial>::type;

    if (pivot)
    {
        const auto& testcases = Mat3TestData::GetTestData();
        for (U32 i = 0; i < testcases.size(); ++i)
            TestSolverTestcase(solver, Matrix(testcases[i].A), Vector(testcases[i].b), Vector(testcases[i].x));
    }
    else
    {

        TestSolverTestcase(solver, Matrix(1, 0, 0, 0, 1, 0, 0, 0, 1), Vector(1, 2, 3), Vector(1, 2, 3));
        TestSolverTestcase(solver, Matrix(-2, -1, -3, -4, -1, -4, -8, -1, -5), Vector(-2, -1, -2), Vector(3, -3, 1));
        TestSolverTestcase(solver, Matrix(2, 1, 3, 4, 1, 4, 8, 1, 5), Vector(2, 1, 2), Vector(3, -3, 1));

        Matrix ANeedPivot(0, 1, 0, 1, 0, 0, 0, 0, 1);
        GDL_CHECK_THROW_DEV(solver(ANeedPivot, Vector()), Exception);
    }
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

BOOST_AUTO_TEST_CASE(TestGaussNoPivotSerial)
{
    SerialSolverPtr solver = Solver::Gauss<Solver::Pivot::NONE>;
    TestSolver(solver, false);
}


// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussNoPivotSSE)
{
    SSESolverPtr solver = Solver::Gauss<Solver::Pivot::NONE>;
    TestSolver(solver, false);
}


// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussPartialPivotSerial)
{
    SerialSolverPtr solver = Solver::Gauss<Solver::Pivot::PARTIAL>;
    TestSolver(solver);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussPartialPivotSSE)
{
    SSESolverPtr solver = Solver::Gauss<Solver::Pivot::PARTIAL>;
    TestSolver(solver);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestLUNoPivotSerial)
{
    SerialSolverPtr solver = Solver::LU<Solver::Pivot::NONE>;
    TestSolver(solver, false);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestLUNoPivotSSE)
{
    SSESolverPtr solver = Solver::LU<Solver::Pivot::NONE>;
    TestSolver(solver, false);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestLUPartialPivotSerial)
{
    SerialSolverPtr solver = Solver::LU<Solver::Pivot::PARTIAL>;
    TestSolver(solver);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestLUPartialPivotSSE)
{
    SSESolverPtr solver = Solver::LU<Solver::Pivot::PARTIAL>;
    TestSolver(solver);
}
