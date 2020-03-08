#include <boost/test/unit_test.hpp>


#include "gdl/base/simd/_transpose/transpose_m256.h"
#include "test/unit/base/simd/transpose_test_template_register.h"



#ifdef __AVX2__
/*
// Transpose 1x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x1)
{
    TestTranspose<__m256, 1, 1>();
}



// Transpose 1x2 ------------------------------------------------------------------------------------------------------



BOOST_AUTO_TEST_CASE(Test_transpose1x2)
{
    TestTranspose<__m256, 1, 2>();
}



// Transpose 1x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x3)
{
    TestTranspose<__m256, 1, 3>();
}



// Transpose 1x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x4)
{
    TestTranspose<__m256, 1, 4>();
}



// Transpose 1x5 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x5)
{
    TestTranspose<__m256, 1, 5>();
}



// Transpose 1x6 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x6)
{
    TestTranspose<__m256, 1, 6>();
}



// Transpose 1x7 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x7)
{
    TestTranspose<__m256, 1, 7>();
}



// Transpose 1x8 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x8)
{
    TestTranspose<__m256, 1, 8>();
}



// Transpose 2x1 ------------------------------------------------------------------------------------------------------



BOOST_AUTO_TEST_CASE(Test_transpose2x1)
{
    TestTranspose<__m256, 2, 1>();
}



// Transpose 2x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x2)
{
    TestTranspose<__m256, 2, 2>();
}



// Transpose 2x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x3)
{
    TestTranspose<__m256, 2, 3>();
}



// Transpose 2x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x4)
{
    TestTranspose<__m256, 2, 4>();
}



// Transpose 2x5 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x5)
{
    TestTranspose<__m256, 2, 5>();
}



// Transpose 2x6 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x6)
{
    TestTranspose<__m256, 2, 6>();
}



// Transpose 2x7 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x7)
{
    TestTranspose<__m256, 2, 7>();
}



// Transpose 2x8 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x8)
{
    TestTranspose<__m256, 2, 8>();
}



// Transpose 3x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x1)
{
    TestTranspose<__m256, 3, 1>();
}



// Transpose 3x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x2)
{
    TestTranspose<__m256, 3, 2>();
}



// Transpose 3x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x3)
{
    TestTranspose<__m256, 3, 3>();
}



// Transpose 3x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x4)
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
    TestTranspose<__m256, 4, 1>();
}



// Transpose 4x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x2)
{
    TestTranspose<__m256, 4, 2>();
}



// Transpose 4x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x3)
{
    TestTranspose<__m256, 4, 3>();
}



// Transpose 4x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x4)
{
    TestTranspose<__m256, 4, 4>();
}



// Transpose 4x5 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x5)
{
    TestTranspose<__m256, 4, 5>();
}



// Transpose 4x6 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x6)
{
    TestTranspose<__m256, 4, 6>();
}



// Transpose 4x7 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x7)
{
    TestTranspose<__m256, 4, 7>();
}



// Transpose 4x8 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x8)
{
    TestTranspose<__m256, 4, 8>();
}



// Transpose 5x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose5x1)
{
    TestTranspose<__m256, 5, 1>();
}



// Transpose 5x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose5x2)
{
    TestTranspose<__m256, 5, 2>();
}



// Transpose 5x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose5x3)
{
    TestTranspose<__m256, 5, 3>();
}

*/

// Transpose 5x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose5x4)
{
    TestTranspose<__m256, 5, 4>();
}

/*

// Transpose 5x5 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose5x5)
{
    TestTranspose<__m256, 5, 5>();
}



// Transpose 5x6 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose5x6)
{
    TestTranspose<__m256, 5, 6>();
}



// Transpose 5x7 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose5x7)
{
    TestTranspose<__m256, 5, 7>();
}



// Transpose 5x8 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose5x8)
{
    TestTranspose<__m256, 5, 8>();
}



// Transpose 6x1
// --------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose6x1)
{
    TestTranspose<__m256, 6, 1>();
}



// Transpose 6x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose6x2)
{
    TestTranspose<__m256, 6, 2>();
}



// Transpose 6x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose6x3)
{
    TestTranspose<__m256, 6, 3>();
}



// Transpose 6x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose6x4)
{
    TestTranspose<__m256, 6, 4>();
}



// Transpose 6x5 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose6x5)
{
    TestTranspose<__m256, 6, 5>();
}



// Transpose 6x6 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose6x6)
{
    TestTranspose<__m256, 6, 6>();
}



// Transpose 6x7 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose6x7)
{
    TestTranspose<__m256, 6, 7>();
}



// Transpose 6x8 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose6x8)
{
    TestTranspose<__m256, 6, 8>();
}



// Transpose 7x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose7x1)
{
    TestTranspose<__m256, 7, 1>();
}



// Transpose 7x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose7x2)
{
    TestTranspose<__m256, 7, 2>();
}



// Transpose 7x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose7x3)
{
    TestTranspose<__m256, 7, 3>();
}



// Transpose 7x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose7x4)
{
    TestTranspose<__m256, 7, 4>();
}



// Transpose 7x5 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose7x5)
{
    TestTranspose<__m256, 7, 5>();
}



// Transpose 7x6 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose7x6)
{
    TestTranspose<__m256, 7, 6>();
}



// Transpose 7x7 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose7x7)
{
    TestTranspose<__m256, 7, 7>();
}



// Transpose 7x8 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose7x8)
{
    TestTranspose<__m256, 7, 8>();
}



// Transpose 8x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose8x1)
{
    TestTranspose<__m256, 8, 1>();
}



// Transpose 8x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose8x2)
{
    TestTranspose<__m256, 8, 2>();
}



// Transpose 8x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose8x3)
{
    TestTranspose<__m256, 8, 3>();
}



// Transpose 8x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose8x4)
{
    TestTranspose<__m256, 8, 4>();
}



// Transpose 8x5 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose8x5)
{
    TestTranspose<__m256, 8, 5>();
}



// Transpose 8x6 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose8x6)
{
    TestTranspose<__m256, 8, 6>();
}



// Transpose 8x7 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose8x7)
{
    TestTranspose<__m256, 8, 7>();
}



// Transpose 8x8 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose8x8)
{
    TestTranspose<__m256, 8, 8>();
}



#else // __AVX2__

BOOST_AUTO_TEST_CASE(No_test)
{
}
*/
#endif // __AVX2__
