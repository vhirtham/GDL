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

template <bool _isCol>
inline F32 Orientation_Alternative1(Vec2fSSE<_isCol> a, Vec2fSSE<_isCol> b, Vec2fSSE<_isCol> c)
{
    __m128 tmp0 = _mm_shuffle_ps(a.DataSSE(), b.DataSSE(), SHUFFLE_4_MASK(0, 1, 0, 1));
    __m128 tmp1 = _mm_shuffle_ps(c.DataSSE(), c.DataSSE(), SHUFFLE_4_MASK(0, 1, 0, 1));
    __m128 tmp2 = _mmx_sub_p(tmp0, tmp1);
    __m128 tmp3 = _mmx_mul_p(tmp2, sse::Swizzle<3, 2, 1, 0>(tmp2));
    return _mm_cvtss_f32(_mm_sub_ps(tmp3, sse::Swizzle<1, 0, 3, 1>(tmp3)));
}


BENCHMARK_F(BM, Orientation2d)(benchmark::State& state)
{
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(Orientation<true>(a, b, c));
    }
}

BENCHMARK_F(BM, Orientation2d_Alternative)(benchmark::State& state)
{
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(Orientation_Alternative1<true>(a, b, c));
    }
}



// Main -------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
