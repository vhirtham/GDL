#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/internalStackAllocator.h"
#include "gdl/resources/memory/utility/globalNewCounter.h"

#include <vector>


using namespace GDL;

BOOST_AUTO_TEST_CASE(STL_Tests)
{
    GlobalNewCounter gnc;
    std::vector<F32, InternalStackAllocator<F32>> v;
    std::map<I32, F32, std::less<I32>, InternalStackAllocator<std::pair<const I32, F32>>> m;
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
    v.push_back(1.);
    v.reserve(200);
    m.emplace(1, 1.);
    m.emplace(2, 1.);
    m.emplace(3, 1.);
    m.emplace(4, 1.);
    m.emplace(5, 1.);
    //    m.emplace(2, 1.);
    //    m.emplace(3, 1.);
    //    m.emplace(4, 1.);
    BOOST_CHECK(gnc.GetNumNewCalls() == 2);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
}
