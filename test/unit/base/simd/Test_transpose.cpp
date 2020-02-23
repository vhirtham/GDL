#include <boost/test/unit_test.hpp>


#include "test/unit/base/simd/transposeTest.h"



// Array sizes --------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ArraySizes_128d)
{
    TestTranspose<__m128d, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1>();
}



BOOST_AUTO_TEST_CASE(ArraySizes_128)
{
    TestTranspose<__m128, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(ArraySizes_256d)
{
    TestTranspose<__m256d, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1>();
}



BOOST_AUTO_TEST_CASE(ArraySizes_256)
{
    TestTranspose<__m256, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1>();
}

#endif //__AVX2__



// Column offsets -----------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ColumnOffset_128d)
{
    TestTranspose<__m128d, 1, 1, 0, 0, 3, 3, 1, 1, 0, 0, 4, 4>();
}



BOOST_AUTO_TEST_CASE(ColumnOffset_128)
{
    TestTranspose<__m128, 1, 1, 0, 0, 5, 5, 1, 1, 0, 0, 16, 16>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(ColumnOffset_256d)
{
    TestTranspose<__m256d, 1, 1, 0, 0, 5, 5, 1, 1, 0, 0, 16, 16>();
}



BOOST_AUTO_TEST_CASE(ColumnOffset_256)
{
    TestTranspose<__m256, 1, 1, 0, 0, 9, 9, 1, 1, 0, 0, 64, 64>();
}

#endif //__AVX2__



// Column strides -----------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ColumnStrides_128d)
{
    TestTranspose<__m128d, 2, 2, 0, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
}



BOOST_AUTO_TEST_CASE(ColumnStrides_128)
{
    TestTranspose<__m128, 2, 2, 0, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
}



#ifdef __AVX2__

BOOST_AUTO_TEST_CASE(ColumnStrides_256d)
{
    TestTranspose<__m256d, 2, 2, 0, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
}



BOOST_AUTO_TEST_CASE(ColumnStrides_256)
{
    TestTranspose<__m256, 2, 2, 0, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
}

#endif //__AVX2__
