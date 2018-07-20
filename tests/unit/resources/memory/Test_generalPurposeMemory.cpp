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

BOOST_AUTO_TEST_CASE(Construction_Destruction)
{
    BOOST_CHECK_NO_THROW(GeneralPurposeMemory(100));

    BOOST_CHECK_THROW(GeneralPurposeMemory(0), Exception);
}


BOOST_AUTO_TEST_CASE(Initialization_Deinitialization)
{
    constexpr size_t memorySize = 100;

    GeneralPurposeMemory gpm{memorySize};

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

//! @brief Helper function which tests if the intial state for each deallocation test edge case is correct.
void CheckDeallocationTestSetup(const GeneralPurposeMemory& gpm, std::array<void*, 10> addresses)
{
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 10);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 0);
    AssertIncreasingAddresses(addresses);
}


//! @brief Tests if the deallocation function updates the internal linked list. All possible edge cases are tested.
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


//! @brief Tests if the allocation function updates the internal linked list and returns the pointer to the expected
//! free memory block. All possible edge cases are tested.
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
    std::array<void*, numAllocations> expectedAddresses;

    // GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(10), Exception);
    gpm.Initialize();


    // |xx| - used memory
    // |  | - free memory
    // |--| - memory that is going to be allocated
    // |- | - memory allocation might leave some free memory depending on the allocation size


    for (U32 i = 0; i < numAllocations; ++i)
    {
        addresses[i] = gpm.Allocate(allocationSize);
        expectedAddresses[i] = addresses[i];
    }
    AssertIncreasingAddresses(expectedAddresses);

    gpm.Deallocate(addresses[8]);
    gpm.Deallocate(addresses[9]);
    addresses[8] = nullptr;
    addresses[9] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);



    // Allocated element is first free and last free
    // |xx|xx|xx|xx|xx|xx|xx|xx|--|- |

    addresses[8] = gpm.Allocate(doubleAllocationSize - freeHeaderSize);
    BOOST_CHECK(expectedAddresses[8] == addresses[8]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 9);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[8]);
    addresses[8] = nullptr;

    addresses[8] = gpm.Allocate(doubleAllocationSize - freeHeaderSize + 1);
    BOOST_CHECK(expectedAddresses[8] == addresses[8]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 9);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 0);
    gpm.Deallocate(addresses[8]);
    addresses[8] = nullptr;

    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);



    // Allocated element is last free but not first free
    // |xx|xx|  |xx|xx|xx|xx|xx|--|- |

    gpm.Deallocate(addresses[2]);
    addresses[2] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    addresses[8] = gpm.Allocate(doubleAllocationSize - freeHeaderSize);
    BOOST_CHECK(expectedAddresses[8] == addresses[8]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);
    gpm.Deallocate(addresses[8]);
    addresses[8] = nullptr;

    addresses[8] = gpm.Allocate(doubleAllocationSize - freeHeaderSize + 1);
    BOOST_CHECK(expectedAddresses[8] == addresses[8]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 8);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[8]);
    addresses[8] = nullptr;

    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);


    // Allocated element is first free but not last free
    // |xx|xx|--|- |xx|xx|xx|xx|  |  |

    gpm.Deallocate(addresses[3]);
    addresses[3] = nullptr;
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);

    addresses[2] = gpm.Allocate(doubleAllocationSize - freeHeaderSize);
    BOOST_CHECK(expectedAddresses[2] == addresses[2]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);
    gpm.Deallocate(addresses[2]);
    addresses[2] = nullptr;

    addresses[2] = gpm.Allocate(doubleAllocationSize - freeHeaderSize + 1);
    BOOST_CHECK(expectedAddresses[2] == addresses[2]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 7);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);
    gpm.Deallocate(addresses[2]);
    addresses[2] = nullptr;

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
    BOOST_CHECK(expectedAddresses[5] == addresses[5]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 3);
    gpm.Deallocate(addresses[5]);
    addresses[5] = nullptr;

    addresses[5] = gpm.Allocate(doubleAllocationSize - freeHeaderSize + 1);
    BOOST_CHECK(expectedAddresses[5] == addresses[5]);
    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 6);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 2);
    gpm.Deallocate(addresses[5]);
    addresses[5] = nullptr;

    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 5);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 3);


    // fill all free blocks
    addresses[3] = gpm.Allocate(allocationSize);
    addresses[5] = gpm.Allocate(allocationSize);
    addresses[6] = gpm.Allocate(allocationSize);
    addresses[8] = gpm.Allocate(allocationSize);
    addresses[9] = gpm.Allocate(allocationSize);

    AssertIncreasingAddresses(addresses);

    for (U32 i = 0; i < numAllocations; ++i)
        gpm.Deallocate(addresses[i]);

    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 0);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);


    BOOST_CHECK(gpm.CountAllocatedMemoryBlocks() == 0);
    BOOST_CHECK(gpm.CountFreeMemoryBlocks() == 1);


    gpm.Deinitialize();
}



//! @brief Tests all possible exceptions that can be thrown during allocation
BOOST_AUTO_TEST_CASE(Allocation_Exceptions)
{
    constexpr U32 numAllocations = 10;
    constexpr size_t alignment = 1;
    constexpr size_t headerSize = sizeof(size_t) + alignment;
    constexpr size_t totalAllocationSize = 20;
    constexpr size_t allocationSize = totalAllocationSize - headerSize;
    constexpr size_t doubleAllocationSize = totalAllocationSize * 2 - headerSize;
    constexpr size_t memorySize = numAllocations * (totalAllocationSize);

    GeneralPurposeMemory gpm{memorySize};
    std::array<void*, numAllocations> addresses;


    // not initialized
    GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(allocationSize), Exception);

    gpm.Initialize();



    for (U32 i = 0; i < numAllocations; ++i)
        addresses[i] = gpm.Allocate(allocationSize);
    AssertIncreasingAddresses(addresses);

    // no more memory left
    BOOST_CHECK_THROW(gpm.Allocate(allocationSize), Exception);



    // create some gaps
    // |xx| - used memory
    // |  | - free memory
    // |  |xx|  |xx|  |xx|  |xx|  |xx|

    for (U32 i = 0; i < numAllocations; i += 2)
    {
        gpm.Deallocate(addresses[i]);
        addresses[i] = nullptr;
    }

    // no fitting memory block available because of fragmentation
    BOOST_CHECK_THROW(gpm.Allocate(doubleAllocationSize), Exception);

    // size is 0
    GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(0), Exception);


    // clear memory
    for (U32 i = 1; i < numAllocations; i += 2)
    {
        gpm.Deallocate(addresses[i]);
        addresses[i] = nullptr;
    }


    // alignment is not power of 2
    GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(allocationSize, 5), Exception);


    // alignment is bigger than maximum alignment (128)
    GDL_CHECK_THROW_DEV_DISABLE(gpm.Allocate(allocationSize, 512), Exception);


    gpm.Deinitialize();
}

BOOST_AUTO_TEST_CASE(Deallocation_Exceptions)
{
}

BOOST_AUTO_TEST_CASE(Alignment)
{
}
