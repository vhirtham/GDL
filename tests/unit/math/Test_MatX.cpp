#include <boost/test/unit_test.hpp>

#include "gdl/math/matXSIMD.inl"
#include "gdl/math/matXSingle.inl"

#include <cmath>
#ifndef NDEBUG
#include <iostream>
#endif
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
bool checkCloseMat(T a, T b, F32 tolerance = 1e-6)
{
    if (a.Rows() != b.Rows())
        return false;
    if (a.Cols() != b.Cols())
        return false;

    for (U32 i = 0; i < a.Rows(); ++i)
        for (U32 j = 0; j < a.Cols(); ++j)
            if (std::fabs(a(i, j) - b(i, j)) > tolerance)
                return false;
    return true;
}


// Fixture definition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <template <typename, int, int> class matrix, typename T>
struct Fixture
{
    matrix<T, 4, 4> A1 = matrix<T, 4, 4>(0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.);
    matrix<T, 4, 4> B1 = matrix<T, 4, 4>(8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2., 6.);
    matrix<T, 3, 5> A2 = matrix<T, 3, 5>(0., 5., 10., 1., 6., 11., 2., 7., 12., 3., 8., 13., 4., 9., 14.);
    matrix<T, 5, 3> B2 = matrix<T, 5, 3>(8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2.);
};


// Construction and Data function %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <template <typename, int, int> class matrix, typename T>
void CtorDataTest()
{
    // array ctor
    std::array<T, 15> expA = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.}};
    matrix<T, 3, 5> A(expA);
    BOOST_CHECK(checkCloseArray(A.Data(), expA));

    std::array<T, 15> expB = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.}};
    matrix<T, 5, 3> B(expB);
    BOOST_CHECK(checkCloseArray(B.Data(), expB));

    std::array<T, 16> expC = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.}};
    matrix<T, 4, 4> C(expC);
    BOOST_CHECK(checkCloseArray(C.Data(), expC));

    // variadic ctor
    matrix<T, 3, 5> A2(0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.);
    BOOST_CHECK(checkCloseArray(A2.Data(), expA));
}

BOOST_AUTO_TEST_CASE(Construction_Single)
{
    CtorDataTest<matXSingle, F32>();
    CtorDataTest<matXSingle, F64>();
}


BOOST_AUTO_TEST_CASE(Construction_SSE)
{
    CtorDataTest<matXSIMD, F32>();
    CtorDataTest<matXSIMD, F64>();
}

// Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <template <typename, int, int> class matrix, typename T>
void AssignmentTest()
{
    Fixture<matrix, T> f;

    matrix<T, 4, 4> A1;
    A1.SetZero();
    A1 = f.A1;
    BOOST_CHECK(checkCloseMat(A1, f.A1));

    matrix<T, 3, 5> A2;
    A2.SetZero();
    A2 = f.A2;
    BOOST_CHECK(checkCloseMat(A2, f.A2));

    matrix<T, 5, 3> B2;
    B2.SetZero();
    B2 = f.B2;
    BOOST_CHECK(checkCloseMat(B2, f.B2));
}

BOOST_AUTO_TEST_CASE(Assignment_Single)
{
    AssignmentTest<matXSingle, F32>();
    AssignmentTest<matXSingle, F64>();
}


BOOST_AUTO_TEST_CASE(Assignment_SIMD)
{
    AssignmentTest<matXSIMD, F32>();
    AssignmentTest<matXSIMD, F64>();
}

// Set zero %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <template <typename, int, int> class matrix, typename T>
void SetZeroTest()
{
    Fixture<matrix, T> f;
    f.A1.SetZero();
    BOOST_CHECK(checkArrayZero(f.A1.Data()));

    f.B1.SetZero();
    BOOST_CHECK(checkArrayZero(f.B1.Data()));

    f.A2.SetZero();
    BOOST_CHECK(checkArrayZero(f.A2.Data()));

    f.B2.SetZero();
    BOOST_CHECK(checkArrayZero(f.B2.Data()));
}

BOOST_AUTO_TEST_CASE(SetZero_Single)
{
    SetZeroTest<matXSingle, F32>();
    SetZeroTest<matXSingle, F64>();
}


BOOST_AUTO_TEST_CASE(SetZero_SIMD)
{
    SetZeroTest<matXSIMD, F32>();
    SetZeroTest<matXSIMD, F64>();
}

// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <template <typename, int, int> class matrix, typename T>
void MultiplicationTest()
{
    Fixture<matrix, T> f;

    // Square Matrix multiplication
    matrix<T, 4, 4> expC1(29., 137., 245., 353., 31., 115., 199., 283., 23., 91., 159., 227., 30., 118., 206., 294.);
    matrix<T, 4, 4> C1 = f.A1 * f.B1;
    BOOST_CHECK(checkCloseMat(C1, expC1));

    matrix<T, 4, 4> expD1(116., 168., 100., 100., 138., 198., 122., 113., 160., 228., 144., 126., 182., 258., 166.,
                          139.);
    matrix<T, 4, 4> D1 = f.B1 * f.A1;
    BOOST_CHECK(checkCloseMat(D1, expD1));

    // Non square matrix multiplication
    matrix<T, 3, 3> expC2(49., 209., 369., 52., 182., 312., 46., 161., 276.);
    matrix<T, 3, 3> C2 = f.A2 * f.B2;
    BOOST_CHECK(checkCloseMat(C2, expC2));

    matrix<T, 5, 5> expD2(70., 65., 75., 95., 55., 87., 87., 90., 108., 69., 104., 109., 105., 121., 83., 121., 131.,
                          120., 134., 97., 138., 153., 135., 147., 111.);
    matrix<T, 5, 5> D2 = f.B2 * f.A2;
    BOOST_CHECK(checkCloseMat(D2, expD2));
}

BOOST_AUTO_TEST_CASE(Multiplication_Single)
{
    MultiplicationTest<matXSingle, F32>();
    MultiplicationTest<matXSingle, F64>();
}


BOOST_AUTO_TEST_CASE(Multiplication_SIMD)
{
    MultiplicationTest<matXSIMD, F32>();
    MultiplicationTest<matXSIMD, F64>();
}

// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <template <typename, int, int> class matrix, typename T>
void AdditionAssignmentTest()
{
    Fixture<matrix, T> f;

    // Square Matrix addition
    matrix<T, 4, 4> expA1(8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.);
    f.A1 += f.B1;
    BOOST_CHECK(checkCloseMat(f.A1, expA1));


    matrix<T, 4, 4> expB1(16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4., 12.);
    f.B1 += f.B1;
    BOOST_CHECK(checkCloseMat(f.B1, expB1));

    // Non square matrix addition
    matrix<T, 3, 5> B2(f.B2.Data());
    matrix<T, 3, 5> expA2(8., 16., 16., 3., 11., 15., 11., 10., 15., 10., 13., 15., 10., 17., 16.);
    f.A2 += B2;
    BOOST_CHECK(checkCloseMat(f.A2, expA2));

    matrix<T, 3, 5> expB2(16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4.);
    B2 += B2;
    BOOST_CHECK(checkCloseMat(B2, expB2));
}

BOOST_AUTO_TEST_CASE(Addition_Assignment_Single)
{
    AdditionAssignmentTest<matXSingle, F32>();
    AdditionAssignmentTest<matXSingle, F64>();
}


BOOST_AUTO_TEST_CASE(Addition_Assignment_SIMD)
{
    AdditionAssignmentTest<matXSIMD, F32>();
    AdditionAssignmentTest<matXSIMD, F64>();
}
