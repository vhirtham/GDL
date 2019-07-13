#include <benchmark/benchmark.h>

#define EIGEN_STACK_ALLOCATION_LIMIT 0

#include "gdl/math/solver/lu.h"
#include "eigen3/Eigen/Dense"

#include <cstdlib>

using namespace GDL;

using Type = F32;
constexpr U32 N = 64;


// Helper functions ---------------------------------------------------------------------------------------------------

std::array<Type, N * N> GetRandomMatrixData()
{
    std::array<Type, N * N> arr;
    for (U32 i = 0; i < arr.size(); ++i)
        arr[i] = rand() % 20 - 10;

    return arr;
}

std::array<Type, N> GetRandomVectorData()
{
    std::array<Type, N> arr;
    for (U32 i = 0; i < arr.size(); ++i)
        arr[i] = rand() % 20 - 10;

    return arr;
}


const std::array<Type, N * N>& MatrixData()
{
    static std::array<Type, N* N> data = GetRandomMatrixData();
    return data;
}


const std::array<Type, N>& VectorData()
{
    static std::array<Type, N> data = GetRandomVectorData();
    return data;
}

// Fixture declaration ------------------------------------------------------------------------------------------------

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

    // clang-format off
    Serial()
        : A8{2,  4, -3,  5,  9, -3, -7,  1,
            3, -5,  2, -1,  1,  1, -3,  1,
            9,  1, -7, -6,  7,  2,  5, -7,
            4, -1,  6, -2, -3, -8, -3,  9,
            7,  2,  7,  3,  9, -5, -3, -4,
           -5,  5, -5,  5,  5, -5,  5, -5,
            5, -6,  3, -3,  2,  1, -6,  5,
            1, -2, -3, -4, -5,  6, -7,  8}
        , b8{7, -7, -8, 0, 6, 0, 4, -2}
        ,A16{2,  4, -3,  5,  9, -3, -7,  1,  7,  1,  3, -1, -2,  4,  5,  1,
             3, -5,  2, -1,  1,  1, -3,  1, -4,  1,  2,  4, -7, -1, -4,  5,
             9,  1, -7, -6,  7,  2,  5, -7,  8,  2,  5,  6,  1,  4, -5,  1,
             4, -1,  6, -2, -3, -8, -3,  9,  3,  3,  3,  1, -1, -1,  4,  1,
             7,  2,  7,  3,  9, -5, -3, -4, -6,  8,  9,  4, -2,  1,  4,  7,
            -5,  5, -5,  5,  5, -5,  5, -5,  7,  2,  5,  6, -4, -4,  1,  1,
             5, -6,  3, -3,  2,  1, -6,  5,  5, -2,  4,  2,  7, -8,  3,  3,
             1, -2, -3, -4, -5,  6, -7,  8, -1, -7, -5,  4,  3, -3,  7, -6,
             2,  4, -5,  1,  5,  2, -5,  9, -2,  1, -2, -3, -4,  5,  8,  9,
             7,  6,  5, -8,  5,  6,  2, -3,  1,  5, -4,  2, -9,  2,  5,  8,
             1,  2,  3,  4,  5,  6,  7,  8, -8, -7, -6, -5, -4, -3, -2, -1,
             1,  2,  3,  4,  5,  6,  7,  8,  1,  2,  3,  4,  5,  6,  7,  8,
             2, -2,  4, -4,  6, -6,  8, -8,  1, -1,  3, -3,  5, -5,  7, -7,
             1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,
             9, -7,  5, -3,  1, -1,  3, -5,  7, -9,  1,  2,  3,  4,  5,  9,
             4, -3, -9,  2,  5,  1, -1,  5, -1, -2, -6, -9,  4, -3,  2,  1}
       , b16{7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5}
        ,A32{2,  4, -3,  5,  9, -3, -7,  1,  7,  1,  3, -1, -2,  4,  5,  1,  3,  6, -3,  1,  1, -3, -2,  1,  2,  1,  3, -1, -2,  4,  5,  1,
             3, -5,  2, -1,  1,  1, -3,  1, -4,  1,  2,  4, -7, -1, -4,  5,  4,  3,  3,  5,  1, -3,  7,  3,  7,  6,  3, -1,  9,  9,  5,  4,
             9,  1, -7, -6,  7,  2,  5, -7,  8,  2,  5,  6,  1,  4, -5,  1,  2,  2, -4,  6,  8, -3,  8,  1,  4,  2,  3, -1,  4,  4,  5,  3,
             4, -1,  6, -2, -3, -8, -3,  9,  3,  3,  3,  1, -1, -1,  4,  1,  1,  1, -6, -5,  6, -3,  6,  2,  1, -9,  3,  4, -2,  1,  5,  2,
             7,  2,  7,  3,  9, -5, -3, -4, -6,  8,  9,  4, -2,  1,  4,  7,  6,  2,  1,  2, -4, -3,  5,  5,  6,  3,  3, -1, -4,  4,  5,  8,
            -5,  5, -5,  5,  5, -5,  5, -5,  7,  2,  5,  6, -4, -4,  1,  1,  7,  8,  2,  1,  5, -3, -3,  1,  8,  1,  3, -1, -3,  2,  5,  4,
             5, -6,  3, -3,  2,  1, -6,  5,  5, -2,  4,  2,  7, -8,  3,  3,  7,  6, -4,  6,  2, -3,  2,  7,  5,  8,  3, -1, -1,  3,  5,  6,
             1, -2, -3, -4, -5,  6, -7,  8, -1, -7, -5,  4,  3, -3,  7, -6,  2,  4,  5,  4,  3, -3,  1,  1,  2,  7,  3, -1,  6,  8,  5,  7,
             2,  7, -5,  1,  5,  2, -5,  9, -2,  1, -2, -3, -4,  5,  8,  9,  4,  3, -2,  8,  1, -3,  9,  6, -7,  6,  3, -1, -1,  7,  3,  8,
             7,  3,  5, -8,  5,  6,  2, -3,  1,  5, -4,  2, -9,  2,  5,  8,  6,  2, -4,  3,  9, -3,  6,  1,  6, -1,  3, -1,  6,  5,  5, -1,
             1,  9,  3,  4,  5,  6,  7,  8, -8, -7, -6, -5, -4, -3, -2, -1,  7,  4, -1,  0,  5, -3, -7,  2,  1,  2,  3, -1, -2, -4,  4,  4,
             1,  2,  3,  4,  5,  6,  7,  8,  1,  2,  3,  4,  5,  6,  7,  8,  4,  8,  2,  2,  9, -3, -1,  2,  2,  1,  3, -1,  1,  2,  1,  3,
             2, -6,  4, -4,  6, -6,  8, -8,  1, -1,  3, -3,  5, -5,  7, -7,  7,  9,  1,  6,  6, -3, -4,  6,  7,  3,  3, -1,  5,  4,  2,  2,
             1,  4,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  8,  3,  5,  2,  2, -3, -6,  5,  4,  1,  3, -1, -2,  3,  3,  1,
             9, -2,  5, -3,  1, -1,  3, -5,  7, -9,  1,  2,  3,  4,  5,  9,  4,  8,  2,  1,  9, -3, -7,  1,  8,  9,  2, -1,  4,  4,  6,  7,
             4, -1, -9,  2,  5,  1, -1,  5, -1, -2, -6, -9,  4, -3,  2,  1,  2,  2,  6,  3,  8, -3, -3,  4,  7,  1,  3, -1,  7,  9,  9, -8,
             7,  1, -3,  5,  9, -3, -7,  1,  7,  1,  3, -1, -2,  4,  5,  1,  3, -3,  1, -9,  1,  1, -2,  3, -1, -1,  2,  4,  8,  6, -8,  6,
             8, -3,  2, -1,  1,  1, -3,  1, -4,  1,  2,  4, -7, -1, -4,  5,  4, -4,  3, -1,  3,  2, -9,  9, -2,  2,  2,  4,  3,  7, -2,  4,
             8,  9, -7, -6,  7,  2,  5, -7,  8,  2,  5,  6,  1,  4, -5,  1,  1, -6,  7,  5,  7,  4,  6, -7, -3,  1,  2,  4,  9,  1,  4,  2,
            -4, -3,  6, -2, -3, -8, -3,  9,  3,  3,  3,  1, -1, -1,  4,  1, -3, -2,  4, -3,  1,  1,  3,  6, -4,  4,  2,  4, -7,  3, -2,  3,
             3,  4,  7,  3,  9, -5, -3, -4, -6,  8,  9,  4, -2,  1,  4,  7,  7, -9,  8,  2,  8,  1,  1,  3, -5,  2,  2,  4, -4,  2, -4,  7,
            -1,  7, -5,  5,  5, -5,  5, -5,  7,  2,  5,  6, -4, -4,  1,  1,  4, -3, -2,  6,  1,  8,  4,  1, -6,  7,  2,  4,  2, -1, -2, -5,
             5,  0,  3, -3,  2,  1, -6,  5,  5, -2,  4,  2,  7, -8,  3,  3,  1, -1, -4,  8,  2,  1, -6,  3, -7,  1,  2,  4,  6,  4,  6, -1,
             8, -4, -3, -4, -5,  6, -7,  8, -1, -7, -5,  4,  3, -3,  7, -6, -3, -2,  5,  7, -1,  7, -5,  2, -8,  5,  2,  4,  1, -1, -3,  6,
             4,  8, -5,  1,  5,  2, -5,  9, -2,  1, -2, -3, -4,  5,  8,  9,  3, -6,  2,  4,  5,  2, -1,  1, -9,  2,  2,  4, -7,  6, -1,  8,
             3,  2,  5, -8,  5,  6,  2, -3,  1,  5, -4,  2, -9,  2,  5,  8, -2, -8,  1,  4,  2,  1, -2,  3,  0,  1,  2,  4,  5,  1, -4, -5,
             2,  8,  3,  4,  5,  6,  7,  8, -8, -7, -6, -5, -4, -3, -2, -1,  1, -7,  3,  3,  1,  6, -0,  6, -1,  2,  2,  4,  3, -2, -3,  4,
             1,  1,  3,  4,  5,  6,  7,  8,  1,  2,  3,  4,  5,  6,  7,  8,  8, -1,  6,  2,  3,  1, -1,  3, -2,  3,  2,  4,  2,  3, -4,  3,
             7, -3,  4, -4,  6, -6,  8, -8,  1, -1,  3, -3,  5, -5,  7, -7,  5, -3,  5,  1,  3,  1, -3,  1, -3,  7,  2,  4,  1,  9, -5,  2,
             9,  7,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9, -7,  3,  8,  2,  4, -2,  2, -4,  1,  2,  4,  5,  1, -4,  1,
             1, -2,  5, -3,  1, -1,  3, -5,  7, -9,  1,  2,  3,  4,  5,  9,  2, -2,  8,  9,  6,  1, -8,  0, -5,  5,  2,  4,  4,  8,  2,  4,
             2, -5, -9,  2,  5,  1, -1,  5, -1, -2, -6, -9,  4, -3,  2,  1,  3, -4,  1,  7,  3,  2, -9,  1, -6,  1,  2,  4,  6, -1, -4,  7},

          b32{7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5, 7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5},
          AN{MatrixData()},
          bN{VectorData()}
    {
    }
    // clang-format on
};



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

    // clang-format off
    SIMD()
        : A8{2,  4, -3,  5,  9, -3, -7,  1,
            3, -5,  2, -1,  1,  1, -3,  1,
            9,  1, -7, -6,  7,  2,  5, -7,
            4, -1,  6, -2, -3, -8, -3,  9,
            7,  2,  7,  3,  9, -5, -3, -4,
           -5,  5, -5,  5,  5, -5,  5, -5,
            5, -6,  3, -3,  2,  1, -6,  5,
            1, -2, -3, -4, -5,  6, -7,  8}
        , b8{7, -7, -8, 0, 6, 0, 4, -2}
        ,A16{2,  4, -3,  5,  9, -3, -7,  1,  7,  1,  3, -1, -2,  4,  5,  1,
             3, -5,  2, -1,  1,  1, -3,  1, -4,  1,  2,  4, -7, -1, -4,  5,
             9,  1, -7, -6,  7,  2,  5, -7,  8,  2,  5,  6,  1,  4, -5,  1,
             4, -1,  6, -2, -3, -8, -3,  9,  3,  3,  3,  1, -1, -1,  4,  1,
             7,  2,  7,  3,  9, -5, -3, -4, -6,  8,  9,  4, -2,  1,  4,  7,
            -5,  5, -5,  5,  5, -5,  5, -5,  7,  2,  5,  6, -4, -4,  1,  1,
             5, -6,  3, -3,  2,  1, -6,  5,  5, -2,  4,  2,  7, -8,  3,  3,
             1, -2, -3, -4, -5,  6, -7,  8, -1, -7, -5,  4,  3, -3,  7, -6,
             2,  4, -5,  1,  5,  2, -5,  9, -2,  1, -2, -3, -4,  5,  8,  9,
             7,  6,  5, -8,  5,  6,  2, -3,  1,  5, -4,  2, -9,  2,  5,  8,
             1,  2,  3,  4,  5,  6,  7,  8, -8, -7, -6, -5, -4, -3, -2, -1,
             1,  2,  3,  4,  5,  6,  7,  8,  1,  2,  3,  4,  5,  6,  7,  8,
             2, -2,  4, -4,  6, -6,  8, -8,  1, -1,  3, -3,  5, -5,  7, -7,
             1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,
             9, -7,  5, -3,  1, -1,  3, -5,  7, -9,  1,  2,  3,  4,  5,  9,
             4, -3, -9,  2,  5,  1, -1,  5, -1, -2, -6, -9,  4, -3,  2,  1}
       , b16{7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5}
        ,A32{2,  4, -3,  5,  9, -3, -7,  1,  7,  1,  3, -1, -2,  4,  5,  1,  3,  6, -3,  1,  1, -3, -2,  1,  2,  1,  3, -1, -2,  4,  5,  1,
             3, -5,  2, -1,  1,  1, -3,  1, -4,  1,  2,  4, -7, -1, -4,  5,  4,  3,  3,  5,  1, -3,  7,  3,  7,  6,  3, -1,  9,  9,  5,  4,
             9,  1, -7, -6,  7,  2,  5, -7,  8,  2,  5,  6,  1,  4, -5,  1,  2,  2, -4,  6,  8, -3,  8,  1,  4,  2,  3, -1,  4,  4,  5,  3,
             4, -1,  6, -2, -3, -8, -3,  9,  3,  3,  3,  1, -1, -1,  4,  1,  1,  1, -6, -5,  6, -3,  6,  2,  1, -9,  3,  4, -2,  1,  5,  2,
             7,  2,  7,  3,  9, -5, -3, -4, -6,  8,  9,  4, -2,  1,  4,  7,  6,  2,  1,  2, -4, -3,  5,  5,  6,  3,  3, -1, -4,  4,  5,  8,
            -5,  5, -5,  5,  5, -5,  5, -5,  7,  2,  5,  6, -4, -4,  1,  1,  7,  8,  2,  1,  5, -3, -3,  1,  8,  1,  3, -1, -3,  2,  5,  4,
             5, -6,  3, -3,  2,  1, -6,  5,  5, -2,  4,  2,  7, -8,  3,  3,  7,  6, -4,  6,  2, -3,  2,  7,  5,  8,  3, -1, -1,  3,  5,  6,
             1, -2, -3, -4, -5,  6, -7,  8, -1, -7, -5,  4,  3, -3,  7, -6,  2,  4,  5,  4,  3, -3,  1,  1,  2,  7,  3, -1,  6,  8,  5,  7,
             2,  7, -5,  1,  5,  2, -5,  9, -2,  1, -2, -3, -4,  5,  8,  9,  4,  3, -2,  8,  1, -3,  9,  6, -7,  6,  3, -1, -1,  7,  3,  8,
             7,  3,  5, -8,  5,  6,  2, -3,  1,  5, -4,  2, -9,  2,  5,  8,  6,  2, -4,  3,  9, -3,  6,  1,  6, -1,  3, -1,  6,  5,  5, -1,
             1,  9,  3,  4,  5,  6,  7,  8, -8, -7, -6, -5, -4, -3, -2, -1,  7,  4, -1,  0,  5, -3, -7,  2,  1,  2,  3, -1, -2, -4,  4,  4,
             1,  2,  3,  4,  5,  6,  7,  8,  1,  2,  3,  4,  5,  6,  7,  8,  4,  8,  2,  2,  9, -3, -1,  2,  2,  1,  3, -1,  1,  2,  1,  3,
             2, -6,  4, -4,  6, -6,  8, -8,  1, -1,  3, -3,  5, -5,  7, -7,  7,  9,  1,  6,  6, -3, -4,  6,  7,  3,  3, -1,  5,  4,  2,  2,
             1,  4,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  8,  3,  5,  2,  2, -3, -6,  5,  4,  1,  3, -1, -2,  3,  3,  1,
             9, -2,  5, -3,  1, -1,  3, -5,  7, -9,  1,  2,  3,  4,  5,  9,  4,  8,  2,  1,  9, -3, -7,  1,  8,  9,  2, -1,  4,  4,  6,  7,
             4, -1, -9,  2,  5,  1, -1,  5, -1, -2, -6, -9,  4, -3,  2,  1,  2,  2,  6,  3,  8, -3, -3,  4,  7,  1,  3, -1,  7,  9,  9, -8,
             7,  1, -3,  5,  9, -3, -7,  1,  7,  1,  3, -1, -2,  4,  5,  1,  3, -3,  1, -9,  1,  1, -2,  3, -1, -1,  2,  4,  8,  6, -8,  6,
             8, -3,  2, -1,  1,  1, -3,  1, -4,  1,  2,  4, -7, -1, -4,  5,  4, -4,  3, -1,  3,  2, -9,  9, -2,  2,  2,  4,  3,  7, -2,  4,
             8,  9, -7, -6,  7,  2,  5, -7,  8,  2,  5,  6,  1,  4, -5,  1,  1, -6,  7,  5,  7,  4,  6, -7, -3,  1,  2,  4,  9,  1,  4,  2,
            -4, -3,  6, -2, -3, -8, -3,  9,  3,  3,  3,  1, -1, -1,  4,  1, -3, -2,  4, -3,  1,  1,  3,  6, -4,  4,  2,  4, -7,  3, -2,  3,
             3,  4,  7,  3,  9, -5, -3, -4, -6,  8,  9,  4, -2,  1,  4,  7,  7, -9,  8,  2,  8,  1,  1,  3, -5,  2,  2,  4, -4,  2, -4,  7,
            -1,  7, -5,  5,  5, -5,  5, -5,  7,  2,  5,  6, -4, -4,  1,  1,  4, -3, -2,  6,  1,  8,  4,  1, -6,  7,  2,  4,  2, -1, -2, -5,
             5,  0,  3, -3,  2,  1, -6,  5,  5, -2,  4,  2,  7, -8,  3,  3,  1, -1, -4,  8,  2,  1, -6,  3, -7,  1,  2,  4,  6,  4,  6, -1,
             8, -4, -3, -4, -5,  6, -7,  8, -1, -7, -5,  4,  3, -3,  7, -6, -3, -2,  5,  7, -1,  7, -5,  2, -8,  5,  2,  4,  1, -1, -3,  6,
             4,  8, -5,  1,  5,  2, -5,  9, -2,  1, -2, -3, -4,  5,  8,  9,  3, -6,  2,  4,  5,  2, -1,  1, -9,  2,  2,  4, -7,  6, -1,  8,
             3,  2,  5, -8,  5,  6,  2, -3,  1,  5, -4,  2, -9,  2,  5,  8, -2, -8,  1,  4,  2,  1, -2,  3,  0,  1,  2,  4,  5,  1, -4, -5,
             2,  8,  3,  4,  5,  6,  7,  8, -8, -7, -6, -5, -4, -3, -2, -1,  1, -7,  3,  3,  1,  6, -0,  6, -1,  2,  2,  4,  3, -2, -3,  4,
             1,  1,  3,  4,  5,  6,  7,  8,  1,  2,  3,  4,  5,  6,  7,  8,  8, -1,  6,  2,  3,  1, -1,  3, -2,  3,  2,  4,  2,  3, -4,  3,
             7, -3,  4, -4,  6, -6,  8, -8,  1, -1,  3, -3,  5, -5,  7, -7,  5, -3,  5,  1,  3,  1, -3,  1, -3,  7,  2,  4,  1,  9, -5,  2,
             9,  7,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9, -7,  3,  8,  2,  4, -2,  2, -4,  1,  2,  4,  5,  1, -4,  1,
             1, -2,  5, -3,  1, -1,  3, -5,  7, -9,  1,  2,  3,  4,  5,  9,  2, -2,  8,  9,  6,  1, -8,  0, -5,  5,  2,  4,  4,  8,  2,  4,
             2, -5, -9,  2,  5,  1, -1,  5, -1, -2, -6, -9,  4, -3,  2,  1,  3, -4,  1,  7,  3,  2, -9,  1, -6,  1,  2,  4,  6, -1, -4,  7},

          b32{7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5, 7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5},
          AN{MatrixData()},
          bN{VectorData()}
    {
    }
    // clang-format on
};


#ifdef EIGEN3_FOUND



class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix<Type, N, N> A;
    Eigen::Matrix<Type, N, 1> b;

    // clang-format off
    Eigen3()
    {

        for (U32 i = 0; i<N;++i)
            for (U32 j = 0; j<N;++j)
                A(i,j) = MatrixData()[i*N+j];
        for (U32 i = 0; i<N;++i)
            b[i] = VectorData()[i];

    }
    // clang-format on
};

#endif // EIGEN3_FOUND



// LU partial pivot ------------------------------------------------------------------------------------------------

BENCHMARK_F(Serial, LUNoPivot8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A8, b8));
}


BENCHMARK_F(Serial, LUNoPivot8x8Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A8));
}



BENCHMARK_F(Serial, LUNoPivot8x8Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A8);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b8));
}



BENCHMARK_F(Serial, LUNoPivot16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A16, b16));
}



BENCHMARK_F(Serial, LUNoPivot16x16Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A16));
}



BENCHMARK_F(Serial, LUNoPivot16x16Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A16);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b16));
}



BENCHMARK_F(Serial, LUNoPivot32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A32, b32));
}



BENCHMARK_F(Serial, LUNoPivot32x32Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A32));
}



BENCHMARK_F(Serial, LUNoPivot32x32Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A32);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b32));
}



BENCHMARK_F(Serial, LUNoPivotNxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(AN, bN));
}



BENCHMARK_F(Serial, LUNoPivotNxNFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(AN));
}



BENCHMARK_F(Serial, LUNoPivotNxNSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(AN);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, bN));
}



BENCHMARK_F(SIMD, LUNoPivot8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A8, b8));
}



BENCHMARK_F(SIMD, LUNoPivot8x8Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A8));
}



BENCHMARK_F(SIMD, LUNoPivot8x8Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A8);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b8));
}



BENCHMARK_F(SIMD, LUNoPivot16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A16, b16));
}



BENCHMARK_F(SIMD, LUNoPivot16x16Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A16));
}



BENCHMARK_F(SIMD, LUNoPivot16x16Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A16);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b16));
}



BENCHMARK_F(SIMD, LUNoPivot32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(A32, b32));
}



BENCHMARK_F(SIMD, LUNoPivot32x32Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(A32));
}



BENCHMARK_F(SIMD, LUNoPivot32x32Solve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(A32);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, b32));
}



BENCHMARK_F(SIMD, LUNoPivotNxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(AN, bN));
}



BENCHMARK_F(SIMD, LUNoPivotNxNFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::NONE>(AN));
}



BENCHMARK_F(SIMD, LUNoPivotNxNSolve)(benchmark::State& state)
{
    auto factorization = Solver::LUFactorization<Solver::Pivot::NONE>(AN);
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::NONE>(factorization, bN));
}



// LU partial pivot ------------------------------------------------------------------------------------------------

BENCHMARK_F(Serial, LUPartialPivot8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A8, b8));
}



BENCHMARK_F(Serial, LUPartialPivot8x8Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A8));
}



BENCHMARK_F(Serial, LUPartialPivot16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A16, b16));
}



BENCHMARK_F(Serial, LUPartialPivot16x16Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A16));
}



BENCHMARK_F(Serial, LUPartialPivot32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A32, b32));
}



BENCHMARK_F(Serial, LUPartialPivot32x32Factorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(A32));
}



BENCHMARK_F(Serial, LUPartialPivotNxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(AN, bN));
}



BENCHMARK_F(Serial, LUPartialPivotNxNFactorize)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::LUFactorization<Solver::Pivot::PARTIAL>(AN));
}


// BENCHMARK_F(SIMD, LUPartialPivot8x8)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A8, b8));
//}



// BENCHMARK_F(SIMD, LUPartialPivot16x16)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A16, b16));
//}



// BENCHMARK_F(SIMD, LUPartialPivot32x32)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(A32, b32));
//}



// BENCHMARK_F(SIMD, LUPartialPivotNxN)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(Solver::LU<Solver::Pivot::PARTIAL>(AN, bN));
//}



// Eigen --------------------------------------------------------------------------------------------------------------

#ifdef EIGEN3_FOUND

// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
// https://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html

// Does not make sense to compare --- only valid for symmetric matrices
// BENCHMARK_F(Eigen3, LLT8x8)(benchmark::State& state)
// {
//     for (auto _ : state)
//         benchmark::DoNotOptimize(A.llt().solve(b));
// }



BENCHMARK_F(Eigen3, LDLTNxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.ldlt().solve(b));
}



BENCHMARK_F(Eigen3, HouseholderQRNxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.householderQr().solve(b));
}



BENCHMARK_F(Eigen3, PartialPivLUNxN)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.partialPivLu().solve(b));
}



#endif // EIGEN3_FOUND



// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();
