#include <benchmark/benchmark.h>

#include "gdl/math/vec2.h"
#include "gdl/physics/collision/functions/orientation.h"

using namespace GDL;



// Fixture ----------------------------------------------------------------------------------------

class BM : public benchmark::Fixture
{
public:
    Vec2fSSE<true> a, b, c;

    BM()
        : a{1, 2}
        , b{4, 5}
        , c{11, 3}
    {
    }
};



// Orientation 2d ---------------------------------------------------------------------------------

BENCHMARK_F(BM, Orientation2d)(benchmark::State& state)
{
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(Orientation<true>(a, b, c));
    }
}



// Main -------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
