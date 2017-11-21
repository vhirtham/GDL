#include "math/matXSIMD.inl"
#include <benchmark/benchmark.h>


using namespace GDL;

const U32 N = 12;

matXSIMD<N, N> A;
matXSIMD<N, N> B;

static void Multiplication_SIMD(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}
BENCHMARK(Multiplication_SIMD);


static void Construction_SIMD(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(matXSIMD<N, N>());
}
BENCHMARK(Construction_SIMD);

BENCHMARK_MAIN()
