#include <boost/test/unit_test.hpp>

#include "gdl/math/sse/matSSE.h"
#include "gdl/math/serial/matSerial.h"

#include "test/tools/arrayValueComparison.h"


using namespace GDL;


// Fixture definition -------------------------------------------------------------------------------------------------

template <template <typename, I32, I32> class _matrix, typename _type>
struct Fixture
{
    _matrix<_type, 4, 4> A1 =
            _matrix<_type, 4, 4>(0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.);
    _matrix<_type, 4, 4> B1 = _matrix<_type, 4, 4>(8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2., 6.);
    _matrix<_type, 3, 5> A2 = _matrix<_type, 3, 5>(0., 5., 10., 1., 6., 11., 2., 7., 12., 3., 8., 13., 4., 9., 14.);
    _matrix<_type, 5, 3> B2 = _matrix<_type, 5, 3>(8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2.);
    _matrix<_type, 2, 6> A3 = _matrix<_type, 2, 6>(1., 7., 2., 8., 3., 9., 4., 10., 5., 11., 6., 12.);
    _matrix<_type, 6, 2> B3 = _matrix<_type, 6, 2>(24., 12., 6., 0., 1., 8., 23., 5., 33., 6., 72., 9.);
};



// Construction and Data function -------------------------------------------------------------------------------------

template <template <typename, I32, I32> class _matrix, typename _type>
void CtorDataTest()
{

    // array ctor
    std::array<_type, 15> expA = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};
    _matrix<_type, 3, 5> A(expA);
    BOOST_CHECK(CheckCloseArray(A.Data(), expA));

    std::array<_type, 15> expB = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};
    _matrix<_type, 5, 3> B(expB);
    BOOST_CHECK(CheckCloseArray(B.Data(), expB));

    std::array<_type, 16> expC = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
    _matrix<_type, 4, 4> C(expC);
    BOOST_CHECK(CheckCloseArray(C.Data(), expC));

    // variadic ctor
    _matrix<_type, 3, 5> A2(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
    BOOST_CHECK(CheckCloseArray(A2.Data(), expA));

    // default ctor - all variables 0
    _matrix<_type, 13, 7> D;
    BOOST_CHECK(CheckArrayZero(D.Data()));
}



BOOST_AUTO_TEST_CASE(Construction_Serial)
{
    CtorDataTest<MatSerial, F32>();
    CtorDataTest<MatSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Construction_SSE)
{
    CtorDataTest<MatSSE, F32>();
    CtorDataTest<MatSSE, F64>();
}



// Comparison Equal ---------------------------------------------------------------------------------------------------

template <I32 _cols, I32 _rows, typename _type, template <typename, I32, I32> class _matrix>
void ComparisonEqualMatrixTest(const _matrix<_type, _rows, _cols>& ref)
{
    BOOST_CHECK(ref == ref);
    BOOST_CHECK(!(ref != ref));


    constexpr _type epsilon = std::numeric_limits<_type>::epsilon();
    std::array<_type, _rows* _cols> refData = ref.Data();
    std::array<_type, _rows * _cols> matEps1Data, matEps2Data;

    for (U32 i = 0; i < _rows * _cols; ++i)
    {
        if (static_cast<I32>(refData[i]) == 0)
        {
            matEps1Data[i] = refData[i] + epsilon;
            matEps2Data[i] = refData[i] + 10 * epsilon;
        }
        else
        {
            matEps1Data[i] = refData[i] + epsilon * refData[i];
            matEps2Data[i] = refData[i] + 10 * epsilon * refData[i];
        }
    }

    _matrix<_type, _rows, _cols> matEps1(matEps1Data);
    _matrix<_type, _rows, _cols> matEps2(matEps2Data);

    BOOST_CHECK(ref == matEps1);
    BOOST_CHECK(!(ref != matEps1));
    BOOST_CHECK(ref != matEps2);
    BOOST_CHECK(!(ref == matEps2));


    for (U32 i = 0; i < _rows * _cols; ++i)
    {
        matEps2Data = refData;
        matEps2 = _matrix<_type, _rows, _cols>(matEps2Data);
        BOOST_CHECK(ref == matEps2);
        BOOST_CHECK(!(ref != matEps2));

        if (static_cast<I32>(refData[i]) == 0)
            matEps2Data[i] = refData[i] + 10 * epsilon;
        else
            matEps2Data[i] = refData[i] + 10 * epsilon * refData[i];

        matEps2 = _matrix<_type, _rows, _cols>(matEps2Data);
        BOOST_CHECK(ref != matEps2);
        BOOST_CHECK(!(ref == matEps2));
    }
}



template <template <typename, I32, I32> class _matrix, typename _type>
void ComparisonEqualTest()
{
    // square matrices
    Fixture<_matrix, _type> f;

    ComparisonEqualMatrixTest(f.A1);
    ComparisonEqualMatrixTest(f.B1);
    ComparisonEqualMatrixTest(f.A2);
    ComparisonEqualMatrixTest(f.B2);
    ComparisonEqualMatrixTest(f.A3);
    ComparisonEqualMatrixTest(f.B3);

    BOOST_CHECK(f.A1 != f.B1);
    BOOST_CHECK(!(f.A1 == f.B1));
}



BOOST_AUTO_TEST_CASE(ComparisonEqual_Serial)
{
    ComparisonEqualTest<MatSerial, F32>();
    ComparisonEqualTest<MatSerial, F64>();
}



BOOST_AUTO_TEST_CASE(ComparisonEqual_SSE)
{
    ComparisonEqualTest<MatSSE, F32>();
    ComparisonEqualTest<MatSSE, F64>();
}



// Operator() test ----------------------------------------------------------------------------------------------------

template <template <typename, I32, I32> class _matrix, typename _type, I32 _rows, I32 _cols>
void ParenthesesOperatorMatrixTest(const _matrix<_type, _rows, _cols>& matrix)
{
    std::array<_type, _rows* _cols> data = matrix.Data();

    for (U32 r = 0; r < matrix.Rows(); ++r)
        for (U32 c = 0; c < matrix.Cols(); ++c)
            BOOST_CHECK(matrix(r, c) == Approx(data[r + c * _rows]));
}



template <template <typename, I32, I32> class _matrix, typename _type>
void ParenthesesOperatorTest()
{
    Fixture<_matrix, _type> f;

    ParenthesesOperatorMatrixTest(f.A1);
    ParenthesesOperatorMatrixTest(f.B1);
    ParenthesesOperatorMatrixTest(f.A2);
    ParenthesesOperatorMatrixTest(f.B2);
    ParenthesesOperatorMatrixTest(f.A3);
    ParenthesesOperatorMatrixTest(f.B3);
}



BOOST_AUTO_TEST_CASE(Parentheses_Operator_Serial)
{
    ParenthesesOperatorTest<MatSerial, F32>();
    ParenthesesOperatorTest<MatSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Parentheses_Operator_SIMD)
{
    ParenthesesOperatorTest<MatSSE, F32>();
    ParenthesesOperatorTest<MatSSE, F64>();
}



// Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <template <typename, I32, I32> class _matrix, typename _type>
void AssignmentTest()
{
    Fixture<_matrix, _type> f;

    _matrix<_type, 4, 4> A1;
    A1.SetZero();
    A1 = f.A1;
    BOOST_CHECK(A1 == f.A1);

    _matrix<_type, 3, 5> A2;
    A2.SetZero();
    A2 = f.A2;
    BOOST_CHECK(A2 == f.A2);

    _matrix<_type, 5, 3> B2;
    B2.SetZero();
    B2 = f.B2;
    BOOST_CHECK(B2 == f.B2);
}



BOOST_AUTO_TEST_CASE(Assignment_Serial)
{
    AssignmentTest<MatSerial, F32>();
    AssignmentTest<MatSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Assignment_SIMD)
{
    AssignmentTest<MatSSE, F32>();
    AssignmentTest<MatSSE, F64>();
}



// Set zero %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <template <typename, I32, I32> class matrix, typename _type>
void SetZeroTest()
{
    Fixture<matrix, _type> f;
    f.A1.SetZero();
    BOOST_CHECK(CheckArrayZero(f.A1.Data()));

    f.B1.SetZero();
    BOOST_CHECK(CheckArrayZero(f.B1.Data()));

    f.A2.SetZero();
    BOOST_CHECK(CheckArrayZero(f.A2.Data()));

    f.B2.SetZero();
    BOOST_CHECK(CheckArrayZero(f.B2.Data()));
}



BOOST_AUTO_TEST_CASE(SetZero_Serial)
{
    SetZeroTest<MatSerial, F32>();
    SetZeroTest<MatSerial, F64>();
}



BOOST_AUTO_TEST_CASE(SetZero_SIMD)
{
    SetZeroTest<MatSSE, F32>();
    SetZeroTest<MatSSE, F64>();
}



// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <template <typename, I32, I32> class _matrix, typename _type>
void AdditionAssignmentTest()
{
    Fixture<_matrix, _type> f;

    // Square Matrix addition
    _matrix<_type, 4, 4> expA1(8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.);
    f.A1 += f.B1;
    BOOST_CHECK(f.A1 == expA1);

    _matrix<_type, 4, 4> expB1(16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4., 12.);
    f.B1 += f.B1;
    BOOST_CHECK(f.B1 == expB1);

    // Non square matrix addition
    _matrix<_type, 3, 5> B2(f.B2.Data());
    _matrix<_type, 3, 5> expA2(8., 16., 16., 3., 11., 15., 11., 10., 15., 10., 13., 15., 10., 17., 16.);
    f.A2 += B2;
    BOOST_CHECK(f.A2 == expA2);

    _matrix<_type, 3, 5> expB2(16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4.);
    B2 += B2;
    BOOST_CHECK(B2 == expB2);
}



BOOST_AUTO_TEST_CASE(Addition_Assignment_Serial)
{
    AdditionAssignmentTest<MatSerial, F32>();
    AdditionAssignmentTest<MatSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Addition_Assignment_SIMD)
{
    AdditionAssignmentTest<MatSSE, F32>();
    AdditionAssignmentTest<MatSSE, F64>();
}



// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <template <typename, I32, I32> class _matrix, typename _type>
void AdditionTest()
{
    Fixture<_matrix, _type> f;

    // Square Matrix addition

    _matrix<_type, 4, 4> C = f.A1 + f.B1;
    _matrix<_type, 4, 4> expC(8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.);

    BOOST_CHECK(C == expC);


    // Non square matrix addition
    _matrix<_type, 3, 5> B2(f.B2.Data());
    _matrix<_type, 3, 5> C2 = f.A2 + B2;
    _matrix<_type, 3, 5> expC2(8., 16., 16., 3., 11., 15., 11., 10., 15., 10., 13., 15., 10., 17., 16.);

    BOOST_CHECK(C2 == expC2);
}



BOOST_AUTO_TEST_CASE(Addition_Serial)
{
    AdditionTest<MatSerial, F32>();
    AdditionTest<MatSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Addition_SIMD)
{
    AdditionTest<MatSSE, F32>();
    AdditionTest<MatSSE, F64>();
}



// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <template <typename, I32, I32> class _matrix, typename _type>
void MultiplicationTest()
{
    Fixture<_matrix, _type> f;

    // Square Matrix multiplication
    _matrix<_type, 4, 4> expC1(29., 137., 245., 353., 31., 115., 199., 283., 23., 91., 159., 227., 30., 118., 206.,
                               294.);
    _matrix<_type, 4, 4> C1 = f.A1 * f.B1;
    BOOST_CHECK(C1 == expC1);

    _matrix<_type, 4, 4> expD1(116., 168., 100., 100., 138., 198., 122., 113., 160., 228., 144., 126., 182., 258., 166.,
                               139.);
    _matrix<_type, 4, 4> D1 = f.B1 * f.A1;
    BOOST_CHECK(D1 == expD1);

    // Non square matrix multiplication
    _matrix<_type, 3, 3> expC2(49., 209., 369., 52., 182., 312., 46., 161., 276.);
    _matrix<_type, 3, 3> C2 = f.A2 * f.B2;
    BOOST_CHECK(C2 == expC2);

    _matrix<_type, 5, 5> expD2(70., 65., 75., 95., 55., 87., 87., 90., 108., 69., 104., 109., 105., 121., 83., 121.,
                               131., 120., 134., 97., 138., 153., 135., 147., 111.);
    _matrix<_type, 5, 5> D2 = f.B2 * f.A2;
    BOOST_CHECK(D2 == expD2);

    _matrix<_type, 2, 2> expC3(119, 425, 570, 1458);
    _matrix<_type, 2, 2> C3 = f.A3 * f.B3;
    BOOST_CHECK(C3 == expC3);


    _matrix<_type, 6, 6> expD3(185, 47, 237, 42, 505, 71, 232, 64, 276, 48, 578, 88, 279, 81, 315, 54, 651, 105, 326,
                               98, 354, 60, 724, 122, 373, 115, 393, 66, 797, 139, 420, 132, 432, 72, 870, 156);
    _matrix<_type, 6, 6> D3 = f.B3 * f.A3;
    BOOST_CHECK(D3 == expD3);
}



BOOST_AUTO_TEST_CASE(Multiplication_Serial)
{
    MultiplicationTest<MatSerial, F32>();
    MultiplicationTest<MatSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Multiplication_SIMD)
{
    MultiplicationTest<MatSSE, F32>();
    MultiplicationTest<MatSSE, F64>();
}



// Transpose %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <template <typename, I32, I32> class _matrix, typename _type, U32 _rows = 8, U32 _cols = 8>
void TransposeTest()
{
    // square matrix
    std::array<_type, _rows * _cols> aData, aTData;
    for (U32 i = 0; i < _rows; ++i)
        for (U32 j = 0; j < _cols; ++j)
        {
            aData[i + j * _rows] = i + j * _rows + 1;
            aTData[i * _cols + j] = i + j * _rows + 1;
        }

    _matrix<_type, _rows, _cols> a(aData);
    _matrix<_type, _cols, _rows> aT(aTData);


    BOOST_CHECK(a == aT.Transpose());
    BOOST_CHECK(a.Transpose() == aT);
    BOOST_CHECK(a == a.Transpose().Transpose());



    if constexpr (_rows == _cols)
    {
        BOOST_CHECK(a != aT);
        BOOST_CHECK(a != a.Transpose());
    }

    if constexpr (_cols == 8 && _rows == 8)
    {
        TransposeTest<_matrix, _type, 9, 8>();
        TransposeTest<_matrix, _type, 8, 9>();
        TransposeTest<_matrix, _type, 9, 9>();
        TransposeTest<_matrix, _type, 11, 15>();
        TransposeTest<_matrix, _type, 13, 10>();
        TransposeTest<_matrix, _type, 14, 14>();
        TransposeTest<_matrix, _type, 16, 16>();
    }
}



BOOST_AUTO_TEST_CASE(Transpose_Serial)
{
    TransposeTest<MatSerial, F32>();
    TransposeTest<MatSerial, F64>();
}



BOOST_AUTO_TEST_CASE(Transpose_SIMD)
{

    TransposeTest<MatSSE, F32>();
    TransposeTest<MatSSE, F64>();
}
