#include <benchmark/benchmark.h>

#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/intrinsics.h"
#include "gdl/base/sse/swizzle.h"

#include <array>

using namespace GDL;


constexpr U32 valueIndex = 3;
using RegisterType = __m128;
using ValueType = decltype(sse::GetDataType<RegisterType>());


//#define DISABLE_BENCHMARK_STATIC_GETTER
//#define DISABLE_BENCHMARK_DYNAMIC_GETTER
//#define DISABLE_BENCHMARK_STATIC_SETTER
//#define DISABLE_BENCHMARK_DYNAMIC_SETTER

// Fixture declaration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename _registerType>
class alignas(sse::alignmentBytes<RegisterType>) FixtureTemplate : public benchmark::Fixture
{
public:
    _registerType reg;

    FixtureTemplate()
    {
        if constexpr (std::is_same<RegisterType, __m128d>::value)
            reg = _mm_setr_pd(1.f, 2.f);
        if constexpr (std::is_same<RegisterType, __m128>::value)
            reg = _mm_setr_ps(1.f, 2.f, 3.f, 4.f);
#ifdef __AVX2__
        if constexpr (std::is_same<RegisterType, __m256d>::value)
            reg = _mm256_setr_pd(1., 2., 3., 4.);
        if constexpr (std::is_same<RegisterType, __m256>::value)
            reg = _mm256_setr_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
#endif // __AVX2__
    }
};

using FixtureBenchmark = FixtureTemplate<RegisterType>;



// Static Getter Implementations %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



template <U32 _index>
ValueType GetterStaticDirectAccessOperator(RegisterType reg)
{
    return reg[_index];
}



template <U32 _index>
ValueType GetterStaticUnion(RegisterType reg)
{
    union Data {
        RegisterType sse;
        ValueType array[sse::numRegisterValues<RegisterType>];
    };

    Data data{reg};
    return data.array[_index];
}



template <U32 _index>
ValueType GetterStaticStore(RegisterType reg)
{
    alignas(sse::alignmentBytes<RegisterType>) ValueType array[sse::numRegisterValues<RegisterType>];
    _mmx_store_p(array, reg);
    return array[_index];
}



template <U32 _index>
ValueType GetterStaticFirstElementShuffle(__m128 reg)
{

    if constexpr (_index == 0)
        return _mm_cvtss_f32(reg);
    else
        return _mm_cvtss_f32(sse::Broadcast<_index>(reg));
}



template <U32 _index>
ValueType GetterStaticFirstElementShuffle(__m128d reg)
{
    if constexpr (_index == 0)
        return _mm_cvtsd_f64(reg);
    else
        return _mm_cvtsd_f64(_mm_unpackhi_pd(reg, reg));
}


#ifdef __AVX2__

template <U32 _index>
ValueType GetterStaticFirstElementShuffle(__m256d reg)
{
    if constexpr (_index < 2)
        return GetterStaticFirstElementShuffle<_index>(_mm256_extractf128_pd(reg, 0));
    else
        return GetterStaticFirstElementShuffle<_index - 2>(_mm256_extractf128_pd(reg, 1));
}



template <U32 _index>
ValueType GetterStaticFirstElementShuffle(__m256 reg)
{
    if constexpr (_index < 4)
        return GetterStaticFirstElementShuffle<_index>(_mm256_extractf128_ps(reg, 0));
    else
        return GetterStaticFirstElementShuffle<_index - 4>(_mm256_extractf128_ps(reg, 1));
}

#endif // __AVX2__



// Static Getter Benchmarks %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef DISABLE_BENCHMARK_STATIC_GETTER
BENCHMARK_F(FixtureBenchmark, GetterStaticDirectAccessOperator)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterStaticDirectAccessOperator<valueIndex>(reg));
}



BENCHMARK_F(FixtureBenchmark, GetterStaticStore)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterStaticStore<valueIndex>(reg));
}



BENCHMARK_F(FixtureBenchmark, GetterStaticUnion)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterStaticUnion<valueIndex>(reg));
}



BENCHMARK_F(FixtureBenchmark, GetterStaticFirstElementShuffle)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterStaticFirstElementShuffle<valueIndex>(reg));
}



BENCHMARK_F(FixtureBenchmark, GetterStaticCurrentImplementation)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(sse::GetValue<valueIndex>(reg));
}
#endif // DISABLE_BENCHMARK_STATIC_GETTER



// Dynamic Getter Implementations %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ValueType GetterDynamicDirectAccessOperator(RegisterType reg, U32 index)
{
    return reg[index];
}



ValueType GetterDynamicUnion(RegisterType reg, U32 index)
{
    union Data {
        RegisterType sse;
        ValueType array[sse::numRegisterValues<RegisterType>];
    };

    Data data{reg};
    return data.array[index];
}



ValueType GetterDynamicStore(RegisterType reg, U32 index)
{
    alignas(sse::alignmentBytes<RegisterType>) ValueType array[sse::numRegisterValues<RegisterType>];
    _mmx_store_p(array, reg);
    return array[index];
}



ValueType GetterDynamicFirstElementShuffle(__m128 reg, U32 index)
{
    switch (index)
    {
    case 0:
        return GetterStaticFirstElementShuffle<0>(reg);
    case 1:
        return GetterStaticFirstElementShuffle<1>(reg);
    case 2:
        return GetterStaticFirstElementShuffle<2>(reg);
    case 3:
        return GetterStaticFirstElementShuffle<3>(reg);
    default:
        throw(int{0});
    }
}


ValueType GetterDynamicFirstElementShuffle(__m256 reg, U32 index)
{
    switch (index)
    {
    case 0:
        return GetterStaticFirstElementShuffle<0>(reg);
    case 1:
        return GetterStaticFirstElementShuffle<1>(reg);
    case 2:
        return GetterStaticFirstElementShuffle<2>(reg);
    case 3:
        return GetterStaticFirstElementShuffle<3>(reg);
    case 4:
        return GetterStaticFirstElementShuffle<4>(reg);
    case 5:
        return GetterStaticFirstElementShuffle<5>(reg);
    case 6:
        return GetterStaticFirstElementShuffle<6>(reg);
    case 7:
        return GetterStaticFirstElementShuffle<7>(reg);
    default:
        throw(int{0});
    }
}


// Dynamic Getter Benchmarks %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef DISABLE_BENCHMARK_DYNAMIC_GETTER
BENCHMARK_F(FixtureBenchmark, GetterDynamicDirectAccessOperator)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterDynamicDirectAccessOperator(reg, valueIndex));
}



BENCHMARK_F(FixtureBenchmark, GetterDynamicStore)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterDynamicStore(reg, valueIndex));
}



BENCHMARK_F(FixtureBenchmark, GetterDynamicUnion)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterDynamicUnion(reg, valueIndex));
}



BENCHMARK_F(FixtureBenchmark, GetterDynamicFirstElementShuffle)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(GetterDynamicFirstElementShuffle(reg, valueIndex));
}



BENCHMARK_F(FixtureBenchmark, GetterDynamicCurrentImplementation)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            benchmark::DoNotOptimize(sse::GetValue(reg, valueIndex));
}
#endif // DISABLE_BENCHMARK_DYNAMIC_GETTER


// Setter Static Implementations %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <U32 _index>
void SetterStaticDirectAccessOperator(RegisterType& reg, ValueType value)
{
    reg[_index] = value;
}



template <U32 _index>
void SetterStaticStoreModifyLoad(RegisterType& reg, ValueType value)
{
    alignas(sse::alignmentBytes<RegisterType>) ValueType array[sse::numRegisterValues<RegisterType>];
    _mmx_store_p(array, reg);
    array[_index] = value;
    reg = _mmx_load_p<RegisterType>(array);
}



template <U32 _index>
void SetterStaticUnion(RegisterType& reg, ValueType value)
{
    union Data {
        RegisterType sse;
        ValueType array[sse::numRegisterValues<RegisterType>];
    };

    Data data{reg};
    data.array[_index] = value;
    reg = data.sse;
}


// Setter Static Benchmarks %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef DISABLE_BENCHMARK_STATIC_SETTER
BENCHMARK_F(FixtureBenchmark, SetterStaticDirectAccessOperator)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            SetterStaticDirectAccessOperator<valueIndex>(reg, static_cast<ValueType>(i));
}



BENCHMARK_F(FixtureBenchmark, SetterStaticStoreModifyLoad)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            SetterStaticStoreModifyLoad<valueIndex>(reg, static_cast<ValueType>(i));
}



BENCHMARK_F(FixtureBenchmark, SetterStaticUnion)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            SetterStaticUnion<valueIndex>(reg, static_cast<ValueType>(i));
}



BENCHMARK_F(FixtureBenchmark, SetterStaticCurrentImplementation)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            sse::SetValue<valueIndex>(reg, static_cast<ValueType>(i));
}

#endif // DISABLE_BENCHMARK_STATIC_GETTER


// Setter Dynamic Implementations %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


void SetterDynamicDirectAccessOperator(RegisterType& reg, ValueType value, U32 index)
{
    reg[index] = value;
}



void SetterDynamicStoreModifyLoad(RegisterType& reg, ValueType value, U32 index)
{
    alignas(sse::alignmentBytes<RegisterType>) ValueType array[sse::numRegisterValues<RegisterType>];
    _mmx_store_p(array, reg);
    array[index] = value;
    reg = _mmx_load_p<RegisterType>(array);
}



void SetterDynamicUnion(RegisterType& reg, ValueType value, U32 index)
{
    union Data {
        RegisterType sse;
        ValueType array[sse::numRegisterValues<RegisterType>];
    };

    Data data{reg};
    data.array[index] = value;
    reg = data.sse;
}



// Setter Dynamic Benchmarks %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef DISABLE_BENCHMARK_DYNAMIC_SETTER
BENCHMARK_F(FixtureBenchmark, SetterDynamicDirectAccessOperator)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            SetterDynamicDirectAccessOperator(reg, static_cast<ValueType>(i), valueIndex);
}



BENCHMARK_F(FixtureBenchmark, SetterDynamicStoreModifyLoad)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            SetterDynamicStoreModifyLoad(reg, static_cast<ValueType>(i), valueIndex);
}



BENCHMARK_F(FixtureBenchmark, SetterDynamicUnion)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            SetterDynamicUnion(reg, static_cast<ValueType>(i), valueIndex);
}



BENCHMARK_F(FixtureBenchmark, SetterDynamicCurrentImplementation)(benchmark::State& state)
{
    for (auto _ : state)
        for (U32 i = 0; i < 100; ++i)
            sse::SetValue(reg, static_cast<ValueType>(i), valueIndex);
}

#endif // DISABLE_BENCHMARK_DYNAMIC_GETTER



// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_MAIN();
