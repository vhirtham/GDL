#include <benchmark/benchmark.h>


#include "gdl/math/sse/vec3fSSE.h"
#include "gdl/physics/collision/functions/pointVolumeTests.h"

using namespace GDL;



// Fixture ----------------------------------------------------------------------------------------

class BM : public benchmark::Fixture
{
public:
    Vec3fSSE<true> a, b, c, d, e;

    BM()
        : a{-1, 0, 0}
        , b{0, 1, 0}
        , c{1, 0, 0}
        , d{0, 0, 1}
        , e{0, 0, 0}
    {
    }
};



// Orientation 2d ---------------------------------------------------------------------------------



BENCHMARK_F(BM, PointInsideSphere)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(PointInsideSphere(e, a, b, c, d));
}



// Main -------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
