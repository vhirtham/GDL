#include <benchmark/benchmark.h>

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
    std::array<REGISTER, 8> matrix_in;
    std::array<REGISTER, 8> matrix_out;
};



BENCHMARK_F(FixtureTemplate, Transpose)(benchmark::State& state)
{
    for (auto _ : state)
    {
        Transpose<ROWS, COLS, OFFSET_IN, OFFSET_OUT>(matrix_in, matrix_out);
        benchmark::ClobberMemory();
    }
}



BENCHMARK_MAIN();
