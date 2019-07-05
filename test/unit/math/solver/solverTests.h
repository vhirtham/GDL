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
class SolverTests
{
public:
    template <Pivot _pivot, typename _solver>
    static void TestSolver(_solver solver);

private:
    static std::array<_type, _size * _size> GetTransposedMatrixData();

    static std::array<_type, _size> GetRhsData();

    static std::array<_type, _size> GetResultData();



    template <typename _solver, typename _matrix, typename _vector>
    static void TestSolverTestcase(_solver solver, _matrix A, _vector b, _vector expRes);



    template <typename _solver, typename _matrix, typename _vector>
    static void TestSolverResult(_solver solver);



    template <typename _solver, typename _matrix, typename _vector>
    static void TestSolverSingularMatrix(_solver solver);



    static auto GetIdentityPermutations();


    template <typename _solver, typename _matrix, typename _vector>
    static void TestSolverPivoting(_solver solver);
};

#include "test/unit/math/solver/solverTests.inl"
