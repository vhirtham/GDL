#include <benchmark/benchmark.h>


#include "benchmark/math/solver/solverBenchmarkData.h"
#include "gdl/math/solver/lu.h"


#ifdef EIGEN3_FOUND

#define EIGEN_STACK_ALLOCATION_LIMIT 0
#include "eigen3/Eigen/Dense"

#endif // EIGEN3_FOUND


using namespace GDL;



// Setup --------------------------------------------------------------------------------------------------------------

using Type = F32;
constexpr U32 N = 64;

// size
#define BENCHMARK_8x8
#define BENCHMARK_16x16
#define BENCHMARK_32x32
#define BENCHMARK_NxN

// pivoting
#define BENCHMARK_NOPIVOT
#define BENCHMARK_PARTIALPIVOT

// factorization
//#define BENCHMARK_FACTORIZATION

// vectorization
#define BENCHMARK_SERIAL
#define BENCHMARK_SIMD

// Eigen
#define BENCHMARK_EIGEN



// Fixture declaration ------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_SERIAL

class Serial : public benchmark::Fixture
{
public:
    MatSerial<Type, 8, 8> A8;
    VecSerial<Type, 8> b8;
    MatSerial<Type, 16, 16> A16;
    VecSerial<Type, 16> b16;
    MatSerial<Type, 32, 32> A32;
    VecSerial<Type, 32> b32;
    MatSerial<Type, N, N> AN;
    VecSerial<Type, N> bN;


    Serial()
        : A8{GetMatrixData8<Type>()}
        , b8{GetVectorData8<Type>()}
        , A16{GetMatrixData16<Type>()}
        , b16{GetVectorData16<Type>()}
        , A32{GetMatrixData32<Type>()}
        , b32{GetVectorData32<Type>()}
        , AN{GetMatrixDataRandom<Type, N>()}
        , bN{GetVectorDataRandom<Type, N>()}
    {
    }
};

#endif // BENCHMARK_SERIAL



#ifdef BENCHMARK_SIMD

class SIMD : public benchmark::Fixture
{
public:
    MatSIMD<Type, 8, 8> A8;
    VecSIMD<Type, 8> b8;
    MatSIMD<Type, 16, 16> A16;
    VecSIMD<Type, 16> b16;
    MatSIMD<Type, 32, 32> A32;
    VecSIMD<Type, 32> b32;
    MatSIMD<Type, N, N> AN;
    VecSIMD<Type, N> bN;

    SIMD()
        : A8{GetMatrixData8<Type>()}
        , b8{GetVectorData8<Type>()}
        , A16{GetMatrixData16<Type>()}
        , b16{GetVectorData16<Type>()}
        , A32{GetMatrixData32<Type>()}
        , b32{GetVectorData32<Type>()}
        , AN{GetMatrixDataRandom<Type, N>()}
        , bN{GetVectorDataRandom<Type, N>()}
    {
    }
};

#endif // BENCHMARK_SIMD



#ifdef BENCHMARK_EIGEN
#ifdef EIGEN3_FOUND

class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix<Type, 8, 8> A8;
    Eigen::Matrix<Type, 8, 1> b8;
    Eigen::Matrix<Type, 16, 16> A16;
    Eigen::Matrix<Type, 16, 1> b16;
    Eigen::Matrix<Type, 32, 32> A32;
    Eigen::Matrix<Type, 32, 1> b32;
    Eigen::Matrix<Type, N, N> AN;
    Eigen::Matrix<Type, N, 1> bN;


    Eigen3()
    {
        SetMatrix<8>(A8, GetMatrixData8<Type>());
        SetVector<8>(b8, GetVectorData8<Type>());
        SetMatrix<16>(A16, GetMatrixData16<Type>());
        SetVector<16>(b16, GetVectorData16<Type>());
        SetMatrix<32>(A32, GetMatrixData32<Type>());
        SetVector<32>(b32, GetVectorData32<Type>());
        SetMatrix<N>(AN, GetMatrixDataRandom<Type, N>());
        SetVector<N>(bN, GetVectorDataRandom<Type, N>());
    }



    template <U32 _size>
    void SetMatrix(Eigen::Matrix<Type, _size, _size>& mat, const std::array<Type, _size * _size>& data)
    {
        for (U32 i = 0; i < _size; ++i)
            for (U32 j = 0; j < _size; ++j)
                mat(i, j) = data[i * _size + j];
    }


    template <U32 _size>
    void SetVector(Eigen::Matrix<Type, _size, 1>& vec, const std::array<Type, _size>& data)
    {
        for (U32 i = 0; i < _size; ++i)
            for (U32 j = 0; j < _size; ++j)
                vec(i, 0) = data[i];
    }
};

#endif // EIGEN3_FOUND
#endif // BENCHMARK_EIGEN


// Serial - No pivot --------------------------------------------------------------------------------------------------


#ifdef BENCHMARK_NOPIVOT
#ifdef BENCHMARK_SERIAL
#ifdef BENCHMARK_8x8

BENCHMARK_F(Serial, NoPivot_8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A8, b8));
}


#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, NoPivot_8x8_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A8));
}



BENCHMARK_F(Serial, NoPivot_8x8_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A8);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b8));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_8x8



#ifdef BENCHMARK_16x16

BENCHMARK_F(Serial, NoPivot_16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A16, b16));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, NoPivot_16x16_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A16));
}



BENCHMARK_F(Serial, NoPivot_16x16_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A16);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b16));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_16x16



#ifdef BENCHMARK_32x32

BENCHMARK_F(Serial, NoPivot_32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A32, b32));
}


#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, NoPivot_32x32_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A32));
}



BENCHMARK_F(Serial, NoPivot_32x32_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A32);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b32));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_32x32



#ifdef BENCHMARK_NxN

BENCHMARK_F(Serial, NoPivot_NxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(AN, bN));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, NoPivot_NxN_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(AN));
}



BENCHMARK_F(Serial, NoPivot_NxN_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(AN);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, bN));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_NxN
#endif // BENCHMARK_SERIAL



// SIMD - No pivot ----------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_SIMD
#ifdef BENCHMARK_8x8

BENCHMARK_F(SIMD, NoPivot_8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A8, b8));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SIMD, NoPivot_8x8__Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A8));
}



BENCHMARK_F(SIMD, NoPivot_8x8__Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A8);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b8));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_8x8



#ifdef BENCHMARK_16x16

BENCHMARK_F(SIMD, NoPivot_16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A16, b16));
}


#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SIMD, NoPivot_16x16_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A16));
}



BENCHMARK_F(SIMD, NoPivot_16x16_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A16);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b16));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_16x16



#ifdef BENCHMARK_32x32

BENCHMARK_F(SIMD, NoPivot_32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A32, b32));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SIMD, NoPivot_32x32_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A32));
}



BENCHMARK_F(SIMD, NoPivot_32x32_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A32);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b32));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_32x32



#ifdef BENCHMARK_NxN

BENCHMARK_F(SIMD, NoPivot_NxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(AN, bN));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(SIMD, NoPivot_NxN_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(AN));
}



BENCHMARK_F(SIMD, NoPivot_NxN_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(AN);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, bN));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_NxN
#endif // BENCHMARK_SIMD
#endif // BENCHMARK_NOPIVOT



// Serial - partial pivot ---------------------------------------------------------------------------------------------

#ifdef BENCHMARK_PARTIALPIVOT
#ifdef BENCHMARK_SERIAL
#ifdef BENCHMARK_8x8

BENCHMARK_F(Serial, PartialPivot_8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A8, b8));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, PartialPivot_8x8_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A8));
}



BENCHMARK_F(SIMD, PartialPivot_8x8_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::PARTIAL>(A8);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(factorization, b8));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_8x8



#ifdef BENCHMARK_16x16

BENCHMARK_F(Serial, PartialPivot_16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A16, b16));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, PartialPivot_16x16_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A16));
}



BENCHMARK_F(SIMD, PartialPivot_16x16_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::PARTIAL>(A16);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(factorization, b16));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_16x16


#ifdef BENCHMARK_32x32

BENCHMARK_F(Serial, PartialPivot_32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A32, b32));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, PartialPivot_32x32_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A32));
}



BENCHMARK_F(SIMD, PartialPivot_32x32_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::PARTIAL>(A32);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(factorization, b32));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_32x32



#ifdef BENCHMARK_NxN

BENCHMARK_F(Serial, PartialPivot_NxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(AN, bN));
}



#ifdef BENCHMARK_FACTORIZATION

BENCHMARK_F(Serial, PartialPivot_NxN_Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(AN));
}



BENCHMARK_F(SIMD, PartialPivot_NxN_Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::PARTIAL>(AN);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(factorization, bN));
}

#endif // BENCHMARK_FACTORIZATION
#endif // BENCHMARK_NxN
#endif // BENCHMARK_SERIAL



// SIMD - partial pivot -----------------------------------------------------------------------------------------------

#ifdef BENCHMARK_SIMD

// BENCHMARK_F(SIMD, PartialPivot_8x8)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A8, b8));
//}



// BENCHMARK_F(SIMD, PartialPivot_16x16)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A16, b16));
//}



// BENCHMARK_F(SIMD, PartialPivot_32x32)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A32, b32));
//}



// BENCHMARK_F(SIMD, PartialPivot_NxN)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(AN, bN));
//}


#endif // BENCHMARK_SIMD
#endif // BENCHMARK_PARTIALPIVOT



// Eigen --------------------------------------------------------------------------------------------------------------

#ifdef BENCHMARK_EIGEN
#ifdef EIGEN3_FOUND

// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
// https://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html



#ifdef BENCHMARK_8x8

BENCHMARK_F(Eigen3, PartialPiv_LU_8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A8.partialPivLu().solve(b8));
}

#endif // BENCHMARK_8x8



#ifdef BENCHMARK_16x16

BENCHMARK_F(Eigen3, PartialPiv_LU_16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A16.partialPivLu().solve(b16));
}

#endif // BENCHMARK_16x16



#ifdef BENCHMARK_32x32

BENCHMARK_F(Eigen3, PartialPiv_LU_32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A32.partialPivLu().solve(b32));
}

#endif // BENCHMARK_32x32



#ifdef BENCHMARK_NxN

BENCHMARK_F(Eigen3, PartialPiv_LU_NxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(AN.partialPivLu().solve(bN));
}

#endif // BENCHMARK_NxN



//#ifdef BENCHMARK_8x8

// BENCHMARK_F(Eigen3, HouseholderQR_8x8)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(A8.householderQr().solve(b8));
//}

//#endif // BENCHMARK_8x8



//#ifdef BENCHMARK_16x16

// BENCHMARK_F(Eigen3, HouseholderQR_16x16)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(A16.householderQr().solve(b16));
//}

//#endif // BENCHMARK_16x16



//#ifdef BENCHMARK_32x32

// BENCHMARK_F(Eigen3, HouseholderQR_32x32)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(A32.householderQr().solve(b32));
//}

//#endif // BENCHMARK_32x32



//#ifdef BENCHMARK_NxN

// BENCHMARK_F(Eigen3, HouseholderQRNxN)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(AN.householderQr().solve(bN));
//}

//#endif // BENCHMARK_NxN
#endif // EIGEN3_FOUND
#endif // BENCHMARK_EIGEN



// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
