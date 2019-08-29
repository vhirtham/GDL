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

#ifdef __AVX2__
#define SIMD_F32 AVX_F32
#define SIMD_F64 AVX_F64
#else
#define SIMD_F32 SSE_F32
#define SIMD_F64 SSE_F64
#endif

// Fixture declaration ------------------------------------------------------------------------------------------------

constexpr U32 N = BENCHMARK_N;

template <typename _type, template <typename, U32, U32> class _matrix,
          template <typename, U32, bool = true> class _vector>
class FixtureTemplate : public benchmark::Fixture
{
public:
    _matrix<_type, 8, 8> A8;
    _vector<_type, 8> b8;
    _matrix<_type, 16, 16> A16;
    _vector<_type, 16> b16;
    _matrix<_type, 24, 24> A24;
    _vector<_type, 24> b24;
    _matrix<_type, 32, 32> A32;
    _vector<_type, 32> b32;
    _matrix<_type, 48, 48> A48;
    _vector<_type, 48> b48;
    _matrix<_type, 56, 56> A56;
    _vector<_type, 56> b56;
    _matrix<_type, 64, 64> A64;
    _vector<_type, 64> b64;
    _matrix<_type, 72, 72> A72;
    _vector<_type, 72> b72;
    _matrix<_type, 80, 80> A80;
    _vector<_type, 80> b80;
    _matrix<_type, 88, 88> A88;
    _vector<_type, 88> b88;
    _matrix<_type, 96, 96> A96;
    _vector<_type, 96> b96;
    _matrix<_type, 104, 104> A104;
    _vector<_type, 104> b104;
    _matrix<_type, 112, 112> A112;
    _vector<_type, 112> b112;
    _matrix<_type, 120, 120> A120;
    _vector<_type, 120> b120;
    _matrix<_type, 128, 128> A128;
    _vector<_type, 128> b128;
    _matrix<_type, N, N> AN;
    _vector<_type, N> bN;


    FixtureTemplate()
        : A8{GetMatrixData8<_type>()}
        , b8{GetVectorData8<_type>()}
        , A16{GetMatrixData16<_type>()}
        , b16{GetVectorData16<_type>()}
        , A24{GetMatrixDataRandom<_type, 24>()}
        , b24{GetVectorDataRandom<_type, 24>()}
        , A32{GetMatrixData32<_type>()}
        , b32{GetVectorData32<_type>()}
        , A48{GetMatrixDataRandom<_type, 48>()}
        , b48{GetVectorDataRandom<_type, 48>()}
        , A56{GetMatrixDataRandom<_type, 56>()}
        , b56{GetVectorDataRandom<_type, 56>()}
        , A64{GetMatrixDataRandom<_type, 64>()}
        , b64{GetVectorDataRandom<_type, 64>()}
        , A72{GetMatrixDataRandom<_type, 72>()}
        , b72{GetVectorDataRandom<_type, 72>()}
        , A80{GetMatrixDataRandom<_type, 80>()}
        , b80{GetVectorDataRandom<_type, 80>()}
        , A88{GetMatrixDataRandom<_type, 88>()}
        , b88{GetVectorDataRandom<_type, 88>()}
        , A96{GetMatrixDataRandom<_type, 96>()}
        , b96{GetVectorDataRandom<_type, 96>()}
        , A104{GetMatrixDataRandom<_type, 104>()}
        , b104{GetVectorDataRandom<_type, 104>()}
        , A112{GetMatrixDataRandom<_type, 112>()}
        , b112{GetVectorDataRandom<_type, 112>()}
        , A120{GetMatrixDataRandom<_type, 120>()}
        , b120{GetVectorDataRandom<_type, 120>()}
        , A128{GetMatrixDataRandom<_type, 128>()}
        , b128{GetVectorDataRandom<_type, 128>()}
        , AN{GetMatrixDataRandom<_type, N>()}
        , bN{GetVectorDataRandom<_type, N>()}
    {
    }
};

using Serial_F32 = FixtureTemplate<F32, MatSerial, VecSerial>;
using Serial_F64 = FixtureTemplate<F64, MatSerial, VecSerial>;
using SIMD_F32 = FixtureTemplate<F32, MatSIMD, VecSIMD>;
using SIMD_F64 = FixtureTemplate<F64, MatSIMD, VecSIMD>;


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
    SOLVER_BENCHMARK(fixture, name, solver, 56)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 64)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 72)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 80)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 88)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 96)                                                                        \
    SOLVER_BENCHMARK(fixture, name, solver, 104)                                                                       \
    SOLVER_BENCHMARK(fixture, name, solver, 112)                                                                       \
    SOLVER_BENCHMARK(fixture, name, solver, 120)                                                                       \
    SOLVER_BENCHMARK(fixture, name, solver, 128)
#endif
#endif
