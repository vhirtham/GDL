#include <benchmark/benchmark.h>

#include "gdl/math/solver/solver3.h"

using namespace GDL;



// Fixture declaration ------------------------------------------------------------------------------------------------

class SIMD : public benchmark::Fixture
{
public:
    Mat3fSSE A;
    Vec3f b;

    SIMD()
        : A{2, 1, 3, 4, 1, 4, 8, 1, 5}
        , b{2, 1, 2}
    {
    }
};



// Cramer -------------------------------------------------------------------------------------------------------------



BENCHMARK_F(SIMD, Cramer)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Cramer(A, b));
}



// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
