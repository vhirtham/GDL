#include <benchmark/benchmark.h>

#include "gdl/math/solver/gauss.h"
#include "eigen3/Eigen/Dense"

using namespace GDL;



// Fixture declaration ------------------------------------------------------------------------------------------------

class SIMD : public benchmark::Fixture
{
public:
    MatSSE<F32, 8, 8> A;
    VecSSE<F32, 8> b;
    MatSSE<F32, 16, 16> A16;
    VecSSE<F32, 16> b16;
    MatSSE<F32, 32, 32> A32;
    VecSSE<F32, 32> b32;

    // clang-format off
    SIMD()
        : A{2,  4, -3,  5,  9, -3, -7,  1,
            3, -5,  2, -1,  1,  1, -3,  1,
            9,  1, -7, -6,  7,  2,  5, -7,
            4, -1,  6, -2, -3, -8, -3,  9,
            7,  2,  7,  3,  9, -5, -3, -4,
           -5,  5, -5,  5,  5, -5,  5, -5,
            5, -6,  3, -3,  2,  1, -6,  5,
            1, -2, -3, -4, -5,  6, -7,  8}
        , b{7, -7, -8, 0, 6, 0, 4, -2}
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

          b32{7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5, 7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5}
    {
    }
    // clang-format on
};


#ifdef EIGEN3_FOUND

class Eigen3 : public benchmark::Fixture
{
public:
    Eigen::Matrix<F32, 8, 8> A;
    Eigen::Matrix<F32, 8, 1> b;
    Eigen::Matrix<F32, 16, 16> A16;
    Eigen::Matrix<F32, 16, 1> b16;
    Eigen::Matrix<F32, 32, 32> A32;
    Eigen::Matrix<F32, 32, 1> b32;

    // clang-format off
    Eigen3()
    {
        A << 2,  4, -3,  5,  9, -3, -7,  1,
             3, -5,  2, -1,  1,  1, -3,  1,
             9,  1, -7, -6,  7,  2,  5, -7,
             4, -1,  6, -2, -3, -8, -3,  9,
             7,  2,  7,  3,  9, -5, -3, -4,
            -5,  5, -5,  5,  5, -5,  5, -5,
             5, -6,  3, -3,  2,  1, -6,  5,
             1, -2, -3, -4, -5,  6, -7,  8;
        b << 7, -7, -8, 0, 6, 0, 4, -2;

        A16 << 2,  4, -3,  5,  9, -3, -7,  1,  7,  1,  3, -1, -2,  4,  5,  1,
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
                4, -3, -9,  2,  5,  1, -1,  5, -1, -2, -6, -9,  4, -3,  2,  1;
        b16 << 7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5;

        A32 << 2,  4, -3,  5,  9, -3, -7,  1,  7,  1,  3, -1, -2,  4,  5,  1,  3,  6, -3,  1,  1, -3, -2,  1,  2,  1,  3, -1, -2,  4,  5,  1,
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
                2, -5, -9,  2,  5,  1, -1,  5, -1, -2, -6, -9,  4, -3,  2,  1,  3, -4,  1,  7,  3,  2, -9,  1, -6,  1,  2,  4,  6, -1, -4,  7;
        b32 << 7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5, 7, -7, -8, 0, 6, 0, 4, -2, 5, 8, -1, 2, 6, 9, 0, -5;
    }
    // clang-format on
};

#endif // EIGEN3_FOUND


// Gauss --------------------------------------------------------------------------------------------------------------


BENCHMARK_F(SIMD, GaussDense8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::GaussPartialPivot(A, b));
}



BENCHMARK_F(SIMD, GaussDense16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::GaussPartialPivot(A16, b16));
}


BENCHMARK_F(SIMD, GaussDense32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(Solver::GaussPartialPivot(A32, b32));
}

// Eigen --------------------------------------------------------------------------------------------------------------

#ifdef EIGEN3_FOUND

// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
// https://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html

// Does not make sense to compare --- only valid for symmetric matrices
// BENCHMARK_F(Eigen3, LLT8x8)(benchmark::State& state)
//{
//    for (auto _ : state)
//        benchmark::DoNotOptimize(A.llt().solve(b));
//}



BENCHMARK_F(Eigen3, LDLT8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.ldlt().solve(b));
}



BENCHMARK_F(Eigen3, HouseholderQR8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.householderQr().solve(b));
}



BENCHMARK_F(Eigen3, PartialPivLU8x8)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A.partialPivLu().solve(b));
}


BENCHMARK_F(Eigen3, LDLT16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A16.ldlt().solve(b16));
}



BENCHMARK_F(Eigen3, HouseholderQR16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A16.householderQr().solve(b16));
}



BENCHMARK_F(Eigen3, PartialPivLU16x16)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A16.partialPivLu().solve(b16));
}


BENCHMARK_F(Eigen3, LDLT32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A32.ldlt().solve(b32));
}



BENCHMARK_F(Eigen3, HouseholderQR32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A32.householderQr().solve(b32));
}


BENCHMARK_F(Eigen3, PartialPivLU32x32)(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(A32.partialPivLu().solve(b32));
}

#endif // EIGEN3_FOUND



// Main ---------------------------------------------------------------------------------------------------------------

BENCHMARK_MAIN();