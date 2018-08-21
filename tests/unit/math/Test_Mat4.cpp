#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/mat4SIMD.inl"
#include "gdl/math/mat4Single.inl"

#include <cmath>

using namespace GDL;

// Helper functions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename T>
bool checkArrayZero(const T& a)
{
    for (U32 i = 0; i < a.size(); ++i)
        if (std::fabs(a[i]) > 0.)
            return false;
    return true;
}

template <typename T>
bool checkCloseArray(const T& a, const T& b, F32 tolerance = 1e-6)
{
    if (a.size() != b.size())
        return false;
    for (U32 i = 0; i < a.size(); ++i)
        if (std::fabs(a[i] - b[i]) > tolerance)
            return false;
    return true;
}

template <typename T>
bool checkClose(T a, T b, F32 tolerance = 1e-6)
{
    for (U32 i = 0; i < 4; ++i)
        for (U32 j = 0; j < 4; ++j)
            if (std::fabs(a(i, j) - b(i, j)) > tolerance)
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


// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename matrix>
void ConstructionTest()
{
    matrix A;
    BOOST_CHECK(checkArrayZero(A.Data()));

    matrix B(0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.);
    std::array<F32, 16> expB{{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.}};
    BOOST_CHECK(checkCloseArray(B.Data(), expB));
}

BOOST_AUTO_TEST_CASE(Construction_Single)
{
    ConstructionTest<mat4Single<F32>>();
}

BOOST_AUTO_TEST_CASE(Construction_SIMD)
{
    ConstructionTest<mat4SIMD>();
}

// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename matrix>
void AdditionAssignmentTest(matrix& A, matrix& B)
{

    A += B;
    matrix expA{8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.};
    BOOST_CHECK(checkClose(A, expA));

    // self assignment
    B += B;
    matrix expB{16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4., 12.};
    BOOST_CHECK(checkClose(B, expB));
}


BOOST_FIXTURE_TEST_CASE(Addition_Assignment_Single, Fixture<mat4Single<F32>>)
{
    AdditionAssignmentTest(A, B);
}


BOOST_FIXTURE_TEST_CASE(Addition_Assignment_SIMD, Fixture<mat4SIMD>)
{
    AdditionAssignmentTest(A, B);
}



// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename matrix>
void MultiplicationTest(const matrix& A, const matrix& B)
{
    auto C = A * B;
    matrix expC{29., 137., 245., 353., 31., 115., 199., 283., 23., 91., 159., 227., 30., 118., 206., 294.};
    BOOST_CHECK(checkClose(C, expC));

    auto D = B * A;
    matrix expD{116., 168., 100., 100., 138., 198., 122., 113., 160., 228., 144., 126., 182., 258., 166., 139.};
    BOOST_CHECK(checkClose(D, expD));
}


BOOST_FIXTURE_TEST_CASE(Multiplication_Single, Fixture<mat4Single<F32>>)
{
    MultiplicationTest<mat4Single<F32>>(A, B);
}


BOOST_FIXTURE_TEST_CASE(Multiplication_SIMD, Fixture<mat4SIMD>)
{
    MultiplicationTest<mat4SIMD>(A, B);
}
