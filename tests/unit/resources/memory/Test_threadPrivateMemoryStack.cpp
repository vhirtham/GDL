#include <boost/test/unit_test.hpp>
#include "tests/tools/ExceptionChecks.h"

#include "gdl/base/Exception.h"
#include "gdl/base/SSESupportFunctions.h"
#include "gdl/resources/memory/threadPrivateMemoryStack.h"

#include <array>
#include <atomic>
#include <memory>
#include <thread>

using namespace GDL;

BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    BOOST_CHECK_NO_THROW(threadPrivateMemoryStack(100));

    BOOST_CHECK_THROW(threadPrivateMemoryStack(0), Exception);
    BOOST_CHECK_THROW(threadPrivateMemoryStack(100, 0), Exception);
    BOOST_CHECK_THROW(threadPrivateMemoryStack(100, 5), Exception);
}


//! @brief Checks if exceptions during initialization and deinitialization are thrown as expected
BOOST_AUTO_TEST_CASE(Initialization_Deinitialization_Exceptions)
{

    constexpr U32 memorySize = 128;
    threadPrivateMemoryStack tpms{memorySize};


    GDL_CHECK_THROW_DEV_DISABLE(tpms.Allocate(10), Exception);
    tpms.Initialize();

    void* address = tpms.Allocate(10);
    // memory still in use
    BOOST_CHECK_THROW(tpms.Deinitialize(), Exception);

    tpms.Deallocate(address);
    tpms.Deinitialize();

    // already deinitialized
    BOOST_CHECK_THROW(tpms.Deinitialize(), Exception);
}

BOOST_AUTO_TEST_CASE(Allocation_and_Deallocation)
{
    constexpr U32 numAllocations = 5;
    constexpr U32 allocationSize = 10;
    constexpr U32 memorySize = allocationSize * numAllocations;
    threadPrivateMemoryStack tpms{memorySize};

    std::array<void*, numAllocations> addresses;
    addresses.fill(nullptr);

    tpms.Initialize();

    GDL_CHECK_THROW_DEV_DISABLE(tpms.Allocate(0), Exception);

    for (U32 i = 0; i < numAllocations; ++i)
        addresses[i] = tpms.Allocate(allocationSize);


    // nullptr
    GDL_CHECK_THROW_DEV_DISABLE(tpms.Deallocate(nullptr), Exception);

    // out of range
    GDL_CHECK_THROW_DEV_DISABLE(tpms.Deallocate(static_cast<U8*>(addresses[0]) - allocationSize), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(tpms.Deallocate(static_cast<U8*>(addresses[numAllocations - 1]) + allocationSize + 1),
                                Exception);


    tpms.Deallocate(addresses[0]);
    // memory full --->  memory is only freed if all allocated memory is deallocated
    BOOST_CHECK_THROW(addresses[0] = tpms.Allocate(allocationSize), Exception);

    for (U32 i = 1; i < numAllocations; ++i)
        tpms.Deallocate(addresses[i]);

    GDL_CHECK_THROW_DEV_DISABLE(tpms.Deallocate(addresses[0]), Exception);



    tpms.Deinitialize();
}


BOOST_AUTO_TEST_CASE(Alignment)
{
    constexpr U32 alignment = 128;
    constexpr U32 memorySize = alignment * 4;
    threadPrivateMemoryStack tpms{memorySize, alignment};
    std::array<void*, 3> addresses{{nullptr, nullptr, nullptr}};

    tpms.Initialize();

    addresses[0] = tpms.Allocate(alignment - alignment / 2);
    addresses[1] = tpms.Allocate(alignment);
    addresses[2] = tpms.Allocate(alignment + alignment / 2);

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK(is_aligned(addresses[i], alignment));

    for (U32 i = 0; i < addresses.size(); ++i)
        tpms.Deallocate(addresses[i]);

    tpms.Deinitialize();
}

//! @brief Checks than only the owning thread can modify the state of the memory stack
BOOST_AUTO_TEST_CASE(Non_owning_thread_access)
{
    std::atomic_bool constructed = false;
    std::atomic_bool initialize = false;
    std::atomic_bool allocated = false;
    std::atomic_bool deinitialize = false;

    std::unique_ptr<threadPrivateMemoryStack> tpms{nullptr};
    void* address = nullptr;

    std::thread thread{[&]() {
        tpms.reset(new threadPrivateMemoryStack{100});
        constructed = true;

        while (!initialize)
            std::this_thread::yield();
        (*tpms).Initialize();
        address = (*tpms).Allocate(10);
        allocated = true;

        while (!deinitialize)
            std::this_thread::yield();
        (*tpms).Deallocate(address);
        (*tpms).Deinitialize();
    }};

    while (!constructed)
        std::this_thread::yield();

    threadPrivateMemoryStack& tpmsRef = *tpms;

    BOOST_CHECK_THROW(tpmsRef.Initialize(), Exception);
    initialize = true;
    while (!allocated)
        std::this_thread::yield();

    GDL_CHECK_THROW_DEV_DISABLE(tpmsRef.Allocate(10), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(tpmsRef.Deallocate(address), Exception);

    BOOST_CHECK_THROW(tpmsRef.Deinitialize(), Exception);
    deinitialize = true;

    thread.join();
}

//-------------------------------------------------------------------------------------------------
// Add test for multiple initializations and deinitializationsb
//-------------------------------------------------------------------------------------------------
