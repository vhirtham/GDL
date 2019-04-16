#include <benchmark/benchmark.h>

#include "gdl/math/solver/solver3.h"
#include "eigen3/Eigen/Dense"

using namespace GDL;



// Fixture declaration ------------------------------------------------------------------------------------------------

class SIMD : public benchmark::Fixture
{
public:
    Mat3fSSE A;
    Vec3f b;

    SIMD()
        : A{2, 1, 3, 4, 1, 4, 8, 1, 5}
        , b{2, 1, 2}
    {
    }
};


#ifdef EIGEN3_FOUND

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

#endif // EIGEN3_FOUND


// Cramer -------------------------------------------------------------------------------------------------------------



BENCHMARK_F(SIMD, Cramer)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Cramer(A, b));
}



// Gauss --------------------------------------------------------------------------------------------------------------


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
