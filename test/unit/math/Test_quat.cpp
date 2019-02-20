#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/sse/quatfSSE.h"
#include "gdl/math/single/quatSingle.h"

#include "test/tools/arrayValueComparison.h"
#include "test/tools/ExceptionChecks.h"

using namespace GDL;



// Fixture definition -------------------------------------------------------------------------------------------------

template <typename _quaternion>
struct Fixture
{
    _quaternion A{3., 7., 2., 9.};
    _quaternion B{8., 11., 6., 5.};
};



// Construction -------------------------------------------------------------------------------------------------------

template <typename _quaternion>
void ConstructionTest()
{
    _quaternion a;
    BOOST_CHECK(CheckArrayZero(a.Data()));

    _quaternion b(0., 1., 2., 3.);
    std::array<F32, 4> expB{{0., 1., 2., 3.}};
    BOOST_CHECK(CheckCloseArray(b.Data(), expB));

    _quaternion b1(expB);
    BOOST_CHECK(CheckCloseArray(b1.Data(), expB));
}



BOOST_AUTO_TEST_CASE(Construction_Single)
{
    ConstructionTest<QuatfSingle>();
}



BOOST_AUTO_TEST_CASE(Construction_SSE)
{
    ConstructionTest<QuatfSSE>();
}

// Operator[] test ----------------------------------------------------------------------------------------------------

template <typename _quaternion>
void DirectAccessOperatorVectorTest(const _quaternion& quaternion)
{
    std::array<F32, 4> data = quaternion.Data();

    for (U32 i = 0; i < 4; ++i)
        BOOST_CHECK(quaternion[i] == Approx(data[i]));

    GDL_CHECK_THROW_DEV_DISABLE([[maybe_unused]] F32 test = quaternion[4], Exception);
}



template <typename _quaternion>
void DirectAccessOperatorTest()
{
    Fixture<_quaternion> fixture;
    DirectAccessOperatorVectorTest<_quaternion>(fixture.A);
    DirectAccessOperatorVectorTest<_quaternion>(fixture.B);
}



BOOST_AUTO_TEST_CASE(Parentheses_Operator_Single)
{
    DirectAccessOperatorTest<QuatfSingle>();
}



BOOST_AUTO_TEST_CASE(Parentheses_Operator_SSE)
{
    DirectAccessOperatorTest<QuatfSSE>();
}



// Comparison ---------------------------------------------------------------------------------------------------------

template <typename _quaternion>
void ComparisonTest(_quaternion& A, _quaternion& B)
{
    BOOST_CHECK(A == A);
    BOOST_CHECK(!(A != A));

    BOOST_CHECK(A != B);
    BOOST_CHECK(!(A == B));


    // Check Tolerances
    constexpr F32 epsilon = std::numeric_limits<F32>::epsilon();
    std::array<F32, 4> quatAData = A.Data();
    std::array<F32, 4> quatEps1Data, quatEps2Data;


    for (U32 i = 0; i < 4; ++i)
    {
        if (static_cast<I32>(quatAData[i]) == 0)
        {
            quatEps1Data[i] = quatAData[i] + epsilon;
            quatEps2Data[i] = quatAData[i] + 10 * epsilon;
        }
        else
        {
            quatEps1Data[i] = quatAData[i] + epsilon * quatAData[i];
            quatEps2Data[i] = quatAData[i] + 10 * epsilon * quatAData[i];
        }
    }

    _quaternion quatEps1(quatEps1Data);
    _quaternion quatEps2(quatEps2Data);

    BOOST_CHECK(A == quatEps1);
    BOOST_CHECK(!(A != quatEps1));

    BOOST_CHECK(A != quatEps2);
    BOOST_CHECK(!(A == quatEps2));



    // Only a single value differs
    for (U32 i = 0; i < 4; ++i)
    {
        quatEps1Data = A.Data();
        quatEps2Data = A.Data();

        if (static_cast<I32>(quatAData[i]) == 0)
        {
            quatEps1Data[i] = quatAData[i] + epsilon;
            quatEps2Data[i] = quatAData[i] + 10 * epsilon;
        }
        else
        {
            quatEps1Data[i] = quatAData[i] + epsilon * quatAData[i];
            quatEps2Data[i] = quatAData[i] + 10 * epsilon * quatAData[i];
        }

        quatEps1 = _quaternion(quatEps1Data);
        quatEps2 = _quaternion(quatEps2Data);

        BOOST_CHECK(A == quatEps1);
        BOOST_CHECK(!(A != quatEps1));

        BOOST_CHECK(A != quatEps2);
        BOOST_CHECK(!(A == quatEps2));
    }
}



BOOST_FIXTURE_TEST_CASE(Comparison_Single, Fixture<QuatfSingle>)
{
    ComparisonTest(A, B);
}



BOOST_FIXTURE_TEST_CASE(Comparison_SSE, Fixture<QuatfSSE>)
{
    ComparisonTest(A, B);
}
