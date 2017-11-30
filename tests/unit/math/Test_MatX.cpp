#include <boost/test/unit_test.hpp>

#include "math/matXSIMD.inl"
#include "math/matXSingle.inl"

#include <cmath>
#ifndef NDEBUG
#include <iostream>
#endif
using namespace GDL;

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

BOOST_AUTO_TEST_CASE(Construction_Single)
{

    std::array<F32, 15> expA = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.}};
    matXSingle<F32, 3, 5> A(expA);
    BOOST_CHECK(checkCloseArray(A.Data(), expA));

    std::array<F32, 15> expB = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.}};
    matXSingle<F32, 5, 3> B(expB);
    BOOST_CHECK(checkCloseArray(B.Data(), expB));

    std::array<F32, 16> expC = {{0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.}};
    matXSingle<F32, 4, 4> C(expC);
    BOOST_CHECK(checkCloseArray(C.Data(), expC));
}

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

    matXSingle<F32, 4, 4> expD1(std::array<F32, 16>{
            {116., 168., 100., 100., 138., 198., 122., 113., 160., 228., 144., 126., 182., 258., 166., 139.}});
    matXSingle<F32, 4, 4> D1 = B1 * A1;
    BOOST_CHECK(checkCloseMat(D1, expD1));


    // Non square matrix multiplication
    matXSingle<F32, 3, 5> A2(std::array<F32, 15>{{0., 5., 10., 1., 6., 11., 2., 7., 12., 3., 8., 13., 4., 9., 14.}});
    matXSingle<F32, 5, 3> B2(std::array<F32, 15>{{8., 11., 6., 2., 5., 4., 9., 3., 3., 7., 5., 2., 6., 8., 2.}});

    matXSingle<F32, 3, 3> expC2(std::array<F32, 9>{{49., 209., 369., 52., 182., 312., 46., 161., 276.}});
    matXSingle<F32, 3, 3> C2 = A2 * B2;
    BOOST_CHECK(checkCloseMat(C2, expC2));

    matXSingle<F32, 5, 5> expD2(
            std::array<F32, 25>{{70.,  65., 75.,  95.,  55.,  87.,  87., 90.,  108., 69.,  104., 109., 105.,
                                 121., 83., 121., 131., 120., 134., 97., 138., 153., 135., 147., 111.}});
    matXSingle<F32, 5, 5> D2 = B2 * A2;
    BOOST_CHECK(checkCloseMat(D2, expD2));

#ifndef NDEBUG
    std::cout << A2 << std::endl;
    std::cout << B2 << std::endl;
    std::cout << C2 << std::endl;
    std::cout << expC2 << std::endl;
#endif
}
