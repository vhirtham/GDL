#include "math/matXSIMD.inl"
#include "math/matXSingle.inl"
#include <benchmark/benchmark.h>

#ifdef EIGEN3_FOUND
#include <eigen3/Eigen/Core>
#endif
using namespace GDL;

const U32 N = 12;

class SIMD : public benchmark::Fixture
{
public:
    matXSIMD<N, N> A;
    matXSIMD<N, N> B;
};

class Single : public benchmark::Fixture
{
public:
    matXSingle<F32, N, N> A;
    matXSingle<F32, N, N> B;
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

BENCHMARK_F(SIMD, Construction_Def)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(matXSIMD<N, N>());
}


BENCHMARK_F(Single, Construction_Def)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(matXSingle<F32, N, N>());
}

// Value Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Construction_Val)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(matXSingle<F32, N, N>(valArray));
}

// Set Zero %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Set_Zero)(benchmark::State& state)
{
    for (auto _ : state)
        A.SetZero();
}
// Addition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(SIMD, Addition_Assignment)(benchmark::State& state)
{

    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}



BENCHMARK_F(Single, Addition_Assignment)(benchmark::State& state)
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

BENCHMARK_F(SIMD, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}


BENCHMARK_F(Single, Multiplication)(benchmark::State& state)
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

BENCHMARK_MAIN()
