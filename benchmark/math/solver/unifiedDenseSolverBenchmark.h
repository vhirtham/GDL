#pragma once



#ifndef SOLVER_NAME
static_assert(false, "Add #define SOLVER_NAME <Name of the solver> to the benchmark");
#endif // SOLVER_NAME



#include "benchmark/math/solver/solverBenchmark.h"



// --------------------------------------------------------------------------------------------------------------------
// Serial
// --------------------------------------------------------------------------------------------------------------------

// F32 ----------------------------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_SERIAL
#ifndef DISABLE_BENCHMARK_F32
#ifndef DISABLE_BENCHMARK_NOPIVOT

BENCHMARK_SOLVER(SOLVER_NAME, Serial_F32, NoPivot)

#endif // DISABLE_BENCHMARK_NOPIVOT



#ifndef DISABLE_BENCHMARK_PARTIALPIVOT

BENCHMARK_SOLVER(SOLVER_NAME, Serial_F32, PartialPivot)

#endif // DISABLE_BENCHMARK_PARTIALPIVOT
#endif // DISABLE_BENCHMARK_F32



// F64 ----------------------------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_F64
#ifndef DISABLE_BENCHMARK_NOPIVOT

BENCHMARK_SOLVER(SOLVER_NAME, Serial_F64, NoPivot)

#endif // DISABLE_BENCHMARK_NOPIVOT



#ifndef DISABLE_BENCHMARK_PARTIALPIVOT

BENCHMARK_SOLVER(SOLVER_NAME, Serial_F64, PartialPivot)

#endif // DISABLE_BENCHMARK_PARTIALPIVOT
#endif // DISABLE_BENCHMARK_F64

#endif // DISABLE_BENCHMARK_SERIAL



// --------------------------------------------------------------------------------------------------------------------
// SIMD
// --------------------------------------------------------------------------------------------------------------------

// F32 ----------------------------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_SIMD
#ifndef DISABLE_BENCHMARK_F32
#ifndef DISABLE_BENCHMARK_NOPIVOT

BENCHMARK_SOLVER(SOLVER_NAME, SIMD_F32, NoPivot)

#endif // DISABLE_BENCHMARK_NOPIVOT


#ifndef DISABLE_BENCHMARK_PARTIALPIVOT

BENCHMARK_SOLVER(SOLVER_NAME, SIMD_F32, PartialPivot)

#endif // DISABLE_BENCHMARK_PARTIALPIVOT
#endif // DISABLE_BENCHMARK_F32


// F64 ----------------------------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_F64
#ifndef DISABLE_BENCHMARK_NOPIVOT

BENCHMARK_SOLVER(SOLVER_NAME, SIMD_F64, NoPivot)

#endif // DISABLE_BENCHMARK_NOPIVOT


#ifndef DISABLE_BENCHMARK_PARTIALPIVOT

BENCHMARK_SOLVER(SOLVER_NAME, SIMD_F64, PartialPivot)

#endif // DISABLE_BENCHMARK_PARTIALPIVOT
#endif // DISABLE_BENCHMARK_F64
#endif // DISABLE_BENCHMARK_SIMD



// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
