#include <boost/test/unit_test.hpp>

#include "gdl/base/approx.h"
#include "gdl/math/matXSIMD.inl"
#include "gdl/math/matXSingle.inl"

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
    {
        if
            constexpr(std::is_floating_point<_type>::value) if (a[i] != Approx(b[i])) return false;
        if
            constexpr(std::is_integral<_type>::value) if (a[i] != b[i]) return false;
    }
    return true;
}

template <typename _type>
bool CheckCloseMat(_type a, _type b)
{
    if (a.Rows() != b.Rows())
        return false;
    if (a.Cols() != b.Cols())
        return false;

    for (U32 i = 0; i < a.Rows(); ++i)
        for (U32 j = 0; j < a.Cols(); ++j)
        {
            if
                constexpr(std::is_floating_point<_type>::value)

                        if (a(i, j) != Approx(b(i, j))) return false;

            if
                constexpr(std::is_integral<_type>::value) if (a(i, j) != b(i, j)) return false;
        }
    return true;
}


// Fixture definition %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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


// Construction and Data function %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
template <template <typename, I32, I32> class _matrix, typename _type>
void AssignmentTest()
{
    Fixture<_matrix, _type> f;

    _matrix<_type, 4, 4> A1;
    A1.SetZero();
    A1 = f.A1;
    BOOST_CHECK(CheckCloseMat(A1, f.A1));

    _matrix<_type, 3, 5> A2;
    A2.SetZero();
    A2 = f.A2;
    BOOST_CHECK(CheckCloseMat(A2, f.A2));

    _matrix<_type, 5, 3> B2;
    B2.SetZero();
    B2 = f.B2;
    BOOST_CHECK(CheckCloseMat(B2, f.B2));
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

template <template <typename, I32, I32> class _matrix, typename _type>
void MultiplicationTest()
{
    Fixture<_matrix, _type> f;

    // Square Matrix multiplication
    _matrix<_type, 4, 4> expC1(29., 137., 245., 353., 31., 115., 199., 283., 23., 91., 159., 227., 30., 118., 206.,
                               294.);
    _matrix<_type, 4, 4> C1 = f.A1 * f.B1;
    BOOST_CHECK(CheckCloseMat(C1, expC1));

    _matrix<_type, 4, 4> expD1(116., 168., 100., 100., 138., 198., 122., 113., 160., 228., 144., 126., 182., 258., 166.,
                               139.);
    _matrix<_type, 4, 4> D1 = f.B1 * f.A1;
    BOOST_CHECK(CheckCloseMat(D1, expD1));

    // Non square matrix multiplication
    _matrix<_type, 3, 3> expC2(49., 209., 369., 52., 182., 312., 46., 161., 276.);
    _matrix<_type, 3, 3> C2 = f.A2 * f.B2;
    BOOST_CHECK(CheckCloseMat(C2, expC2));

    _matrix<_type, 5, 5> expD2(70., 65., 75., 95., 55., 87., 87., 90., 108., 69., 104., 109., 105., 121., 83., 121.,
                               131., 120., 134., 97., 138., 153., 135., 147., 111.);
    _matrix<_type, 5, 5> D2 = f.B2 * f.A2;
    BOOST_CHECK(CheckCloseMat(D2, expD2));

    _matrix<_type, 2, 2> expC3(119, 425, 570, 1458);
    _matrix<_type, 2, 2> C3 = f.A3 * f.B3;
    BOOST_CHECK(CheckCloseMat(C3, expC3));


    _matrix<_type, 6, 6> expD3(185, 47, 237, 42, 505, 71, 232, 64, 276, 48, 578, 88, 279, 81, 315, 54, 651, 105, 326,
                               98, 354, 60, 724, 122, 373, 115, 393, 66, 797, 139, 420, 132, 432, 72, 870, 156);
    _matrix<_type, 6, 6> D3 = f.B3 * f.A3;
    BOOST_CHECK(CheckCloseMat(D3, expD3));
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

template <template <typename, I32, I32> class _matrix, typename _type>
void AdditionAssignmentTest()
{
    Fixture<_matrix, _type> f;

    // Square Matrix addition
    _matrix<_type, 4, 4> expA1(8., 15., 14., 14., 6., 9., 18., 16., 5., 13., 15., 16., 9., 15., 13., 21.);
    f.A1 += f.B1;
    BOOST_CHECK(CheckCloseMat(f.A1, expA1));


    _matrix<_type, 4, 4> expB1(16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4., 12.);
    f.B1 += f.B1;
    BOOST_CHECK(CheckCloseMat(f.B1, expB1));

    // Non square matrix addition
    _matrix<_type, 3, 5> B2(f.B2.Data());
    _matrix<_type, 3, 5> expA2(8., 16., 16., 3., 11., 15., 11., 10., 15., 10., 13., 15., 10., 17., 16.);
    f.A2 += B2;
    BOOST_CHECK(CheckCloseMat(f.A2, expA2));

    _matrix<_type, 3, 5> expB2(16., 22., 12., 4., 10., 8., 18., 6., 6., 14., 10., 4., 12., 16., 4.);
    B2 += B2;
    BOOST_CHECK(CheckCloseMat(B2, expB2));
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
