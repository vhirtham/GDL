#include <benchmark/benchmark.h>

#include "gdl/base/sse/swizzle.h"
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
    __m128 tmp0 = sse::Shuffle<0, 1, 0, 1>(a.DataSSE(), b.DataSSE());
    __m128 tmp1 = sse::Shuffle<0, 1, 0, 1>(c.DataSSE(), c.DataSSE());
    __m128 tmp2 = _mmx_sub_p(tmp0, tmp1);
    __m128 tmp3 = _mmx_mul_p(tmp2, sse::Permute<3, 2, 1, 0>(tmp2));
    return _mm_cvtss_f32(_mm_sub_ps(tmp3, sse::Permute<1, 0, 3, 1>(tmp3)));
}



template <bool _isCol>
inline F32 Orientation_Alternative2(Vec2fSSE<_isCol> a, Vec2fSSE<_isCol> b, Vec2fSSE<_isCol> c)
{
    __m128 tmp0 = _mmx_sub_p(b.DataSSE(), c.DataSSE());

    __m128 tmp1 = sse::Shuffle<1, 0, 1, 0>(tmp0, c.DataSSE());
    __m128 tmp2 =
            _mm_xor_ps(sse::Shuffle<0, 1, 0, 1>(a.DataSSE(), b.DataSSE()), _mmx_setr_p<__m128>(0.f, -0.f, 0.f, -0.f));
    return sse::DotProductF32(tmp1, tmp2);
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
