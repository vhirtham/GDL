#include "gdl/math/simd/mat4fAVX.h"
#include "gdl/math/simd/mat4fSSE.h"
#include "gdl/math/simd/vec4fSSE.h"
#include "gdl/math/serial/mat4Serial.h"
#include "gdl/math/serial/vec4Serial.h"
#include <benchmark/benchmark.h>

#ifdef EIGEN3_FOUND
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#endif



using namespace GDL;

//#define DISABLE_BENCHMARK_CTORS
//#define DISABLE_BENCHMARK_COMPARISON
//#define DISABLE_BENCHMARK_ADDITION
//#define DISABLE_BENCHMARK_MULTIPLICATION
//#define DISABLE_BENCHMARK_MULTIPLICATION_WITH_VECTOR
//#define DISABLE_BENCHMARK_DETERMINANT
//#define DISABLE_BENCHMARK_TRANSPOSE



// Fixture declaration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifdef __AVX2__
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
#endif // __AVX2__



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


class Serial : public benchmark::Fixture
{
public:
    Mat4Serial<F32> A;
    Mat4Serial<F32> B;
    Vec4fSerial<true> V;

    Serial()
        : A{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
        , B{8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6}
        , V{1, 2, 3, 4}
    {
    }
};

#ifdef EIGEN3_FOUND
class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix4f A;
    Eigen::Matrix4f B;
    // Vec4fSerial<true> V;

    Eigen3()
    {
        std::array<F32, 16> valuesA = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
        std::array<F32, 16> valuesB = {{8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6}};
        for (U32 i = 0; i < 4; ++i)
            for (U32 j = 0; j < 4; ++j)
            {
                A(i, j) = valuesA[j * 4 + i];
                B(i, j) = valuesB[j * 4 + i];
            }
    }
};
#endif



// Value Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_CTORS

BENCHMARK_F(Serial, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        Mat4Serial<F32> C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}


BENCHMARK_F(SIMD, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        Mat4fSSE C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}


#ifdef __AVX2__
BENCHMARK_F(AVX, Construction)(benchmark::State& state)
{
    for (auto _ : state)
        Mat4fAVX C(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}
#endif // __AVX2__

#endif // DISABLE_BENCHMARK_CTORS



// Comparison %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_COMPARISON

BENCHMARK_F(Serial, Comparison_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == A);
}

BENCHMARK_F(SIMD, Comparison_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == A);
}


#ifdef __AVX2__
BENCHMARK_F(AVX, Comparison_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == A);
}
#endif // __AVX2__



BENCHMARK_F(Serial, Comparison_Not_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == B);
}

BENCHMARK_F(SIMD, Comparison_Not_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == B);
}

#ifdef __AVX2__
BENCHMARK_F(AVX, Comparison_Not_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == B);
}
#endif // __AVX2__

#endif // DISABLE_BENCHMARK_COMPARISON

// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_ADDITION

BENCHMARK_F(Serial, Addition_Assignment)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}

BENCHMARK_F(SIMD, Addition_Assignment)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}

#ifdef __AVX2__
BENCHMARK_F(AVX, Addition_Assignment)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}
#endif // __AVX2__



// Addition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Serial, Addition)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A + B);
}

BENCHMARK_F(SIMD, Addition)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A + B);
}

#ifdef __AVX2__
BENCHMARK_F(AVX, Addition)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A + B);
}
#endif // __AVX2__

#endif // DISABLE_BENCHMARK_ADDITION



// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_MULTIPLICATION


BENCHMARK_F(Serial, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}


BENCHMARK_F(SIMD, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}

#ifdef __AVX2__
BENCHMARK_F(AVX, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * B);
}

#endif // __AVX2__

#ifdef EIGEN3_FOUND
BENCHMARK_F(Eigen3, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize((A * B).eval());
}
#endif // EIGEN3_FOUND


#endif // DISABLE_BENCHMARK_MULTIPLICATION



// Multiplication Matrix-Vector %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_MULTIPLICATION_WITH_VECTOR

BENCHMARK_F(Serial, Multiplication_Matrix_Vector)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * V);
}


BENCHMARK_F(SIMD, Multiplication_Matrix_Vector)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * V);
}

#ifdef __AVX2__
BENCHMARK_F(AVX, Multiplication_Matrix_Vector)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A * V);
}

#endif // __AVX2__

#endif // DISABLE_BENCHMARK_MULTIPLICATION_WITH_VECTOR



// Determinant %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_DETERMINANT

BENCHMARK_F(Serial, Determinant)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(B.Det());
}


BENCHMARK_F(SIMD, Determinant)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(B.Det());
}

#ifdef __AVX2__
BENCHMARK_F(AVX, Determinant)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(B.Det());
}
#endif // __AVX2__

#ifdef EIGEN3_FOUND
BENCHMARK_F(Eigen3, Determinant)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.determinant());
}
#endif // EIGEN3_FOUND

#endif // DISABLE_BENCHMARK_DETERMINANT



// Transpose %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_TRANSPOSE

BENCHMARK_F(Serial, Transpose)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.Transpose());
}


BENCHMARK_F(SIMD, Transpose)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.Transpose());
}

#ifdef __AVX2__
BENCHMARK_F(AVX, Transpose)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.Transpose());
}
#endif // __AVX2__

#endif // DISABLE_BENCHMARK_TRANSPOSE



// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
BENCHMARK_MAIN();
