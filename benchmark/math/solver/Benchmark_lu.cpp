// Setup --------------------------------------------------------------------------------------------------------------

#ifndef OVERRIDE_SETUP

// Benchmark type
#define BENCHMARK_SINGLE
//#define BENCHMARK_SHORT
//#define BENCHMARK_N 8


// Data type
//#define DISABLE_BENCHMARK_F32
//#define DISABLE_BENCHMARK_F64

// Pivoting
//#define DISABLE_BENCHMARK_NOPIVOT
//#define DISABLE_BENCHMARK_PARTIALPIVOT


// Factorization
//#define ENABLE_BENCHMARK_FACTORIZATION


// Vectorization
//#define DISABLE_BENCHMARK_SERIAL
//#define DISABLE_BENCHMARK_SIMD

#endif // OVERRIDE_SETUP



// Includes -----------------------------------------------------------------------------------------------------------

#include "benchmark/math/solver/solverBenchmark.h"
#include "gdl/math/solver/lu.h"
#include <benchmark/benchmark.h>



// --------------------------------------------------------------------------------------------------------------------
// Serial
// --------------------------------------------------------------------------------------------------------------------

// F32 ----------------------------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_SERIAL
#ifndef DISABLE_BENCHMARK_F32
#ifndef DISABLE_BENCHMARK_NOPIVOT

BENCHMARK_SOLVER(LU, Serial_F32, NoPivot)

#endif // DISABLE_BENCHMARK_NOPIVOT



#ifndef DISABLE_BENCHMARK_PARTIALPIVOT

BENCHMARK_SOLVER(LU, Serial_F32, PartialPivot)

#endif // DISABLE_BENCHMARK_PARTIALPIVOT
#endif // DISABLE_BENCHMARK_F32



// F64 ----------------------------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_F64
#ifndef DISABLE_BENCHMARK_NOPIVOT

BENCHMARK_SOLVER(LU, Serial_F64, NoPivot)

#endif // DISABLE_BENCHMARK_NOPIVOT



#ifndef DISABLE_BENCHMARK_PARTIALPIVOT

BENCHMARK_SOLVER(LU, Serial_F64, PartialPivot)

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

BENCHMARK_SOLVER(LU, SIMD_F32, NoPivot)

#endif // DISABLE_BENCHMARK_NOPIVOT


#ifndef DISABLE_BENCHMARK_PARTIALPIVOT

BENCHMARK_SOLVER(LU, SIMD_F32, PartialPivot)

#endif // DISABLE_BENCHMARK_PARTIALPIVOT
#endif // DISABLE_BENCHMARK_F32


// F64 ----------------------------------------------------------------------------------------------------------------

#ifndef DISABLE_BENCHMARK_F64
#ifndef DISABLE_BENCHMARK_NOPIVOT

BENCHMARK_SOLVER(LU, SIMD_F64, NoPivot)

#endif // DISABLE_BENCHMARK_NOPIVOT


#ifndef DISABLE_BENCHMARK_PARTIALPIVOT

BENCHMARK_SOLVER(LU, SIMD_F64, PartialPivot)

#endif // DISABLE_BENCHMARK_PARTIALPIVOT
#endif // DISABLE_BENCHMARK_F64
#endif // DISABLE_BENCHMARK_SIMD



// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
