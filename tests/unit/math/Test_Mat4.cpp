#include <boost/test/unit_test.hpp>

#include "GDLTypedefs.h"
#include "math/mat4SIMD.inl"
#include "math/mat4Single.inl"

#include <cmath>

using namespace GDL;

template <typename T>
bool checkClose(T a, T b, F32 tolerance = 1e-6)
{
    for (U32 i = 0; i < 4; ++i)
        for (U32 j = 0; j < 4; ++j)
            if (std::fabs(a(i, j) - b(i, j)) > tolerance)
                return false;
    return true;
}

template <typename matrix>
void MultiplicationTest()
{
    matrix A(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    matrix B(8, 5, 3, 6, 11, 4, 7, 8, 6, 9, 5, 2, 2, 3, 2, 6);

    auto C = A * B;
    matrix expC(29, 31, 23, 30, 137, 115, 91, 118, 245, 199, 159, 206, 353, 283, 227, 294);
    BOOST_CHECK(checkClose(C, expC));

    auto D = B * A;
    matrix expD(116, 138, 160, 182, 168, 198, 228, 258, 100, 122, 144, 166, 100, 113, 126, 139);
    BOOST_CHECK(checkClose(D, expD));
}

BOOST_AUTO_TEST_CASE(Multiplication_Single)
{
    MultiplicationTest<mat4Single<F32>>();
}


BOOST_AUTO_TEST_CASE(Multiplication_SIMD)
{
    MultiplicationTest<mat4SIMD>();
}
