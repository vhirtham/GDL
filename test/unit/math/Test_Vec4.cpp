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
