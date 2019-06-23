#include <boost/test/unit_test.hpp>


#include "gdl/math/serial/mat4Serial.h"
#include "gdl/math/serial/vec4Serial.h"
#include "gdl/math/solver/solver4.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;

struct LinearSystem4x4
{
    std::array<F32, 16> A;
    std::array<F32, 4> b;
    std::array<F32, 4> x;

    LinearSystem4x4(std::array<F32, 16> pA, std::array<F32, 4> pb, std::array<F32, 4> px)
        : A{pA}
        , b{pb}
        , x{px}
    {
    }
};


class Mat4TestData
{
    std::vector<LinearSystem4x4> mTests;

    Mat4TestData()
    {
        std::array<F32, 16> A0 = {{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};
        std::array<F32, 4> b0 = {{1, 2, 3, 4}};
        std::array<F32, 4> x0 = {{1, 2, 3, 4}};
        CreatePermutations(A0, b0, x0);

        std::array<F32, 16> A1 = {{2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5}};
        std::array<F32, 4> b1 = {{-6, 0, -21, 18}};
        std::array<F32, 4> x1 = {{-1.5, 3, 1, -6}};
        CreatePermutations(A1, b1, x1);
    }

    void CreatePermutations(const std::array<F32, 16>& A, const std::array<F32, 4>& b, const std::array<F32, 4>& x)
    {
        for (U32 i = 0; i < 4; ++i)
            for (U32 j = 0; j < 4; ++j)
                for (U32 k = 0; k < 4; ++k)
                    for (U32 l = 0; l < 4; ++l)
                        if (i != j && i != k && i != l && j != k && j != l && k != l)
                            AddPermutation(A, b, x, {{i, j, k, l}});
    }

    void AddPermutation(const std::array<F32, 16>& A, const std::array<F32, 4>& b, const std::array<F32, 4>& x,
                        const std::array<U32, 4>& permutation)
    {
        std::array<F32, 16> AP;
        std::array<F32, 4> bP;
        for (U32 i = 0; i < 4; ++i)
        {
            bP[i] = b[permutation[i]];
            for (U32 j = 0; j < 4; ++j)
                AP[i + j * 4] = A[permutation[i] + j * 4];
        }
        mTests.emplace_back(AP, bP, x);
        //        std::cout << Mat4Serial<F32>(AP) << std::endl;
        //        std::cout << Vec4Serial<F32, true>(bP) << std::endl;
    }

public:
    static const std::vector<LinearSystem4x4>& GetTestData()
    {
        static Mat4TestData testData;
        return testData.mTests;
    }
};



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


    if (pivot)
    {
        const auto& testcases = Mat4TestData::GetTestData();
        for (U32 i = 0; i < testcases.size(); ++i)
            TestSolverTestcase(solver, Matrix(testcases[i].A), Vector(testcases[i].b), Vector(testcases[i].x));
    }
    else
    {
        TestSolverTestcase(solver, Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), Vector(1, 2, 3, 4),
                           Vector(1, 2, 3, 4));
        TestSolverTestcase(solver, Matrix(2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5), Vector(-6, 0, -21, 18),
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

#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(TestGaussNoPivotAVX)
{
    AVXSolverPtr solver = Solver::GaussNoPivot;
    TestSolver(solver, false);
}

#endif // __AVX2__



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



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(TestGaussPartialPivotAVX)
{
    AVXSolverPtr solver = Solver::GaussPartialPivot;
    TestSolver(solver);
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestLUNoPivotSerial)
{
    SerialSolverPtr solver = Solver::LU<Solver::Pivot::NONE>;
    TestSolver(solver, false);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestLUNoPivotSSE)
{
    SSESolverPtr solver = Solver::LUNoPivot;
    TestSolver(solver, false);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestLUPartialPivotSerial)
{
    SerialSolverPtr solver = Solver::LU<Solver::Pivot::PARTIAL>;
    TestSolver(solver);
}
