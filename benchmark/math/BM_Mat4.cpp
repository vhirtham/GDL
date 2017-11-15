#include "math/mat4f.inl"
#include <benchmark/benchmark.h>


GDL::mat4f A(__m128{0, 4, 8, 12}, __m128{1, 5, 9, 13}, __m128{2, 6, 10, 14}, __m128{3, 7, 11, 15});
GDL::mat4f B(__m128{8, 11, 6, 2}, __m128{5, 4, 9, 3}, __m128{3, 7, 5, 2}, __m128{6, 8, 2, 6});


static void BM_Multiplication(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}

// Register the function as a benchmark
BENCHMARK(BM_Multiplication);

BENCHMARK_MAIN()
