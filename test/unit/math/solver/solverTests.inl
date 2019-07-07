#pragma once

#include "test/unit/math/solver/solverTests.h"


#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"

#include "gdl/base/simd/directAccess.h"


// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
template <Pivot _pivot>
void SolverTests<_type, _size, _solver>::RunTests(_solver solver)
{
    static bool alreadyTested = false;
    EXCEPTION(alreadyTested, "Testcase already tested. Copy and paste error?");


    TestSolve(solver);

    if constexpr (_pivot != Pivot::NONE)
        TestPivoting(solver);

    if constexpr (isSIMD)
    {
        using RegisterType = typename Matrix::RegisterType;
        constexpr U32 numRegisterValues = simd::numRegisterValues<RegisterType>;

        if constexpr (_size % numRegisterValues > 0)
            TestSIMDPivotingUnusedValues(solver);
    }

#ifndef NDEVEXCEPTION
    TestSingularMatrixException(solver);
#endif // NDEVEXCEPTION



    alreadyTested = true;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
auto SolverTests<_type, _size, _solver>::GetPermutationIndices()
{
    using permArr = std::array<U32, _size>;

    if constexpr (_size == 2)
        // clang-format off
    return std::array<permArr, 2>{{{{0, 1}},
                                   {{1, 0}}}};
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
std::array<_type, _size> SolverTests<_type, _size, _solver>::GetResultData()
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



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
std::array<_type, _size> SolverTests<_type, _size, _solver>::GetRhsData()
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


// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
std::array<_type, _size * _size> SolverTests<_type, _size, _solver>::GetTransposedMatrixData()
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
                      1, -5,  2, -1,  1,  1, -3,  1,  2,
                      6,  1, -7, -6, -7,  4,  5, -7,  7,
                      4, -1,  6, -2, -3,  8, -4,  9, -9,
                      7,  2, -7,  3,  9, -5, -3, -4,  5,
                     -5,  9, -5,  7,  5, -5,  8, -5, -4,
                      5, -6,  5, -3,  2,  1, -6,  5, -2,
                     -9, -2, -5, -4, -5,  6,  7, -8,  6,
                      4,  5,  2, -7,  1,  8, -2,  3, -7}};
    // clang-format on
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
void SolverTests<_type, _size, _solver>::SolveAndCheckResult(_solver solver, Matrix A, Vector r, Vector expRes)
{
    Vector res = solver(A, r);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
void SolverTests<_type, _size, _solver>::TestPivoting(_solver solver)
{

    auto permutations = GetPermutationIndices();

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

            Matrix A(matrixValues);
            Vector b(vectorValues);
            Vector expRes(expValues);

            SolveAndCheckResult(solver, A, b, expRes);
        }
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
void SolverTests<_type, _size, _solver>::TestSolve(_solver solver)
{
    Matrix A = Matrix(GetTransposedMatrixData()).Transpose();

    Vector b(GetRhsData());

    Vector expRes(GetResultData());

    SolveAndCheckResult(solver, A, b, expRes);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
void SolverTests<_type, _size, _solver>::TestSIMDPivotingUnusedValues([[maybe_unused]] _solver solver)
{
    if constexpr (isSIMD)
    {
        using RegisterType = typename Matrix::RegisterType;
        constexpr U32 numColRegisters = simd::CalcMinNumArrayRegisters<RegisterType>(_size);
        constexpr U32 numRegisterValues = simd::numRegisterValues<RegisterType>;

        std::array<_type, _size> expectedResultValues;
        std::array<_type, _size> vectorValues;
        std::array<RegisterType, numColRegisters * _size> matrixValues{{0}};

        for (U32 i = 0; i < _size; ++i)
        {
            expectedResultValues[i] = static_cast<_type>(i);
            vectorValues[i] = static_cast<_type>(i);
            for (U32 j = 0; j < numColRegisters; ++j)
                for (U32 k = 0; k < numRegisterValues; ++k)
                {
                    U32 globalColIdx = j * numRegisterValues + k;
                    if (globalColIdx == i)
                        simd::SetValue(matrixValues[i * numColRegisters + j], k, 1);
                    else if (globalColIdx < _size)
                        simd::SetValue(matrixValues[i * numColRegisters + j], k, 0);
                    else
                        simd::SetValue(matrixValues[i * numColRegisters + j], k, 500);
                }
        }


        Matrix A(matrixValues);
        Vector b(vectorValues);
        Vector expRes(expectedResultValues);

        for (U32 i = 0; i < _size; ++i)
            for (U32 j = 0; j < numColRegisters; ++j)
                for (U32 k = 0; k < numRegisterValues; ++k)
                    if (j * numRegisterValues + k >= _size)
                        EXCEPTION(simd::GetValue(matrixValues[i * numColRegisters + j], k) != Approx<_type>(500),
                                  "Testcase invalid. Unused memory of matrix is not set as expected.");

        SolveAndCheckResult(solver, A, b, expRes);
    }
    else
        THROW("Function should only be used by SIMD based solvers");
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, typename _solver>
void SolverTests<_type, _size, _solver>::TestSingularMatrixException(_solver solver)
{
    std::array<_type, _size* _size> matData = GetTransposedMatrixData();

    for (U32 i = 0; i < _size * _size; i += _size)
        matData[i + _size - 1] = matData[i];


    Matrix A = Matrix(matData).Transpose();

    Vector b(GetRhsData());
    Vector res;

    BOOST_CHECK_THROW(res = solver(A, b), Exception);
}
