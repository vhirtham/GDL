#include <benchmark/benchmark.h>

#include "gdl/math/solver/solver4.h"
#include "eigen3/Eigen/Dense"

using namespace GDL;



// Fixture declaration ------------------------------------------------------------------------------------------------

class SIMD : public benchmark::Fixture
{
public:
    Mat4fSSE A;
    Vec4f b;

    SIMD()
        : A{2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5}
        , b{-6, 0, -21, 18}
    {
    }
};


#ifdef EIGEN3_FOUND

class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix4f A;
    Eigen::Vector4f b;

    Eigen3()
    {
        A << 2, 2, 3, 2, 0, 2, 0, 1, 4, -3, 0, 1, 6, 1, -6, -5;
        b << -6, 0, -21, 18;
    }
};

#endif // EIGEN3_FOUND


// Cramer -------------------------------------------------------------------------------------------------------------



BENCHMARK_F(SIMD, Cramer)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Cramer(A, b));
}



// Gauss --------------------------------------------------------------------------------------------------------------


BENCHMARK_F(SIMD, GaussNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::GaussNoPivot(A, b));
}



BENCHMARK_F(SIMD, GaussPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::GaussPartialPivot(A, b));
}



// Eigen --------------------------------------------------------------------------------------------------------------

#ifdef EIGEN3_FOUND

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

#endif // EIGEN3_FOUND


// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
