#include <benchmark/benchmark.h>

#include "gdl/math/solver/gauss.h"
#include "eigen3/Eigen/Dense"

using namespace GDL;



// Fixture declaration ------------------------------------------------------------------------------------------------

class SIMD : public benchmark::Fixture
{
public:
    MatSSE<F32, 8, 8> A;
    VecSSE<F32, 8> b;

    // clang-format off
    SIMD()
        : A{2,  4, -3,  5,  9, -3, -7,  1,
            3, -5,  2, -1,  1,  1, -3,  1,
            9,  1, -7, -6,  7,  2,  5, -7,
            4, -1,  6, -2, -3, -8, -3,  9,
            7,  2,  7,  3,  9, -5, -3, -4,
           -5,  5, -5,  5,  5, -5,  5, -5,
            5, -6,  3, -3,  2,  1, -6,  5,
            1, -2, -3, -4, -5,  6, -7,  8}
        , b{7, -7, -8, 0, 6, 0, 4, -2}
    {
    }
    // clang-format on
};


#ifdef EIGEN3_FOUND

class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix<F32, 8, 8> A;
    Eigen::Matrix<F32, 1, 8> b;

    // clang-format off
    Eigen3()
    {
        A << 2,  4, -3,  5,  9, -3, -7,  1,
             3, -5,  2, -1,  1,  1, -3,  1,
             9,  1, -7, -6,  7,  2,  5, -7,
             4, -1,  6, -2, -3, -8, -3,  9,
             7,  2,  7,  3,  9, -5, -3, -4,
            -5,  5, -5,  5,  5, -5,  5, -5,
             5, -6,  3, -3,  2,  1, -6,  5,
             1, -2, -3, -4, -5,  6, -7,  8;
        b << 7, -7, -8, 0, 6, 0, 4, -2;
    }
    // clang-format on
};

#endif // EIGEN3_FOUND


// Gauss --------------------------------------------------------------------------------------------------------------


BENCHMARK_F(SIMD, GaussDense8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::GaussPartialPivot(A, b));
}



// Eigen --------------------------------------------------------------------------------------------------------------

#ifdef EIGEN3_FOUND

// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
// https://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html

// Does not make sense to compare --- only valid for symmetric matrices
// BENCHMARK_F(Eigen3, LLT8x8)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(A.llt().solve(b));
//}



BENCHMARK_F(Eigen3, LDLT8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.ldlt().solve(b));
}



BENCHMARK_F(Eigen3, HouseholderQR8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.householderQr().solve(b));
}


BENCHMARK_F(Eigen3, LU8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.lu().solve(b));
}



BENCHMARK_F(Eigen3, PartialPivLU8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.partialPivLu().solve(b));
}

#endif // EIGEN3_FOUND



// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
