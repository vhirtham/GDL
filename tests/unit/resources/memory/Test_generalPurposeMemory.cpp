#include <boost/test/unit_test.hpp>
#include "tests/tools/ExceptionChecks.h"

#include "gdl/base/Exception.h"
#include "gdl/base/SSESupportFunctions.h"
#include "gdl/resources/memory/generalPurposeMemory.h"

#include <array>

#include <iostream>

using namespace GDL;

void AssertIncreasingAddresses(std::array<void*, 10> addresses)
{
    for (U32 i = 1; i < addresses.size(); ++i)
        assert(addresses[i - 1] < addresses[i] && "Addresses not in ascending order");
}

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


void CheckDeallocationTestSetup(const GeneralPurposeMemory& gpm, std::array<void*, 10> addresses)
{
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 10);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 0);
    AssertIncreasingAddresses(addresses);
}


BOOST_AUTO_TEST_CASE(Deallocation)
{
    constexpr U32 numAllocations = 10;
    constexpr size_t alignment = 1;
    constexpr size_t headerSize = sizeof(size_t) + alignment;
    constexpr size_t totalAllocationSize = 20;
    constexpr size_t allocationSize = totalAllocationSize - headerSize;
    constexpr size_t doubleAllocationSize = totalAllocationSize * 2 - headerSize;
    constexpr size_t tripleAllocationSize = totalAllocationSize * 3 - headerSize;
    constexpr size_t memorySize = numAllocations * (totalAllocationSize);

    GeneralPurposeMemory gpm{memorySize};
    std::array<void*, numAllocations> addresses;
    void* expectedAddress = nullptr;


    gpm.Initialize();


    for (U32 i = 0; i < numAllocations; ++i)
        addresses[i] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);

    // |xx| - used memory
    // |  | - free memory
    // |--| - memory that is going to be deallocated


    // Free if memory is totally filled
    // |xx|xx|xx|xx|xx|--|xx|xx|xx|xx|

    gpm.Deallocate(addresses[5]);
    addresses[5] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 9);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);


    // refill
    addresses[5] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);



    // Free in front of first free - no merge
    // |xx|xx|--|xx|xx|  |xx|xx|xx|xx|

    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[2]);
    addresses[5] = nullptr;
    addresses[2] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    // refill
    addresses[2] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);



    // Free behind last free - no merge
    // |xx|xx|xx|xx|xx|  |xx|xx|--|xx|

    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[8]);
    addresses[5] = nullptr;
    addresses[8] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    // refill
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);



    // Free between 2 blocks - no merge
    // |xx|xx|  |xx|xx|--|xx|xx|  |xx|

    gpm.Deallocate(addresses[2]);
    gpm.Deallocate(addresses[8]);
    gpm.Deallocate(addresses[5]);
    addresses[2] = nullptr;
    addresses[5] = nullptr;
    addresses[8] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 3);

    // refill
    addresses[2] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);



    // Free in front of first free - merge with previous
    // |xx|xx|xx|xx|--|  |xx|xx|xx|xx|

    expectedAddress = addresses[4];

    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[4]);
    addresses[5] = nullptr;
    addresses[4] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);

    // check Merge
    addresses[4] = gpm.Allocate(doubleAllocationSize);
    BOOST_CHECK(expectedAddress == addresses[4]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 9);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 0);
    gpm.Deallocate(addresses[4]);
    addresses[4] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);

    // refill
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);



    // Free behind last free - merge with next
    // |xx|xx|xx|xx|xx|  |--|xx|xx|xx|

    expectedAddress = addresses[5];

    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[6]);
    addresses[5] = nullptr;
    addresses[6] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);


    // check Merge
    addresses[5] = gpm.Allocate(doubleAllocationSize);
    BOOST_CHECK(expectedAddress == addresses[5]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 9);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 0);
    gpm.Deallocate(addresses[5]);
    addresses[5] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);


    // refill
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);



    // Free - merge with previous
    // |xx|  |xx|xx|--|  |xx|xx|xx|xx|

    expectedAddress = addresses[4];

    gpm.Deallocate(addresses[1]);
    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[4]);
    addresses[1] = nullptr;
    addresses[5] = nullptr;
    addresses[4] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    // check Merge
    addresses[1] = gpm.Allocate(doubleAllocationSize);
    BOOST_CHECK(addresses[1] == expectedAddress);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[1]);
    addresses[1] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);


    // refill
    addresses[1] = gpm.Allocate(allocationSize);
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);



    // Free - merge with next
    // |xx|xx|xx|xx|  |--|xx|xx|  |xx|

    expectedAddress = addresses[4];

    gpm.Deallocate(addresses[4]);
    gpm.Deallocate(addresses[8]);
    gpm.Deallocate(addresses[5]);
    addresses[4] = nullptr;
    addresses[8] = nullptr;
    addresses[5] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);


    // check Merge
    addresses[4] = gpm.Allocate(doubleAllocationSize);
    BOOST_CHECK(expectedAddress == addresses[4]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[4]);
    addresses[4] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    // refill
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);


    // Free - merge with previous and next
    // |xx|  |xx|xx|  |--|  |xx|  |xx|

    expectedAddress = addresses[4];

    gpm.Deallocate(addresses[1]);
    gpm.Deallocate(addresses[4]);
    gpm.Deallocate(addresses[6]);
    gpm.Deallocate(addresses[8]);
    gpm.Deallocate(addresses[5]);
    addresses[1] = nullptr;
    addresses[4] = nullptr;
    addresses[6] = nullptr;
    addresses[8] = nullptr;
    addresses[5] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 5);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 3);



    // check Merge
    addresses[1] = gpm.Allocate(tripleAllocationSize);
    BOOST_CHECK(expectedAddress == addresses[1]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);
    gpm.Deallocate(addresses[1]);
    addresses[1] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 5);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 3);


    // refill
    addresses[1] = gpm.Allocate(allocationSize);
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);



    // Free - merge with previous and next - previous is first
    // |xx|xx|xx|xx|  |--|  |xx|  |xx|

    expectedAddress = addresses[4];

    gpm.Deallocate(addresses[4]);
    gpm.Deallocate(addresses[6]);
    gpm.Deallocate(addresses[8]);
    gpm.Deallocate(addresses[5]);
    addresses[4] = nullptr;
    addresses[6] = nullptr;
    addresses[8] = nullptr;
    addresses[5] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);


    // check Merge
    addresses[4] = gpm.Allocate(tripleAllocationSize);
    BOOST_CHECK(expectedAddress == addresses[4]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[4]);
    addresses[4] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    // refill
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);



    // Free - merge with previous and next - next is last
    // |xx|  |xx|xx|  |--|  |xx|xx|xx|

    expectedAddress = addresses[4];

    gpm.Deallocate(addresses[1]);
    gpm.Deallocate(addresses[4]);
    gpm.Deallocate(addresses[6]);
    gpm.Deallocate(addresses[5]);
    addresses[1] = nullptr;
    addresses[4] = nullptr;
    addresses[6] = nullptr;
    addresses[5] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);


    // check Merge
    addresses[1] = gpm.Allocate(tripleAllocationSize);
    BOOST_CHECK(expectedAddress == addresses[1]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[1]);
    addresses[1] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    // refill
    addresses[1] = gpm.Allocate(allocationSize);
    addresses[4] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    CheckDeallocationTestSetup(gpm, addresses);


    for (U32 i = 0; i < numAllocations; ++i)
        gpm.Deallocate(addresses[i]);

    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 0);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);

    gpm.Deinitialize();
}



BOOST_AUTO_TEST_CASE(Allocation)
{
    // TODO Check pointer addresses

    constexpr U32 numAllocations = 10;
    constexpr size_t alignment = 1;
    constexpr size_t headerSize = sizeof(size_t) + alignment;
    constexpr size_t freeHeaderSize = 2 * sizeof(size_t);
    constexpr size_t totalAllocationSize = 20;
    constexpr size_t allocationSize = totalAllocationSize - headerSize;
    constexpr size_t doubleAllocationSize = totalAllocationSize * 2 - headerSize;
    constexpr size_t memorySize = numAllocations * (headerSize + allocationSize);

    GeneralPurposeMemory gpm{memorySize};
    std::array<void*, numAllocations> addresses;

    // GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(10), Exception);
    gpm.Initialize();


    // |xx| - used memory
    // |  | - free memory
    // |--| - memory that is going to be allocated
    // |- | - memory allocation might leave some free memory depending on the allocation size


    for (U32 i = 0; i < numAllocations - 2; ++i)
        addresses[i] = gpm.Allocate(allocationSize);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);



    // Allocated element is first free and last free
    // |xx|xx|xx|xx|xx|xx|xx|xx|--|- |

    addresses[8] = gpm.Allocate(doubleAllocationSize - freeHeaderSize);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 9);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[8]);
    addresses[8] = gpm.Allocate(doubleAllocationSize - freeHeaderSize + 1);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 9);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 0);
    gpm.Deallocate(addresses[8]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);



    // Allocated element is last free but not first free
    // |xx|xx|  |xx|xx|xx|xx|xx|--|- |

    gpm.Deallocate(addresses[2]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    addresses[8] = gpm.Allocate(doubleAllocationSize - freeHeaderSize);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);
    gpm.Deallocate(addresses[8]);
    addresses[8] = gpm.Allocate(doubleAllocationSize - freeHeaderSize + 1);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[8]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);


    // Allocated element is first free but not last free
    // |xx|xx|--|- |xx|xx|xx|xx|  |  |

    gpm.Deallocate(addresses[3]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    addresses[2] = gpm.Allocate(doubleAllocationSize - freeHeaderSize);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);
    gpm.Deallocate(addresses[2]);
    addresses[2] = gpm.Allocate(doubleAllocationSize - freeHeaderSize + 1);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[2]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);


    // Allocated element is not the first nor the last free
    // |xx|xx|xx|  |xx|--|- |xx|  |  |

    addresses[2] = gpm.Allocate(allocationSize);
    gpm.Deallocate(addresses[5]);
    gpm.Deallocate(addresses[6]);

    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 5);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 3);

    addresses[5] = gpm.Allocate(doubleAllocationSize - freeHeaderSize);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 3);
    gpm.Deallocate(addresses[5]);
    addresses[5] = gpm.Allocate(doubleAllocationSize - freeHeaderSize + 1);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);
    gpm.Deallocate(addresses[5]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 5);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 3);


    // fill all free blocks
    addresses[3] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    addresses[9] = gpm.Allocate(allocationSize);

    for (U32 i = 0; i < numAllocations; ++i)
        gpm.Deallocate(addresses[i]);

    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 0);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);


    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 0);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);

    AssertIncreasingAddresses(addresses);

    gpm.Deinitialize();
}

// BOOST_AUTO_TEST_CASE(Alignment)
//{
//    struct alignas(32) AlignMe
//    {
//        float myVar;
//    };

//    std::vector<AlignMe> test;
//    test.emplace_back();
//    test.emplace_back();
//    test.emplace_back();
//    std::cout << alignof(AlignMe) << std::endl;
//    std::cout << is_aligned(&test[0], 32) << std::endl;
//    std::cout << is_aligned(&test[1], 32) << std::endl;
//    std::cout << is_aligned(&test[2], 32) << std::endl;
//}
