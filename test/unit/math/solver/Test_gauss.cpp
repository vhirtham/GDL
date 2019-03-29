#include <boost/test/unit_test.hpp>


#include "gdl/math/sse/vecSSE.h"
#include "gdl/math/sse/matSSE.h"
#include "gdl/math/solver/gauss.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;

BOOST_AUTO_TEST_CASE(TestGaussDense)
{
    // clang-format off
    MatSSE<F32, 8, 8> A = MatSSE<F32, 8, 8>(1, 7, 1, 1, 1, 1, 1, 8,
                                            3, 3, 5, 9, 1, 0, 8, 9,
                                            9, 2, 1, 2, 8, 0, 4, 7,
                                            4, 9, 4, 2, 8, 3, 6, 1,
                                            2, 2, 3, 3, 3, 7, 7, 9,
                                            6, 8, 6, 4, 7, 9, 1, 2,
                                            4, 0, 6, 0, 5, 1, 9, 4,
                                            5, 2, 1, 9, 0, 7, 3, 8).Transpose();
    // clang-format on

    VecSSE<F32, 8> b(1, 2, 3, 4, 5, 6, 7, 8);
    Solver::GaussPartialPivot(A, b);
}
