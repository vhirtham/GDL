#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/simd/vec4fSSE.h"
#include "gdl/math/serial/vec4Serial.h"

#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;



// Fixture definition -------------------------------------------------------------------------------------------------

template <template <bool> class _vector>
struct Fixture
{
    _vector<true> cA{3., 7., 2., 9.};
    _vector<true> cB{8., 11., 6., 5.};
    _vector<false> rA{3., 7., 2., 9.};
    _vector<false> rB{8., 11., 6., 5.};
};



// Construction -------------------------------------------------------------------------------------------------------

template <typename _vector>
void ConstructionTest()
{
    _vector a;
    BOOST_CHECK(CheckArrayZero(a.Data()));

    _vector b(0., 1., 2., 3.);
    std::array<F32, 4> expB{{0., 1., 2., 3.}};
    BOOST_CHECK(CheckCloseArray(b.Data(), expB));

    _vector b1(expB);
    BOOST_CHECK(CheckCloseArray(b1.Data(), expB));
}



BOOST_AUTO_TEST_CASE(Construction_Serial)
{
    ConstructionTest<Vec4fSerial<true>>();
    ConstructionTest<Vec4fSerial<false>>();
}



BOOST_AUTO_TEST_CASE(Construction_SSE)
{
    ConstructionTest<Vec4fSSE<true>>();
    ConstructionTest<Vec4fSSE<false>>();
}

// Operator[] test ----------------------------------------------------------------------------------------------------

template <bool _isCol, template <bool> class _vector>
void DirectAccessOperatorVectorTest(const _vector<_isCol>& vector)
{
    std::array<F32, 4> data = vector.Data();

    for (U32 i = 0; i < 4; ++i)
        BOOST_CHECK(vector[i] == Approx(data[i]));

    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] F32 test = vector[4], Exception);
}



template <template <bool> class _vector>
void DirectAccessOperatorTest()
{
    Fixture<_vector> fixture;
    DirectAccessOperatorVectorTest<true, _vector>(fixture.cA);
    DirectAccessOperatorVectorTest<true, _vector>(fixture.cB);
    DirectAccessOperatorVectorTest<false, _vector>(fixture.rA);
    DirectAccessOperatorVectorTest<false, _vector>(fixture.rB);
}



BOOST_AUTO_TEST_CASE(Parentheses_Operator_Serial)
{
    DirectAccessOperatorTest<Vec4fSerial>();
}



BOOST_AUTO_TEST_CASE(Parentheses_Operator_SSE)
{
    DirectAccessOperatorTest<Vec4fSSE>();
}



// Comparison ---------------------------------------------------------------------------------------------------------

template <typename _vector>
void ComparisonTest(_vector& A, _vector& B)
{
    BOOST_CHECK(A == A);
    BOOST_CHECK(!(A != A));

    BOOST_CHECK(A != B);
    BOOST_CHECK(!(A == B));


    // Check Tolerances
    constexpr F32 epsilon = std::numeric_limits<F32>::epsilon();
    std::array<F32, 4> vecAData = A.Data();
    std::array<F32, 4> vecEps1Data, vecEps2Data;


    for (U32 i = 0; i < 4; ++i)
    {
        if (static_cast<I32>(vecAData[i]) == 0)
        {
            vecEps1Data[i] = vecAData[i] + epsilon;
            vecEps2Data[i] = vecAData[i] + 10 * epsilon;
        }
        else
        {
            vecEps1Data[i] = vecAData[i] + epsilon * vecAData[i];
            vecEps2Data[i] = vecAData[i] + 10 * epsilon * vecAData[i];
        }
    }

    _vector vecEps1(vecEps1Data);
    _vector vecEps2(vecEps2Data);

    BOOST_CHECK(A == vecEps1);
    BOOST_CHECK(!(A != vecEps1));

    BOOST_CHECK(A != vecEps2);
    BOOST_CHECK(!(A == vecEps2));



    // Only a single value differs
    for (U32 i = 0; i < 4; ++i)
    {
        vecEps1Data = A.Data();
        vecEps2Data = A.Data();

        if (static_cast<I32>(vecAData[i]) == 0)
        {
            vecEps1Data[i] = vecAData[i] + epsilon;
            vecEps2Data[i] = vecAData[i] + 10 * epsilon;
        }
        else
        {
            vecEps1Data[i] = vecAData[i] + epsilon * vecAData[i];
            vecEps2Data[i] = vecAData[i] + 10 * epsilon * vecAData[i];
        }

        vecEps1 = _vector(vecEps1Data);
        vecEps2 = _vector(vecEps2Data);

        BOOST_CHECK(A == vecEps1);
        BOOST_CHECK(!(A != vecEps1));

        BOOST_CHECK(A != vecEps2);
        BOOST_CHECK(!(A == vecEps2));
    }
}



BOOST_FIXTURE_TEST_CASE(Comparison_Serial, Fixture<Vec4fSerial>)
{
    ComparisonTest(cA, cB);
    ComparisonTest(rA, rB);
}



BOOST_FIXTURE_TEST_CASE(Comparison_SSE, Fixture<Vec4fSSE>)
{
    ComparisonTest(cA, cB);
    ComparisonTest(rA, rB);
}



// Dot ----------------------------------------------------------------------------------------------------------------

template <typename _vector, typename _vector2>
void DotProductTest(_vector& A, _vector2& B)
{
    std::array<F32, 4> vecAData = A.Data();
    std::array<F32, 4> vecBData = B.Data();

    F32 expectedResult = 0;
    for (U32 i = 0; i < 4; ++i)
        expectedResult += vecAData[i] * vecBData[i];

    BOOST_CHECK(A.Dot(B) == Approx(expectedResult));
}



BOOST_FIXTURE_TEST_CASE(Dot_Product_Serial, Fixture<Vec4fSerial>)
{
    DotProductTest(cA, cB);
    DotProductTest(cA, rA);
    DotProductTest(rA, rB);
    DotProductTest(rA, cA);
}



BOOST_FIXTURE_TEST_CASE(Dot_Product_SSE, Fixture<Vec4fSSE>)
{
    DotProductTest(cA, cB);
    DotProductTest(cA, rA);
    DotProductTest(rA, rB);
    DotProductTest(rA, cA);
}



// Length -------------------------------------------------------------------------------------------------------------

template <bool _isCol, template <bool> class _vector>
void LengthTest()
{
    _vector<_isCol> vec1{4, 2, 6, 5};
    BOOST_CHECK(vec1.Length() == Approx(9.f));

    for (U32 i = 0; i < 4; ++i)
    {
        std::array<F32, 4> data{{0, 0, 0, 0}};
        data[i] = 1.f;
        _vector<_isCol> vec2{data};
        BOOST_CHECK(vec2.Length() == Approx(1.f));
    }
}



BOOST_AUTO_TEST_CASE(Length_Test_Serial)
{
    LengthTest<true, Vec4fSerial>();
    LengthTest<false, Vec4fSerial>();
}



BOOST_AUTO_TEST_CASE(Length_Test_SSE)
{
    LengthTest<true, Vec4fSSE>();
    LengthTest<false, Vec4fSSE>();
}



// Normalize ----------------------------------------------------------------------------------------------------------

template <typename _vector>
void NormalizeTestVector(_vector& a)
{
    _vector b = a;
    b.Normalize();

    BOOST_CHECK(b.Length() == Approx(1.f));

    F32 lengthA = a.Length();
    for (U32 i = 0; i < 4; ++i)
        BOOST_CHECK(b[i] * lengthA == Approx(a[i]));
}



template <template <bool> class _vector>
void NormalizeTest()
{
    Fixture<_vector> fixture;

    GDL_CHECK_THROW_DEV_DISABLE(_vector<true>().Normalize(), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(_vector<false>().Normalize(), Exception);

    NormalizeTestVector(fixture.cA);
    NormalizeTestVector(fixture.cB);
    NormalizeTestVector(fixture.rA);
    NormalizeTestVector(fixture.rB);
}



BOOST_AUTO_TEST_CASE(Normalize_Serial)
{
    NormalizeTest<Vec4fSerial>();
}



BOOST_AUTO_TEST_CASE(Normalize_SSE)
{
    NormalizeTest<Vec4fSSE>();
}



// Addition assignment ------------------------------------------------------------------------------------------------

template <typename _vector>
void AdditionAssignmentTestVector()
{
    _vector a(8, 3, 7, 2);
    _vector b(3, 2, 5, 7);

    _vector expResult(11, 5, 12, 9);

    b += a;
    BOOST_CHECK(b == expResult);
}



template <template <bool> class _vector>
void AdditionAssignmentTest()
{
    AdditionAssignmentTestVector<_vector<true>>();
    AdditionAssignmentTestVector<_vector<false>>();
}



BOOST_AUTO_TEST_CASE(Addition_Assignment_Serial)
{
    AdditionAssignmentTest<Vec4fSerial>();
}



BOOST_AUTO_TEST_CASE(Addition_Assignment_SSE)
{
    AdditionAssignmentTest<Vec4fSSE>();
}



// Substraction assignment --------------------------------------------------------------------------------------------

template <typename _vector>
void SubstractionAssignmentTestVector()
{
    _vector a(8, 3, 7, 2);
    _vector b(3, 2, 5, 7);

    _vector expResult(-5, -1, -2, 5);

    b -= a;
    BOOST_CHECK(b == expResult);
}



template <template <bool> class _vector>
void SubstractionAssignmentTest()
{
    SubstractionAssignmentTestVector<_vector<true>>();
    SubstractionAssignmentTestVector<_vector<false>>();
}



BOOST_AUTO_TEST_CASE(Substraction_Assignment_Serial)
{
    SubstractionAssignmentTest<Vec4fSerial>();
}



BOOST_AUTO_TEST_CASE(Substraction_Assignment_SSE)
{
    SubstractionAssignmentTest<Vec4fSSE>();
}



// Multiplication with scalar -----------------------------------------------------------------------------------------

template <typename _vector>
void MultiplicationWithScalarTestVector()
{
    _vector a(8, -3, 7, 2);
    _vector b(-3, 2, 5, 7);

    _vector expResultA(16, -6, 14, 4);
    _vector expResultB(-9, 6, 15, 21);
    BOOST_CHECK(a * 2 == expResultA);
    BOOST_CHECK(3 * b == expResultB);
}



template <template <bool> class _vector>
void MultiplicationWithScalarTest()
{
    MultiplicationWithScalarTestVector<_vector<true>>();
    MultiplicationWithScalarTestVector<_vector<false>>();
}



BOOST_AUTO_TEST_CASE(Multiplication_With_Scalar_Serial)
{
    MultiplicationWithScalarTest<Vec4fSerial>();
}



BOOST_AUTO_TEST_CASE(Multiplication_With_Scalar_SSE)
{
    MultiplicationWithScalarTest<Vec4fSSE>();
}
