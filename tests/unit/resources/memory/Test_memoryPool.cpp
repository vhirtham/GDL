#include <boost/test/unit_test.hpp>

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"
#include "gdl/resources/memory/memoryPool.h"

#include <atomic>
#include <array>
#include <cstring>
#include <thread>
#include <vector>

using namespace GDL;


BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    BOOST_CHECK_NO_THROW(MemoryPool(16, 3));
    BOOST_CHECK_THROW(MemoryPool(1, 3), Exception);
}

//!@brief This test checks if allocations and deallocations work. Because of the internal design, memory which is
//! deallocated first is also allocated first again (FIFO). To expose this for the test, the memory is completly filled
//! before the first deallocation.
BOOST_AUTO_TEST_CASE(Allocation_and_Deallocation)
{
    std::array<void*, 5> addresses;
    addresses.fill(nullptr);

    MemoryPool mp(16, 5);

    // Object too big
    BOOST_CHECK_THROW(mp.Allocate(100), Exception);

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(addresses[i] = mp.Allocate(8));

    std::array<void*, 5> refAddresses{addresses};
    BOOST_CHECK_NO_THROW(mp.CheckConsistency());

    std::array<U32, 4> indices{{4, 1, 2, 0}};
    for (U32 i = 0; i < indices.size(); ++i)
    {
        U32 index = indices[i];
        BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[index]));
        addresses[index] = nullptr;
    }
    BOOST_CHECK_NO_THROW(mp.CheckConsistency());

    for (U32 i = 0; i < indices.size(); ++i)
        BOOST_CHECK_NO_THROW(addresses[indices[i]] = mp.Allocate(8));

    BOOST_CHECK_NO_THROW(mp.CheckConsistency());

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK(addresses[i] == refAddresses[i]);

    // Memory full
    BOOST_CHECK_THROW(mp.Allocate(8), Exception);

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[i]));
}

//! @brief This test checks if the exceptions are triggered correctly. Therefore the internal list is destroyed by
//! keeping a pointer to the memory and modify it after deallocation.
BOOST_AUTO_TEST_CASE(Consistency_Check_Exceptions)
{
    void* valueToWrite = nullptr;
    MemoryPool mp(16, 5);

    void* addressToModify = mp.Allocate(8);
    void* address2 = mp.Allocate(8);

    mp.Deallocate(addressToModify);
    mp.Deallocate(address2);

    BOOST_CHECK_NO_THROW(mp.CheckConsistency());
    std::memcpy(addressToModify, &valueToWrite, sizeof(void*));
    BOOST_CHECK_THROW(mp.CheckConsistency(), Exception);

    // restore value
    std::memcpy(addressToModify, &address2, sizeof(void*));
    BOOST_CHECK_NO_THROW(mp.CheckConsistency());

    // build loop by pointing to own position
    valueToWrite = addressToModify;
    std::memcpy(addressToModify, &valueToWrite, sizeof(void*));

    BOOST_CHECK_THROW(mp.CheckConsistency(), Exception);
}


//! @brief This test checks if the access of the memory pool is thread safe.
BOOST_AUTO_TEST_CASE(Thread_Safety)
{
    constexpr U32 numThreads = 4;
    constexpr U32 numThreadAllocs = 5;
    constexpr U32 numAllocationRuns = 100;

    MemoryPool mp(16, numThreadAllocs * numThreads);

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
                        addresses[k] = mp.Allocate(16);
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
}
