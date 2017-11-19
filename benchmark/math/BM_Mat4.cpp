#include "math/mat4SIMD.inl"
#include "math/mat4Single.inl"
#include <benchmark/benchmark.h>

using namespace GDL;

mat4Single<F32> A_Single(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
mat4Single<F32> B_Single(8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6);

static void Multiplication_Single(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A_Single * B_Single);
}
BENCHMARK(Multiplication_Single);

mat4SIMD A_SSE(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
mat4SIMD B_SSE(8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6);


static void Multiplication_SSE(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A_SSE * B_SSE);
}
BENCHMARK(Multiplication_SSE);


static void Constructor1_Single(benchmark::State& state)
{
    for (auto _ : state)
        mat4Single<F32> C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}
BENCHMARK(Constructor1_Single);

static void Constructor_SSE(benchmark::State& state)
{
    for (auto _ : state)
        mat4SIMD C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}
BENCHMARK(Constructor_SSE);

BENCHMARK_MAIN()
