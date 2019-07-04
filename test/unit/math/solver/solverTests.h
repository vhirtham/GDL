#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/math/serial/matSerial.h"
#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/simd/matSIMD.h"

#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;



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



template <typename _type, U32 _size, typename _solver>
void TestSolverResult(_solver solver)
{
    static bool alreadyTested = false;

    EXCEPTION(alreadyTested, "Testcase already tested. Copy and paste error?");

    using Vector = decltype(solver({}, {}));
    using Matrix = typename std::conditional<std::is_same<Vector, VecSIMD<_type, _size, true>>::value,
                                             MatSIMD<_type, _size, _size>, MatSerial<_type, _size, _size>>::type;

    Matrix A = Matrix(GetTransposedMatrixData<_type, _size>()).Transpose();

    Vector b(GetRhsData<_type, _size>());
    Vector res = solver(A, b);


    Vector expRes(GetResultData<_type, _size>());

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));

    alreadyTested = true;
}
