#include <benchmark/benchmark.h>

#include "gdl/math/sse/mat2fSSE.h"
#include "gdl/math/sse/vec2fSSE.h"
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



template <bool _isCol>
inline F32 Orientation_Alternative2(Vec2fSSE<_isCol> a, Vec2fSSE<_isCol> b, Vec2fSSE<_isCol> c)
{
    __m128 tmp0 = _mmx_sub_p(b.DataSSE(), c.DataSSE());

    __m128 tmp1 = _mm_shuffle_ps(tmp0, c.DataSSE(), SHUFFLE_4_MASK(1, 0, 1, 0));
    __m128 tmp2 = _mm_xor_ps(_mm_shuffle_ps(a.DataSSE(), b.DataSSE(), SHUFFLE_4_MASK(0, 1, 0, 1)),
                             _mmx_setr_p<__m128>(0.f, -0.f, 0.f, -0.f));
    return sse::DotProduct(tmp1, tmp2);
}



BENCHMARK_F(BM, Orientation2d_Alternative1)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Orientation_Alternative1<true>(a, b, c));
}


BENCHMARK_F(BM, Orientation2d_Alternative2)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Orientation_Alternative2<true>(a, b, c));
}


BENCHMARK_F(BM, Orientation2d)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Orientation<true>(a, b, c));
}



// Main -------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
