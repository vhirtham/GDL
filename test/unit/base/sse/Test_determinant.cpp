#include <boost/test/unit_test.hpp>


#include "gdl/base/approx.h"
#include "gdl/base/sse/determinant.h"

#include <array>

using namespace GDL;
using namespace GDL::sse;

void TestDeterminant2x2(const std::array<F32, 4>& mat, F32 expectedResult)
{
    BOOST_CHECK(Determinant2x2(_mmx_setr_p<__m128>(mat[0], mat[1], mat[2], mat[3])) == Approx(expectedResult));

    BOOST_CHECK(Determinant2x2(_mmx_setr_p<__m128>(mat[0], mat[1], 0, 0), _mmx_setr_p<__m128>(mat[2], mat[3], 0, 0)) ==
                Approx(expectedResult));
}



BOOST_AUTO_TEST_CASE(Test_Determinant2x2)
{
    TestDeterminant2x2({{1, 2, 3, 4}}, -2);
    TestDeterminant2x2({{7, 8, 1, 5}}, 27);
    TestDeterminant2x2({{3, 6, 1, 2}}, 0);
}



void TestDeterminant3x3(const std::array<F32, 9>& mat, F32 expectedResult)
{
    __m128 col0 = _mmx_setr_p<__m128>(mat[0], mat[1], mat[2], 0);
    __m128 col1 = _mmx_setr_p<__m128>(mat[3], mat[4], mat[5], 0);
    __m128 col2 = _mmx_setr_p<__m128>(mat[6], mat[7], mat[8], 0);

    BOOST_CHECK(Determinant3x3(col0, col1, col2) == Approx(expectedResult));
}



BOOST_AUTO_TEST_CASE(Test_Determinant3x3)
{
    TestDeterminant3x3({{1, 2, 3, 4, 5, 6, 7, 8, 9}}, 0);
    TestDeterminant3x3({{5, 7, 1, 6, 3, 3, 9, 4, 5}}, -9);
    TestDeterminant3x3({{6, 2, 2, 7, 5, 1, 2, 1, 3}}, 40);
}



#ifdef __AVX2__
#endif // __AVX2__
