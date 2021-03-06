#include <boost/test/unit_test.hpp>


#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/simd/mat4fAVX.h"
#include "gdl/math/simd/mat4fSSE.h"
#include "gdl/math/serial/mat4Serial.h"
#include "gdl/math/serial/vec4Serial.h"
#include "gdl/math/simd/vec4fSSE.h"


#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;


// Fixture definition -------------------------------------------------------------------------------------------------

template <typename matrix>
struct Fixture
{
    matrix A{0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.};
    matrix B{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2., 6.};
};



// Construction -------------------------------------------------------------------------------------------------------

template <typename matrix>
void ConstructionTest()
{
    matrix A;
    BOOST_CHECK(CheckArrayZero(A.Data()));

    matrix B(0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.);
    std::array<F32, 16> expB{{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.}};
    BOOST_CHECK(CheckCloseArray(B.Data(), expB));

    matrix B1(expB);
    BOOST_CHECK(CheckCloseArray(B1.Data(), expB));
}



BOOST_AUTO_TEST_CASE(Construction_Serial)
{
    ConstructionTest<Mat4Serial<F32>>();
}



BOOST_AUTO_TEST_CASE(Construction_SSE)
{
    ConstructionTest<Mat4fSSE>();
}

#ifdef __AVX2__
BOOST_AUTO_TEST_CASE(Construction_AVX)
{
    ConstructionTest<Mat4fAVX>();
}
#endif // __AVX2__


// Comparison ---------------------------------------------------------------------------------------------------------

template <typename matrix>
void ComparisonTest(matrix& A, matrix& B)
{
    BOOST_CHECK(A == A);
    BOOST_CHECK(!(A != A));

    BOOST_CHECK(A != B);
    BOOST_CHECK(!(A == B));


    // Check Tolerances
    constexpr F32 epsilon = std::numeric_limits<F32>::epsilon();
    std::array<F32, 16> matAData = A.Data();
    std::array<F32, 16> matEps1Data, matEps2Data;


    for (U32 i = 0; i < 16; ++i)
    {
        if (static_cast<I32>(matAData[i]) == 0)
        {
            matEps1Data[i] = matAData[i] + epsilon;
            matEps2Data[i] = matAData[i] + 10 * epsilon;
        }
        else
        {
            matEps1Data[i] = matAData[i] + epsilon * matAData[i];
            matEps2Data[i] = matAData[i] + 10 * epsilon * matAData[i];
        }
    }

    matrix matEps1(matEps1Data);
    matrix matEps2(matEps2Data);

    BOOST_CHECK(A == matEps1);
    BOOST_CHECK(!(A != matEps1));

    BOOST_CHECK(A != matEps2);
    BOOST_CHECK(!(A == matEps2));
}



BOOST_FIXTURE_TEST_CASE(Comparison_Serial, Fixture<Mat4Serial<F32>>)
{
    ComparisonTest(A, B);
}



BOOST_FIXTURE_TEST_CASE(Comparison_SSE, Fixture<Mat4fSSE>)
{
    ComparisonTest(A, B);
}


#ifdef __AVX2__
BOOST_FIXTURE_TEST_CASE(Comparison_AVX, Fixture<Mat4fAVX>)
{
    ComparisonTest(A, B);
}
#endif // __AVX2__



// Operator() test ----------------------------------------------------------------------------------------------------

template <typename _matrix>
void ParenthesesOperatorMatrixTest(const _matrix& matrix)
{
    std::array<F32, 16> data = matrix.Data();

    for (U32 r = 0; r < 4; ++r)
        for (U32 c = 0; c < 4; ++c)
            BOOST_CHECK(matrix(r, c) == Approx(data[r + c * 4]));

    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] F32 value = matrix(4, 1), Exception);
    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] F32 value = matrix(1, 4), Exception);
}



template <typename _matrix>
void ParenthesesOperatorTest(_matrix A, _matrix B)
{
    ParenthesesOperatorMatrixTest(A);
    ParenthesesOperatorMatrixTest(B);
}



BOOST_FIXTURE_TEST_CASE(Parentheses_Operator_Serial, Fixture<Mat4Serial<F32>>)
{
    ParenthesesOperatorTest(A, B);
}



BOOST_FIXTURE_TEST_CASE(Parentheses_Operator_SSE, Fixture<Mat4fSSE>)
{
    ParenthesesOperatorTest(A, B);
}


#ifdef __AVX2__
BOOST_FIXTURE_TEST_CASE(Parentheses_Operator_AVX, Fixture<Mat4fAVX>)
{
    ParenthesesOperatorTest(A, B);
}
#endif // __AVX2__



// Addition Assignment ------------------------------------------------------------------------------------------------

template <typename matrix>
void AdditionAssignmentTest(matrix& A, matrix& B)
{

    A += B;
    matrix expA{8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.};
    BOOST_CHECK(A == expA);

    // self assignment
    B += B;
    matrix expB{16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4., 12.};
    BOOST_CHECK(B == expB);
}



BOOST_FIXTURE_TEST_CASE(Addition_Assignment_Serial, Fixture<Mat4Serial<F32>>)
{
    AdditionAssignmentTest(A, B);
}



BOOST_FIXTURE_TEST_CASE(Addition_Assignment_SSE, Fixture<Mat4fSSE>)
{
    AdditionAssignmentTest(A, B);
}


#ifdef __AVX2__
BOOST_FIXTURE_TEST_CASE(Addition_Assignment_AVX, Fixture<Mat4fAVX>)
{
    AdditionAssignmentTest(A, B);
}
#endif // __AVX2__



// Addition -----------------------------------------------------------------------------------------------------------

template <typename matrix>
void AdditionTest(matrix& A, matrix& B)
{

    matrix C = A + B;
    matrix expC{8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.};
    BOOST_CHECK(C == expC);
}



BOOST_FIXTURE_TEST_CASE(Addition_Serial, Fixture<Mat4Serial<F32>>)
{
    AdditionTest(A, B);
}



BOOST_FIXTURE_TEST_CASE(Addition_SSE, Fixture<Mat4fSSE>)
{
    AdditionTest(A, B);
}



#ifdef __AVX2__
BOOST_FIXTURE_TEST_CASE(Addition_AVX, Fixture<Mat4fAVX>)
{
    AdditionTest(A, B);
}
#endif // __AVX2__



// Multiplication -----------------------------------------------------------------------------------------------------

template <typename matrix>
void MultiplicationTest(const matrix& A, const matrix& B)
{
    auto C = A * B;
    matrix expC{29., 137., 245., 353., 31., 115., 199., 283., 23., 91., 159., 227., 30., 118., 206., 294.};
    BOOST_CHECK(C == expC);

    auto D = B * A;
    matrix expD{116., 168., 100., 100., 138., 198., 122., 113., 160., 228., 144., 126., 182., 258., 166., 139.};
    BOOST_CHECK(D == expD);
}



BOOST_FIXTURE_TEST_CASE(Multiplication_Serial, Fixture<Mat4Serial<F32>>)
{
    MultiplicationTest<Mat4Serial<F32>>(A, B);
}



BOOST_FIXTURE_TEST_CASE(Multiplication_SSE, Fixture<Mat4fSSE>)
{
    MultiplicationTest<Mat4fSSE>(A, B);
}



#ifdef __AVX2__
BOOST_FIXTURE_TEST_CASE(Multiplication_AVX, Fixture<Mat4fAVX>)
{
    MultiplicationTest<Mat4fAVX>(A, B);
}
#endif // __AVX2__



// Matrix-Vector Multiplication ---------------------------------------------------------------------------------------

template <typename _matrix, typename _vector>
void MatrixVectorMultiplicationTest(const _matrix& a, const _matrix& b)
{

    _vector vec(2, 3, 2, 5);

    auto c = a * vec;
    _vector expC(22, 70, 118, 166);
    BOOST_CHECK(c == expC);

    auto d = b * vec;
    _vector expD{67, 88, 59, 47};
    BOOST_CHECK(d == expD);
}



BOOST_FIXTURE_TEST_CASE(Matrix_Vector_Multiplication_Serial, Fixture<Mat4Serial<F32>>)
{
    MatrixVectorMultiplicationTest<Mat4Serial<F32>, Vec4fSerial<true>>(A, B);
}



BOOST_FIXTURE_TEST_CASE(Matrix_Vector_Multiplication_SSE, Fixture<Mat4fSSE>)
{
    MatrixVectorMultiplicationTest<Mat4fSSE, Vec4fSSE<true>>(A, B);
}

#ifdef __AVX2__
BOOST_FIXTURE_TEST_CASE(Matrix_Vector_Multiplication_AVX, Fixture<Mat4fAVX>)
{
    MatrixVectorMultiplicationTest<Mat4fAVX, Vec4fSSE<true>>(A, B);
}
#endif // __AVX2__

// Transpose ----------------------------------------------------------------------------------------------------------

template <typename matrix>
void Transpose(const matrix& A)
{
    matrix expected(0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.);
    BOOST_CHECK(A != expected);
    BOOST_CHECK(A == expected.Transpose());
    BOOST_CHECK(A.Transpose() == expected);
    BOOST_CHECK(A != A.Transpose());
    BOOST_CHECK(A == A.Transpose().Transpose());
}



BOOST_FIXTURE_TEST_CASE(Transpose_Serial, Fixture<Mat4Serial<F32>>)
{
    Transpose<Mat4Serial<F32>>(A);
}



BOOST_FIXTURE_TEST_CASE(Transpose_SSE, Fixture<Mat4fSSE>)
{
    Transpose<Mat4fSSE>(A);
}

#ifdef __AVX2__
BOOST_FIXTURE_TEST_CASE(Transpose_AVX, Fixture<Mat4fAVX>)
{
    Transpose<Mat4fAVX>(A);
}
#endif // __AVX2__

// Determinant --------------------------------------------------------------------------------------------------------

template <typename matrix>
void Determinant(const matrix& A, const matrix& B)
{

    BOOST_CHECK(A.Det() == ApproxZero<F32>());
    BOOST_CHECK(B.Det() == Approx(-1004.f));
    BOOST_CHECK(A.Transpose().Det() == ApproxZero<F32>());
    BOOST_CHECK(B.Transpose().Det() == Approx(-1004.f));

    //    BOOST_CHECK(matrix(1, 2, 3, 1, 2, 7, 2, 4, 5).Det() == ApproxZero<F32>());
}



BOOST_FIXTURE_TEST_CASE(Determinant_Serial, Fixture<Mat4Serial<F32>>)
{
    Determinant<Mat4Serial<F32>>(A, B);
}



BOOST_FIXTURE_TEST_CASE(Determinant_SSE, Fixture<Mat4fSSE>)
{
    Determinant<Mat4fSSE>(A, B);
}


#ifdef __AVX2__
BOOST_FIXTURE_TEST_CASE(Determinant_AVX, Fixture<Mat4fAVX>)
{
    Determinant<Mat4fAVX>(A, B);
}
#endif // __AVX2__
