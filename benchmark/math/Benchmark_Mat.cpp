#include "gdl/math/matSSE.h"
#include "gdl/math/matSingle.h"
#include <benchmark/benchmark.h>

#ifdef EIGEN3_FOUND
#include <eigen3/Eigen/Core>
#endif
using namespace GDL;

// SETUP %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

using Type = F32;
constexpr U32 N = 32;
constexpr U32 M = N - 1;

//#define DISABLE_BENCHMARK_CTORS
//#define DISABLE_BENCHMARK_SETZERO
//#define DISABLE_BENCHMARK_TRANSPOSE
//#define DISABLE_BENCHMARK_DATA
//#define DISABLE_BENCHMARK_COMPARISON
//#define DISABLE_BENCHMARK_ADDITION
//#define DISABLE_BENCHMARK_MULTIPLICATION
//#define DISABLE_BENCHMARK_EXPRESSION



// Fixture declaration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class SIMD : public benchmark::Fixture
{
public:
    MatSSE<Type, N, N> A;
    MatSSE<Type, N, N> B;
    MatSingle<Type, M, N> C;
    std::array<Type, N * N> valArray;
    std::array<Type, M * N> valArray2;

    SIMD()
    {
        for (U32 i = 0; i < N * N; ++i)
            valArray[i] = 2 * i;
        for (U32 i = 0; i < M * N; ++i)
            valArray2[i] = 2 * i;

        A = MatSSE<Type, N, N>(valArray);
        B = MatSSE<Type, N, N>(valArray);
    }
};


class Single : public benchmark::Fixture
{
public:
    MatSingle<Type, N, N> A;
    MatSingle<Type, N, N> B;
    std::array<Type, N * N> valArray;

    Single()
    {
        for (U32 i = 0; i < N * N; ++i)
            valArray[i] = 2 * i;
        A = MatSingle<Type, N, N>(valArray);
        B = MatSingle<Type, N, N>(valArray);
    }
};


#ifdef EIGEN3_FOUND
class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix<Type, N, N> A;
    Eigen::Matrix<Type, N, N> B;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
#endif



// Default Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_CTORS

BENCHMARK_F(Single, Construction_Def)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSingle<Type, N, N>());
}


BENCHMARK_F(SIMD, Construction_Def)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSSE<Type, N, N>());
}

// Value Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Construction_Val)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSingle<Type, N, N>(valArray));
}



BENCHMARK_F(SIMD, Construction_Val)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSSE<Type, N, N>(valArray));
}



BENCHMARK_F(SIMD, Construction_Val_non_full_registers)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSSE<Type, M, N>(valArray2));
}

BENCHMARK_F(Single, Construction_Val_Variadic_12x12)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSingle<Type, 12, 12>(
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.));
}

BENCHMARK_F(SIMD, Construction_Val_Variadic_12x12)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(MatSSE<Type, 12, 12>(
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.,
                1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.));
}

#endif // DISABLE_BENCHMARK_CTORS



// Set Zero %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_SETZERO

BENCHMARK_F(Single, Set_Zero)(benchmark::State& state)
{
    for (auto _ : state)
        A.SetZero();
}


BENCHMARK_F(SIMD, Set_Zero)(benchmark::State& state)
{
    for (auto _ : state)
        A.SetZero();
}

#endif // DISABLE_BENCHMARK_SETZERO



// Transpose %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_TRANSPOSE

BENCHMARK_F(Single, Assign_Transposed)(benchmark::State& state)
{
    for (auto _ : state)
        B = A.Transpose();
}


BENCHMARK_F(SIMD, Assign_Transposed)(benchmark::State& state)
{
    for (auto _ : state)
        B = A.Transpose();
}

#ifdef EIGEN3_FOUND
BENCHMARK_F(Eigen3, Assign_Transposed)(benchmark::State& state)
{
    for (auto _ : state)
        B = A.transpose();
}
#endif

#endif // DISABLE_BENCHMARK_TRANSPOSE



// Data %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_DATA
BENCHMARK_F(Single, Data)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.Data());
}


BENCHMARK_F(SIMD, Data)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.Data());
}


BENCHMARK_F(SIMD, Data_non_full_registers)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(C.Data());
}

#endif // DISABLE_BENCHMARK_DATA



// Compare Equal %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_COMPARISON

BENCHMARK_F(Single, Compare_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A == A);
}


BENCHMARK_F(SIMD, Compare_Equal)(benchmark::State& state)
{

    for (auto _ : state)
        benchmark::DoNotOptimize(A == A);
}


// Compare NOT Equal %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Compare_Not_Equal)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A != A);
}


BENCHMARK_F(SIMD, Compare_Not_Equal)(benchmark::State& state)
{

    for (auto _ : state)
        benchmark::DoNotOptimize(A != A);
}

#endif // DISABLE_BENCHMARK_COMPARISON



// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_ADDITION

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


#ifdef EIGEN3_FOUND
BENCHMARK_F(Eigen3, Addition_Assignment)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A += B);
}
#endif



// Addition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_F(Single, Addition)(benchmark::State& state)
{
    decltype(A) C;
    for (auto _ : state)
        benchmark::DoNotOptimize(C = A + B);
}


BENCHMARK_F(SIMD, Addition)(benchmark::State& state)
{
    decltype(A) C;
    for (auto _ : state)
        benchmark::DoNotOptimize(C = A + B);
}


#ifdef EIGEN3_FOUND
BENCHMARK_F(Eigen3, Addition)(benchmark::State& state)
{
    decltype(A) C;
    for (auto _ : state)
        benchmark::DoNotOptimize((C = (A + B).eval()).eval());
}
#endif

#endif // DISABLE_BENCHMARK_ADDITION



// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_MULTIPLICATION
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


#ifdef EIGEN3_FOUND
BENCHMARK_F(Eigen3, Multiplication)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize((A * B).eval());
}
#endif

#endif // DISABLE_BENCHMARK_MULTIPLICATION



// Expression %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef DISABLE_BENCHMARK_EXPRESSION
BENCHMARK_F(Single, Expression)(benchmark::State& state)
{
    for (auto _ : state)
        B = A.Transpose() * A + A * A;
}


BENCHMARK_F(SIMD, Expression)(benchmark::State& state)
{
    for (auto _ : state)
        B = A.Transpose() * A + A * A;
}


#ifdef EIGEN3_FOUND
BENCHMARK_F(Eigen3, Expression)(benchmark::State& state)
{
    for (auto _ : state)
        B = A.transpose() * A + A * A;
}
#endif

#endif // DISABLE_BENCHMARK_EXPRESSION


// Main %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BENCHMARK_MAIN();
