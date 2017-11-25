#include "math/mat4SIMD.inl"
#include "math/mat4Single.inl"
#include <benchmark/benchmark.h>

using namespace GDL;



static void Multiplication_Single(benchmark::State& state)
{
    mat4Single<F32> A_Single(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    mat4Single<F32> B_Single(8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6);
    for (auto _ : state)
        benchmark::DoNotOptimize(A_Single * B_Single);
}
BENCHMARK(Multiplication_Single);



static void Multiplication_SIMD(benchmark::State& state)
{
    mat4SIMD A_SIMD(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    mat4SIMD B_SIMD(8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6);
    for (auto _ : state)
        benchmark::DoNotOptimize(A_SIMD * B_SIMD);
}
BENCHMARK(Multiplication_SIMD);


static void Addition_PE_Single(benchmark::State& state)
{
    mat4Single<F32> A_Single(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    mat4Single<F32> B_Single(8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6);
    for (auto _ : state)
        benchmark::DoNotOptimize(A_Single += B_Single);
}
BENCHMARK(Addition_PE_Single);



static void Addition_PE_SIMD(benchmark::State& state)
{
    mat4SIMD A_SIMD(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    mat4SIMD B_SIMD(8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6);
    for (auto _ : state)
        benchmark::DoNotOptimize(A_SIMD += B_SIMD);
}
BENCHMARK(Addition_PE_SIMD);



static void Constructor1_Single(benchmark::State& state)
{
    for (auto _ : state)
        mat4Single<F32> C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}
BENCHMARK(Constructor1_Single);

static void Constructor_SIMD(benchmark::State& state)
{
    for (auto _ : state)
        mat4SIMD C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}
BENCHMARK(Constructor_SIMD);

BENCHMARK_MAIN()
