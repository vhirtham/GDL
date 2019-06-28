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



// --------------------------------------------------------------------------------------------------------------------

class Mat4TestData
{
    std::vector<LinearSystem4x4> mTests;
    std::vector<LinearSystem4x4> mSymmetricTests;

    Mat4TestData()
    {
        U32 numTestSystems = 0;
        U32 numSymmetricTestSystems = 0;

        std::array<F32, 16> A0 = {{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};
        std::array<F32, 4> b0 = {{1, 2, 3, 4}};
        std::array<F32, 4> x0 = {{1, 2, 3, 4}};
        CreateRowPermutations(A0, b0, x0, mTests, numTestSystems);

        std::array<F32, 16> A1 = {{2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5}};
        std::array<F32, 4> b1 = {{-6, 0, -21, 18}};
        std::array<F32, 4> x1 = {{-1.5, 3, 1, -6}};
        CreateRowPermutations(A1, b1, x1, mTests, numTestSystems);
        CreateDiagonalPermutations(A1, b1, x1, mTests, numTestSystems);

        constexpr U32 numPermutations = 4 * 3 * 2 * 1;

        EXCEPTION(mTests.size() < numPermutations * numTestSystems || numTestSystems == 0,
                  "Testcases are not added as expected");


        std::array<F32, 16> S0 = {{4, 2, 4, 4, 2, 10, 5, 2, 4, 5, 9, 6, 4, 2, 6, 9}};
        std::array<F32, 4> r0 = {{36, 45, 65, 62}};
        std::array<F32, 4> y0 = {{1, 2, 3, 4}};
        CreateDiagonalPermutations(S0, r0, y0, mSymmetricTests, numSymmetricTestSystems);

        EXCEPTION(mTests.size() < numPermutations * numSymmetricTestSystems || numSymmetricTestSystems == 0,
                  "Testcases are not added as expected");

        for (U32 i = 0; i < mSymmetricTests.size(); ++i)
            CheckSymmetry(mSymmetricTests[i].A);
    }



    void CreateRowPermutations(const std::array<F32, 16>& A, const std::array<F32, 4>& b, const std::array<F32, 4>& x,
                               std::vector<LinearSystem4x4>& dataContainer, U32& numTestSystems)
    {
        ++numTestSystems;
        for (U32 i = 0; i < 4; ++i)
            for (U32 j = 0; j < 4; ++j)
                for (U32 k = 0; k < 4; ++k)
                    for (U32 l = 0; l < 4; ++l)
                        if (i != j && i != k && i != l && j != k && j != l && k != l)
                            AddRowPermutation(A, b, x, {{i, j, k, l}}, dataContainer);
    }



    void CreateDiagonalPermutations(const std::array<F32, 16>& A, const std::array<F32, 4>& b,
                                    const std::array<F32, 4>& x, std::vector<LinearSystem4x4>& dataContainer,
                                    U32& numTestSystems)
    {
        ++numTestSystems;
        for (U32 i = 0; i < 4; ++i)
            for (U32 j = 0; j < 4; ++j)
                for (U32 k = 0; k < 4; ++k)
                    for (U32 l = 0; l < 4; ++l)
                        if (i != j && i != k && i != l && j != k && j != l && k != l)
                            AddDiagonalPermutation(A, b, x, {{i, j, k, l}}, dataContainer);
    }



    void AddRowPermutation(const std::array<F32, 16>& A, const std::array<F32, 4>& b, const std::array<F32, 4>& x,
                           const std::array<U32, 4>& permutation, std::vector<LinearSystem4x4>& dataContainer)
    {
        std::array<F32, 16> AP;
        std::array<F32, 4> bP;
        for (U32 i = 0; i < 4; ++i)
        {
            bP[i] = b[permutation[i]];
            for (U32 j = 0; j < 4; ++j)
                AP[i + j * 4] = A[permutation[i] + j * 4];
        }
        dataContainer.emplace_back(AP, bP, x);
    }



    void AddDiagonalPermutation(const std::array<F32, 16>& A, const std::array<F32, 4>& b, const std::array<F32, 4>& x,
                                const std::array<U32, 4>& permutation, std::vector<LinearSystem4x4>& dataContainer)
    {
        std::array<F32, 16> AP;
        std::array<F32, 4> bP;
        std::array<F32, 4> xP;

        for (U32 i = 0; i < 4; ++i)
        {
            bP[i] = b[permutation[i]];
            xP[i] = x[permutation[i]];
            for (U32 j = 0; j < 4; ++j)
                AP[i + j * 4] = A[permutation[i] + permutation[j] * 4];
        }

        dataContainer.emplace_back(AP, bP, xP);
    }


    void CheckSymmetry(const std::array<F32, 16>& A)
    {
        bool symmetric = true;
        for (U32 i = 1; i < 4; ++i)
            for (U32 j = 0; j < i; ++j)
                symmetric &= A[i + j * 4] == Approx(A[j + i * 4]);
        EXCEPTION(!symmetric, "Created a non-symmetric matrix for a symmetric testcase!");
    }



public:
    static const Mat4TestData& Instance()
    {
        static Mat4TestData testData;
        return testData;
    }

    static const std::vector<LinearSystem4x4>& GetTestData()
    {
        return Instance().mTests;
    }


    static const std::vector<LinearSystem4x4>& GetSymmetricTestData()
    {
        return Instance().mSymmetricTests;
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
        BOOST_CHECK(res[i] == Approx(exp[i], 60));
        BOOST_CHECK(b2[i] == Approx(b[i], 60));
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _solver>
void TestSolver(_solver solver, bool pivot = true, bool symmetric = false)
{

    using Vector = decltype(solver({}, {}));
    using Matrix = typename std::conditional<
            std::is_same<_solver, SSESolverPtr>::value, Mat4fSSE,
            typename std::conditional<std::is_same<_solver, AVXSolverPtr>::value, Mat4fAVX, Mat4fSerial>::type>::type;

    if (not symmetric)
    {
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
            TestSolverTestcase(solver, Matrix(2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5),
                               Vector(-6, 0, -21, 18), Vector(-1.5, 3, 1, -6));

            Matrix ANeedPivot(0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
            GDL_CHECK_THROW_DEV(solver(ANeedPivot, Vector()), Exception);
        }
    }

    const auto& testcasesSymmetric = Mat4TestData::GetSymmetricTestData();
    for (U32 i = 0; i < testcasesSymmetric.size(); ++i)
        TestSolverTestcase(solver, Matrix(testcasesSymmetric[i].A), Vector(testcasesSymmetric[i].b),
                           Vector(testcasesSymmetric[i].x));


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
    SSESolverPtr solver = Solver::Gauss<Solver::Pivot::NONE>;
    TestSolver(solver, false);
}



// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(TestGaussNoPivotSerial)
{
    SerialSolverPtr solver = Solver::Gauss<Solver::Pivot::NONE>;
    TestSolver(solver, false);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(TestGaussNoPivotAVX)
{
    AVXSolverPtr solver = Solver::Gauss<Solver::Pivot::NONE>;
    TestSolver(solver, false);
}

#endif // __AVX2__



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

#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(TestGaussPartialPivotAVX)
{
    AVXSolverPtr solver = Solver::Gauss<Solver::Pivot::PARTIAL>;
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
