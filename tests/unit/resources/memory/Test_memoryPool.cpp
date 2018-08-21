#include <boost/test/unit_test.hpp>
#include "tests/tools/ExceptionChecks.h"

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/resources/memory/memoryPool.h"
#include "gdl/resources/memory/utility/heapAllocationCounter.h"


#include <atomic>
#include <array>
#include <cstring>
#include <thread>
#include <vector>



//! Uncomment next line if this test shoul be run with valgrind
//#define DISABLE_HEAP_ALLOCATION_COUNTER



using namespace GDL;


BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    BOOST_CHECK_NO_THROW(MemoryPool(16_B, 3));

    // Minimal size is sizeof(void*)
    BOOST_CHECK_THROW(MemoryPool(1_B, 3), Exception);

    // Zero Elements not allowed
    BOOST_CHECK_THROW(MemoryPool(16_B, 0), Exception);
}


BOOST_AUTO_TEST_CASE(Initialization_Deinitialization_Exceptions)
{
    MemoryPool mp(16_B, 5);

    GDL_CHECK_THROW_DEV_DISABLE(mp.Allocate(16), Exception);
    BOOST_CHECK_THROW(mp.CheckMemoryConsistency(), Exception);

    mp.Initialize();
    // Already initialized
    BOOST_CHECK_THROW(mp.Initialize(), Exception);

    void* address = mp.Allocate(16);
    BOOST_CHECK_THROW(mp.Deinitialize(), Exception);
    BOOST_CHECK_NO_THROW(mp.Deallocate(address));
    BOOST_CHECK_NO_THROW(mp.Deinitialize());

    // Trying to deallocate from uninitialized memory pool
    GDL_CHECK_THROW_DEV_DISABLE(mp.Deallocate(address), Exception);

    // Already deinitialized
    BOOST_CHECK_THROW(mp.Deinitialize(), Exception);
}

//!@brief This test checks if allocations and deallocations work. Because of the internal design, memory which is
//! deallocated first is also allocated first again (FIFO). To expose this for the test, the memory is completly filled
//! before the first deallocation.
BOOST_AUTO_TEST_CASE(Allocation_and_Deallocation)
{
    constexpr MemorySize elementSize = 16_B;
    constexpr U32 numElements = 5;
    std::array<void*, numElements> addresses;
    addresses.fill(nullptr);

    MemoryPool mp(elementSize, numElements);
    mp.Initialize();

    // Object too big
    GDL_CHECK_THROW_DEV_DISABLE(mp.Allocate(100), Exception);

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(
                addresses[i] = mp.Allocate(elementSize.GetNumBytes() - i)); // -i to check that smaller sizes alos work

    std::array<void*, numElements> refAddresses{addresses};
    BOOST_CHECK_NO_THROW(mp.CheckMemoryConsistency());

    std::array<U32, 4> indices{{4, 1, 2, 0}};
    for (U32 i = 0; i < indices.size(); ++i)
    {
        U32 index = indices[i];
        BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[index]));
        addresses[index] = nullptr;
    }
    BOOST_CHECK_NO_THROW(mp.CheckMemoryConsistency());

    for (U32 i = 0; i < indices.size(); ++i)
        BOOST_CHECK_NO_THROW(addresses[indices[i]] = mp.Allocate(elementSize.GetNumBytes()));

    BOOST_CHECK_NO_THROW(mp.CheckMemoryConsistency());

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK(addresses[i] == refAddresses[i]);

    // Memory full
    BOOST_CHECK_THROW(mp.Allocate(elementSize.GetNumBytes()), Exception);

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[i]));

    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}

//! @brief This test checks if the exceptions are triggered correctly. Therefore the internal list is destroyed by
//! keeping a pointer to the memory and modify it after deallocation.
BOOST_AUTO_TEST_CASE(Consistency_Check_Exceptions)
{
    constexpr MemorySize elementSize = 16_B;
    constexpr U32 numElements = 5;

    void* valueToWrite = nullptr;
    MemoryPool mp(elementSize, numElements);
    mp.Initialize();

    void* addressToModify = mp.Allocate(elementSize.GetNumBytes());
    void* address2 = mp.Allocate(elementSize.GetNumBytes());

    mp.Deallocate(addressToModify);
    mp.Deallocate(address2);

    BOOST_CHECK_NO_THROW(mp.CheckMemoryConsistency());
    std::memcpy(addressToModify, &valueToWrite, sizeof(void*));
    BOOST_CHECK_THROW(mp.CheckMemoryConsistency(), Exception);

    // restore value
    std::memcpy(addressToModify, &address2, sizeof(void*));
    BOOST_CHECK_NO_THROW(mp.CheckMemoryConsistency());

    // build loop by pointing to own position
    valueToWrite = addressToModify;
    std::memcpy(addressToModify, &valueToWrite, sizeof(void*));

    BOOST_CHECK_THROW(mp.CheckMemoryConsistency(), Exception);

    // restore value
    std::memcpy(addressToModify, &address2, sizeof(void*));
    BOOST_CHECK_NO_THROW(mp.CheckMemoryConsistency());

    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}

//! @brief Tests exceptions that can be thrown during deallocation
BOOST_AUTO_TEST_CASE(Deallocation_Exceptions)
{
    constexpr MemorySize elementSize = 16_B;
    constexpr U32 numElements = 5;

    std::array<void*, numElements> addresses;
    addresses.fill(nullptr);

    MemoryPool mp(elementSize, numElements);
    mp.Initialize();

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(addresses[i] = mp.Allocate(elementSize.GetNumBytes()));

    // double free
    BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[2]));
    GDL_CHECK_THROW_DEBUG_DISABLE(mp.Deallocate(addresses[2]), Exception);

    // nullptr
    GDL_CHECK_THROW_DEV_DISABLE(mp.Deallocate(nullptr), Exception);

    // out of range
    GDL_CHECK_THROW_DEV_DISABLE(mp.Deallocate(static_cast<U8*>(addresses[0]) - 100), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(mp.Deallocate(static_cast<U8*>(addresses[4]) + 100), Exception);

    // not a valid element start
    GDL_CHECK_THROW_DEV_DISABLE(mp.Deallocate(static_cast<U8*>(addresses[2]) + 1), Exception);

    addresses[2] = mp.Allocate(elementSize.GetNumBytes());
    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[i]));

    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}

BOOST_AUTO_TEST_CASE(Alignment)
{

    constexpr U32 alignment = 128;
    constexpr MemorySize elementSize = alignment * 1_B;
    constexpr U32 numElements = 5;

    MemoryPool mp(elementSize, numElements, alignment);
    mp.Initialize();

    std::array<void*, numElements> addresses;
    addresses.fill(nullptr);

    for (U32 i = 0; i < addresses.size(); ++i)
    {
        BOOST_CHECK_NO_THROW(addresses[i] = mp.Allocate(elementSize.GetNumBytes()));
        BOOST_CHECK(IsAligned(addresses[i], alignment));
    }

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[i]));

    // allocation alignment must be smaller than memory alignment
    void* address = nullptr;
    BOOST_CHECK_NO_THROW(address = mp.Allocate(elementSize.GetNumBytes(), 1));
    mp.Deallocate(address);
    BOOST_CHECK_NO_THROW(address = mp.Allocate(elementSize.GetNumBytes(), 16));
    mp.Deallocate(address);
    BOOST_CHECK_NO_THROW(address = mp.Allocate(elementSize.GetNumBytes(), alignment));
    mp.Deallocate(address);

    GDL_CHECK_THROW_DEV_DISABLE(mp.Allocate(elementSize.GetNumBytes(), alignment * 2), Exception);

    // alignment must be power of 2 (Allocation)
    GDL_CHECK_THROW_DEV_DISABLE(mp.Allocate(elementSize.GetNumBytes(), 3), Exception);


    // alignment must be power of 2 (Construction)
    BOOST_CHECK_THROW(MemoryPool(16_B, 5, 5), Exception);

    // element size must be a multiple of alignment
    BOOST_CHECK_THROW(MemoryPool(16_B, 5, 32), Exception);

    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}

//! @brief Checks if the memory pool can be initialized again after deinitialization and works correct
BOOST_AUTO_TEST_CASE(Multiple_Initialization)
{
    constexpr MemorySize elementSize = 16_B;
    constexpr U32 numElements = 5;
    MemoryPool mp(elementSize, numElements);
    mp.Initialize();
    void* address = mp.Allocate(elementSize.GetNumBytes());
    mp.Deallocate(address);
    BOOST_CHECK_NO_THROW(mp.Deinitialize());

    BOOST_CHECK_NO_THROW(mp.Initialize());
    BOOST_CHECK_NO_THROW(address = mp.Allocate(elementSize.GetNumBytes()));
    BOOST_CHECK_NO_THROW(mp.Deallocate(address));
    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}



//! @brief This test checks if the access of the memory pool is thread safe.
BOOST_AUTO_TEST_CASE(Thread_Safety)
{
    constexpr U32 numThreads = 4;
    constexpr U32 numThreadAllocs = 5;
    constexpr U32 numAllocationRuns = 100;

    constexpr MemorySize elementSize = 16_B;
    constexpr U32 numElements = numThreadAllocs * numThreads;

    MemoryPool mp(elementSize, numElements);
    mp.Initialize();

    std::atomic_bool kickoff = false;
    std::atomic_bool exceptionThrown = false;

    std::vector<std::thread> threads;
    for (U32 i = 0; i < numThreads; ++i)
        threads.emplace_back([&]() {
            try
            {
                std::array<void*, numThreadAllocs> addresses;
                addresses.fill(nullptr);
                while (!kickoff)
                    std::this_thread::yield();
                for (U32 j = 0; j < numAllocationRuns; ++j)
                {
                    for (U32 k = 0; k < numThreadAllocs; ++k)
                        addresses[k] = mp.Allocate(elementSize.GetNumBytes());

                    mp.CheckMemoryConsistency();

                    for (U32 k = 0; k < numThreadAllocs; ++k)
                    {
                        mp.Deallocate(addresses[k]);
                        addresses[k] = nullptr;
                    }
                }
            }
            catch (...)
            {

                exceptionThrown = true;
            }
        });

    kickoff = true;

    for (U32 i = 0; i < numThreads; ++i)
        threads[i].join();

    BOOST_CHECK(exceptionThrown == false);
    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}



//! @brief This test checks if the number of allocations is as expected
//! @remark The exceptions in the other tests call new due to the internal usage of strings. This makes it hard to
//! keep track of the expected number of allocations since it can vary for each exception depending on the message.
BOOST_AUTO_TEST_CASE(No_hidden_allocations)
{
    constexpr MemorySize elementSize = 16_B;
    constexpr U32 numElements = 5;

    HeapAllocationCounter gnc;

    void* address = nullptr;

    MemoryPool mp(elementSize, numElements);
    mp.Initialize();
    address = mp.Allocate(elementSize.GetNumBytes());
    mp.Deallocate(address);
    mp.Deinitialize();

    BOOST_CHECK(gnc.GetNumNewCalls() == 1);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 1);

    mp.Initialize();
    address = mp.Allocate(elementSize.GetNumBytes());
    mp.Deallocate(address);
    mp.Deinitialize();

    BOOST_CHECK(gnc.GetNumNewCalls() == 2);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 2);
}
