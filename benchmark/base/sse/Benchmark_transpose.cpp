#include <benchmark/benchmark.h>

#include "gdl/base/simd/constants.h"
#include "gdl/base/simd/transpose.h"

#include <array>

#ifndef REGISTER
#define REGISTER __m256
#endif

#ifndef ROWS
#define ROWS 1
#endif

#ifndef COLS
#define COLS 1
#endif

#ifndef OFFSET_IN
#define OFFSET_IN 0
#endif

#ifndef OFFSET_OUT
#define OFFSET_OUT 0
#endif


#define DIFF ROWS - COLS

using namespace GDL::simd;

struct FixtureTemplate : public benchmark::Fixture
{
    std::array<REGISTER, numRegisterValues<REGISTER>> matrix_in;
    std::array<REGISTER, numRegisterValues<REGISTER>> matrix_out;
};



BENCHMARK_F(FixtureTemplate, Transpose_keep_unused)(benchmark::State& state)
{
    for (auto _ : state)
    {
        Transpose<ROWS, COLS, OFFSET_IN, OFFSET_OUT, 0, 0, 1, 1, false>(matrix_in, matrix_out);
        benchmark::ClobberMemory();
    }
}



BENCHMARK_F(FixtureTemplate, Transpose_set_zero)(benchmark::State& state)
{
    for (auto _ : state)
    {
        Transpose<ROWS, COLS, OFFSET_IN, OFFSET_OUT, 0, 0, 1, 1, true, true>(matrix_in, matrix_out);
        benchmark::ClobberMemory();
    }
}



BENCHMARK_F(FixtureTemplate, Transpose_overwrite)(benchmark::State& state)
{
    for (auto _ : state)
    {
        Transpose<ROWS, COLS, OFFSET_IN, OFFSET_OUT, 0, 0, 1, 1, true, false>(matrix_in, matrix_out);
        benchmark::ClobberMemory();
    }
}



BENCHMARK_MAIN();
