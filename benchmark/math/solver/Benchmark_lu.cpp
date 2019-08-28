// Setup --------------------------------------------------------------------------------------------------------------

#ifndef OVERRIDE_SETUP

// Benchmark type
#define BENCHMARK_SINGLE
//#define BENCHMARK_SHORT
//#define BENCHMARK_N 8

// pivoting
//#define DISABLE_BENCHMARK_NOPIVOT
//#define DISABLE_BENCHMARK_PARTIALPIVOT

// factorization
//#define ENABLE_BENCHMARK_FACTORIZATION

// vectorization
//#define DISABLE_BENCHMARK_SERIAL
//#define DISABLE_BENCHMARK_SIMD

#endif // OVERRIDE_SETUP



// Includes -----------------------------------------------------------------------------------------------------------

#include "benchmark/math/solver/solverBenchmark.h"
#include "gdl/math/solver/lu.h"
#include <benchmark/benchmark.h>



// Serial - No pivot --------------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_NOPIVOT
#ifndef DISABLE_BENCHMARK_SERIAL

BENCHMARK_SOLVER(LU, Serial, NoPivot)

#endif // DISABLE_BENCHMARK_SERIAL



// Serial - partial pivot ---------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_PARTIALPIVOT
#ifndef DISABLE_BENCHMARK_SERIAL

BENCHMARK_SOLVER(LU, Serial, PartialPivot)

#endif // DISABLE_BENCHMARK_SERIAL



// SIMD - No pivot ----------------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_SIMD

BENCHMARK_SOLVER(LU, SIMD, NoPivot)

#endif // DISABLE_BENCHMARK_SIMD
#endif // DISABLE_BENCHMARK_NOPIVOT



// SIMD - partial pivot -----------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_SIMD

BENCHMARK_SOLVER(LU, SIMD, PartialPivot)

#endif // DISABLE_BENCHMARK_SIMD
#endif // DISABLE_BENCHMARK_PARTIALPIVOT



// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
