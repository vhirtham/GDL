#include <boost/test/unit_test.hpp>

#include "test/unit/math/solver/solverTests.h"


#include "gdl/math/simd/vecSIMD.h"
#include "gdl/math/simd/matSIMD.h"
#include "gdl/math/solver/qr.h"



using namespace GDL;



// --------------------------------------------------------------------------------------------------------------------

using namespace GDL::Solver;

template <typename _type, U32 _rows, U32 _cols>
using SerialSolverPtr = VecSerial<_type, _rows, true> (*)(const MatSerial<_type, _rows, _cols>&,
                                                          const VecSerial<_type, _rows, true>&);
template <typename _type, U32 _rows, U32 _cols>
using SIMDSolverPtr = VecSIMD<_type, _rows, true> (*)(const MatSIMD<_type, _rows, _cols>&,
                                                      const VecSIMD<_type, _rows, true>&);



// --------------------------------------------------------------------------------------------------------------------

enum class SolverType
{
    SERIAL,
    SIMD
};



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _rows, U32 _cols, Pivot _pivot, SolverType _solverType>
void TestQR()
{


    if constexpr (_solverType == SolverType::SERIAL)
    {
        SerialSolverPtr<_type, _rows, _cols> solver = Solver::QR<_pivot, _type, _rows, _cols>;
        SolverTests<_type, _rows, decltype(solver)>::template RunTests<_pivot>(solver);
    }
    else
    {
        SIMDSolverPtr<_type, _rows, _cols> solver = Solver::QR<_pivot, _type, _rows, _cols>;
        SolverTests<_type, _rows, decltype(solver)>::template RunTests<_pivot>(solver);
    }
}



// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// No pivoting
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------


// Test 2x2 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_2x2_F32_Serial)
{
    TestQR<F32, 2, 2, Pivot::NONE, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_2x2_F64_Serial)
{
    TestQR<F64, 2, 2, Pivot::NONE, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_2x2_F32_SIMD)
//{
//    TestQR<F32, 2, 2, Pivot::NONE, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_2x2_F64_SIMD)
//{
//    TestQR<F64, 2, Pivot::NONE, SolverType::SIMD>();
//}



// Test 3x3 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_3x3_F32_Serial)
{
    TestQR<F32, 3, 3, Pivot::NONE, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_3x3_F64_Serial)
{
    TestQR<F64, 3, 3, Pivot::NONE, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_3x3_F32_SIMD)
//{
//    TestQR<F32, 3, 3, Pivot::NONE, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_3x3_F64_SIMD)
//{
//    TestQR<F64, 3, 3, Pivot::NONE, SolverType::SIMD>();
//}



// Test 4x4 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_4x4_F32_Serial)
{
    TestQR<F32, 4, 4, Pivot::NONE, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_4x4_F64_Serial)
{
    TestQR<F64, 4, 4, Pivot::NONE, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_4x4_F32_SIMD)
//{
//    TestQR<F32, 4, 4, Pivot::NONE, SolverType::SIMD>();
//}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_4x4_F64_SIMD)
{
    TestQR<F64, 4, 4, Pivot::NONE, SolverType::SIMD>();
}



// Test 5x5 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_5x5_F32_Serial)
{
    TestQR<F32, 5, 5, Pivot::NONE, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_5x5_F64_Serial)
{
    TestQR<F64, 5, 5, Pivot::NONE, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_5x5_F32_SIMD)
//{
//    TestQR<F32, 5, 5, Pivot::NONE, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_5x5_F64_SIMD)
//{
//    TestQR<F64, 5, 5, Pivot::NONE, SolverType::SIMD>();
//}



// Test 6x6 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_6x6_F32_Serial)
{
    TestQR<F32, 6, 6, Pivot::NONE, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_6x6_F64_Serial)
{
    TestQR<F64, 6, 6, Pivot::NONE, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_6x6_F32_SIMD)
//{
//    TestQR<F32, 6, Pivot::NONE, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_6x6_F64_SIMD)
//{
//    TestQR<F64, 6, Pivot::NONE, SolverType::SIMD>();
//}



// Test 7x7 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_7x7_F32_Serial)
{
    TestQR<F32, 7, 7, Pivot::NONE, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_7x7_F64_Serial)
{
    TestQR<F64, 7, 7, Pivot::NONE, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_7x7_F32_SIMD)
//{
//    TestQR<F32, 7, Pivot::NONE, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_7x7_F64_SIMD)
//{
//    TestQR<F64, 7, Pivot::NONE, SolverType::SIMD>();
//}



// Test 8x8 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_8x8_F32_Serial)
{
    TestQR<F32, 8, 8, Pivot::NONE, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_8x8_F64_Serial)
{
    TestQR<F64, 8, 8, Pivot::NONE, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_8x8_F32_SIMD)
{
    TestQR<F32, 8, 8, Pivot::NONE, SolverType::SIMD>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_8x8_F64_SIMD)
//{
//    TestQR<F64, 8, 8, Pivot::NONE, SolverType::SIMD>();
//}



// Test 9x9 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_9x9_F32_Serial)
{
    TestQR<F32, 9, 9, Pivot::NONE, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_9x9_F64_Serial)
{
    TestQR<F64, 9, 9, Pivot::NONE, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_9x9_F32_SIMD)
//{
//    TestQR<F32, 9, Pivot::NONE, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_NoPivot_9x9_F64_SIMD)
//{
//    TestQR<F64, 9, Pivot::NONE, SolverType::SIMD>();
//}



// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// Partial pivoting
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------



// Test 2x2 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_2x2_F32_Serial)
{
    TestQR<F32, 2, 2, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_2x2_F64_Serial)
{
    TestQR<F64, 2, 2, Pivot::PARTIAL, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_2x2_F32_SIMD)
//{
//    TestQR<F32, 2, Pivot::PARTIAL, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_2x2_F64_SIMD)
//{
//    TestQR<F64, 2, Pivot::PARTIAL, SolverType::SIMD>();
//}



// Test 3x3 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_3x3_F32_Serial)
{
    TestQR<F32, 3, 3, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_3x3_F64_Serial)
{
    TestQR<F64, 3, 3, Pivot::PARTIAL, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_3x3_F32_SIMD)
//{
//    TestQR<F32, 3, Pivot::PARTIAL, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_3x3_F64_SIMD)
//{
//    TestQR<F64, 3, Pivot::PARTIAL, SolverType::SIMD>();
//}



// Test 4x4 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_4x4_F32_Serial)
{
    TestQR<F32, 4, 4, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_4x4_F64_Serial)
{
    TestQR<F64, 4, 4, Pivot::PARTIAL, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_4x4_F32_SIMD)
//{
//    TestQR<F32, 4, Pivot::PARTIAL, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_4x4_F64_SIMD)
//{
//    TestQR<F64, 4, Pivot::PARTIAL, SolverType::SIMD>();
//}



// Test 5x5 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_5x5_F32_Serial)
{
    TestQR<F32, 5, 5, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_5x5_F64_Serial)
{
    TestQR<F64, 5, 5, Pivot::PARTIAL, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_5x5_F32_SIMD)
//{
//    TestQR<F32, 5, Pivot::PARTIAL, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_5x5_F64_SIMD)
//{
//    TestQR<F64, 5, Pivot::PARTIAL, SolverType::SIMD>();
//}



// Test 6x6 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_6x6_F32_Serial)
{
    TestQR<F32, 6, 6, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_6x6_F64_Serial)
{
    TestQR<F64, 6, 6, Pivot::PARTIAL, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_6x6_F32_SIMD)
//{
//    TestQR<F32, 6, Pivot::PARTIAL, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_6x6_F64_SIMD)
//{
//    TestQR<F64, 6, Pivot::PARTIAL, SolverType::SIMD>();
//}



// Test 7x7 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_7x7_F32_Serial)
{
    TestQR<F32, 7, 7, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_7x7_F64_Serial)
{
    TestQR<F64, 7, 7, Pivot::PARTIAL, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_7x7_F32_SIMD)
//{
//    TestQR<F32, 7, Pivot::PARTIAL, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_7x7_F64_SIMD)
//{
//    TestQR<F64, 7, Pivot::PARTIAL, SolverType::SIMD>();
//}



// Test 8x8 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_8x8_F32_Serial)
{
    TestQR<F32, 8, 8, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_8x8_F64_Serial)
{
    TestQR<F64, 8, 8, Pivot::PARTIAL, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_8x8_F32_SIMD)
//{
//    TestQR<F32, 8, Pivot::PARTIAL, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_8x8_F64_SIMD)
//{
//    TestQR<F64, 8, Pivot::PARTIAL, SolverType::SIMD>();
//}



// Test 9x9 -----------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_9x9_F32_Serial)
{
    TestQR<F32, 9, 9, Pivot::PARTIAL, SolverType::SERIAL>();
}



BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_9x9_F64_Serial)
{
    TestQR<F64, 9, 9, Pivot::PARTIAL, SolverType::SERIAL>();
}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_9x9_F32_SIMD)
//{
//    TestQR<F32, 9, Pivot::PARTIAL, SolverType::SIMD>();
//}



// BOOST_AUTO_TEST_CASE(Test_QR_PartialPivot_9x9_F64_SIMD)
//{
//    TestQR<F64, 9, Pivot::PARTIAL, SolverType::SIMD>();
//}
