// Setup --------------------------------------------------------------------------------------------------------------

#ifndef OVERRIDE_SETUP

// Benchmark type
#define BENCHMARK_SINGLE
//#define BENCHMARK_SHORT
//#define BENCHMARK_N 8


// Data type
//#define DISABLE_BENCHMARK_F32
#define DISABLE_BENCHMARK_F64

// Pivoting
//#define DISABLE_BENCHMARK_NOPIVOT
//#define DISABLE_BENCHMARK_PARTIALPIVOT


// Factorization
//#define ENABLE_BENCHMARK_FACTORIZATION


// Vectorization
//#define DISABLE_BENCHMARK_SERIAL
//#define DISABLE_BENCHMARK_SIMD

#endif // OVERRIDE_SETUP



// Run benchmark ------------------------------------------------------------------------------------------------------

// REMOVE IF SIMD VERSION IS IMPLEMENTED
#define DISABLE_BENCHMARK_SIMD


#define SOLVER_NAME QR
#include "gdl/math/solver/qr.h"

#include "benchmark/math/solver/unifiedDenseSolverBenchmark.h"
