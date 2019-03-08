#include <boost/test/unit_test.hpp>


#include "gdl/math/sse/vecSSE.h"
#include "gdl/math/sse/matSSE.h"
#include "gdl/math/solver/gauss.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;

BOOST_AUTO_TEST_CASE(TestGaussDense)
{
    // clang-format off
    MatSSE<F32, 8, 8> A = MatSSE<F32, 8, 8>(1, 1, 1, 1, 1, 1, 1, 8,
                                            2, 2, 3, 4, 5, 6, 7, 8,
                                            3, 2, 3, 4, 5, 6, 7, 8,
                                            4, 3, 3, 4, 5, 6, 7, 8,
                                            5, 2, 3, 4, 5, 6, 7, 8,
                                            6, 2, 3, 4, 5, 6, 7, 8,
                                            7, 2, 3, 4, 5, 6, 7, 8,
                                            8, 8, 8, 8, 8, 8, 8, 8).Transpose();
    // clang-format on

    VecSSE<F32, 8> b(1, 2, 3, 4, 5, 6, 7, 8);
    Solver::GaussPartialPivot(A, b);
}
