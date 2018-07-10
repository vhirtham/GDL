#include <boost/test/unit_test.hpp>
#include "tests/tools/ExceptionChecks.h"

#include "gdl/base/Exception.h"
#include "gdl/resources/memory/generalPurposeMemory.h"

using namespace GDL;

BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    BOOST_CHECK_NO_THROW(GeneralPurposeMemory(100));

    BOOST_CHECK_THROW(GeneralPurposeMemory(0), Exception);
}


BOOST_AUTO_TEST_CASE(Initialization_Deinitialization)
{
    constexpr U32 memorySize = 128;
    GeneralPurposeMemory gpm{memorySize};


    // GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(10), Exception);
    gpm.Initialize();

    // already initialized
    BOOST_CHECK_THROW(gpm.Initialize(), Exception);

    void* address = gpm.Allocate(10);
    void* address2 = gpm.Allocate(10);
    // memory still in use
    // BOOST_CHECK_THROW(gpm.Deinitialize(), Exception);

    // gpm.Deallocate(address);
    gpm.Deinitialize();

    // already deinitialized
    BOOST_CHECK_THROW(gpm.Deinitialize(), Exception);
}
