#include <boost/test/unit_test.hpp>
#include "tests/tools/ExceptionChecks.h"

#include "gdl/base/Exception.h"
#include "gdl/resources/memory/generalPurposeMemory.h"

#include <array>

#include <iostream>

using namespace GDL;

BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    BOOST_CHECK_NO_THROW(GeneralPurposeMemory(100));

    BOOST_CHECK_THROW(GeneralPurposeMemory(0), Exception);
}


BOOST_AUTO_TEST_CASE(Initialization_Deinitialization)
{
    constexpr size_t memorySize = 100;

    GeneralPurposeMemory gpm{memorySize};

    GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(10), Exception);
    gpm.Initialize();

    // already initialized
    BOOST_CHECK_THROW(gpm.Initialize(), Exception);

    void* address = gpm.Allocate(20);

    // memory still in use
    BOOST_CHECK_THROW(gpm.Deinitialize(), Exception);

    gpm.Deallocate(address);
    gpm.Deinitialize();

    // already deinitialized
    BOOST_CHECK_THROW(gpm.Deinitialize(), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(gpm.Deallocate(address), Exception);
}


BOOST_AUTO_TEST_CASE(Deallocation)
{
    constexpr U32 numAllocations = 10;
    constexpr size_t headerSize = sizeof(size_t) + 1;
    constexpr size_t totalAllocationSize = 20;
    constexpr size_t allocationSize = totalAllocationSize - headerSize;
    constexpr size_t doubleAllocationSize = totalAllocationSize * 2 - headerSize;
    constexpr size_t tripleAllocationSize = totalAllocationSize * 3 - headerSize;
    constexpr size_t memorySize = numAllocations * (totalAllocationSize);

    GeneralPurposeMemory gpm{memorySize};
    std::array<void*, numAllocations> addresses;

    gpm.Initialize();


    for (U32 i = 0; i < numAllocations; ++i)
        addresses[i] = gpm.Allocate(allocationSize);

    // |xx| - used memory
    // |  | - free memory
    // |--| - memory that is going to be deallocated


    // Free if memory is totally filled
    // |xx|xx|xx|xx|xx|--|xx|xx|xx|xx|

    gpm.Deallocate(addresses[5]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[5] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();



    // Free in front of first free - no merge
    // |xx|xx|--|xx|xx|  |xx|xx|xx|xx|

    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[2]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[2] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[2] < addresses[5]);



    // Free behind last free - no merge
    // |xx|xx|xx|xx|xx|  |xx|xx|--|xx|

    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[8]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[5] < addresses[8]);



    // Free between 2 blocks - no merge
    // |xx|xx|  |xx|xx|--|xx|xx|  |xx|

    gpm.Deallocate(addresses[2]);
    gpm.Deallocate(addresses[8]);
    gpm.Deallocate(addresses[5]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[2] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[2] < addresses[5]);
    assert(addresses[5] < addresses[8]);



    // Free in front of first free - merge with previous
    // |xx|xx|xx|xx|--|  |xx|xx|xx|xx|

    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[4]);
    gpm.CheckMemoryConsistency();

    // check Merge
    addresses[4] = gpm.Allocate(doubleAllocationSize);
    gpm.Deallocate(addresses[4]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[4] < addresses[5]);


    // Free behind last free - merge with next
    // |xx|xx|xx|xx|xx|  |--|xx|xx|xx|

    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[6]);
    gpm.CheckMemoryConsistency();

    // check Merge
    addresses[5] = gpm.Allocate(doubleAllocationSize);
    gpm.Deallocate(addresses[5]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[5] < addresses[6]);



    // Free - merge with previous
    // |xx|  |xx|xx|--|  |xx|xx|xx|xx|

    gpm.Deallocate(addresses[1]);
    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[4]);
    gpm.CheckMemoryConsistency();


    // check Merge
    addresses[1] = gpm.Allocate(doubleAllocationSize);
    gpm.Deallocate(addresses[1]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[1] = gpm.Allocate(allocationSize);
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[1] < addresses[4]);
    assert(addresses[4] < addresses[5]);



    // Free - merge with next
    // |xx|xx|xx|xx|  |--|xx|xx|  |xx|

    gpm.Deallocate(addresses[4]);
    gpm.Deallocate(addresses[8]);
    gpm.Deallocate(addresses[5]);
    gpm.CheckMemoryConsistency();


    // check Merge
    addresses[4] = gpm.Allocate(doubleAllocationSize);
    gpm.Deallocate(addresses[4]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[4] < addresses[5]);
    assert(addresses[5] < addresses[8]);


    // Free - merge with previous and next
    // |xx|  |xx|xx|  |--|  |xx|  |xx|

    gpm.Deallocate(addresses[1]);
    gpm.Deallocate(addresses[4]);
    gpm.Deallocate(addresses[6]);
    gpm.Deallocate(addresses[8]);
    gpm.Deallocate(addresses[5]);
    gpm.CheckMemoryConsistency();


    // check Merge
    addresses[1] = gpm.Allocate(tripleAllocationSize);
    gpm.Deallocate(addresses[1]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[1] = gpm.Allocate(allocationSize);
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[1] < addresses[4]);
    assert(addresses[4] < addresses[5]);
    assert(addresses[5] < addresses[6]);
    assert(addresses[6] < addresses[8]);



    // Free - merge with previous and next - previous is first
    // |xx|xx|xx|xx|  |--|  |xx|  |xx|

    gpm.Deallocate(addresses[4]);
    gpm.Deallocate(addresses[6]);
    gpm.Deallocate(addresses[8]);
    gpm.Deallocate(addresses[5]);
    gpm.CheckMemoryConsistency();


    // check Merge
    addresses[4] = gpm.Allocate(tripleAllocationSize);
    gpm.Deallocate(addresses[4]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[4] < addresses[5]);
    assert(addresses[5] < addresses[6]);
    assert(addresses[6] < addresses[8]);



    // Free - merge with previous and next - next is last
    // |xx|  |xx|xx|  |--|  |xx|xx|xx|

    gpm.Deallocate(addresses[1]);
    gpm.Deallocate(addresses[4]);
    gpm.Deallocate(addresses[6]);
    gpm.Deallocate(addresses[5]);
    gpm.CheckMemoryConsistency();


    // check Merge
    addresses[1] = gpm.Allocate(tripleAllocationSize);
    gpm.Deallocate(addresses[1]);
    gpm.CheckMemoryConsistency();

    // refill
    addresses[1] = gpm.Allocate(allocationSize);
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    gpm.CheckMemoryConsistency();
    assert(addresses[1] < addresses[4]);
    assert(addresses[4] < addresses[5]);
    assert(addresses[5] < addresses[6]);


    for (U32 i = 0; i < numAllocations; ++i)
        gpm.Deallocate(addresses[i]);

    gpm.CheckMemoryConsistency();

    gpm.Deinitialize();
}

BOOST_AUTO_TEST_CASE(Allocation)
{
    //    constexpr U32 numAllocations = 10;
    //    constexpr size_t headerSize = sizeof(size_t) + 1;
    //    constexpr size_t allocationSize = 20 - headerSize;
    //    constexpr size_t memorySize = numAllocations * (headerSize + allocationSize);

    //    GeneralPurposeMemory gpm{memorySize};
    //    std::array<void*, numAllocations> addresses;

    //    // GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(10), Exception);
    //    gpm.Initialize();

    //    // already initialized
    //    BOOST_CHECK_THROW(gpm.Initialize(), Exception);

    //    for (U32 i = 0; i < numAllocations; ++i)
    //        addresses[i] = gpm.Allocate(allocationSize);


    //    gpm.CheckMemoryConsistency();

    //    gpm.Deallocate(addresses[4]);
    //    gpm.Deallocate(addresses[2]);
    //    gpm.Deallocate(addresses[3]);

    //    gpm.CheckMemoryConsistency();
    //    addresses[2] = gpm.Allocate(allocationSize);
    //    gpm.CheckMemoryConsistency();
    //    addresses[4] = gpm.Allocate(allocationSize);

    //    // memory still in use
    //    // BOOST_CHECK_THROW(gpm.Deinitialize(), Exception);

    //    // gpm.Deallocate(address);
    //    gpm.Deinitialize();

    //    // already deinitialized
    //    BOOST_CHECK_THROW(gpm.Deinitialize(), Exception);
}
