#include <boost/test/unit_test.hpp>
#include "tests/tools/ExceptionChecks.h"

#include "gdl/base/Exception.h"
#include "gdl/resources/memory/generalPurposeMemory.h"

#include <array>

using namespace GDL;

BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    BOOST_CHECK_NO_THROW(GeneralPurposeMemory(100));

    BOOST_CHECK_THROW(GeneralPurposeMemory(0), Exception);
}


BOOST_AUTO_TEST_CASE(Initialization_Deinitialization)
{
    constexpr U32 numAllocations = 10;
    constexpr size_t headerSize = sizeof(size_t) + 1;
    constexpr size_t allocationSize = 20 - headerSize;
    constexpr size_t memorySize = numAllocations * (headerSize + allocationSize);

    GeneralPurposeMemory gpm{memorySize};
    std::array<void*, numAllocations> addresses;

    // GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(10), Exception);
    gpm.Initialize();

    // already initialized
    BOOST_CHECK_THROW(gpm.Initialize(), Exception);

    for (U32 i = 0; i < numAllocations; ++i)
        addresses[i] = gpm.Allocate(allocationSize);


    gpm.CheckMemoryConsistency();

    gpm.Deallocate(addresses[4]);
    gpm.Deallocate(addresses[2]);
    gpm.Deallocate(addresses[3]);

    gpm.CheckMemoryConsistency();
    addresses[2] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    addresses[4] = gpm.Allocate(allocationSize);

    // memory still in use
    // BOOST_CHECK_THROW(gpm.Deinitialize(), Exception);

    // gpm.Deallocate(address);
    gpm.Deinitialize();

    // already deinitialized
    BOOST_CHECK_THROW(gpm.Deinitialize(), Exception);
}
