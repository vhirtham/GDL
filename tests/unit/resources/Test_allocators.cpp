#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/internalStackAllocator.h"
#include "gdl/resources/memory/utility/globalNewCounter.h"

#include <vector>


using namespace GDL;

constexpr U32 memorySize = 2000;

BOOST_AUTO_TEST_CASE(Memory_Manger_Setup)
{
    GlobalNewCounter gnc;
    MemoryManager::SetInternalThreadStackSize(memorySize);
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
    MemoryManager::Initialize();
    BOOST_CHECK(gnc.GetNumNewCalls() == 1);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

    BOOST_CHECK_THROW(MemoryManager::SetInternalThreadStackSize(10000), Exception);
}

BOOST_AUTO_TEST_CASE(STL_Vector)
{
    GlobalNewCounter gnc;
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

    std::vector<I32, InternalStackAllocator<I32>> v;
    for (I32 i = 0; i < 20; ++i)
        v.push_back(i);

    for (I32 i = 0; i < 20; ++i)
        BOOST_CHECK(v[i] == i);

    BOOST_CHECK(gnc.GetNumNewCalls() == 2);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);

    BOOST_CHECK_THROW(v.reserve(memorySize), Exception);
}


BOOST_AUTO_TEST_CASE(STL_Map)
{
    GlobalNewCounter gnc;
    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
    std::map<I32, F32, std::less<I32>, InternalStackAllocator<std::pair<const I32, F32>>> m;

    for (I32 i = 0; i < 20; ++i)
        m.emplace(i, static_cast<F32>(i));

    for (I32 i = 0; i < 20; ++i)
        BOOST_CHECK(m.find(i) != m.end());

    BOOST_CHECK(gnc.GetNumNewCalls() == 0);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 0);
}
