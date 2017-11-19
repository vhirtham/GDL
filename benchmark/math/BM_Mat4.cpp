#include "math/mat4SIMD.inl"
#include "math/mat4Single.inl"
#include <benchmark/benchmark.h>

using namespace GDL;

mat4Single<F32> A_Single(std::array<F32, 16>{{0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}});
mat4Single<F32> B_Single(std::array<F32, 16>{{8, 11, 6, 2, 5, 4, 9, 3, 3, 7, 5, 2, 6, 8, 2, 6}});

static void Multiplication_Single(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A_Single * B_Single);
}
BENCHMARK(Multiplication_Single);

mat4SIMD A_SSE(__m128{0, 4, 8, 12}, __m128{1, 5, 9, 13}, __m128{2, 6, 10, 14}, __m128{3, 7, 11, 15});
mat4SIMD B_SSE(__m128{8, 11, 6, 2}, __m128{5, 4, 9, 3}, __m128{3, 7, 5, 2}, __m128{6, 8, 2, 6});


static void Multiplication_SSE(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A_SSE * B_SSE);
}
BENCHMARK(Multiplication_SSE);


BENCHMARK_MAIN()
