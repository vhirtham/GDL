#include "gdl/math/sse/mat4fAVX.h"
#include "gdl/math/sse/mat4fSSE.h"
#include "gdl/math/sse/vec4fSSE.h"
#include "gdl/math/single/mat4Single.h"
#include "gdl/math/single/vec4Single.h"
#include <benchmark/benchmark.h>



using namespace GDL;

// Fixture declaration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifdef __AVX__
class AVX : public benchmark::Fixture
{
public:
    Mat4fAVX A;
    Mat4fAVX B;
    Vec4fSSE<true> V;

    AVX()
        : A{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
        , B{8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6}
        , V{1, 2, 3, 4}
    {
    }
};
#endif // __AVX__



class SIMD : public benchmark::Fixture
{
public:
    Mat4fSSE A;
    Mat4fSSE B;
    Vec4fSSE<true> V;

    SIMD()
        : A{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
        , B{8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6}
        , V{1, 2, 3, 4}
    {
    }
};


class Single : public benchmark::Fixture
{
public:
    Mat4Single<F32> A;
    Mat4Single<F32> B;
    Vec4fSingle<true> V;

    Single()
        : A{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
        , B{8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6}
        , V{1, 2, 3, 4}
    {
    }
};



// Value Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        Mat4Single<F32> C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}


BENCHMARK_F(SIMD, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        Mat4fSSE C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}


#ifdef __AVX__
BENCHMARK_F(AVX, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        Mat4fAVX C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}
#endif // __AVX__



// Comparison %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Comparison_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == A);
}

BENCHMARK_F(SIMD, Comparison_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == A);
}


#ifdef __AVX__
BENCHMARK_F(AVX, Comparison_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == A);
}
#endif // __AVX__



BENCHMARK_F(Single, Comparison_Not_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == B);
}

BENCHMARK_F(SIMD, Comparison_Not_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == B);
}

#ifdef __AVX__
BENCHMARK_F(AVX, Comparison_Not_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == B);
}
#endif // __AVX__



// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Addition_Assignment)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}

BENCHMARK_F(SIMD, Addition_Assignment)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}

#ifdef __AVX__
BENCHMARK_F(AVX, Addition_Assignment)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}
#endif // __AVX__



// Addition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Addition)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A + B);
}

BENCHMARK_F(SIMD, Addition)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A + B);
}

#ifdef __AVX__
BENCHMARK_F(AVX, Addition)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A + B);
}
#endif // __AVX__



// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}


BENCHMARK_F(SIMD, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}

#ifdef __AVX__
BENCHMARK_F(AVX, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}

#endif // __AVX__



// Multiplication Matrix-Vector %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Multiplication_Matrix_Vector)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * V);
}


BENCHMARK_F(SIMD, Multiplication_Matrix_Vector)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * V);
}

#ifdef __AVX__
BENCHMARK_F(AVX, Multiplication_Matrix_Vector)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * V);
}

#endif // __AVX__



// Determinant %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Determinant)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(B.Det());
}


BENCHMARK_F(SIMD, Determinant)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(B.Det());
}

#ifdef __AVX__
#endif // __AVX__

// Transpose %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Transpose)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.Transpose());
}


BENCHMARK_F(SIMD, Transpose)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.Transpose());
}

#ifdef __AVX__
#endif // __AVX__

// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
BENCHMARK_MAIN();
