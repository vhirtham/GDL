#include <boost/test/unit_test.hpp>


#include "gdl/base/simd/_transpose/transpose_m128d.h"
#include "test/unit/base/simd/transpose_test_template_register.h"



// Transpose 1x1 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x1)
{
    TestTranspose<__m128d, 1, 1>();
}



// Transpose 1x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose1x2)
{
    TestTranspose<__m128d, 1, 2>();
}



// Transpose 2x1 ------------------------------------------------------------------------------------------------------


BOOST_AUTO_TEST_CASE(Test_transpose2x1)
{
    TestTranspose<__m128d, 2, 1>();
}



// Transpose 2x2 ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_transpose2x2)
{
    TestTranspose<__m128d, 2, 2>();
}
