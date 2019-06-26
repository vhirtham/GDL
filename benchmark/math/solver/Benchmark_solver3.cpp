#include <benchmark/benchmark.h>

#include "gdl/math/solver/solver3.h"
#include "eigen3/Eigen/Dense"

using namespace GDL;



// OPTIONS ------------------------------------------------------------------------------------------------------------

// solver types
#define BENCHMARK_CRAMER
#define BENCHMARK_GAUSS
#define BENCHMARK_LU

// pivoting
#define BENCHMARK_NOPIVOT
#define BENCHMARK_PARTIALPIVOT

// factorization
#define BENCHMARK_FACTORIZATION

// vectorization
#define BENCHMARK_SERIAL
#define BENCHMARK_SSE

// Eigen
#define BENCHMARK_EIGEN



// Fixture declaration ------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_SERIAL

class Serial : public benchmark::Fixture
{
public:
    Mat3Serial<F32> A;
    Vec3Serial<F32, true> b;

    Serial()
        : A{2, 1, 3, 4, 1, 4, 8, 1, 5}
        , b{2, 1, 2}
    {
    }
};

#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

class SSE : public benchmark::Fixture
{
public:
    Mat3fSSE A;
    Vec3f b;

    SSE()
        : A{2, 1, 3, 4, 1, 4, 8, 1, 5}
        , b{2, 1, 2}
    {
    }
};

#endif // BENCHMARK_SSE



#ifdef EIGEN3_FOUND
#ifdef BENCHMARK_EIGEN

class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix3f A;
    Eigen::Vector3f b;

    Eigen3()
    {
        A << 2, 4, 8, 1, 1, 1, 3, 4, 5;
        b << 2, 1, 2;
    }
};

#endif // BENCHMARK_EIGEN
#endif // EIGEN3_FOUND



// Cramer -------------------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_CRAMER
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, Cramer)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Cramer(A, b));
}

#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, Cramer)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Cramer(A, b));
}

#endif // BENCHMARK_SSE
#endif // BENCHMARK_CRAMER



// Gauss - no pivot ---------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_GAUSS
#ifdef BENCHMARK_NOPIVOT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, GaussNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Gauss<Solver::Pivot::NONE>(A, b));
}

#endif // BENCHMARK_SERIAL
#endif // BENCHMARK_NOPIVOT


// Gauss - partial pivot ----------------------------------------------------------------------------------------------

#ifdef BENCHMARK_PARTIALPIVOT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, GaussPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Gauss<Solver::Pivot::PARTIAL>(A, b));
}

#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, GaussPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::GaussPartialPivot(A, b));
}

#endif // BENCHMARK_SSE
#endif // BENCHMARK_PARTIALPIVOT
#endif // BENCHMARK_GAUSS



// LU - no pivot ------------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_LU
#ifdef BENCHMARK_NOPIVOT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, LUNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A, b));
}

#ifdef BENCHMARK_FACTORIZATION



BENCHMARK_F(Serial, LUNoPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A));
}



BENCHMARK_F(Serial, LUNoPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, LUNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SSE, LUNoPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A));
}



BENCHMARK_F(SSE, LUNoPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SSE
#endif // BENCHMARK_NOPIVOT



// LU - partial pivot -------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_PARTIALPIVOT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, LUPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, LUPartialPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A));
}



BENCHMARK_F(Serial, LUPartialPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::PARTIAL>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, LUPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SSE, LUPartialPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A));
}



BENCHMARK_F(SSE, LUPartialPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::PARTIAL>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SSE
#endif // BENCHMARK_PARTIALPIVOT
#endif // BENCHMARK_LU



// Eigen --------------------------------------------------------------------------------------------------------------

#ifdef EIGEN3_FOUND
#ifdef BENCHMARK_EIGEN

// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
// https://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html

BENCHMARK_F(Eigen3, LLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.llt().solve(b));
}



BENCHMARK_F(Eigen3, LDLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.ldlt().solve(b));
}



BENCHMARK_F(Eigen3, HouseholderQR)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.householderQr().solve(b));
}



BENCHMARK_F(Eigen3, LU)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.lu().solve(b));
}



BENCHMARK_F(Eigen3, PartialPivLU)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.partialPivLu().solve(b));
}

#endif // BENCHMARK_EIGEN
#endif // EIGEN3_FOUND



// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
