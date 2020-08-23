#include <benchmark/benchmark.h>

#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/constants.h"
#include "gdl/base/simd/registerSum.h"

#include <array>



#ifndef REGISTER
#define REGISTER __m256
#endif



using namespace GDL;
using namespace GDL::simd;



static constexpr U32 register_size = numRegisterValues<REGISTER>;
using ValueType = decltype(GetDataType<REGISTER>());



void bm_register_sum(benchmark::State& state)
{
    REGISTER data = _mm_setzero<REGISTER>();
    for (U32 i = 0; i < register_size / 2; ++i)
    {
        SetValue(data, 2 * i, static_cast<ValueType>(i));
        SetValue(data, 2 * i + 1, -static_cast<ValueType>(i));
    }

    for (auto _ : state)
        data = RegisterSum(data);
    benchmark::DoNotOptimize(data);
}
BENCHMARK(bm_register_sum);



void bm_register_multi_sum(benchmark::State& state)
{
    std::array<REGISTER, register_size> data;
    for (UST i = 0; i < register_size; ++i)
        for (UST j = 0; j < register_size / 2; ++j)
        {
            SetValue(data[i], static_cast<U32>(2 * j), static_cast<ValueType>(1 + i + j));
            SetValue(data[i], static_cast<U32>(2 * j + 1), -static_cast<ValueType>(1 + i + j));
        }
    for (auto _ : state)
        data[0] = RegisterMultiSum(data);
    benchmark::DoNotOptimize(data);
}
BENCHMARK(bm_register_multi_sum);

BENCHMARK_MAIN();
