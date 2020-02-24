#include <boost/test/unit_test.hpp>


#include "gdl/base/simd/_transpose/transpose_m128.h"
#include "test/unit/base/simd/transpose_test_template_register.h"



// Transpose 1x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x1)
{
    TestTranspose<__m128, 1, 1>();
}



// Transpose 1x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x2)
{
    TestTranspose<__m128, 1, 2>();
}



// Transpose 1x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x3)
{
    TestTranspose<__m128, 1, 3>();
}



// Transpose 1x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x4)
{
    TestTranspose<__m128, 1, 4>();
}



// Transpose 2x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x1)
{
    TestTranspose<__m128, 2, 1>();
}



// Transpose 2x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x2)
{
    TestTranspose<__m128, 2, 2>();
}



// Transpose 2x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x3)
{
    TestTranspose<__m128, 2, 3>();
}



// Transpose 2x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x4)
{
    TestTranspose<__m128, 2, 4>();
}



// Transpose 3x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x1)
{
    TestTranspose<__m128, 3, 1>();
}



// Transpose 3x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x2)
{
    TestTranspose<__m128, 3, 2>();
}



// Transpose 3x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x3)
{
    TestTranspose<__m128, 3, 3>();
}



// Transpose 3x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose3x4)
{
    TestTranspose<__m128, 3, 4>();
}



// Transpose 4x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x1)
{
    TestTranspose<__m128, 4, 1>();
}



// Transpose 4x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x2)
{
    TestTranspose<__m128, 4, 2>();
}



// Transpose 4x3 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x3)
{
    TestTranspose<__m128, 4, 3>();
}



// Transpose 4x4 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose4x4)
{
    TestTranspose<__m128, 4, 4>();
}
