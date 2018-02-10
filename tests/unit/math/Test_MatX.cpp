#include <boost/test/unit_test.hpp>

#include "math/matXSIMD.inl"
#include "math/matXSingle.inl"

#include <cmath>
#ifndef NDEBUG
#include <iostream>
#endif
using namespace GDL;

// Helper functions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

template <typename T>
bool checkCloseArray(const T& a, const T& b, F32 tolerance = 1e-6)
{
    for (U32 i = 0; i < a.size(); ++i)
        if (std::fabs(a[i] - b[i]) > tolerance)
            return false;
    return true;
}

template <typename T>
bool checkCloseMat(T a, T b, F32 tolerance = 1e-6)
{
    for (U32 i = 0; i < a.Rows(); ++i)
        for (U32 j = 0; j < a.Cols(); ++j)
            if (std::fabs(a(i, j) - b(i, j)) > tolerance)
                return false;
    return true;
}


// Construction %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
BOOST_AUTO_TEST_CASE(Construction_Single)
{
    // array ctor
    std::array<F32, 15> expA = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.}};
    matXSingle<F32, 3, 5> A(expA);
    BOOST_CHECK(checkCloseArray(A.Data(), expA));

    std::array<F32, 15> expB = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.}};
    matXSingle<F32, 5, 3> B(expB);
    BOOST_CHECK(checkCloseArray(B.Data(), expB));

    std::array<F32, 16> expC = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.}};
    matXSingle<F32, 4, 4> C(expC);
    BOOST_CHECK(checkCloseArray(C.Data(), expC));

    // variadic ctor
    matXSingle<F32, 3, 5> A2(0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.);
    BOOST_CHECK(checkCloseArray(A2.Data(), expA));
}


BOOST_AUTO_TEST_CASE(Construction_SSE)
{

    std::array<F32, 15> expA = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.}};
    matXSIMD<3, 5> A(expA);
    BOOST_CHECK(checkCloseArray(A.Data(), expA));

    std::array<F32, 15> expB = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.}};
    matXSIMD<5, 3> B(expB);
    BOOST_CHECK(checkCloseArray(B.Data(), expB));

    std::array<F32, 16> expC = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.}};
    matXSIMD<4, 4> C(expC);
    BOOST_CHECK(checkCloseArray(C.Data(), expC));

    // variadic ctor
    matXSIMD<3, 5> A2(0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.);
    BOOST_CHECK(checkCloseArray(A2.Data(), expA));
}

// Multiplication %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BOOST_AUTO_TEST_CASE(Multiplication_Single)
{
    // Square Matrix multiplication
    matXSingle<F32, 4, 4> A1(
            std::array<F32, 16>{{0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.}});
    matXSingle<F32, 4, 4> B1(std::array<F32, 16>{{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2., 6.}});

    matXSingle<F32, 4, 4> expC1(std::array<F32, 16>{
            {29., 137., 245., 353., 31., 115., 199., 283., 23., 91., 159., 227., 30., 118., 206., 294.}});
    matXSingle<F32, 4, 4> C1 = A1 * B1;
    BOOST_CHECK(checkCloseMat(C1, expC1));
    BOOST_CHECK(checkCloseArray(C1.Data(), expC1.Data()));

    matXSingle<F32, 4, 4> expD1(std::array<F32, 16>{
            {116., 168., 100., 100., 138., 198., 122., 113., 160., 228., 144., 126., 182., 258., 166., 139.}});
    matXSingle<F32, 4, 4> D1 = B1 * A1;
    BOOST_CHECK(checkCloseMat(D1, expD1));
    BOOST_CHECK(checkCloseArray(D1.Data(), expD1.Data()));

    // Non square matrix multiplication
    matXSingle<F32, 3, 5> A2(std::array<F32, 15>{{0., 5., 10., 1., 6., 11., 2., 7., 12., 3., 8., 13., 4., 9., 14.}});
    matXSingle<F32, 5, 3> B2(std::array<F32, 15>{{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2.}});

    matXSingle<F32, 3, 3> expC2(std::array<F32, 9>{{49., 209., 369., 52., 182., 312., 46., 161., 276.}});
    matXSingle<F32, 3, 3> C2 = A2 * B2;
    BOOST_CHECK(checkCloseMat(C2, expC2));
    BOOST_CHECK(checkCloseArray(C2.Data(), expC2.Data()));

    matXSingle<F32, 5, 5> expD2(
            std::array<F32, 25>{{70.,  65., 75.,  95.,  55.,  87.,  87., 90.,  108., 69.,  104., 109., 105.,
                                 121., 83., 121., 131., 120., 134., 97., 138., 153., 135., 147., 111.}});
    matXSingle<F32, 5, 5> D2 = B2 * A2;
    BOOST_CHECK(checkCloseMat(D2, expD2));
    BOOST_CHECK(checkCloseArray(D2.Data(), expD2.Data()));
    //#ifndef NDEBUG;
    //    std::cout << A2 << std::endl;
    //    std::cout << B2 << std::endl;
    //    std::cout << C2 << std::endl;
    //    std::cout << expC2 << std::endl;
    //#endif
}


BOOST_AUTO_TEST_CASE(Multiplication_SIMD)
{
    // Square Matrix multiplication
    matXSIMD<4, 4> A1(std::array<F32, 16>{{0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.}});
    matXSIMD<4, 4> B1(std::array<F32, 16>{{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2., 6.}});

    matXSIMD<4, 4> expC1(std::array<F32, 16>{
            {29., 137., 245., 353., 31., 115., 199., 283., 23., 91., 159., 227., 30., 118., 206., 294.}});
    matXSIMD<4, 4> C1 = A1 * B1;
    BOOST_CHECK(checkCloseMat(C1, expC1));
    BOOST_CHECK(checkCloseArray(C1.Data(), expC1.Data()));

    matXSIMD<4, 4> expD1(std::array<F32, 16>{
            {116., 168., 100., 100., 138., 198., 122., 113., 160., 228., 144., 126., 182., 258., 166., 139.}});
    matXSIMD<4, 4> D1 = B1 * A1;
    BOOST_CHECK(checkCloseMat(D1, expD1));
    BOOST_CHECK(checkCloseArray(D1.Data(), expD1.Data()));

    // Non square matrix multiplication
    matXSIMD<3, 5> A2(std::array<F32, 15>{{0., 5., 10., 1., 6., 11., 2., 7., 12., 3., 8., 13., 4., 9., 14.}});
    matXSIMD<5, 3> B2(std::array<F32, 15>{{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2.}});

    matXSIMD<3, 3> expC2(std::array<F32, 9>{{49., 209., 369., 52., 182., 312., 46., 161., 276.}});
    matXSIMD<3, 3> C2 = A2 * B2;
    BOOST_CHECK(checkCloseMat(C2, expC2));
    BOOST_CHECK(checkCloseArray(C2.Data(), expC2.Data()));

    matXSIMD<5, 5> expD2(
            std::array<F32, 25>{{70.,  65., 75.,  95.,  55.,  87.,  87., 90.,  108., 69.,  104., 109., 105.,
                                 121., 83., 121., 131., 120., 134., 97., 138., 153., 135., 147., 111.}});
    matXSIMD<5, 5> D2 = B2 * A2;
    BOOST_CHECK(checkCloseMat(D2, expD2));
    BOOST_CHECK(checkCloseArray(D2.Data(), expD2.Data()));

    //#ifndef NDEBUG
    //    std::cout << A2 << std::endl;
    //    std::cout << B2 << std::endl;
    //    std::cout << C2 << std::endl;
    //    std::cout << expC2 << std::endl;
    //#endif
}

// Addition Assignment %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BOOST_AUTO_TEST_CASE(Addition_Assignment_Single)
{
    // Square Matrix addition
    matXSingle<F32, 4, 4> A1(
            std::array<F32, 16>{{0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.}});
    matXSingle<F32, 4, 4> B1(std::array<F32, 16>{{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2., 6.}});

    matXSingle<F32, 4, 4> expA1(
            std::array<F32, 16>{{8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.}});
    A1 += B1;
    BOOST_CHECK(checkCloseMat(A1, expA1));
    BOOST_CHECK(checkCloseArray(A1.Data(), expA1.Data()));


    matXSingle<F32, 4, 4> expB1(
            std::array<F32, 16>{{16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4., 12.}});
    B1 += B1;
    BOOST_CHECK(checkCloseMat(B1, expB1));
    BOOST_CHECK(checkCloseArray(B1.Data(), expB1.Data()));

    // Non square matrix addition
    matXSingle<F32, 3, 5> A2(std::array<F32, 15>{{0., 5., 10., 1., 6., 11., 2., 7., 12., 3., 8., 13., 4., 9., 14.}});
    matXSingle<F32, 3, 5> B2(std::array<F32, 15>{{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2.}});

    matXSingle<F32, 3, 5> expA2(
            std::array<F32, 15>{{8., 16., 16., 3., 11., 15., 11., 10., 15., 10., 13., 15., 10., 17., 16.}});
    A2 += B2;
    BOOST_CHECK(checkCloseMat(A2, expA2));
    BOOST_CHECK(checkCloseArray(A2.Data(), expA2.Data()));

    matXSingle<F32, 3, 5> expB2(
            std::array<F32, 15>{{16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4.}});
    B2 += B2;
    BOOST_CHECK(checkCloseMat(B2, expB2));
    BOOST_CHECK(checkCloseArray(B2.Data(), expB2.Data()));
}


BOOST_AUTO_TEST_CASE(Addition_Assignment_SIMD)
{
    // Square Matrix addition
    matXSIMD<4, 4> A1(std::array<F32, 16>{{0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.}});
    matXSIMD<4, 4> B1(std::array<F32, 16>{{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2., 6.}});

    matXSIMD<4, 4> expA1(
            std::array<F32, 16>{{8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.}});
    A1 += B1;
    BOOST_CHECK(checkCloseMat(A1, expA1));
    BOOST_CHECK(checkCloseArray(A1.Data(), expA1.Data()));


    matXSIMD<4, 4> expB1(
            std::array<F32, 16>{{16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4., 12.}});
    B1 += B1;
    BOOST_CHECK(checkCloseMat(B1, expB1));
    BOOST_CHECK(checkCloseArray(B1.Data(), expB1.Data()));


    // Non square matrix addition
    matXSIMD<3, 5> A2(std::array<F32, 15>{{0., 5., 10., 1., 6., 11., 2., 7., 12., 3., 8., 13., 4., 9., 14.}});
    matXSIMD<3, 5> B2(std::array<F32, 15>{{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2.}});

    matXSIMD<3, 5> expA2(
            std::array<F32, 15>{{8., 16., 16., 3., 11., 15., 11., 10., 15., 10., 13., 15., 10., 17., 16.}});
    A2 += B2;
    BOOST_CHECK(checkCloseMat(A2, expA2));
    BOOST_CHECK(checkCloseArray(A2.Data(), expA2.Data()));

    matXSIMD<3, 5> expB2(std::array<F32, 15>{{16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4.}});
    B2 += B2;
    BOOST_CHECK(checkCloseMat(B2, expB2));
    BOOST_CHECK(checkCloseArray(B2.Data(), expB2.Data()));
}
