#include <benchmark/benchmark.h>

#include "gdl/base/sse/directAccess.h"

#include <array>

using namespace GDL;


constexpr U32 i_f128 = 2;

// Fixture declaration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class Fixture : public benchmark::Fixture
{
public:
    F32 a = 0.f;
    __m128 f128 = _mm_setr_ps(1.f, 2.f, 3.f, 4.f);
};


// __m128 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <U32 _index>
F32 DirectAccessOperator(__m128 reg)
{
    return reg[_index];
}

template <U32 _index>
F32 Union(__m128 reg)
{
    union Data {
        __m128 sse;
        F32 array[4];
    };

    Data data{reg};
    return data.array[_index];
}


template <U32 _index>
F32 Store(__m128 reg)
{
    alignas(16) F32 array[4];
    _mm_store_ps(array, reg);
    return array[_index];
}


BENCHMARK_F(Fixture, DirectAccessOperator)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(a = DirectAccessOperator<i_f128>(f128));
}

BENCHMARK_F(Fixture, Store)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(a = Store<i_f128>(f128));
}

BENCHMARK_F(Fixture, Union)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(a = Union<i_f128>(f128));
}

BENCHMARK_F(Fixture, GetValue)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(a = sse::GetValue<i_f128>(f128));
}

// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_MAIN();
