#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/mat4SIMD.h"
#include "gdl/math/mat4Single.h"

#include <cmath>

using namespace GDL;

// Helper functions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename _type>
bool CheckArrayZero(const _type& a)
{
    for (U32 i = 0; i < a.size(); ++i)
        if (std::fabs(a[i]) > static_cast<decltype(a[i])>(0.))
            return false;
    return true;
}

template <typename _type>
bool CheckCloseArray(const _type& a, const _type& b)
{
    if (a.size() != b.size())
        return false;
    for (U32 i = 0; i < a.size(); ++i)
        if (a[i] != Approx(b[i]))
            return false;
    return true;
}



// Fixture definition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename matrix>
struct Fixture
{
    matrix A{0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.};
    matrix B{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2., 6.};
};


// Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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

BOOST_AUTO_TEST_CASE(Construction_Single)
{
    ConstructionTest<Mat4Single<F32>>();
}

BOOST_AUTO_TEST_CASE(Construction_SIMD)
{
    ConstructionTest<Mat4SIMD>();
}


// Comparison %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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


BOOST_FIXTURE_TEST_CASE(Comparison_Single, Fixture<Mat4Single<F32>>)
{
    ComparisonTest(A, B);
}


BOOST_FIXTURE_TEST_CASE(Comparison_SIMD, Fixture<Mat4SIMD>)
{
    ComparisonTest(A, B);
}



// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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


BOOST_FIXTURE_TEST_CASE(Addition_Assignment_Single, Fixture<Mat4Single<F32>>)
{
    AdditionAssignmentTest(A, B);
}


BOOST_FIXTURE_TEST_CASE(Addition_Assignment_SIMD, Fixture<Mat4SIMD>)
{
    AdditionAssignmentTest(A, B);
}



// Addition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename matrix>
void AdditionTest(matrix& A, matrix& B)
{

    matrix C = A + B;
    matrix expC{8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.};
    BOOST_CHECK(C == expC);
}


BOOST_FIXTURE_TEST_CASE(Addition_Single, Fixture<Mat4Single<F32>>)
{
    AdditionTest(A, B);
}


BOOST_FIXTURE_TEST_CASE(Addition_SIMD, Fixture<Mat4SIMD>)
{
    AdditionTest(A, B);
}



// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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


BOOST_FIXTURE_TEST_CASE(Multiplication_Single, Fixture<Mat4Single<F32>>)
{
    MultiplicationTest<Mat4Single<F32>>(A, B);
}


BOOST_FIXTURE_TEST_CASE(Multiplication_SIMD, Fixture<Mat4SIMD>)
{
    MultiplicationTest<Mat4SIMD>(A, B);
}
