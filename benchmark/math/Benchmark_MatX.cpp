#include "gdl/math/matSIMD.h"
#include "gdl/math/matSingle.h"
#include <benchmark/benchmark.h>

#ifdef EIGEN3_FOUND
#include <eigen3/Eigen/Core>
#endif
using namespace GDL;

constexpr U32 N = 32;
constexpr U32 M = N - 1;


// Fixture declaration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class SIMD : public benchmark::Fixture
{
public:
    MatSIMD<F32, N, N> A;
    MatSIMD<F32, N, N> B;
    MatSingle<F32, M, N> C;
    std::array<F32, N * N> valArray;
    std::array<F32, M * N> valArray2;

    SIMD()
    {
        for (U32 i = 0; i < N * N; ++i)
            valArray[i] = 2 * i;
        for (U32 i = 0; i < M * N; ++i)
            valArray2[i] = 2 * i;
    }
};


class Single : public benchmark::Fixture
{
public:
    MatSingle<F32, N, N> A;
    MatSingle<F32, N, N> B;
    std::array<F32, N * N> valArray;

    Single()
    {
        for (U32 i = 0; i < N * N; ++i)
            valArray[i] = 2 * i;
    }
};


#ifdef EIGEN3_FOUND
class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix<F32, N, N> A;
    Eigen::Matrix<F32, N, N> B;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
#endif



// Default Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Construction_Def)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSingle<F32, N, N>());
}


BENCHMARK_F(SIMD, Construction_Def)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSIMD<F32, N, N>());
}

// Value Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Construction_Val)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSingle<F32, N, N>(valArray));
}



BENCHMARK_F(SIMD, Construction_Val)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSIMD<F32, N, N>(valArray));
}



BENCHMARK_F(SIMD, Construction_Val_non_full_registers)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSIMD<F32, M, N>(valArray2));
}

BENCHMARK_F(Single, Construction_Val_Variadic_12x12)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSingle<F32, 12, 12>(
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.));
}

BENCHMARK_F(SIMD, Construction_Val_Variadic_12x12)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSIMD<F32, 12, 12>(
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.));
}


// Set Zero %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Set_Zero)(benchmark::State& state)
{
    for (auto _ : state)
        A.SetZero();
}


BENCHMARK_F(SIMD, Set_Zero)(benchmark::State& state)
{
    for (auto _ : state)
        A.SetZero();
}



// Data %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Data)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.Data());
}


BENCHMARK_F(SIMD, Data)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.Data());
}


BENCHMARK_F(SIMD, Data_non_full_registers)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(C.Data());
}



// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Addition_Assignment)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}


BENCHMARK_F(SIMD, Addition_Assignment)(benchmark::State& state)
{

    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}


#ifdef EIGEN3_FOUND
BENCHMARK_F(Eigen3, Addition_Assignment)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}
#endif



// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}


BENCHMARK_F(SIMD, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}


#ifdef EIGEN3_FOUND
BENCHMARK_F(Eigen3, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize((A * B).eval());
}
#endif



// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_MAIN();
