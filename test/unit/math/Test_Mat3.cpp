#include <boost/test/unit_test.hpp>


#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/single/mat3Single.h"
#include "gdl/math/single/vec3Single.h"
#include "gdl/math/sse/mat3fSSE.h"
#include "gdl/math/sse/vec3fSSE.h"


#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;


// Fixture definition -------------------------------------------------------------------------------------------------

template <typename matrix>
struct Fixture
{
    matrix A{0., 3., 6., 1., 4., 7., 2., 5., 8.};
    matrix B{8., 11., 6., 2., 5., 4., 9., 3., 3.};
};



// Construction -------------------------------------------------------------------------------------------------------

template <typename matrix>
void ConstructionTest()
{
    matrix A;
    BOOST_CHECK(CheckArrayZero(A.Data()));

    matrix B(0., 1., 2., 3., 4., 5., 6., 7., 8.);
    std::array<F32, 9> expB{{0., 1., 2., 3., 4., 5., 6., 7., 8.}};
    BOOST_CHECK(CheckCloseArray(B.Data(), expB));

    matrix B1(expB);
    BOOST_CHECK(CheckCloseArray(B1.Data(), expB));
}



BOOST_AUTO_TEST_CASE(Construction_Single)
{
    ConstructionTest<Mat3Single<F32>>();
}



BOOST_AUTO_TEST_CASE(Construction_SSE)
{
    ConstructionTest<Mat3fSSE>();
}



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
    std::array<F32, 9> matAData = A.Data();
    std::array<F32, 9> matEps1Data, matEps2Data;


    for (U32 i = 0; i < 9; ++i)
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



BOOST_FIXTURE_TEST_CASE(Comparison_Single, Fixture<Mat3Single<F32>>)
{
    ComparisonTest(A, B);
}



BOOST_FIXTURE_TEST_CASE(Comparison_SSE, Fixture<Mat3fSSE>)
{
    ComparisonTest(A, B);
}



// Operator() test ----------------------------------------------------------------------------------------------------

template <typename _matrix>
void ParenthesesOperatorMatrixTest(const _matrix& matrix)
{
    std::array<F32, 9> data = matrix.Data();

    for (U32 r = 0; r < 3; ++r)
        for (U32 c = 0; c < 3; ++c)
            BOOST_CHECK(matrix(r, c) == Approx(data[r + c * 3]));

    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] F32 value = matrix(3, 1), Exception);
    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] F32 value = matrix(1, 3), Exception);
}



template <typename _matrix>
void ParenthesesOperatorTest(_matrix A, _matrix B)
{
    ParenthesesOperatorMatrixTest(A);
    ParenthesesOperatorMatrixTest(B);
}



BOOST_FIXTURE_TEST_CASE(Parentheses_Operator_Single, Fixture<Mat3Single<F32>>)
{
    ParenthesesOperatorTest(A, B);
}



BOOST_FIXTURE_TEST_CASE(Parentheses_Operator_SSE, Fixture<Mat3fSSE>)
{
    ParenthesesOperatorTest(A, B);
}



// Addition Assignment ------------------------------------------------------------------------------------------------

template <typename matrix>
void AdditionAssignmentTest(matrix& A, matrix& B)
{

    A += B;
    matrix expA{8., 14., 12., 3., 9., 11., 11., 8., 11.};
    BOOST_CHECK(A == expA);

    // self assignment
    B += B;
    matrix expB{16., 22., 12., 4., 10., 8., 18., 6., 6.};
    BOOST_CHECK(B == expB);
}



BOOST_FIXTURE_TEST_CASE(Addition_Assignment_Single, Fixture<Mat3Single<F32>>)
{
    AdditionAssignmentTest(A, B);
}



BOOST_FIXTURE_TEST_CASE(Addition_Assignment_SSE, Fixture<Mat3fSSE>)
{
    AdditionAssignmentTest(A, B);
}



// Addition -----------------------------------------------------------------------------------------------------------

template <typename matrix>
void AdditionTest(matrix& A, matrix& B)
{

    matrix C = A + B;
    matrix expC{8., 14., 12., 3., 9., 11., 11., 8., 11.};
    ;
    BOOST_CHECK(C == expC);
}



BOOST_FIXTURE_TEST_CASE(Addition_Single, Fixture<Mat3Single<F32>>)
{
    AdditionTest(A, B);
}



BOOST_FIXTURE_TEST_CASE(Addition_SSE, Fixture<Mat3fSSE>)
{
    AdditionTest(A, B);
}



// Multiplication -----------------------------------------------------------------------------------------------------

template <typename matrix>
void MultiplicationTest(const matrix& A, const matrix& B)
{
    auto C = A * B;
    matrix expC{23., 98., 173., 13., 46., 79., 9., 54., 99.};
    BOOST_CHECK(C == expC);

    auto D = B * A;
    matrix expD{60., 33., 30., 79., 52., 43., 98., 71., 56.};
    BOOST_CHECK(D == expD);
}



BOOST_FIXTURE_TEST_CASE(Multiplication_Single, Fixture<Mat3Single<F32>>)
{
    MultiplicationTest<Mat3Single<F32>>(A, B);
}



BOOST_FIXTURE_TEST_CASE(Multiplication_SSE, Fixture<Mat3fSSE>)
{
    MultiplicationTest<Mat3fSSE>(A, B);
}



// Matrix-Vector Multiplication ---------------------------------------------------------------------------------------

template <typename _matrix, typename _vector>
void MatrixVectorMultiplicationTest(const _matrix& a, const _matrix& b)
{

    _vector vec(2, 3, 2);

    auto c = a * vec;
    _vector expC(7, 28, 49);
    BOOST_CHECK(c == expC);

    auto d = b * vec;
    _vector expD{40, 43, 30};
    BOOST_CHECK(d == expD);
}



BOOST_FIXTURE_TEST_CASE(Matrix_Vector_Multiplication_Single, Fixture<Mat3Single<F32>>)
{
    MatrixVectorMultiplicationTest<Mat3Single<F32>, Vec3fSingle<true>>(A, B);
}



BOOST_FIXTURE_TEST_CASE(Matrix_Vector_Multiplication_SSE, Fixture<Mat3fSSE>)
{
    MatrixVectorMultiplicationTest<Mat3fSSE, Vec3fSSE<true>>(A, B);
}



// Transpose ----------------------------------------------------------------------------------------------------------

template <typename matrix>
void Transpose(const matrix& A)
{
    matrix expected(0., 1., 2., 3., 4., 5., 6., 7., 8.);
    BOOST_CHECK(A != expected);
    BOOST_CHECK(A == expected.Transpose());
    BOOST_CHECK(A.Transpose() == expected);
    BOOST_CHECK(A != A.Transpose());
    BOOST_CHECK(A == A.Transpose().Transpose());
}



BOOST_FIXTURE_TEST_CASE(Transpose_Single, Fixture<Mat3Single<F32>>)
{
    Transpose<Mat3Single<F32>>(A);
}



BOOST_FIXTURE_TEST_CASE(Transpose_SSE, Fixture<Mat3fSSE>)
{
    Transpose<Mat3fSSE>(A);
}



// Determinant --------------------------------------------------------------------------------------------------------

template <typename matrix>
void Determinant(const matrix& A, const matrix& B)
{
    BOOST_CHECK(A.Det() == ApproxZero<F32>());
    BOOST_CHECK(B.Det() == Approx(120.f));
    BOOST_CHECK(A.Transpose().Det() == ApproxZero<F32>());
    BOOST_CHECK(B.Transpose().Det() == Approx(120.f));

    BOOST_CHECK(matrix(1, 2, 3, 1, 2, 7, 2, 4, 5).Det() == ApproxZero<F32>());
}



BOOST_FIXTURE_TEST_CASE(Determinant_Single, Fixture<Mat3Single<F32>>)
{
    Determinant<Mat3Single<F32>>(A, B);
}



BOOST_FIXTURE_TEST_CASE(Determinant_SSE, Fixture<Mat3fSSE>)
{
    Determinant<Mat3fSSE>(A, B);
}
