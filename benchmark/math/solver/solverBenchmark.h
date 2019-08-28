#pragma once

#include <benchmark/benchmark.h>
#include "gdl/math/serial/matSerial.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/solver/pivotEnum.h"
#include "benchmark/math/solver/solverBenchmarkData.h"

using namespace GDL;
using namespace GDL::Solver;

#ifndef BENCHMARK_N
#define BENCHMARK_N 64
#endif // BENCHMARK_N



// Fixture declaration ------------------------------------------------------------------------------------------------

using Type = F32;
constexpr U32 N = BENCHMARK_N;

template <template <typename, U32, U32> class _matrix, template <typename, U32, bool = true> class _vector>
class FixtureTemplate : public benchmark::Fixture
{
public:
    _matrix<Type, 8, 8> A8;
    _vector<Type, 8> b8;
    _matrix<Type, 16, 16> A16;
    _vector<Type, 16> b16;
    _matrix<Type, 24, 24> A24;
    _vector<Type, 24> b24;
    _matrix<Type, 32, 32> A32;
    _vector<Type, 32> b32;
    _matrix<Type, 48, 48> A48;
    _vector<Type, 48> b48;
    _matrix<Type, 64, 64> A64;
    _vector<Type, 64> b64;
    _matrix<Type, 72, 72> A72;
    _vector<Type, 72> b72;
    _matrix<Type, N, N> AN;
    _vector<Type, N> bN;


    FixtureTemplate()
        : A8{GetMatrixData8<Type>()}
        , b8{GetVectorData8<Type>()}
        , A16{GetMatrixData16<Type>()}
        , b16{GetVectorData16<Type>()}
        , A24{GetMatrixDataRandom<Type, 24>()}
        , b24{GetVectorDataRandom<Type, 24>()}
        , A32{GetMatrixData32<Type>()}
        , b32{GetVectorData32<Type>()}
        , A48{GetMatrixDataRandom<Type, 48>()}
        , b48{GetVectorDataRandom<Type, 48>()}
        , A64{GetMatrixDataRandom<Type, 64>()}
        , b64{GetVectorDataRandom<Type, 64>()}
        , A72{GetMatrixDataRandom<Type, 72>()}
        , b72{GetVectorDataRandom<Type, 72>()}
        , AN{GetMatrixDataRandom<Type, N>()}
        , bN{GetVectorDataRandom<Type, N>()}
    {
    }
};

using Serial = FixtureTemplate<MatSerial, VecSerial>;
using SIMD = FixtureTemplate<MatSIMD, VecSIMD>;



// Helpers ------------------------------------------------------------------------------------------------------------

constexpr Pivot PivotEnumNoPivot = Solver::Pivot::NONE;
constexpr Pivot PivotEnumPartialPivot = Solver::Pivot::PARTIAL;


// Benchmark Macro ----------------------------------------------------------------------------------------------------

#ifdef ENABLE_BENCHMARK_FACTORIZATION

#define SOLVER_BENCHMARK(fixture, name, solver, size)                                                                  \
    BENCHMARK_F(fixture, name##_##size##x##size)(benchmark::State & state)                                             \
    {                                                                                                                  \
        for (auto _ : state)                                                                                           \
            benchmark::DoNotOptimize(solver<PivotEnum##name>(A##size, b##size));                                       \
    }                                                                                                                  \
                                                                                                                       \
                                                                                                                       \
                                                                                                                       \
    BENCHMARK_F(fixture, name##_##size##x##size##_Factorize)(benchmark::State & state)                                 \
    {                                                                                                                  \
        for (auto _ : state)                                                                                           \
            benchmark::DoNotOptimize(solver##Factorization<PivotEnum##name>(A##size));                                 \
    }                                                                                                                  \
                                                                                                                       \
                                                                                                                       \
                                                                                                                       \
    BENCHMARK_F(fixture, name##_##size##x##size##_Solve)(benchmark::State & state)                                     \
    {                                                                                                                  \
        auto factorization = solver##Factorization<PivotEnum##name>(A##size);                                          \
        for (auto _ : state)                                                                                           \
            benchmark::DoNotOptimize(solver<PivotEnum##name>(factorization, b##size));                                 \
    }

#else // ENABLE_BENCHMARK_FACTORIZATION

#define SOLVER_BENCHMARK(fixture, name, solver, size)                                                                  \
    BENCHMARK_F(fixture, name##_##size##x##size)(benchmark::State & state)                                             \
    {                                                                                                                  \
        for (auto _ : state)                                                                                           \
            benchmark::DoNotOptimize(solver<PivotEnum##name>(A##size, b##size));                                       \
    }

#endif // ENABLE_BENCHMARK_FACTORIZATION


#ifdef BENCHMARK_SINGLE
#define BENCHMARK_SOLVER(solver, fixture, name) SOLVER_BENCHMARK(fixture, name, solver, N)
#else
#ifdef BENCHMARK_SHORT
#define BENCHMARK_SOLVER(solver, fixture, name)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 8)                                                                         \
    SOLVER_BENCHMARK(fixture, name, solver, 16)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 32)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 64)
#else
#define BENCHMARK_SOLVER(solver, fixture, name)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 8)                                                                         \
    SOLVER_BENCHMARK(fixture, name, solver, 16)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 24)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 32)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 48)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 64)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 72)
#endif
#endif
