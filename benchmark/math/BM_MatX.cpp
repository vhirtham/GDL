#include "math/matXSIMD.inl"
#include "math/matXSingle.inl"
#include <benchmark/benchmark.h>


using namespace GDL;

const U32 N = 24;
class SIMD : public benchmark::Fixture
{
public:
    matXSIMD<N, N> A_SIMD;
    matXSIMD<N, N> B_SIMD;
};

class Single : public benchmark::Fixture
{
public:
    matXSingle<F32, N, N> A_Single;
    matXSingle<F32, N, N> B_Single;
};


// Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(SIMD, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(matXSIMD<N, N>());
}


BENCHMARK_F(Single, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(matXSingle<F32, N, N>());
}


// Addition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(SIMD, Addition_PE_SIMD)(benchmark::State& state)
{

    for (auto _ : state)
        benchmark::DoNotOptimize(A_SIMD += B_SIMD);
}



BENCHMARK_F(Single, Addition_PE_Single)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A_Single += B_Single);
}



// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(SIMD, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A_SIMD * B_SIMD);
}


BENCHMARK_F(Single, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A_Single * B_Single);
}

BENCHMARK_MAIN()
