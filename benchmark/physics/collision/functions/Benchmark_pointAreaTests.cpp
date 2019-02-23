#include <benchmark/benchmark.h>


#include "gdl/math/sse/vec2fSSE.h"
#include "gdl/physics/collision/functions/pointAreaTests.h"

using namespace GDL;



// Fixture ----------------------------------------------------------------------------------------

class BM : public benchmark::Fixture
{
public:
    Vec2fSSE<true> a, b, c, d;

    BM()
        : a{1, 0}
        , b{0, 1}
        , c{-1, 0}
        , d{2, 2}
    {
    }
};



// Orientation 2d ---------------------------------------------------------------------------------



BENCHMARK_F(BM, Orientation2d)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(PointInsideCircle(d, a, b, c));
}



// Main -------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
