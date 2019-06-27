#include <boost/test/unit_test.hpp>


#include "gdl/base/approx.h"
#include "gdl/base/sse/determinant.h"

#include <array>

using namespace GDL;
using namespace GDL::simd;

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



void TestDeterminant4x4(const std::array<F32, 16>& mat, F32 expectedResult)
{
    __m128 col0 = _mmx_setr_p<__m128>(mat[0], mat[1], mat[2], mat[3]);
    __m128 col1 = _mmx_setr_p<__m128>(mat[4], mat[5], mat[6], mat[7]);
    __m128 col2 = _mmx_setr_p<__m128>(mat[8], mat[9], mat[10], mat[11]);
    __m128 col3 = _mmx_setr_p<__m128>(mat[12], mat[13], mat[14], mat[15]);

    BOOST_CHECK(Determinant4x4(col0, col1, col2, col3) == Approx(expectedResult));

#ifdef __AVX2__
    __m256 col01 = _mmx_setr_p<__m256>(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7]);
    __m256 col23 = _mmx_setr_p<__m256>(mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);

    BOOST_CHECK(Determinant4x4(col01, col23) == Approx(expectedResult));
#endif // __AVX2__
}



BOOST_AUTO_TEST_CASE(Test_Determinant4x4)
{
    TestDeterminant4x4({{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}}, 0);
    TestDeterminant4x4({{6, 7, 8, 3, 2, 5, 1, 1, 1, 9, 5, 8, 4, 6, 2, 7}}, 985);
    TestDeterminant4x4({{1, 9, 2, 3, 4, 8, 2, 1, 7, 1, 3, 1, 8, 9, 7, 9}}, -75);
}
