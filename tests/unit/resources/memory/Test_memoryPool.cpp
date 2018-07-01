#include <boost/test/unit_test.hpp>
#include "tests/tools/ExceptionChecks.h"

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"
#include "gdl/base/SSESupportFunctions.h"
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

    // Minimal size is sizeof(void*)
    BOOST_CHECK_THROW(MemoryPool(1, 3), Exception);

    // Zero Elements not allowed
    BOOST_CHECK_THROW(MemoryPool(16, 0), Exception);
}


BOOST_AUTO_TEST_CASE(Not_Initialized_Exceptions)
{
    MemoryPool mp(16, 5);

    BOOST_CHECK_THROW(mp.Allocate(16), Exception);
    BOOST_CHECK_THROW(mp.CheckMemoryConsistency(), Exception);

    mp.Initialize();
    void* address = mp.Allocate(16);
    BOOST_CHECK_THROW(mp.Deinitialize(), Exception);
    BOOST_CHECK_NO_THROW(mp.Deallocate(address));
    BOOST_CHECK_NO_THROW(mp.Deinitialize());
    BOOST_CHECK_THROW(mp.Deallocate(address), Exception);

    BOOST_CHECK_THROW(mp.Deinitialize(), Exception);
}

//!@brief This test checks if allocations and deallocations work. Because of the internal design, memory which is
//! deallocated first is also allocated first again (FIFO). To expose this for the test, the memory is completly filled
//! before the first deallocation.
BOOST_AUTO_TEST_CASE(Allocation_and_Deallocation)
{
    constexpr U32 elementSize = 16;
    constexpr U32 numElements = 5;
    std::array<void*, numElements> addresses;
    addresses.fill(nullptr);

    MemoryPool mp(elementSize, numElements);
    mp.Initialize();

    // Object too big
    BOOST_CHECK_THROW(mp.Allocate(100), Exception);

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(addresses[i] = mp.Allocate(elementSize - i)); // -i to check that smaller sizes alos work

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
        BOOST_CHECK_NO_THROW(addresses[indices[i]] = mp.Allocate(elementSize));

    BOOST_CHECK_NO_THROW(mp.CheckMemoryConsistency());

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK(addresses[i] == refAddresses[i]);

    // Memory full
    BOOST_CHECK_THROW(mp.Allocate(elementSize), Exception);

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[i]));

    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}

//! @brief This test checks if the exceptions are triggered correctly. Therefore the internal list is destroyed by
//! keeping a pointer to the memory and modify it after deallocation.
BOOST_AUTO_TEST_CASE(Consistency_Check_Exceptions)
{
    constexpr U32 elementSize = 16;
    constexpr U32 numElements = 5;

    void* valueToWrite = nullptr;
    MemoryPool mp(elementSize, numElements);
    mp.Initialize();

    void* addressToModify = mp.Allocate(elementSize);
    void* address2 = mp.Allocate(elementSize);

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
    constexpr U32 elementSize = 16;
    constexpr U32 numElements = 5;

    std::array<void*, numElements> addresses;
    addresses.fill(nullptr);

    MemoryPool mp(elementSize, numElements);
    mp.Initialize();

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(addresses[i] = mp.Allocate(elementSize));

    // double free
    BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[2]));
#ifndef NDEBUG
    BOOST_CHECK_THROW(mp.Deallocate(addresses[2]), Exception);
#endif

    // nullptr
    BOOST_CHECK_THROW(mp.Deallocate(nullptr), Exception);

    // out of range
    BOOST_CHECK_THROW(mp.Deallocate(static_cast<U8*>(addresses[0]) - 100), Exception);
    BOOST_CHECK_THROW(mp.Deallocate(static_cast<U8*>(addresses[4]) + 100), Exception);

    // not a valid element start
    BOOST_CHECK_THROW(mp.Deallocate(static_cast<U8*>(addresses[2]) + 1), Exception);

    addresses[2] = mp.Allocate(elementSize);
    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[i]));

    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}

BOOST_AUTO_TEST_CASE(Alignment)
{

    constexpr U32 alignment = 128;
    constexpr U32 elementSize = alignment;
    constexpr U32 numElements = 5;

    MemoryPool mp(elementSize, numElements, alignment);
    mp.Initialize();

    std::array<void*, numElements> addresses;
    addresses.fill(nullptr);

    for (U32 i = 0; i < addresses.size(); ++i)
    {
        BOOST_CHECK_NO_THROW(addresses[i] = mp.Allocate(elementSize));
        BOOST_CHECK(is_aligned(addresses[i], alignment));
    }

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK_NO_THROW(mp.Deallocate(addresses[i]));

    // alignment must be power of 2
    BOOST_CHECK_THROW(MemoryPool(16, 5, 5), Exception);

    // element size must be a multiple of alignment
    BOOST_CHECK_THROW(MemoryPool(16, 5, 32), Exception);

    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}

//! @brief Checks if the memory pool can be initialized again after deinitialization and works correct
BOOST_AUTO_TEST_CASE(Multiple_Initialization)
{
    constexpr U32 elementSize = 16;
    constexpr U32 numElements = 5;
    MemoryPool mp(elementSize, numElements);
    mp.Initialize();
    void* address = mp.Allocate(elementSize);
    mp.Deallocate(address);
    BOOST_CHECK_NO_THROW(mp.Deinitialize());

    BOOST_CHECK_NO_THROW(mp.Initialize());
    BOOST_CHECK_NO_THROW(address = mp.Allocate(elementSize));
    BOOST_CHECK_NO_THROW(mp.Deallocate(address));
    BOOST_CHECK_NO_THROW(mp.Deinitialize());
}



//! @brief This test checks if the access of the memory pool is thread safe.
BOOST_AUTO_TEST_CASE(Thread_Safety)
{
    constexpr U32 numThreads = 4;
    constexpr U32 numThreadAllocs = 5;
    constexpr U32 numAllocationRuns = 100;

    constexpr U32 elementSize = 16;
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
                        addresses[k] = mp.Allocate(elementSize);
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



// ################################################################################################
// Valgrind conflicts with the global new counter because it overrides global new. If tested on release builds, valgrind
// will throw a lot of errors. Find a solution for this. Until then, remove the test when using valgrind in release


#include "gdl/resources/memory/utility/globalNewCounter.h"

//! @brief This test checks if the number of allocations is as expected
//! @remark The exceptions in the other tests call new due to the internal usage of strings. This makes it hard to
//! keep track of the expected number of allocations since it can vary for each exception depending on the message.
BOOST_AUTO_TEST_CASE(No_hidden_allocations)
{
    constexpr size_t elementSize = 16;
    constexpr U32 numElements = 5;

    GlobalNewCounter gnc;

    void* address = nullptr;

    MemoryPool mp(elementSize, numElements);
    mp.Initialize();
    address = mp.Allocate(elementSize);
    mp.Deallocate(address);
    mp.Deinitialize();

    BOOST_CHECK(gnc.GetNumNewCalls() == 1);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 1);

    mp.Initialize();
    address = mp.Allocate(elementSize);
    mp.Deallocate(address);
    mp.Deinitialize();

    BOOST_CHECK(gnc.GetNumNewCalls() == 2);
    BOOST_CHECK(gnc.GetNumDeleteCalls() == 2);
}
