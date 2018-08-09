#include <boost/test/unit_test.hpp>

#include "gdl/GDLTypedefs.h"
#include "gdl/resources/memory/generalPurposeAllocator.h"


#include "memoryManagerSetup.h"

#include <iostream>
#include <vector>

using namespace GDL;


template <template <typename> class _allocator>
void VectorTest()
{
    constexpr U32 numElements = 100;

    std::vector<I32, _allocator<I32>> v;

    for (U32 i = 0; i < numElements; ++i)
        v.push_back(static_cast<I32>(i) * 2);
    for (U32 i = 0; i < numElements; ++i)
        BOOST_CHECK(v[i] == static_cast<I32>(i) * 2);

    v.clear();
    v.shrink_to_fit();
}


BOOST_AUTO_TEST_CASE(Vector)
{
    InitializeMemoryManager();

    BOOST_CHECK_NO_THROW(VectorTest<GeneralPurposeAllocator>());

    DeinitializeMemoryManager();
}



template <template <typename> class _allocator>
void MapTest()
{
    constexpr U32 numElements = 100;

    std::map<U32, I32, std::less<U32>, _allocator<std::pair<const U32, I32>>> m;

    for (U32 i = 0; i < numElements; ++i)
        m.emplace(i, static_cast<I32>(i));

    for (U32 i = 0; i < numElements; ++i)
    {
        const auto& entry = m.find(i);
        BOOST_CHECK(entry != m.end());
        BOOST_CHECK(entry->second == static_cast<I32>(i));
    }

    m.clear();
}

BOOST_AUTO_TEST_CASE(Map)
{
    InitializeMemoryManager();

    BOOST_CHECK_NO_THROW(MapTest<GeneralPurposeAllocator>());

    DeinitializeMemoryManager();
}
