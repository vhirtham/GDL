#include <boost/test/unit_test.hpp>


#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/simd/vec3fSSE.h"
#include "gdl/math/serial/vec3Serial.h"
#include "gdl/math/simd/vec4fSSE.h"
#include "gdl/math/serial/vec4Serial.h"

#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;



// Fixture definition -------------------------------------------------------------------------------------------------

template <template <bool> class _vector>
struct Fixture
{
    _vector<true> cA{3., 7., 2.};
    _vector<true> cB{8., 11., 6.};
    _vector<false> rA{3., 7., 2.};
    _vector<false> rB{8., 11., 6.};
};



// Construction -------------------------------------------------------------------------------------------------------

template <typename _vector>
constexpr auto GetVec4()
{
    if constexpr (std::is_same<_vector, Vec3fSSE<true>>::value)
        return Vec4fSSE<true>();
    if constexpr (std::is_same<_vector, Vec3fSSE<false>>::value)
        return Vec4fSSE<false>();
    if constexpr (std::is_same<_vector, Vec3fSerial<true>>::value)
        return Vec4fSerial<true>();
    if constexpr (std::is_same<_vector, Vec3fSerial<false>>::value)
        return Vec4fSerial<false>();
}



template <typename _vector>
void ConstructionTest()
{
    _vector a;
    BOOST_CHECK(CheckArrayZero(a.Data()));

    _vector b(0., 1., 2.);
    std::array<F32, 3> expB{{0., 1., 2.}};
    BOOST_CHECK(CheckCloseArray(b.Data(), expB));

    _vector b1(expB);
    BOOST_CHECK(CheckCloseArray(b1.Data(), expB));

    using _vector4 = decltype(GetVec4<_vector>());

    _vector c(_vector4(3.f, 5.f, 1.f, 2.f));
    std::array<F32, 3> expC{{3.f, 5.f, 1.f}};
    BOOST_CHECK(CheckCloseArray(c.Data(), expC));
}



BOOST_AUTO_TEST_CASE(Construction_Serial)
{
    ConstructionTest<Vec3Serial<F32, true>>();
    ConstructionTest<Vec3Serial<F32, false>>();
}



BOOST_AUTO_TEST_CASE(Construction_SSE)
{
    ConstructionTest<Vec3fSSE<true>>();
    ConstructionTest<Vec3fSSE<false>>();
}



// Operator[] test ----------------------------------------------------------------------------------------------------

template <bool _isCol, template <bool> class _vector>
void DirectAccessOperatorVectorTest(const _vector<_isCol>& vector)
{
    std::array<F32, 3> data = vector.Data();

    for (U32 i = 0; i < 3; ++i)
        BOOST_CHECK(vector[i] == Approx(data[i]));

    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] F32 test = vector[3], Exception);
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
    DirectAccessOperatorTest<Vec3fSerial>();
}



BOOST_AUTO_TEST_CASE(Parentheses_Operator_SSE)
{
    DirectAccessOperatorTest<Vec3fSSE>();
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
    std::array<F32, 3> vecAData = A.Data();
    std::array<F32, 3> vecEps1Data, vecEps2Data;


    for (U32 i = 0; i < 3; ++i)
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
    for (U32 i = 0; i < 3; ++i)
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



BOOST_FIXTURE_TEST_CASE(Comparison_Serial, Fixture<Vec3fSerial>)
{
    ComparisonTest(cA, cB);
    ComparisonTest(rA, rB);
}



BOOST_FIXTURE_TEST_CASE(Comparison_SSE, Fixture<Vec3fSSE>)
{
    ComparisonTest(cA, cB);
    ComparisonTest(rA, rB);
}



// Dot ----------------------------------------------------------------------------------------------------------------

template <typename _vector, typename _vector2>
void DotProductTest(_vector& A, _vector2& B)
{
    std::array<F32, 3> vecAData = A.Data();
    std::array<F32, 3> vecBData = B.Data();

    F32 expectedResult = 0;
    for (U32 i = 0; i < 3; ++i)
        expectedResult += vecAData[i] * vecBData[i];

    BOOST_CHECK(A.Dot(B) == Approx(expectedResult));
}



BOOST_FIXTURE_TEST_CASE(Dot_Product_Serial, Fixture<Vec3fSerial>)
{
    DotProductTest(cA, cB);
    DotProductTest(cA, rA);
    DotProductTest(rA, rB);
    DotProductTest(rA, cA);
}



BOOST_FIXTURE_TEST_CASE(Dot_Product_SSE, Fixture<Vec3fSSE>)
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
    _vector<_isCol> vec1{3, 2, 6};
    BOOST_CHECK(vec1.Length() == Approx(7.f));

    for (U32 i = 0; i < 3; ++i)
    {
        std::array<F32, 3> data{{0, 0, 0}};
        data[i] = 1.f;
        _vector<_isCol> vec2{data};
        BOOST_CHECK(vec2.Length() == Approx(1.f));
    }
}



BOOST_AUTO_TEST_CASE(Length_Test_Serial)
{
    LengthTest<true, Vec3fSerial>();
    LengthTest<false, Vec3fSerial>();
}



BOOST_AUTO_TEST_CASE(Length_Test_SSE)
{
    LengthTest<true, Vec3fSSE>();
    LengthTest<false, Vec3fSSE>();
}



// Normalize ----------------------------------------------------------------------------------------------------------

template <typename _vector>
void NormalizeTestVector(_vector& a)
{
    _vector b = a;
    b.Normalize();

    BOOST_CHECK(b.Length() == Approx(1.f));

    F32 lengthA = a.Length();
    for (U32 i = 0; i < 3; ++i)
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
    NormalizeTest<Vec3fSerial>();
}



BOOST_AUTO_TEST_CASE(Normalize_SSE)
{
    NormalizeTest<Vec3fSSE>();
}



// Cross --------------------------------------------------------------------------------------------------------------

template <typename _vector>
void CrossProductTest()
{
    _vector a1{7, 1, 0};
    _vector a2{2, 5, 0};

    _vector aExpNorm{0, 0, 1};
    _vector aCross = a1.Cross(a2);

    BOOST_CHECK(aCross.Normalize() == aExpNorm);

    _vector b1{2, 0, 2};
    _vector b2{0, 2, 0};

    _vector bExpNorm{_vector(-2, 0, 2).Normalize()};
    _vector bCross = b1.Cross(b2);

    BOOST_CHECK(bCross.Normalize() == bExpNorm);

    _vector zeroVec;

    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] _vector tmp = zeroVec.Cross(a1), Exception);
    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] _vector tmp = a1.Cross(zeroVec), Exception);
}



BOOST_FIXTURE_TEST_CASE(Cross_Product_Serial, Fixture<Vec3fSerial>)
{
    CrossProductTest<Vec3fSerial<true>>();
    CrossProductTest<Vec3fSerial<false>>();
}



BOOST_FIXTURE_TEST_CASE(Cross_Product_SSE, Fixture<Vec3fSSE>)
{
    CrossProductTest<Vec3fSSE<true>>();
    CrossProductTest<Vec3fSSE<false>>();
}



// Addition assignment ------------------------------------------------------------------------------------------------

template <typename _vector>
void AdditionAssignmentTestVector()
{
    _vector a(8, 3, 7);
    _vector b(3, 2, 5);

    _vector expResult(11, 5, 12);

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
    AdditionAssignmentTest<Vec3fSerial>();
}



BOOST_AUTO_TEST_CASE(Addition_Assignment_SSE)
{
    AdditionAssignmentTest<Vec3fSSE>();
}



// Substraction assignment --------------------------------------------------------------------------------------------

template <typename _vector>
void SubstractionAssignmentTestVector()
{
    _vector a(8, 3, 7);
    _vector b(3, 2, 5);

    _vector expResult(-5, -1, -2);

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
    SubstractionAssignmentTest<Vec3fSerial>();
}



BOOST_AUTO_TEST_CASE(Substraction_Assignment_SSE)
{
    SubstractionAssignmentTest<Vec3fSSE>();
}



// Addition -----------------------------------------------------------------------------------------------------------

template <typename _vector>
void AdditionTestVector()
{
    _vector a(8, 3, 7);
    _vector b(3, 2, 5);
    _vector expResult(11, 5, 12);

    BOOST_CHECK(a + b == expResult);
}



template <template <bool> class _vector>
void AdditionTest()
{
    AdditionTestVector<_vector<true>>();
    AdditionTestVector<_vector<false>>();
}



BOOST_AUTO_TEST_CASE(Addition_Serial)
{
    AdditionTest<Vec3fSerial>();
}



BOOST_AUTO_TEST_CASE(Addition_SSE)
{
    AdditionTest<Vec3fSSE>();
}



// Substraction -------------------------------------------------------------------------------------------------------

template <typename _vector>
void SubstractionTestVector()
{
    _vector a(8, 3, 7);
    _vector b(3, 2, 5);

    _vector expResult(-5, -1, -2);

    BOOST_CHECK(b - a == expResult);
}



template <template <bool> class _vector>
void SubstractionTest()
{
    SubstractionTestVector<_vector<true>>();
    SubstractionTestVector<_vector<false>>();
}



BOOST_AUTO_TEST_CASE(Substraction_Serial)
{
    SubstractionTest<Vec3fSerial>();
}



BOOST_AUTO_TEST_CASE(Substraction_SSE)
{
    SubstractionTest<Vec3fSSE>();
}



// Multiplication with scalar -----------------------------------------------------------------------------------------

template <typename _vector>
void MultiplicationWithScalarTestVector()
{
    _vector a(8, -3, 7);
    _vector b(-3, 2, 5);

    _vector expResultA(16, -6, 14);
    _vector expResultB(-9, 6, 15);
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
    MultiplicationWithScalarTest<Vec3fSerial>();
}



BOOST_AUTO_TEST_CASE(Multiplication_With_Scalar_SSE)
{
    MultiplicationWithScalarTest<Vec3fSSE>();
}
