#include <benchmark/benchmark.h>

#include "gdl/math/solver/solver4.h"

#ifdef EIGEN3_FOUND
#include "eigen3/Eigen/Dense"
#endif

using namespace GDL;


// OPTIONS ------------------------------------------------------------------------------------------------------------

// Symmetric matrix
#define BENCHMARK_SYMMETRIC

// solver types
//#define BENCHMARK_CRAMER
//#define BENCHMARK_GAUSS
#define BENCHMARK_LU
//#define BENCHMARK_LLT
//#define BENCHMARK_LDLT


// pivoting
#define BENCHMARK_NOPIVOT
#define BENCHMARK_PARTIALPIVOT

// factorization
#define BENCHMARK_FACTORIZATION

// vectorization
//#define BENCHMARK_SERIAL
#define BENCHMARK_SSE
//#define BENCHMARK_AVX

// Eigen
//#define BENCHMARK_EIGEN



// Fixture declaration ------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_SERIAL

class Serial : public benchmark::Fixture
{
public:
    Mat4Serial<F32> A;
    Vec4Serial<F32, true> b;

#ifdef BENCHMARK_SYMMETRIC
    Serial()
        : A{4, 2, 4, 4, 2, 10, 5, 2, 4, 5, 9, 6, 4, 2, 6, 9}
        , b{36, 45, 65, 62}
    {
    }
#else
    Serial()
        : A{2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5}
        , b{-6, 0, -21, 18}
    {
    }
#endif
};

#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

class SSE : public benchmark::Fixture
{
public:
    Mat4fSSE A;
    Vec4f b;

#ifdef BENCHMARK_SYMMETRIC
    SSE()
        : A{4, 2, 4, 4, 2, 10, 5, 2, 4, 5, 9, 6, 4, 2, 6, 9}
        , b{36, 45, 65, 62}
    {
    }
#else
    SSE()
        : A{2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5}
        , b{-6, 0, -21, 18}
    {
    }
#endif
};

#endif // BENCHMARK_SSE



#ifdef __AVX2__
#ifdef BENCHMARK_AVX

class AVX : public benchmark::Fixture
{
public:
    Mat4fAVX A;
    Vec4f b;

#ifdef BENCHMARK_SYMMETRIC
    AVX()
        : A{4, 2, 4, 4, 2, 10, 5, 2, 4, 5, 9, 6, 4, 2, 6, 9}
        , b{36, 45, 65, 62}
    {
    }
#else
    AVX()
        : A{2, 0, 4, 6, 2, 2, -3, 1, 3, 0, 0, -6, 2, 1, 1, -5}
        , b{-6, 0, -21, 18}
    {
    }
#endif
};

#endif // BENCHMARK_AVX
#endif // __AVX2__



#ifdef EIGEN3_FOUND
#ifdef BENCHMARK_EIGEN

class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix4f A;
    Eigen::Vector4f b;

#ifdef BENCHMARK_SYMMETRIC
    Eigen3()
    {
        A << 4, 2, 4, 4, 2, 10, 5, 2, 4, 5, 9, 6, 4, 2, 6, 9;
        b << 36, 45, 65, 62;
    }
#else
    Eigen3()
    {
        A << 2, 2, 3, 2, 0, 2, 0, 1, 4, -3, 0, 1, 6, 1, -6, -5;
        b << -6, 0, -21, 18;
    }
#endif
};

#endif // BENCHMARK_EIGEN
#endif // EIGEN3_FOUND


// Cramer -------------------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_CRAMER
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, Cramer)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Cramer(A, b));
}

#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, Cramer)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Cramer(A, b));
}

#endif // BENCHMARK_SSE



#ifdef __AVX2__
#ifdef BENCHMARK_AVX

BENCHMARK_F(AVX, Cramer)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Cramer(A, b));
}

#endif // BENCHMARK_AVX
#endif // __AVX2__
#endif // BENCHMARK_CRAMER



// Gauss - no pivot ---------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_GAUSS
#ifdef BENCHMARK_NOPIVOT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, GaussNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Gauss<Solver::Pivot::NONE>(A, b));
}

#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, GaussNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Gauss<Solver::Pivot::NONE>(A, b));
}

#endif // BENCHMARK_SSE



#ifdef __AVX2__
#ifdef BENCHMARK_AVX
BENCHMARK_F(AVX, GaussNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Gauss<Solver::Pivot::NONE>(A, b));
}

#endif // BENCHMARK_AVX
#endif // __AVX2__
#endif // BENCHMARK_NOPIVOT



// Gauss - partial pivot ----------------------------------------------------------------------------------------------

#ifdef BENCHMARK_PARTIALPIVOT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, GaussPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Gauss<Solver::Pivot::PARTIAL>(A, b));
}

#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, GaussPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Gauss<Solver::Pivot::PARTIAL>(A, b));
}

#endif // BENCHMARK_SSE



#ifdef __AVX2__
#ifdef BENCHMARK_AVX

BENCHMARK_F(AVX, GaussPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::Gauss<Solver::Pivot::PARTIAL>(A, b));
}

#endif // BENCHMARK_AVX
#endif // __AVX2__
#endif // BENCHMARK_PARTIALPIVOT
#endif // BENCHMARK_GAUSS



// LU - no pivot ------------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_LU
#ifdef BENCHMARK_NOPIVOT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, LUNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A, b));
}


#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, LUNoPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A));
}



BENCHMARK_F(Serial, LUNoPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, LUNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A, b));
}


#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SSE, LUNoPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A));
}



BENCHMARK_F(SSE, LUNoPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SSE



#ifdef __AVX2__
#ifdef BENCHMARK_AVX

BENCHMARK_F(AVX, LUNoPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A, b));
}


#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(AVX, LUNoPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A));
}



BENCHMARK_F(AVX, LUNoPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_AVX
#endif //  __AVX2__
#endif // BENCHMARK_NOPIVOT



// LU - partial pivot -------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_PARTIALPIVOT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, LUPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, LUPartialPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A));
}



BENCHMARK_F(Serial, LUPartialPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::PARTIAL>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, LUPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SSE, LUPartialPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A));
}



BENCHMARK_F(SSE, LUPartialPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::PARTIAL>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SSE



#ifdef __AVX2__
#ifdef BENCHMARK_AVX

BENCHMARK_F(AVX, LUPartialPivot)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(AVX, LUPartialPivotFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A));
}



BENCHMARK_F(AVX, LUPartialPivotSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::PARTIAL>(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_AVX
#endif // __AVX2__
#endif // BENCHMARK_PARTIALPIVOT
#endif // BENCHMARK_LU



// LLT - --------------------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_SYMMETRIC
#ifdef BENCHMARK_LLT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, LLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LLT(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, LLTFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LLTFactorization(A));
}



BENCHMARK_F(Serial, LLTSolve)(benchmark::State& state)
{
    auto factorization = Solver::LLTFactorization(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LLT(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, LLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LLT(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SSE, LLTFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LLTFactorization(A));
}



BENCHMARK_F(SSE, LLTSolve)(benchmark::State& state)
{
    auto factorization = Solver::LLTFactorization(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LLT(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SSE



#ifdef __AVX2__
#ifdef BENCHMARK_AVX

BENCHMARK_F(AVX, LLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LLT(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(AVX, LLTFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LLTFactorization(A));
}



BENCHMARK_F(AVX, LLTSolve)(benchmark::State& state)
{
    auto factorization = Solver::LLTFactorization(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LLT(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_AVX
#endif // __AVX2__
#endif // BENCHMARK_LLT



// LDLT ---------------------------------------------------------------------------------------------------------------


#ifdef BENCHMARK_LDLT
#ifdef BENCHMARK_SERIAL

BENCHMARK_F(Serial, LDLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LDLT(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, LDLTFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LDLTFactorization(A));
}



BENCHMARK_F(Serial, LDLTSolve)(benchmark::State& state)
{
    auto factorization = Solver::LDLTFactorization(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LDLT(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SSE

BENCHMARK_F(SSE, LDLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LDLT(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SSE, LDLTFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LDLTFactorization(A));
}



BENCHMARK_F(SSE, LDLTSolve)(benchmark::State& state)
{
    auto factorization = Solver::LDLTFactorization(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LDLT(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_SSE



#ifdef __AVX2__
#ifdef BENCHMARK_AVX

BENCHMARK_F(AVX, LDLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LDLT(A, b));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(AVX, LDLTFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LDLTFactorization(A));
}



BENCHMARK_F(AVX, LDLTSolve)(benchmark::State& state)
{
    auto factorization = Solver::LDLTFactorization(A);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LDLT(factorization, b));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_AVX
#endif // __AVX2__
#endif // BENCHMARK_LDLT
#endif // BENCHMARK_SYMMETRIC



// Eigen --------------------------------------------------------------------------------------------------------------

#ifdef EIGEN3_FOUND
#ifdef BENCHMARK_EIGEN

// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
// https://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html

#ifdef BENCHMARK_SYMMETRIC

BENCHMARK_F(Eigen3, LLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.llt().solve(b));
}



BENCHMARK_F(Eigen3, LDLT)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.ldlt().solve(b));
}

#endif // BENCHMARK_SYMMETRIC



BENCHMARK_F(Eigen3, HouseholderQR)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.householderQr().solve(b));
}


BENCHMARK_F(Eigen3, LU)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.lu().solve(b));
}



BENCHMARK_F(Eigen3, PartialPivLU)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.partialPivLu().solve(b));
}

#endif // BENCHMARK_EIGEN
#endif // EIGEN3_FOUND


// Main
// ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
