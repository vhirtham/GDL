#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/math/sse/vec4SSE.h"
#include "gdl/math/single/vec4Single.h"

#include "test/tools/arrayValueComparison.h"

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



BOOST_AUTO_TEST_CASE(Construction_Single)
{
    ConstructionTest<Vec4fSingle<true>>();
    ConstructionTest<Vec4fSingle<false>>();
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



BOOST_AUTO_TEST_CASE(Parentheses_Operator_Single)
{
    DirectAccessOperatorTest<Vec4fSingle>();
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



BOOST_FIXTURE_TEST_CASE(Comparison_Single, Fixture<Vec4fSingle>)
{
    ComparisonTest(cA, cB);
    ComparisonTest(rA, rB);
}



BOOST_FIXTURE_TEST_CASE(Comparison_SSE, Fixture<Vec4SSE>)
{
    ComparisonTest(cA, cB);
    ComparisonTest(rA, rB);
}
