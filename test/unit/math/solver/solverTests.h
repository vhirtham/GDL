#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functionTraits.h"
#include "gdl/math/serial/vecSerial.h"
#include "gdl/math/solver/pivotEnum.h"



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
    //! @brief Runs all tests for the solver
    //! @tparam _pivot: Pivoting strategy
    //! @param solver: Solver
    template <Pivot _pivot>
    static void RunTests(_solver solver);

private:
    //! @param Gets multiple arrays of permutations
    //! @return Arrays with permutations
    static auto GetPermutationIndices();

    //! @brief Gets the expected result vector data depending on the system size
    //! @return Expeted result vector data
    static std::array<_type, _size> GetResultData();

    //! @brief Gets rhs vector data depending on the system size
    //! @return Rhs Vector data
    static std::array<_type, _size> GetRhsData();

    //! @brief Gets the Matrix data depending on the system size
    //! @return Matrix data
    static std::array<_type, _size * _size> GetTransposedMatrixData();

    //! @brief Solves the passed system with the passed solver and checks the result
    //! @param solver: Solver
    //! @param A: Matrix
    //! @param r: Right-hand side vector
    //! @param expRes: Expected result
    static void SolveAndCheckResult(_solver solver, Matrix A, Vector r, Vector expRes);

    //! @brief Tests if pivoting is really turned off
    //! @param solver: Solver
    static void TestNoPivoting(_solver solver);

    //! @brief Tests if the pivoting strategy prevents zeros on the main diagonal.
    //! @param solver: Solver
    static void TestPivoting(_solver solver);

    //! @brief Solves a size dependent system and checks the result for correctness
    //! @param solver: Solver
    static void TestSolve(_solver solver);

    //! @brief Checks if unused register values are ignored during the pivoting step.
    //! @param solver: Solver
    static void TestSIMDPivotingUnusedValues(_solver solver);

    //! @brief Checks if a singular matrix exception causes an exception
    //! @param solver: Solver
    static void TestSingularMatrixException(_solver solver);
};

#include "test/unit/math/solver/solverTests.inl"
