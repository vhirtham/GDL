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
    matrix A{0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.};
    matrix B{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2., 6.};

    auto C = A * B;
    matrix expC{29., 137., 245., 353., 31., 115., 199., 283., 23., 91., 159., 227., 30., 118., 206., 294.};
    BOOST_CHECK(checkClose(C, expC));

    auto D = B * A;
    matrix expD{116., 168., 100., 100., 138., 198., 122., 113., 160., 228., 144., 126., 182., 258., 166., 139.};
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
