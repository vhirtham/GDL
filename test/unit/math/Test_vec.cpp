#include <boost/test/unit_test.hpp>

#include "gdl/math/sse/vecSSE.h"
#include "gdl/math/single/vecSingle.h"

#include "test/tools/arrayValueComparison.h"


using namespace GDL;


// Fixture definition -------------------------------------------------------------------------------------------------

template <template <typename, I32, bool> class _vector, typename _type>
struct Fixture
{
    _vector<_type, 16, true> A1 =
            _vector<_type, 16, true>(0., 4., 8., 12., 1., 5., 9., 13., 2., 6., 10., 14., 3., 7., 11., 15.);
};



// Construction and Data function -------------------------------------------------------------------------------------

template <template <typename, I32, bool> class _vector, typename _type, bool _isClol>
void CtorDataTest()
{
    _vector<_type, 16, _isClol> a;
    BOOST_CHECK(CheckArrayZero(a.Data()));
}



BOOST_AUTO_TEST_CASE(Construction_Single)
{
    CtorDataTest<VecSingle, F32, true>();
    CtorDataTest<VecSingle, F32, false>();
    CtorDataTest<VecSingle, F64, true>();
    CtorDataTest<VecSingle, F64, false>();
}


BOOST_AUTO_TEST_CASE(Construction_SSE)
{
    CtorDataTest<VecSSE, F32, true>();
    CtorDataTest<VecSSE, F32, false>();
    CtorDataTest<VecSSE, F64, true>();
    CtorDataTest<VecSSE, F64, false>();
}
