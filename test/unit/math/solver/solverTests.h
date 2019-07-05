#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/math/serial/matSerial.h"
#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/solver/pivotEnum.h"

#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;
using namespace GDL::Solver;


template <typename _type, U32 _size>
std::array<_type, _size * _size> GetTransposedMatrixData()
{
    if constexpr (_size == 2)
        // clang-format off
        return {{-5,  6,
                  3, -5}};
    // clang-format on


    if constexpr (_size == 3)
        // clang-format off
        return {{-5,  6, -5,
                  3, -5,  2,
                  9,  8, -7}};
    // clang-format on


    if constexpr (_size == 4)
        // clang-format off
        return {{-5,  4, -5,  4,
                  3, -5,  2, -1,
                  9,  8, -7, -2,
                  4, -1,  9, -5}};
    // clang-format on


    if constexpr (_size == 5)
        // clang-format off
        return {{5,  4, -3,  8, -9,
                 3, -5,  2, -1,  1,
                 9,  1, -7, -6,  7,
                 4, -1,  6, -2, -3,
                 7,  2,  7,  3, -9}};
    // clang-format on


    if constexpr (_size == 7)
        // clang-format off
        return {{2,  4, -3,  5,  9, -3, -7,
                 3, -5,  2, -1,  1,  1, -2,
                 9,  1, -7, -6,  7,  2, -2,
                 4, -1,  6, -2, -3, -2,  1,
                 7,  2,  7,  3, -9, -5,  4,
                -5,  5, -5,  5,  5, -6,  1,
                 5, -6,  3, -3,  6,  3, -6}};
    // clang-format on


    if constexpr (_size == 8)
        // clang-format off
        return {{2,  4, -3,  5,  9, -3, -7,  1,
                 3, -5,  2, -1,  1,  1, -3,  1,
                 9,  1, -7, -6,  7,  2,  5, -7,
                 4, -1,  6, -2, -3, -8, -3,  9,
                 7,  2,  7,  3,  9, -5, -3, -4,
                -5,  5, -5,  5,  5, -5,  5, -5,
                 5, -6,  3, -3,  2,  1, -6,  5,
                 1, -2, -3, -4, -5,  6, -7,  8}};
    // clang-format on


    if constexpr (_size == 9)
        // clang-format off
        return {{-2,  4, -3,  5,  9, -1, -7,  1, -2,
                  3, -6,  2, -1,  1,  1, -3,  1,  2,
                  6,  1, -7, -6, -7,  4,  5, -7,  7,
                  4, -1,  6, -2, -3,  8, -4,  9, -9,
                  7,  2, -7,  3,  9, -5, -3, -4,  5,
                 -5,  9, -5,  7,  5, -5,  8, -5, -4,
                  5, -6,  5, -3,  2,  1, -6,  5, -2,
                 -9, -2, -5, -4, -5,  6,  7, -8,  6,
                  4,  5,  2, -7,  1,  8, -2,  3, -7}};
    // clang-format on
}

template <typename _type, U32 _size>
std::array<_type, _size> GetRhsData()
{
    if constexpr (_size == 2)
        return {{7, -7}};


    if constexpr (_size == 3)
        return {{-8, -1, 4}};


    if constexpr (_size == 4)
        return {{4, -5, -4, 9}};


    if constexpr (_size == 5)
        return {{-9, 0, 1, -3, -1}};


    if constexpr (_size == 7)
        return {{-1, -8, -1, -8, -3, 6, -4}};


    if constexpr (_size == 8)
        return {{7, -7, -8, 0, 6, 0, 4, -2}};


    if constexpr (_size == 9)
        return {{-3, 9, -6, 8, 9, 1, -8, 6, -8}};
}

template <typename _type, U32 _size>
std::array<_type, _size> GetResultData()
{
    if constexpr (_size == 2)
        return {{1, 2}};


    if constexpr (_size == 3)
        return {{1, 2, 3}};


    if constexpr (_size == 4)
        return {{1, 2, 3, 4}};


    if constexpr (_size == 5)
        return {{1, 2, 3, 4, 5}};


    if constexpr (_size == 7)
        return {{1, 2, 3, 4, 5, 6, 7}};


    if constexpr (_size == 8)
        return {{1, 2, 3, 4, 5, 6, 7, 8}};


    if constexpr (_size == 9)
        return {{1, 2, 3, 4, 5, 6, 7, 8, 9}};
}



template <typename _type, U32 _size, typename _solver, typename _matrix, typename _vector>
void TestSolverTestcase(_solver solver, _matrix A, _vector b, _vector expRes)
{
    _vector res = solver(A, b);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}



template <typename _type, U32 _size, typename _solver, typename _matrix, typename _vector>
void TestSolverResult(_solver solver)
{
    _matrix A = _matrix(GetTransposedMatrixData<_type, _size>()).Transpose();

    _vector b(GetRhsData<_type, _size>());

    _vector expRes(GetResultData<_type, _size>());

    TestSolverTestcase<_type, _size, _solver, _matrix, _vector>(solver, A, b, expRes);
}



template <typename _type, U32 _size, typename _solver, typename _matrix, typename _vector>
void TestSolverSingularMatrix(_solver solver)
{
    std::array<_type, _size* _size> matData = GetTransposedMatrixData<_type, _size>();

    for (U32 i = 0; i < _size * _size; i += _size)
        matData[i + _size - 1] = matData[i];


    _matrix A = _matrix(matData).Transpose();

    _vector b(GetRhsData<_type, _size>());
    _vector res;

    BOOST_CHECK_THROW(res = solver(A, b), Exception);
}



template <U32 _size>
auto GetIdentityPermutations()
{
    using permArr = std::array<U32, _size>;

    if constexpr (_size == 2)
        // clang-format off
        return std::array<permArr, 2>{{{{0, 1}}, {{1, 0}}}};
    // clang-format on


    if constexpr (_size == 3)
        // clang-format off
        return std::array<permArr, 3>{{{{0, 1, 2}},
                                       {{2, 1, 0}},
                                       {{1, 2, 0}}}};
    // clang-format on

    if constexpr (_size == 4)
        // clang-format off
        return std::array<permArr, 3>{{{{0, 1, 2, 3}},
                                       {{3, 2, 1, 0}},
                                       {{1, 3, 2, 0}}}};
    // clang-format on


    if constexpr (_size == 5)
        // clang-format off
        return std::array<permArr, 3>{{{{0, 1, 2, 3, 4}},
                                       {{4, 3, 2, 1, 0}},
                                       {{3, 2, 4, 0, 1}}}};
    // clang-format on


    if constexpr (_size == 7)
        // clang-format off
        return std::array<permArr, 3>{{{{0, 1, 2, 3, 4, 5, 6}},
                                       {{6, 5, 4, 3, 2, 1, 0}},
                                       {{5, 3, 0, 4, 6, 1, 2}}}};
    // clang-format on


    if constexpr (_size == 8)
        // clang-format off
        return std::array<permArr, 3>{{{{0, 1, 2, 3, 4, 5, 6, 7}},
                                       {{7, 6, 5, 4, 3, 2, 1, 0}},
                                       {{3, 6, 0, 2, 7, 1, 4, 5}}}};
    // clang-format on


    if constexpr (_size == 9)
        // clang-format off
        return std::array<permArr, 3>{{{{0, 1, 2, 3, 4, 5, 6, 7, 8}},
                                       {{8, 7, 6, 5, 4, 3, 2, 1, 0}},
                                       {{5, 0, 4, 2, 3, 8, 7, 1, 6}}}};
    // clang-format on
}


template <typename _type, U32 _size, typename _solver, typename _matrix, typename _vector>
void TestSolverPivoting(_solver solver)
{

    auto permutations = GetIdentityPermutations<_size>();

    for (U32 i = 0; i < permutations.size(); ++i)
        for (U32 j = 0; j < _size; ++j)
        {
            if (permutations[i][j] >= _size)
                THROW("Testcase invalid. Index must be in the range [0, system size]");
            for (U32 k = j + 1; k < _size; ++k)
                if (permutations[i][j] == permutations[i][k])
                    THROW("Testcase invalid. Value found multiple times");
        }


    std::array<_type, _size> expValues;
    std::array<_type, _size> vectorValues;
    std::array<_type, _size * _size> matrixValues;

    for (U32 i = 0; i < 2; ++i)
    {
        _type posNeg = (i < 1) ? -1. : 1.;
        for (U32 j = 0; j < permutations.size(); ++j)
        {
            for (U32 k = 0; k < _size; ++k)
            {
                expValues[k] = static_cast<_type>(k);
                vectorValues[k] = static_cast<_type>(permutations[j][k]) * posNeg;
                for (U32 l = 0; l < _size; ++l)
                    if (permutations[j][k] == l)
                        matrixValues[l * _size + k] = 1 * posNeg;
                    else
                        matrixValues[l * _size + k] = 0;
            }

            _matrix A(matrixValues);
            _vector b(vectorValues);
            _vector expRes(expValues);

            TestSolverTestcase<_type, _size, _solver, _matrix, _vector>(solver, A, b, expRes);
        }
    }
}



template <typename _type, U32 _size, Pivot _pivot, typename _solver>
void TestSolver(_solver solver)
{
    static bool alreadyTested = false;

    EXCEPTION(alreadyTested, "Testcase already tested. Copy and paste error?");

    using Vector = decltype(solver({}, {}));
    using Matrix = typename std::conditional<std::is_same<Vector, VecSIMD<_type, _size, true>>::value,
                                             MatSIMD<_type, _size, _size>, MatSerial<_type, _size, _size>>::type;


    TestSolverResult<_type, _size, _solver, Matrix, Vector>(solver);

    if constexpr (_pivot != Pivot::NONE)
        TestSolverPivoting<_type, _size, _solver, Matrix, Vector>(solver);

#ifndef NDEVEXCEPTION
    TestSolverSingularMatrix<_type, _size, _solver, Matrix, Vector>(solver);
#endif // NDEVEXCEPTION

    alreadyTested = true;
}
