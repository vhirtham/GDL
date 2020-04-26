#include <boost/test/unit_test.hpp>


#include "test/unit/base/simd/transposeTest.h"



// Necessary tests:
// - all branches - correct function selected -> test always 3 sizes with differen settings of the bool template
// parameters
// - different array sizes and offsets - in and output registers are selected correctly
// - strides in input and output columns
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



// Array sizes --------------------------------------------------------------------------------------------------------

// template <typename _registerType,
// U32 _rows,
// U32 _cols,
// U32 _stepRowIn = 1,
// U32 _stepRowOut = 1,
// U32 _stepColIn = 0,
// U32 _stepColOut = 0,
// U32 _colStrideIn = 1,
// U32 _colStrideOut = 1,
// U32 _stepArrSizeIn = 0,
// U32 _stepArrSizeOut = 0,
// std::size_t _arrSizeIn = _cols,
// std::size_t _arrSizeOut = _rows>

// BOOST_AUTO_TEST_CASE(ArraySizes_128d)
//{
//    TestTranspose<__m128d, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1>();
//}



// BOOST_AUTO_TEST_CASE(ArraySizes_128)
//{
//    TestTranspose<__m128, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1>();
//}



//#ifdef __AVX2__

// BOOST_AUTO_TEST_CASE(ArraySizes_256d)
//{
//    TestTranspose<__m256d, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1>();
//}



// BOOST_AUTO_TEST_CASE(ArraySizes_256)
//{
//    TestTranspose<__m256, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1>();
//}

//#endif //__AVX2__



//// Column offsets
///-----------------------------------------------------------------------------------------------------

// BOOST_AUTO_TEST_CASE(ColumnOffset_128d)
//{
//    TestTranspose<__m128d, 1, 1, 0, 0, 3, 3, 1, 1, 0, 0, 4, 4>();
//}



// BOOST_AUTO_TEST_CASE(ColumnOffset_128)
//{
//    TestTranspose<__m128, 1, 1, 0, 0, 5, 5, 1, 1, 0, 0, 16, 16>();
//}



//#ifdef __AVX2__

// BOOST_AUTO_TEST_CASE(ColumnOffset_256d)
//{
//    TestTranspose<__m256d, 1, 1, 0, 0, 5, 5, 1, 1, 0, 0, 16, 16>();
//}



// BOOST_AUTO_TEST_CASE(ColumnOffset_256)
//{
//    TestTranspose<__m256, 1, 1, 0, 0, 9, 9, 1, 1, 0, 0, 64, 64>();
//}

//#endif //__AVX2__



//// Column strides
///-----------------------------------------------------------------------------------------------------

// BOOST_AUTO_TEST_CASE(ColumnStrides_128d)
//{
//    TestTranspose<__m128d, 2, 2, 0, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
//}



// BOOST_AUTO_TEST_CASE(ColumnStrides_128)
//{
//    TestTranspose<__m128, 2, 2, 0, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
//}



//#ifdef __AVX2__

// BOOST_AUTO_TEST_CASE(ColumnStrides_256d)
//{
//    TestTranspose<__m256d, 2, 2, 0, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
//}



// BOOST_AUTO_TEST_CASE(ColumnStrides_256)
//{
//    TestTranspose<__m256, 2, 2, 0, 0, 1, 1, 2, 2, 0, 0, 4, 4>();
//}

//#endif //__AVX2__
