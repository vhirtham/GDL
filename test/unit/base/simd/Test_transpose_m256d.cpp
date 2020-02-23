#include <boost/test/unit_test.hpp>


#include "gdl/base/simd/transpose.h"
#include "test/unit/base/simd/transpose_test_template_register.h"



#ifdef __AVX2__

// Transpose 1x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x1)
{
    TestTranspose<__m256d, 1, 1>();
}



// Transpose 1x2 ------------------------------------------------------------------------------------------------------



BOOST_AUTO_TEST_CASE(Test_transpose1x2)
{
    TestTranspose<__m256d, 1, 2>();
}



// Transpose 1x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x3)
{
    TestTranspose<__m256d, 1, 3>();
}



// Transpose 1x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x4)
{
    TestTranspose<__m256d, 1, 4>();
}



// Transpose 2x1 ------------------------------------------------------------------------------------------------------



BOOST_AUTO_TEST_CASE(Test_transpose2x1)
{
    TestTranspose<__m256d, 2, 1>();
}



// Transpose 2x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x2)
{
    TestTranspose<__m256d, 2, 2>();
}



// Transpose 2x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x3)
{
    TestTranspose<__m256d, 2, 3>();
}



// Transpose 2x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x4)
{
    TestTranspose<__m256d, 2, 4>();
}



// Transpose 3x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x1)
{
    TestTranspose<__m256d, 3, 1>();
}



// Transpose 3x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x2)
{
    TestTranspose<__m256d, 3, 2>();
}



// Transpose 3x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x3)
{
    TestTranspose<__m256d, 3, 3>();
}



// Transpose 3x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x4)
{
    TestTranspose<__m256d, 3, 4>();
}



BOOST_AUTO_TEST_CASE(Transpose3x4_256d)
{
    TestTranspose<__m256d, 3, 4>();
}



BOOST_AUTO_TEST_CASE(Transpose3x4_256)
{
    TestTranspose<__m256, 3, 4>();
}



// Transpose 3x5 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Transpose3x5_256)
{
    TestTranspose<__m256, 3, 5>();
}



// Transpose 3x6 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Transpose3x6_256)
{
    TestTranspose<__m256, 3, 6>();
}



// Transpose 3x7 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Transpose3x7_256)
{
    TestTranspose<__m256, 3, 7>();
}



// Transpose 3x8 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Transpose3x8_256)
{
    TestTranspose<__m256, 3, 8>();
}



// Transpose 4x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x1)
{
    TestTranspose<__m256d, 4, 1>();
}



// Transpose 4x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x2)
{
    TestTranspose<__m256d, 4, 2>();
}



// Transpose 4x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x3)
{
    TestTranspose<__m256d, 4, 3>();
}



// Transpose 4x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x4)
{
    TestTranspose<__m256d, 4, 4>();
}



#else // __AVX2__

BOOST_AUTO_TEST_CASE(No_test)
{
}

#endif // __AVX2__
