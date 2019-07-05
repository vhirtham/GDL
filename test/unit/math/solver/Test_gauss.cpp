#include <boost/test/unit_test.hpp>

#include "test/unit/math/solver/solverTests.h"


#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/solver/gauss.h"


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
    using MatrixType = MatSIMD<_type, _size, _size>;
    using VectorType = VecSIMD<_type, _size>;
};



//// Test pivoting
///------------------------------------------------------------------------------------------------------

// template <template <typename, U32> class _fixture, typename _type, U32 _size, bool _neg = false>
// void TestGaussDensePivotingTestcase(std::array<U32, _size> indices)
//{
//    using MatrixType = typename _fixture<_type, _size>::MatrixType;
//    using VectorType = typename _fixture<_type, _size>::VectorType;


//    for (U32 i = 0; i < _size; ++i)
//    {
//        if (indices[i] >= _size)
//            THROW("Testcase invalid. Index must be in the range [0, system size]");
//        for (U32 j = i + 1; j < _size; ++j)
//            if (indices[i] == indices[j])
//                THROW("Testcase invalid. Value found multiple times");
//    }

//    std::array<_type, _size> expRes;
//    std::array<_type, _size> vectorValues;
//    std::array<_type, _size * _size> matrixValues;

//    _type posNeg = (_neg) ? -1. : 1.;

//    for (U32 i = 0; i < _size; ++i)
//    {
//        expRes[i] = static_cast<_type>(i);
//        vectorValues[i] = static_cast<_type>(indices[i]) * posNeg;
//        for (U32 j = 0; j < _size; ++j)
//            if (indices[i] == j)
//                matrixValues[j * _size + i] = 1 * posNeg;
//            else
//                matrixValues[j * _size + i] = 0;
//    }

//    VectorType b(vectorValues);
//    MatrixType A(matrixValues);

//    VectorType res = Solver::Gauss(A, b);

//    BOOST_CHECK(CheckCloseArray(res.Data(), expRes, 1));

//    if constexpr (!_neg)
//        TestGaussDensePivotingTestcase<_fixture, _type, _size, true>(indices);
//}



// template <template <typename, U32> class _fixture, typename _type>
// void TestGaussDensePivoting()
//{
//    TestGaussDensePivotingTestcase<_fixture, _type, 3>({{2, 0, 1}});
//    TestGaussDensePivotingTestcase<_fixture, _type, 5>({{0, 4, 2, 1, 3}});
//    TestGaussDensePivotingTestcase<_fixture, _type, 8>({{0, 1, 2, 3, 4, 5, 6, 7}});
//    TestGaussDensePivotingTestcase<_fixture, _type, 8>({{1, 5, 4, 2, 0, 7, 6, 3}});
//    TestGaussDensePivotingTestcase<_fixture, _type, 9>({{7, 2, 6, 4, 0, 8, 3, 5, 1}});
//    TestGaussDensePivotingTestcase<_fixture, _type, 16>({{13, 5, 1, 11, 12, 7, 8, 0, 3, 14, 2, 6, 9, 15, 4, 10}});
//}



// BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F32_Serial)
//{
//    TestGaussDensePivoting<FixtureSerial, F32>();
//}



// BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F64_Serial)
//{
//    TestGaussDensePivoting<FixtureSerial, F64>();
//}



// BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F32_SSE)
//{
//    TestGaussDensePivoting<FixtureSSE, F32>();
//}



// BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_Pivoting_F64_SSE)
//{
//    TestGaussDensePivoting<FixtureSSE, F64>();
//}



// Test pivoting - no invalid pivot index -----------------------------------------------------------------------------


//! @brief Matrices with a size that is not a multiple of the number of register values have a register at the end of
//! each column which contains unused values. This test checks if high values in this unused memory locations are
//! ignored as expected.
template <typename _type, U32 _size>
void TestGaussDenseSSENoInvalidPivotIndexTestcase()
{
    using RegisterType = typename MatSIMD<_type, _size, _size>::RegisterType;
    constexpr U32 numColRegisters = simd::CalcMinNumArrayRegisters<RegisterType>(_size);
    constexpr U32 numRegisterValues = simd::numRegisterValues<RegisterType>;

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
                    simd::SetValue(matrixValues[i * numColRegisters + j], k, 1);
                else if (globalColIdx < _size)
                    simd::SetValue(matrixValues[i * numColRegisters + j], k, 0);
                else
                    simd::SetValue(matrixValues[i * numColRegisters + j], k, 500);
            }
    }


    VecSIMD<_type, _size> b(vectorValues);
    MatSIMD<_type, _size, _size> A(matrixValues);

    for (U32 i = 0; i < _size; ++i)
        for (U32 j = 0; j < numColRegisters; ++j)
            for (U32 k = 0; k < numRegisterValues; ++k)
                if (j * numRegisterValues + k >= _size)
                    EXCEPTION(simd::GetValue(matrixValues[i * numColRegisters + j], k) != Approx<_type>(500),
                              "Testcase invalid. Unused memory of matrix is not set as expected.");



    VecSIMD<_type, _size> res = Solver::Gauss(A, b);

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



// --------------------------------------------------------------------------------------------------------------------

using namespace GDL::Solver;

template <typename _type, U32 _size>
using SerialSolverPtr = VecSerial<_type, _size, true> (*)(const MatSerial<_type, _size, _size>&,
                                                          const VecSerial<_type, _size, true>&);
template <typename _type, U32 _size>
using SIMDSolverPtr = VecSIMD<_type, _size, true> (*)(const MatSIMD<_type, _size, _size>&,
                                                      const VecSIMD<_type, _size, true>&);



// --------------------------------------------------------------------------------------------------------------------

enum class SolverType
{
    SERIAL,
    SIMD
};



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot, SolverType _solverType>
void TestGauss()
{


    if constexpr (_solverType == SolverType::SERIAL)
    {
        SerialSolverPtr<_type, _size> solver = Solver::Gauss<_pivot, _type, _size>;
        SolverTests<_type, _size>::template TestSolver<_pivot>(solver);
    }
    else
    {
        SIMDSolverPtr<_type, _size> solver = Solver::Gauss<_pivot, _type, _size>;
        SolverTests<_type, _size>::template TestSolver<_pivot>(solver);
    }
}



// Test 2x2 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_2x2_F32_Serial)
{
    TestGauss<F32, 2, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_2x2_F64_Serial)
{
    TestGauss<F64, 2, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_2x2_F32_SSE)
{
    TestGauss<F32, 2, Pivot::PARTIAL, SolverType::SIMD>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_2x2_F64_SSE)
{
    TestGauss<F64, 2, Pivot::PARTIAL, SolverType::SIMD>();
}



// Test 3x3 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_3x3_F32_Serial)
{
    TestGauss<F32, 3, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_3x3_F64_Serial)
{
    TestGauss<F64, 3, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_3x3_F32_SSE)
{
    TestGauss<F32, 3, Pivot::PARTIAL, SolverType::SIMD>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_3x3_F64_SSE)
{
    TestGauss<F64, 3, Pivot::PARTIAL, SolverType::SIMD>();
}



// Test 4x4 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_4x4_F32_Serial)
{
    TestGauss<F32, 4, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_4x4_F64_Serial)
{
    TestGauss<F64, 4, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_4x4_F32_SSE)
{
    TestGauss<F32, 4, Pivot::PARTIAL, SolverType::SIMD>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_4x4_F64_SSE)
{
    TestGauss<F64, 4, Pivot::PARTIAL, SolverType::SIMD>();
}



// Test 5x5 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_5x5_F32_Serial)
{
    TestGauss<F32, 5, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_5x5_F64_Serial)
{
    TestGauss<F64, 5, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_5x5_F32_SSE)
{
    TestGauss<F32, 5, Pivot::PARTIAL, SolverType::SIMD>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_5x5_F64_SSE)
{
    TestGauss<F64, 5, Pivot::PARTIAL, SolverType::SIMD>();
}



// Test 7x7 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_7x7_F32_Serial)
{
    TestGauss<F32, 7, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_7x7_F64_Serial)
{
    TestGauss<F64, 7, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_7x7_F32_SSE)
{
    TestGauss<F32, 7, Pivot::PARTIAL, SolverType::SIMD>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_7x7_F64_SSE)
{
    TestGauss<F64, 7, Pivot::PARTIAL, SolverType::SIMD>();
}



// Test 8x8 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_8x8_F32_Serial)
{
    TestGauss<F32, 8, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_8x8_F64_Serial)
{
    TestGauss<F64, 8, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_8x8_F32_SSE)
{
    TestGauss<F32, 8, Pivot::PARTIAL, SolverType::SIMD>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_8x8_F64_SSE)
{
    TestGauss<F64, 8, Pivot::PARTIAL, SolverType::SIMD>();
}



// Test 9x9 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_9x9_F32_Serial)
{
    TestGauss<F32, 9, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_9x9_F64_Serial)
{
    TestGauss<F64, 9, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_PartialPivot_9x9_F32_SSE)
{
    TestGauss<F32, 9, Pivot::PARTIAL, SolverType::SIMD>();
}



BOOST_AUTO_TEST_CASE(Test_Gauss_Dense_9x9_F64_SSE)
{
    TestGauss<F64, 9, Pivot::PARTIAL, SolverType::SIMD>();
}
