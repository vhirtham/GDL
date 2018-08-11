#include <boost/test/unit_test.hpp>

#include "gdl/GDLTypedefs.h"
#include "gdl/base/SSESupportFunctions.h"
#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/utility/globalNewCounter.h"



#include "memoryManagerSetup.h"

#include <iostream>
#include <array>
#include <vector>

using namespace GDL;


// Helper structs and functions -------------------------------------------------------------------

constexpr U32 alignment = 32;
struct alignas(alignment) AlignedStruct
{
    AlignedStruct(I32 val)
        : mMember{val}
    {
    }
    I32 mMember;
};

template <template <typename> class _allocator>
void CheckNoAllocations([[maybe_unused]] const GlobalNewCounter& gnc)
{
    EXCEPTION(true, "Default version should not be used.");
}

template <>
void CheckNoAllocations<GeneralPurposeAllocator>(const GlobalNewCounter& gnc)
{
#ifndef NO_GENERAL_PURPOSE_MEMORY
    BOOST_CHECK(gnc.CheckNumCallsExpected(0, 0));
#else
    BOOST_CHECK(!gnc.CheckNumCallsExpected(0, 0));
#endif
}

template <>
void CheckNoAllocations<PoolAllocator>(const GlobalNewCounter& gnc)
{
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL))
    BOOST_CHECK(gnc.CheckNumCallsExpected(0, 0));
#else
    BOOST_CHECK(!gnc.CheckNumCallsExpected(0, 0));
#endif
}


// Vector -----------------------------------------------------------------------------------------

template <template <typename> class _allocator>
void VectorTest()
{

    GlobalNewCounter gnc;
    constexpr U32 numElements = 100;

    std::vector<I32, _allocator<I32>> v;

    for (U32 i = 0; i < numElements; ++i)
        v.push_back(static_cast<I32>(i) * 2);
    for (U32 i = 0; i < numElements; ++i)
        BOOST_CHECK(v[i] == static_cast<I32>(i) * 2);



    // Alignment test
    std::vector<AlignedStruct, _allocator<AlignedStruct>> v2;
    for (U32 i = 0; i < numElements; ++i)
        v.push_back(static_cast<I32>(i) * 2);
    for (U32 i = 0; i < numElements; ++i)
        BOOST_CHECK(is_aligned(&v2[i], alignment));

    CheckNoAllocations<_allocator>(gnc);
}


BOOST_AUTO_TEST_CASE(Vector)
{
    InitializeMemoryManager();

    BOOST_CHECK_NO_THROW(VectorTest<GeneralPurposeAllocator>());

    DeinitializeMemoryManager();
}



// Map --------------------------------------------------------------------------------------------

template <template <typename> class _allocator>
void MapTest()
{
    GlobalNewCounter gnc;
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


    // Alignment
    std::map<U32, AlignedStruct, std::less<U32>, _allocator<std::pair<const U32, AlignedStruct>>> m2;
    for (U32 i = 0; i < numElements; ++i)
        m2.emplace(i, static_cast<I32>(i));
    for (const auto& it : m2)
        BOOST_CHECK(is_aligned(&it.second, alignment));

    CheckNoAllocations<_allocator>(gnc);
}

BOOST_AUTO_TEST_CASE(Map)
{
    InitializeMemoryManager();

    BOOST_CHECK_NO_THROW(MapTest<GeneralPurposeAllocator>());
    BOOST_CHECK_NO_THROW(MapTest<PoolAllocator>());

    DeinitializeMemoryManager();
}
