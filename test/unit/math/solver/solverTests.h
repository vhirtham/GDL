#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functionTraits.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/math/serial/matSerial.h"
#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/solver/pivotEnum.h"

#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;
using namespace GDL::Solver;



template <typename _type, U32 _size, typename _solver>
class SolverTests
{

    using Vector = typename FunctionTraits<_solver>::ReturnType;
    using Matrix = typename FunctionTraits<_solver>::template Argument<0>::ValueType;
    static constexpr bool isSIMD = !std::is_same<Vector, VecSerial<_type, _size, true>>::value;


    static_assert(std::is_function<typename std::remove_pointer<_solver>::type>::value, "_solver must be a function.");
    static_assert(FunctionTraits<_solver>::numArguments == 2, "Expected solver function must have 2 arguments.");
    static_assert(std::is_same<Vector, typename FunctionTraits<_solver>::template Argument<1>::ValueType>::value,
                  "Second solver function argument must be equal to the return type");



public:
    template <Pivot _pivot>
    static void TestSolver(_solver solver);

private:
    static std::array<_type, _size * _size> GetTransposedMatrixData();

    static std::array<_type, _size> GetRhsData();

    static std::array<_type, _size> GetResultData();



    static void TestSolverTestcase(_solver solver, Matrix A, Vector b, Vector expRes);



    static void TestSolverResult(_solver solver);



    static void TestSolverSingularMatrix(_solver solver);



    static auto GetIdentityPermutations();


    static void TestSolverPivoting(_solver solver);
};

#include "test/unit/math/solver/solverTests.inl"
