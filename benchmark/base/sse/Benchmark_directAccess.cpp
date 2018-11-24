#include <benchmark/benchmark.h>

#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/intrinsics.h"

#include <array>

using namespace GDL;


constexpr U32 valueIndex = 0;
using registerType = __m256;
using valueType = decltype(sse::GetDataType<registerType>());


//#define DISABLE_BENCHMARK_GETTER


// Fixture declaration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename _registerType>
class alignas(sse::alignmentBytes<registerType>) FixtureTemplate : public benchmark::Fixture
{
public:
    _registerType reg;

    FixtureTemplate()
    {
        if constexpr (std::is_same<registerType, __m128d>::value)
            reg = _mm_setr_pd(1.f, 2.f);
        if constexpr (std::is_same<registerType, __m128>::value)
            reg = _mm_setr_ps(1.f, 2.f, 3.f, 4.f);
#ifdef __AVX2__
        if constexpr (std::is_same<registerType, __m256d>::value)
            reg = _mm256_setr_pd(1., 2., 3., 4.);
        if constexpr (std::is_same<registerType, __m256>::value)
            reg = _mm256_setr_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
#endif // __AVX2__
    }
};

using FixtureBenchmark = FixtureTemplate<registerType>;


// Getter Implementations %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_GETTER


template <U32 _index>
valueType GetterDirectAccessOperator(registerType reg)
{
    return reg[_index];
}

template <U32 _index>
valueType GetterUnion(registerType reg)
{
    union Data {
        registerType sse;
        valueType array[sse::numRegisterValues<registerType>];
    };

    Data data{reg};
    return data.array[_index];
}


template <U32 _index>
valueType GetterStore(registerType reg)
{
    alignas(sse::alignmentBytes<registerType>) valueType array[sse::numRegisterValues<registerType>];
    _mmx_store_p(array, reg);
    return array[_index];
}


template <U32 _index>
valueType GetterFirstElementShuffle(__m128 reg)
{

    if constexpr (_index == 0)
        return _mm_cvtss_f32(reg);
    else
        return _mm_cvtss_f32(_mm_shuffle_ps(reg, reg, _MM_SHUFFLE(_index, _index, _index, _index)));
}
template <U32 _index>
valueType GetterFirstElementShuffle(__m128d reg)
{
    if constexpr (_index == 0)
        return _mm_cvtsd_f64(reg);
    else
        return _mm_cvtsd_f64(_mm_unpackhi_pd(reg, reg));
}

#ifdef __AVX2__

template <U32 _index>
valueType GetterFirstElementShuffle(__m256d reg)
{
    if constexpr (_index < 2)
        return GetterFirstElementShuffle<_index>(_mm256_extractf128_pd(reg, 0));
    else
        return GetterFirstElementShuffle<_index - 2>(_mm256_extractf128_pd(reg, 1));
}

template <U32 _index>
valueType GetterFirstElementShuffle(__m256 reg)
{
    if constexpr (_index < 4)
        return GetterFirstElementShuffle<_index>(_mm256_extractf128_ps(reg, 0));
    else
        return GetterFirstElementShuffle<_index - 4>(_mm256_extractf128_ps(reg, 1));
}

#endif // __AVX2__



// Getter Benchmarks %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(FixtureBenchmark, GetterDirectAccessOperator)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterDirectAccessOperator<valueIndex>(reg));
}

BENCHMARK_F(FixtureBenchmark, GetterStore)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterStore<valueIndex>(reg));
}

BENCHMARK_F(FixtureBenchmark, GetterUnion)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterUnion<valueIndex>(reg));
}

BENCHMARK_F(FixtureBenchmark, GetterFirstElementShuffle)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterFirstElementShuffle<valueIndex>(reg));
}

BENCHMARK_F(FixtureBenchmark, Current_Implementation)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(sse::GetValue<valueIndex>(reg));
}
#endif // DISABLE_BENCHMARK_GETTER



// Setter Implementations %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <U32 _index>
void SetterDirectAccessOperator(registerType& reg, valueType value)
{
    reg[_index] = value;
}



template <U32 _index>
void SetterStoreModifyLoad(registerType& reg, valueType value)
{
    alignas(sse::alignmentBytes<registerType>) valueType array[sse::numRegisterValues<registerType>];
    _mmx_store_p(array, reg);
    array[_index] = value;
    reg = _mmx_load_p<registerType>(array);
}



template <U32 _index>
void SetterUnion(registerType& reg, valueType value)
{
    union Data {
        registerType sse;
        valueType array[sse::numRegisterValues<registerType>];
    };

    Data data{reg};
    data.array[_index] = value;
    reg = data.sse;
}


// Setter Benchmarks %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


BENCHMARK_F(FixtureBenchmark, SetterDirectAccessOperator)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            SetterDirectAccessOperator<valueIndex>(reg, static_cast<valueType>(i));
}



BENCHMARK_F(FixtureBenchmark, SetterStoreModifyLoad)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            SetterStoreModifyLoad<valueIndex>(reg, static_cast<valueType>(i));
}



BENCHMARK_F(FixtureBenchmark, SetterUnion)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            SetterUnion<valueIndex>(reg, static_cast<valueType>(i));
}


// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_MAIN();
