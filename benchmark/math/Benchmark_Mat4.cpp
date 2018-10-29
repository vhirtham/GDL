#include "gdl/math/mat4SIMD.h"
#include "gdl/math/mat4Single.h"
#include <benchmark/benchmark.h>

using namespace GDL;

// Fixture declaration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class SIMD : public benchmark::Fixture
{
public:
    Mat4SIMD A;
    Mat4SIMD B;

    SIMD()
        : A{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
        , B{8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6}
    {
    }
};


class Single : public benchmark::Fixture
{
public:
    Mat4Single<F32> A;
    Mat4Single<F32> B;

    Single()
        : A{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
        , B{8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6}
    {
    }
};



// Value Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        Mat4Single<F32> C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}


BENCHMARK_F(SIMD, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        Mat4SIMD C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
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



// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
BENCHMARK_MAIN();
