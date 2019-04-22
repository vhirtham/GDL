#include <boost/test/unit_test.hpp>


#include "gdl/math/sse/vecSSE.h"
#include "gdl/math/sse/matSSE.h"
#include "gdl/math/solver/gauss.h"
#include "test/tools/ExceptionChecks.h"

#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"



using namespace GDL;

// Helper classes -----------------------------------------------------------------------------------------------------


template <typename _type, U32 _size>
struct FixtureSerial
{
    using MatrixType = MatSerial<_type, _size, _size>;
    using VectorType = VecSerial<_type, _size>;
};



template <typename _type, U32 _size>
struct FixtureSSE
{
    using MatrixType = MatSSE<_type, _size, _size>;
    using VectorType = VecSSE<_type, _size>;
};



// Test pivoting ------------------------------------------------------------------------------------------------------

template <template <typename, U32> class _fixture, typename _type, U32 _size, bool _neg = false>
void TestGaussDensePivotingTestcase(std::array<U32, _size> indices)
{
    using MatrixType = typename _fixture<_type, _size>::MatrixType;
    using VectorType = typename _fixture<_type, _size>::VectorType;


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

    VectorType b(vectorValues);
    MatrixType A(matrixValues);

    VectorType res = Solver::GaussPartialPivot(A, b);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes, 1));

    if constexpr (!_neg)
        TestGaussDensePivotingTestcase<_fixture, _type, _size, true>(indices);
}



template <template <typename, U32> class _fixture, typename _type>
void TestGaussDensePivoting()
{
    TestGaussDensePivotingTestcase<_fixture, _type, 3>({{2, 0, 1}});
    TestGaussDensePivotingTestcase<_fixture, _type, 5>({{0, 4, 2, 1, 3}});
    TestGaussDensePivotingTestcase<_fixture, _type, 8>({{0, 1, 2, 3, 4, 5, 6, 7}});
    TestGaussDensePivotingTestcase<_fixture, _type, 8>({{1, 5, 4, 2, 0, 7, 6, 3}});
    TestGaussDensePivotingTestcase<_fixture, _type, 9>({{7, 2, 6, 4, 0, 8, 3, 5, 1}});
    TestGaussDensePivotingTestcase<_fixture, _type, 16>({{13, 5, 1, 11, 12, 7, 8, 0, 3, 14, 2, 6, 9, 15, 4, 10}});
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F32_Serial)
{
    TestGaussDensePivoting<FixtureSerial, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F64_Serial)
{
    TestGaussDensePivoting<FixtureSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F32_SSE)
{
    TestGaussDensePivoting<FixtureSSE, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F64_SSE)
{
    TestGaussDensePivoting<FixtureSSE, F64>();
}



// Test pivoting - no invalid pivot index -----------------------------------------------------------------------------


//! @brief Matrices with a size that is not a multiple of the number of register values have a register at the end of
//! each column which contains unused values. This test checks if high values in this unused memory locations are
//! ignored as expected.
template <typename _type, U32 _size>
void TestGaussDenseSSENoInvalidPivotIndexTestcase()
{
    using RegisterType = typename MatSSE<_type, _size, _size>::RegisterType;
    constexpr U32 numColRegisters = sse::CalcMinNumArrayRegisters<RegisterType>(_size);
    constexpr U32 numRegisterValues = sse::numRegisterValues<RegisterType>;

    std::array<_type, _size> expRes;
    std::array<_type, _size> vectorValues;
    std::array<RegisterType, numColRegisters * _size> matrixValues{{0}};



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
void TestGaussDenseSSENoInvalidPivotIndex()
{
    TestGaussDenseSSENoInvalidPivotIndexTestcase<_type, 3>();
    TestGaussDenseSSENoInvalidPivotIndexTestcase<_type, 5>();
    TestGaussDenseSSENoInvalidPivotIndexTestcase<_type, 7>();
    TestGaussDenseSSENoInvalidPivotIndexTestcase<_type, 9>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_No_Invalid_Pivot_Index_F32)
{
    TestGaussDenseSSENoInvalidPivotIndex<F32>();
}

BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_No_Invalid_Pivot_Index_F64)
{
    TestGaussDenseSSENoInvalidPivotIndex<F64>();
}



// Test for singular matrix -------------------------------------------------------------------------------------------

#ifndef NDEVEXCEPTION

template <template <typename, U32> class _fixture, typename _type>
void TestGaussDenseSingularMatrix()
{
    using MatrixType = typename _fixture<_type, 4>::MatrixType;
    using VectorType = typename _fixture<_type, 4>::VectorType;

    // clang-format off
    MatrixType A = MatrixType(0,  4, -5,  4,
                                                0, -5,  2, -1,
                                                0, -5,  2, -1,
                                                0, -1,  9, -5).Transpose();
    // clang-format on

    VectorType b(4, -5, -4, 9);
    VectorType res;
    BOOST_CHECK_THROW(res = Solver::GaussPartialPivot(A, b), Exception);
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Singular_F32_Serial)
{
    TestGaussDenseSingularMatrix<FixtureSerial, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Singular_F64_Serial)
{
    TestGaussDenseSingularMatrix<FixtureSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Singular_F32_SSE)
{
    TestGaussDenseSingularMatrix<FixtureSSE, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Singular_F64_SSE)
{
    TestGaussDenseSingularMatrix<FixtureSSE, F64>();
}

#endif



// Test 2x2 -----------------------------------------------------------------------------------------------------------

template <template <typename, U32> class _fixture, typename _type>
void TestGaussDense_2x2()
{
    using MatrixType = typename _fixture<_type, 2>::MatrixType;
    using VectorType = typename _fixture<_type, 2>::VectorType;


    // clang-format off
    MatrixType A = MatrixType( -5,  6,
                                3, -5).Transpose();
    // clang-format on

    VectorType b(7, -7);
    VectorType res = Solver::GaussPartialPivot(A, b);


    VectorType expRes(1, 2);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_2x2_F32_Serial)
{
    TestGaussDense_2x2<FixtureSerial, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_2x2_F64_Serial)
{
    TestGaussDense_2x2<FixtureSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_2x2_F32_SSE)
{
    TestGaussDense_2x2<FixtureSSE, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_2x2_F64_SSE)
{
    TestGaussDense_2x2<FixtureSSE, F64>();
}



// Test 3x3 -----------------------------------------------------------------------------------------------------------

template <template <typename, U32> class _fixture, typename _type>
void TestGaussDense_3x3()
{
    using MatrixType = typename _fixture<_type, 3>::MatrixType;
    using VectorType = typename _fixture<_type, 3>::VectorType;

    // clang-format off
    MatrixType A = MatrixType(-5,  6, -5,
                               3, -5,  2,
                               9,  8, -7).Transpose();
    // clang-format on

    VectorType b(-8, -1, 4);
    VectorType res = Solver::GaussPartialPivot(A, b);


    VectorType expRes(1, 2, 3);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_3x3_F32_Serial)
{
    TestGaussDense_3x3<FixtureSerial, F32>();
}


BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_3x3_F64_Serial)
{
    TestGaussDense_3x3<FixtureSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_3x3_F32_SSE)
{
    TestGaussDense_3x3<FixtureSSE, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_3x3_F64_SSE)
{
    TestGaussDense_3x3<FixtureSSE, F64>();
}



// Test 4x4 -----------------------------------------------------------------------------------------------------------

template <template <typename, U32> class _fixture, typename _type>
void TestGaussDense_4x4()
{
    using MatrixType = typename _fixture<_type, 4>::MatrixType;
    using VectorType = typename _fixture<_type, 4>::VectorType;

    // clang-format off
    MatrixType A = MatrixType(-5,  4, -5,  4,
                               3, -5,  2, -1,
                               9,  8, -7, -2,
                               4, -1,  9, -5).Transpose();
    // clang-format on

    VectorType b(4, -5, -4, 9);
    VectorType res = Solver::GaussPartialPivot(A, b);


    VectorType expRes(1, 2, 3, 4);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_4x4_F32_Serial)
{
    TestGaussDense_4x4<FixtureSerial, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_4x4_F64_Serial)
{
    TestGaussDense_4x4<FixtureSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_4x4_F32_SSE)
{
    TestGaussDense_4x4<FixtureSSE, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_4x4_F64_SSE)
{
    TestGaussDense_4x4<FixtureSSE, F64>();
}



// Test 5x5 -----------------------------------------------------------------------------------------------------------

template <template <typename, U32> class _fixture, typename _type>
void TestGaussDense_5x5()
{
    using MatrixType = typename _fixture<_type, 5>::MatrixType;
    using VectorType = typename _fixture<_type, 5>::VectorType;

    // clang-format off
    MatrixType A = MatrixType(5,  4, -3,  8, -9,
                              3, -5,  2, -1,  1,
                              9,  1, -7, -6,  7,
                              4, -1,  6, -2, -3,
                              7,  2,  7,  3, -9).Transpose();
    // clang-format on

    VectorType b(-9, 0, 1, -3, -1);
    VectorType res = Solver::GaussPartialPivot(A, b);


    VectorType expRes(1, 2, 3, 4, 5);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_5x5_F32_Serial)
{
    TestGaussDense_5x5<FixtureSerial, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_5x5_F64_Serial)
{
    TestGaussDense_5x5<FixtureSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_5x5_F32_SSE)
{
    TestGaussDense_5x5<FixtureSSE, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_5x5_F64_SSE)
{
    TestGaussDense_5x5<FixtureSSE, F64>();
}



// Test 7x7 -----------------------------------------------------------------------------------------------------------

template <template <typename, U32> class _fixture, typename _type>
void TestGaussDense_7x7()
{
    using MatrixType = typename _fixture<_type, 7>::MatrixType;
    using VectorType = typename _fixture<_type, 7>::VectorType;

    // clang-format off
    MatrixType A = MatrixType(2,  4, -3,  5,  9, -3, -7,
                              3, -5,  2, -1,  1,  1, -2,
                              9,  1, -7, -6,  7,  2, -2,
                              4, -1,  6, -2, -3, -2,  1,
                              7,  2,  7,  3, -9, -5,  4,
                             -5,  5, -5,  5,  5, -6,  1,
                              5, -6,  3, -3,  6,  3, -6).Transpose();
    // clang-format on

    VectorType b(-1, -8, -1, -8, -3, 6, -4);
    VectorType res = Solver::GaussPartialPivot(A, b);


    VectorType expRes(1, 2, 3, 4, 5, 6, 7);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_7x7_F32_Serial)
{
    TestGaussDense_7x7<FixtureSerial, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_7x7_F64_Serial)
{
    TestGaussDense_7x7<FixtureSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_7x7_F32_SSE)
{
    TestGaussDense_7x7<FixtureSSE, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_7x7_F64_SSE)
{
    TestGaussDense_7x7<FixtureSSE, F64>();
}



// Test 8x8 -----------------------------------------------------------------------------------------------------------

template <template <typename, U32> class _fixture, typename _type>
void TestGaussDense_8x8()
{
    using MatrixType = typename _fixture<_type, 8>::MatrixType;
    using VectorType = typename _fixture<_type, 8>::VectorType;

    // clang-format off
    MatrixType A = MatrixType(2,  4, -3,  5,  9, -3, -7,  1,
                              3, -5,  2, -1,  1,  1, -3,  1,
                              9,  1, -7, -6,  7,  2,  5, -7,
                              4, -1,  6, -2, -3, -8, -3,  9,
                              7,  2,  7,  3,  9, -5, -3, -4,
                             -5,  5, -5,  5,  5, -5,  5, -5,
                              5, -6,  3, -3,  2,  1, -6,  5,
                              1, -2, -3, -4, -5,  6, -7,  8).Transpose();
    // clang-format on

    VectorType b(7, -7, -8, 0, 6, 0, 4, -2);
    VectorType res = Solver::GaussPartialPivot(A, b);


    VectorType expRes(1, 2, 3, 4, 5, 6, 7, 8);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_8x8_F32_Serial)
{
    TestGaussDense_8x8<FixtureSerial, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_8x8_F64_Serial)
{
    TestGaussDense_8x8<FixtureSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_8x8_F32_SSE)
{
    TestGaussDense_8x8<FixtureSSE, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_8x8_F64_SSE)
{
    TestGaussDense_8x8<FixtureSSE, F64>();
}



// Test 9x9 -----------------------------------------------------------------------------------------------------------

template <template <typename, U32> class _fixture, typename _type>
void TestGaussDense_9x9()
{
    using MatrixType = typename _fixture<_type, 9>::MatrixType;
    using VectorType = typename _fixture<_type, 9>::VectorType;

    // clang-format off
    MatrixType A = MatrixType(-2,  4, -3,  5,  9, -1, -7,  1, -2,
                               3, -6,  2, -1,  1,  1, -3,  1,  2,
                               6,  1, -7, -6, -7,  4,  5, -7,  7,
                               4, -1,  6, -2, -3,  8, -4,  9, -9,
                               7,  2, -7,  3,  9, -5, -3, -4,  5,
                              -5,  9, -5,  7,  5, -5,  8, -5, -4,
                               5, -6,  5, -3,  2,  1, -6,  5, -2,
                              -9, -2, -5, -4, -5,  6,  7, -8,  6,
                               4,  5,  2, -7,  1,  8, -2,  3, -7).Transpose();
    // clang-format on

    VectorType b(-3, 9, -6, 8, 9, 1, -8, 6, -8);
    VectorType res = Solver::GaussPartialPivot(A, b);


    VectorType expRes(1, 2, 3, 4, 5, 6, 7, 8, 9);

    BOOST_CHECK(CheckCloseArray(res.Data(), expRes.Data(), 100));
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_9x9_F32_Serial)
{
    TestGaussDense_9x9<FixtureSerial, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_9x9_F64_Serial)
{
    TestGaussDense_9x9<FixtureSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_9x9_F32_SSE)
{
    TestGaussDense_9x9<FixtureSSE, F32>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_9x9_F64_SSE)
{
    TestGaussDense_9x9<FixtureSSE, F64>();
}
