#include <boost/test/unit_test.hpp>


#include "gdl/math/sse/vecSSE.h"
#include "gdl/math/sse/matSSE.h"
#include "gdl/math/solver/gauss.h"
#include "test/tools/ExceptionChecks.h"

#include "test/tools/arrayValueComparison.h"


using namespace GDL;


//! Check throw if matrix is singular


// Test pivoting ------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, bool _neg = false>
void TestGaussDensePivotingTestcase(std::array<U32, _size> indices)
{

    for (U32 i = 0; i < _size; ++i)
    {
        if (indices[i] >= _size)
            THROW("Testcase invalid. Index must be in the range [0, system size]");
        for (U32 j = i + 1; j < _size; ++j)
            if (indices[i] == indices[j])
                THROW("Testcase invalid. Value found multiple times");
    }

    std::array<_type, _size> expRes;
    std::array<_type, _size> vectorValues;
    std::array<_type, _size * _size> matrixValues;

    _type posNeg = (_neg) ? -1. : 1.;

    for (U32 i = 0; i < _size; ++i)
    {
        expRes[i] = static_cast<_type>(i);
        vectorValues[i] = static_cast<_type>(indices[i]) * posNeg;
        for (U32 j = 0; j < _size; ++j)
            if (indices[i] == j)
                matrixValues[j * _size + i] = 1 * posNeg;
            else
                matrixValues[j * _size + i] = 0;
    }

    VecSSE<_type, _size> b(vectorValues);
    MatSSE<_type, _size, _size> A(matrixValues);

    VecSSE<_type, _size> res = Solver::GaussPartialPivot(A, b);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes, 1));

    if constexpr (!_neg)
        TestGaussDensePivotingTestcase<_type, _size, true>(indices);
}



template <typename _type>
void TestGaussDensePivoting()
{
    TestGaussDensePivotingTestcase<_type, 3>({{2, 0, 1}});
    TestGaussDensePivotingTestcase<_type, 5>({{0, 4, 2, 1, 3}});
    TestGaussDensePivotingTestcase<_type, 8>({{0, 1, 2, 3, 4, 5, 6, 7}});
    TestGaussDensePivotingTestcase<_type, 8>({{1, 5, 4, 2, 0, 7, 6, 3}});
    TestGaussDensePivotingTestcase<_type, 9>({{7, 2, 6, 4, 0, 8, 3, 5, 1}});
    TestGaussDensePivotingTestcase<_type, 16>({{13, 5, 1, 11, 12, 7, 8, 0, 3, 14, 2, 6, 9, 15, 4, 10}});
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F32)
{
    TestGaussDensePivoting<F32>();
}

BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F64)
{
    TestGaussDensePivoting<F64>();
}



// Test pivoting - no invalid pivot index -----------------------------------------------------------------------------


//! @brief Matrices with a size that is not a multiple of the number of register values have a register at the end of
//! each column which contains unused values. This test checks if high values in this unused memory locations are
//! ignored as expected.
template <typename _type, U32 _size>
void TestGaussDenseNoInvalidPivotIndexTestcase()
{
    using RegisterType = typename MatSSE<_type, _size, _size>::RegisterType;
    constexpr U32 numColRegisters = sse::CalcMinNumArrayRegisters<RegisterType>(_size);
    constexpr U32 numRegisterValues = sse::numRegisterValues<RegisterType>;

    std::array<_type, _size> expRes;
    std::array<_type, _size> vectorValues;
    std::array<RegisterType, numColRegisters * _size> matrixValues;



    for (U32 i = 0; i < _size; ++i)
    {
        expRes[i] = static_cast<_type>(i);
        vectorValues[i] = static_cast<_type>(i);
        for (U32 j = 0; j < numColRegisters; ++j)
            for (U32 k = 0; k < numRegisterValues; ++k)
            {
                U32 globalColIdx = j * numRegisterValues + k;
                if (globalColIdx == i)
                    sse::SetValue(matrixValues[i * numColRegisters + j], k, 1);
                else if (globalColIdx < _size)
                    sse::SetValue(matrixValues[i * numColRegisters + j], k, 0);
                else
                    sse::SetValue(matrixValues[i * numColRegisters + j], k, 500);
            }
    }


    VecSSE<_type, _size> b(vectorValues);
    MatSSE<_type, _size, _size> A(matrixValues);

    for (U32 i = 0; i < _size; ++i)
        for (U32 j = 0; j < numColRegisters; ++j)
            for (U32 k = 0; k < numRegisterValues; ++k)
                if (j * numRegisterValues + k >= _size)
                    EXCEPTION(sse::GetValue(matrixValues[i * numColRegisters + j], k) != Approx<_type>(500),
                              "Testcase invalid. Unused memory of matrix is not set as expected.");



    VecSSE<_type, _size> res = Solver::GaussPartialPivot(A, b);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes, 1));
}



template <typename _type>
void TestGaussDenseNoInvalidPivotIndex()
{
    TestGaussDenseNoInvalidPivotIndexTestcase<_type, 3>();
    TestGaussDenseNoInvalidPivotIndexTestcase<_type, 5>();
    TestGaussDenseNoInvalidPivotIndexTestcase<_type, 7>();
    TestGaussDenseNoInvalidPivotIndexTestcase<_type, 9>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_No_Invalid_Pivot_Index_F32)
{
    TestGaussDenseNoInvalidPivotIndex<F32>();
}

BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_No_Invalid_Pivot_Index_F64)
{
    TestGaussDenseNoInvalidPivotIndex<F64>();
}


// Test 2x2 -----------------------------------------------------------------------------------------------------------

template <typename _type>
void TestGaussDense_2x2()
{
    // clang-format off
    MatSSE<_type, 2, 2> A = MatSSE<_type, 2, 2>( -5,  6,
                                                  3, -5).Transpose();
    // clang-format on

    VecSSE<_type, 2> b(7, -7);
    VecSSE<_type, 2> res = Solver::GaussPartialPivot(A, b);


    VecSSE<_type, 2> expRes(1, 2);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_2x2_F32)
{
    TestGaussDense_2x2<F32>();
}

BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_2x2_F64)
{
    TestGaussDense_2x2<F64>();
}

// Test 3x3 -----------------------------------------------------------------------------------------------------------

template <typename _type>
void TestGaussDense_3x3()
{
    // clang-format off
    MatSSE<_type, 3, 3> A = MatSSE<_type, 3, 3>(-5,  6, -5,
                                                 3, -5,  2,
                                                 9,  8, -7).Transpose();
    // clang-format on

    VecSSE<_type, 3> b(-8, -1, 4);
    VecSSE<_type, 3> res = Solver::GaussPartialPivot(A, b);


    VecSSE<_type, 3> expRes(1, 2, 3);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_3x3_F32)
{
    TestGaussDense_3x3<F32>();
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_3x3_F64)
{
    TestGaussDense_3x3<F64>();
}


// Test 4x4 -----------------------------------------------------------------------------------------------------------

template <typename _type>
void TestGaussDense_4x4()
{
    // clang-format off
    MatSSE<_type, 4, 4> A = MatSSE<_type, 4, 4>(-5,  4, -5,  4,
                                                 3, -5,  2, -1,
                                                 9,  8, -7, -2,
                                                 4, -1,  9, -5).Transpose();
    // clang-format on

    VecSSE<_type, 4> b(4, -5, -4, 9);
    VecSSE<_type, 4> res = Solver::GaussPartialPivot(A, b);


    VecSSE<_type, 4> expRes(1, 2, 3, 4);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_4x4_F32)
{
    TestGaussDense_4x4<F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_4x4_F64)
{
    TestGaussDense_4x4<F64>();
}



// Test 5x5 -----------------------------------------------------------------------------------------------------------

template <typename _type>
void TestGaussDense_5x5()
{
    // clang-format off
    MatSSE<_type, 5, 5> A = MatSSE<_type, 5, 5>(5,  4, -3,  8, -9,
                                                3, -5,  2, -1,  1,
                                                9,  1, -7, -6,  7,
                                                4, -1,  6, -2, -3,
                                                7,  2,  7,  3, -9).Transpose();
    // clang-format on

    VecSSE<_type, 5> b(-9, 0, 1, -3, -1);
    VecSSE<_type, 5> res = Solver::GaussPartialPivot(A, b);


    VecSSE<_type, 5> expRes(1, 2, 3, 4, 5);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_5x5_F32)
{
    TestGaussDense_5x5<F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_5x5_F64)
{
    TestGaussDense_5x5<F64>();
}


// Test 7x7 -----------------------------------------------------------------------------------------------------------

template <typename _type>
void TestGaussDense_7x7()
{
    // clang-format off
    MatSSE<_type, 7, 7> A = MatSSE<_type, 7, 7>(2,  4, -3,  5,  9, -3, -7,
                                                3, -5,  2, -1,  1,  1, -2,
                                                9,  1, -7, -6,  7,  2, -2,
                                                4, -1,  6, -2, -3, -2,  1,
                                                7,  2,  7,  3, -9, -5,  4,
                                               -5,  5, -5,  5,  5, -6,  1,
                                                5, -6,  3, -3,  6,  3, -6).Transpose();
    // clang-format on

    VecSSE<_type, 7> b(-1, -8, -1, -8, -3, 6, -4);
    VecSSE<_type, 7> res = Solver::GaussPartialPivot(A, b);


    VecSSE<_type, 7> expRes(1, 2, 3, 4, 5, 6, 7);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_7x7_F32)
{
    TestGaussDense_7x7<F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_7x7_F64)
{
    TestGaussDense_7x7<F64>();
}



// Test 8x8 -----------------------------------------------------------------------------------------------------------

template <typename _type>
void TestGaussDense_8x8()
{
    // clang-format off
    MatSSE<_type, 8, 8> A = MatSSE<_type, 8, 8>(2,  4, -3,  5,  9, -3, -7,  1,
                                                3, -5,  2, -1,  1,  1, -3,  1,
                                                9,  1, -7, -6,  7,  2,  5, -7,
                                                4, -1,  6, -2, -3, -8, -3,  9,
                                                7,  2,  7,  3,  9, -5, -3, -4,
                                               -5,  5, -5,  5,  5, -5,  5, -5,
                                                5, -6,  3, -3,  2,  1, -6,  5,
                                                1, -2, -3, -4, -5,  6, -7,  8).Transpose();
    // clang-format on

    VecSSE<_type, 8> b(7, -7, -8, 0, 6, 0, 4, -2);
    VecSSE<_type, 8> res = Solver::GaussPartialPivot(A, b);


    VecSSE<_type, 8> expRes(1, 2, 3, 4, 5, 6, 7, 8);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_8x8_F32)
{
    TestGaussDense_8x8<F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_8x8_F64)
{
    TestGaussDense_8x8<F64>();
}


// Test 9x9 -----------------------------------------------------------------------------------------------------------

template <typename _type>
void TestGaussDense_9x9()
{
    // clang-format off
    MatSSE<_type, 9, 9> A = MatSSE<_type, 9, 9>(-2,  4, -3,  5,  9, -1, -7,  1, -2,
                                                 3, -6,  2, -1,  1,  1, -3,  1,  2,
                                                 6,  1, -7, -6, -7,  4,  5, -7,  7,
                                                 4, -1,  6, -2, -3,  8, -4,  9, -9,
                                                 7,  2, -7,  3,  9, -5, -3, -4,  5,
                                                -5,  9, -5,  7,  5, -5,  8, -5, -4,
                                                 5, -6,  5, -3,  2,  1, -6,  5, -2,
                                                -9, -2, -5, -4, -5,  6,  7, -8,  6,
                                                 4,  5,  2, -7,  1,  8, -2,  3, -7).Transpose();
    // clang-format on

    VecSSE<_type, 9> b(-3, 9, -6, 8, 9, 1, -8, 6, -8);
    VecSSE<_type, 9> res = Solver::GaussPartialPivot(A, b);


    VecSSE<_type, 9> expRes(1, 2, 3, 4, 5, 6, 7, 8, 9);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_9x9_F32)
{
    TestGaussDense_9x9<F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_9x9_F64)
{
    TestGaussDense_9x9<F64>();
}
