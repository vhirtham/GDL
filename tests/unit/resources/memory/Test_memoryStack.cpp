#include <boost/test/unit_test.hpp>
#include "tests/tools/ExceptionChecks.h"

#include "gdl/base/Exception.h"
#include "gdl/base/SSESupportFunctions.h"
#include "gdl/resources/memory/memoryStack.h"

#include <array>
#include <atomic>
#include <memory>
#include <thread>

using namespace GDL;

template <bool _ThreadPrivate>
void ConstructionDestructionTest()
{
    BOOST_CHECK_NO_THROW(memoryStackTemplate<_ThreadPrivate>(100));

    BOOST_CHECK_THROW(memoryStackTemplate<_ThreadPrivate>(0), Exception);
}

BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    ConstructionDestructionTest<true>();
    ConstructionDestructionTest<false>();
}


template <bool _ThreadPrivate>
void InitializationDeinitializationExceptions()
{
    constexpr U32 memorySize = 128;
    memoryStackTemplate<_ThreadPrivate> tpms{memorySize};


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


//! @brief Checks if exceptions during initialization and deinitialization are thrown as expected
BOOST_AUTO_TEST_CASE(Initialization_Deinitialization_Exceptions)
{
    InitializationDeinitializationExceptions<true>();
    InitializationDeinitializationExceptions<false>();
}



template <bool _ThreadPrivate>
void AllocationDeallocation()
{
    constexpr U32 numAllocations = 5;
    constexpr U32 allocationSize = 10;
    constexpr U32 memorySize = allocationSize * numAllocations;
    memoryStackTemplate<_ThreadPrivate> tpms{memorySize};

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
    BOOST_CHECK_THROW(addresses[0] = tpms.Allocate(1), Exception);

    for (U32 i = 1; i < numAllocations; ++i)
        tpms.Deallocate(addresses[i]);

    GDL_CHECK_THROW_DEV_DISABLE(tpms.Deallocate(addresses[0]), Exception);



    tpms.Deinitialize();
}

BOOST_AUTO_TEST_CASE(Allocation_and_Deallocation)
{
    AllocationDeallocation<true>();
    AllocationDeallocation<false>();
}


template <bool _ThreadPrivate>
void AlignedAllocation()
{
    constexpr U32 alignment = 128;
    constexpr U32 memorySize = alignment * 5;
    memoryStackTemplate<_ThreadPrivate> tpms{memorySize};
    std::array<void*, 3> addresses{{nullptr, nullptr, nullptr}};

    tpms.Initialize();

    // The following lines allocate memory with total size of 4 * <alignment>. Another allocation might fail, because
    // the chances are high that the first pointer after allocation needs to be corrected. So there is probably no space
    // left for another element with size <alignment> even if it should not be aligned.
    addresses[0] = tpms.Allocate(0.5 * alignment, alignment);
    addresses[1] = tpms.Allocate(1.5 * alignment, alignment);
    addresses[2] = tpms.Allocate(alignment, alignment);

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK(is_aligned(addresses[i], alignment));

    for (U32 i = 0; i < addresses.size(); ++i)
        tpms.Deallocate(addresses[i]);

    tpms.Deinitialize();
}


BOOST_AUTO_TEST_CASE(Alignment)
{
    AlignedAllocation<true>();
    AlignedAllocation<false>();
}

//! @brief Checks than only the owning thread can modify the state of the memory stack
BOOST_AUTO_TEST_CASE(Thread_safety_thread_private)
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
