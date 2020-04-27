#include <boost/test/unit_test.hpp>


#include "test/unit/base/simd/transposeTest.h"



// Necessary tests:
// - all branches - correct function selected -> test always 3 sizes with differen settings of the bool template
// parameters
// - strides and offsets together -> just single test without iteration, for example: offset 2, stride 2



// array offsets ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ArrayOffsets_128d)
{
    TestArrayOffset<__m128d, 2, 1>();
}



BOOST_AUTO_TEST_CASE(ArrayOffsets_128)
{
    TestArrayOffset<__m128, 2, 4>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(ArrayOffsets_256d)
{
    TestArrayOffset<__m256d, 3, 4>();
}



BOOST_AUTO_TEST_CASE(ArrayOffsets_256)
{
    TestArrayOffset<__m256, 5, 7>();
}

#endif //__AVX2__



// array strides ------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ArrayStrides_128d)
{
    TestArrayStride<__m128d, 2, 2>();
}



BOOST_AUTO_TEST_CASE(ArrayStrides_128)
{
    TestArrayStride<__m128, 3, 4>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(ArrayStrides_256d)
{
    TestArrayStride<__m256d, 3, 4>();
}



BOOST_AUTO_TEST_CASE(ArrayStrides_256)
{
    TestArrayStride<__m256, 5, 7>();
}

#endif //__AVX2__
