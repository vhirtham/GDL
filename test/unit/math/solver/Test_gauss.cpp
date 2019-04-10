#include <boost/test/unit_test.hpp>


#include "gdl/math/sse/vecSSE.h"
#include "gdl/math/sse/matSSE.h"
#include "gdl/math/solver/gauss.h"
#include "test/tools/ExceptionChecks.h"

#include "test/tools/arrayValueComparison.h"


using namespace GDL;

template <typename _type>
void TestGaussDense()
{
    // clang-format off
    MatSSE<_type, 8, 8> A = MatSSE<_type, 8, 8>(2,  4, -3,  5,  9, -3, -7,  1,
                                            3, -5,  2, -1,  1,  1, -3,  1,
                                            9,  1, -7, -6,  7,  2,  5, -7,
                                            4, -1,  6, -2, -3, -8, -3,  9,
                                            7,  2,  7,  3,  9, -5, -3, -4,
                                           -5,  5, -5,  5,  5, -5,  5, -5,
                                            5, -6,  3, -3,  2,  1, -6,  5,
                                            1, -2, -3, -4, -5,  6, -7,  8).Transpose();
    // clang-format on

    VecSSE<_type, 8> b(7, -7, -8, 0, 6, 0, 4, -2);
    VecSSE<_type, 8> res = Solver::GaussPartialPivot(A, b);


    VecSSE<_type, 8> expResult(1, 2, 3, 4, 5, 6, 7, 8);

    BOOST_CHECK(CheckCloseArray(res.Data(), expResult.Data(), 100));
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense)
{
    TestGaussDense<F32>();
    TestGaussDense<F64>();
}
