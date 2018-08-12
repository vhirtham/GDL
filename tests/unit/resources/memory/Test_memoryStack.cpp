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
    BOOST_CHECK_NO_THROW(memoryStackTemplate<_ThreadPrivate>(100_B));

    BOOST_CHECK_THROW(memoryStackTemplate<_ThreadPrivate>(0_B), Exception);
}

BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    ConstructionDestructionTest<true>();
    ConstructionDestructionTest<false>();
}


template <bool _ThreadPrivate>
void InitializationDeinitializationExceptions()
{
    constexpr MemorySize memorySize = 128_B;
    memoryStackTemplate<_ThreadPrivate> ms{memorySize};


    GDL_CHECK_THROW_DEV_DISABLE(ms.Allocate(10), Exception);
    ms.Initialize();

    // already initialized
    BOOST_CHECK_THROW(ms.Initialize(), Exception);

    void* address = ms.Allocate(10);
    // memory still in use
    BOOST_CHECK_THROW(ms.Deinitialize(), Exception);

    ms.Deallocate(address);
    ms.Deinitialize();

    // already deinitialized
    BOOST_CHECK_THROW(ms.Deinitialize(), Exception);
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
    constexpr MemorySize memorySize = allocationSize * numAllocations * 1_B;
    memoryStackTemplate<_ThreadPrivate> ms{memorySize};

    std::array<void*, numAllocations> addresses;
    addresses.fill(nullptr);

    ms.Initialize();

    GDL_CHECK_THROW_DEV_DISABLE(ms.Allocate(0), Exception);

    for (U32 i = 0; i < numAllocations; ++i)
        addresses[i] = ms.Allocate(allocationSize);


    // nullptr
    GDL_CHECK_THROW_DEV_DISABLE(ms.Deallocate(nullptr), Exception);

    // out of range
    GDL_CHECK_THROW_DEV_DISABLE(ms.Deallocate(static_cast<U8*>(addresses[0]) - allocationSize), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(ms.Deallocate(static_cast<U8*>(addresses[numAllocations - 1]) + allocationSize + 1),
                                Exception);


    ms.Deallocate(addresses[0]);
    // memory full --->  memory is only freed if all allocated memory is deallocated
    BOOST_CHECK_THROW(addresses[0] = ms.Allocate(1), Exception);

    for (U32 i = 1; i < numAllocations; ++i)
        ms.Deallocate(addresses[i]);

    GDL_CHECK_THROW_DEV_DISABLE(ms.Deallocate(addresses[0]), Exception);



    ms.Deinitialize();
}

BOOST_AUTO_TEST_CASE(Allocation_and_Deallocation)
{
    AllocationDeallocation<true>();
    AllocationDeallocation<false>();
}


template <bool _ThreadPrivate>
void MultipleInitialization()
{
    constexpr U32 numAllocations = 5;
    constexpr U32 allocationSize = 10;
    constexpr MemorySize memorySize = allocationSize * numAllocations * 1_B;
    memoryStackTemplate<_ThreadPrivate> ms{memorySize};

    std::array<void*, numAllocations> addresses;
    addresses.fill(nullptr);

    for (U32 k = 0; k < 3; ++k)
    {
        ms.Initialize();

        for (U32 i = 0; i < numAllocations; ++i)
            addresses[i] = ms.Allocate(allocationSize);



        for (U32 i = 0; i < numAllocations; ++i)
        {
            ms.Deallocate(addresses[i]);
            addresses[i] = nullptr;
        }

        ms.Deinitialize();
    }
}

BOOST_AUTO_TEST_CASE(Multiple_Initialization)
{
    MultipleInitialization<true>();
    MultipleInitialization<false>();
}



template <bool _ThreadPrivate>
void AlignedAllocation()
{
    constexpr U32 alignment = 128;
    constexpr MemorySize memorySize = alignment * 5_B;
    memoryStackTemplate<_ThreadPrivate> ms{memorySize};
    std::array<void*, 3> addresses{{nullptr, nullptr, nullptr}};

    ms.Initialize();

    // alignment is no power of 2
    GDL_CHECK_THROW_DEV_DISABLE(ms.Allocate(alignment, 5), Exception);

    // The following lines allocate memory with total size of 4 * <alignment>. Another allocation might fail, because
    // the chances are high that the first pointer after allocation needs to be corrected. So there is probably no space
    // left for another element with size <alignment> even if it should not be aligned.
    addresses[0] = ms.Allocate(0.5 * alignment, alignment);
    addresses[1] = ms.Allocate(1.5 * alignment, alignment);
    addresses[2] = ms.Allocate(alignment, alignment);

    for (U32 i = 0; i < addresses.size(); ++i)
        BOOST_CHECK(is_aligned(addresses[i], alignment));

    for (U32 i = 0; i < addresses.size(); ++i)
        ms.Deallocate(addresses[i]);

    ms.Deinitialize();
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

    std::unique_ptr<threadPrivateMemoryStack> ms{nullptr};
    void* address = nullptr;

    std::thread thread{[&]() {
        ms.reset(new threadPrivateMemoryStack{100_B});
        constructed = true;

        while (!initialize)
            std::this_thread::yield();
        (*ms).Initialize();
        address = (*ms).Allocate(10);
        allocated = true;

        while (!deinitialize)
            std::this_thread::yield();
        (*ms).Deallocate(address);
        (*ms).Deinitialize();
    }};

    while (!constructed)
        std::this_thread::yield();

    threadPrivateMemoryStack& msRef = *ms;

    BOOST_CHECK_THROW(msRef.Initialize(), Exception);
    initialize = true;
    while (!allocated)
        std::this_thread::yield();

    GDL_CHECK_THROW_DEV_DISABLE(msRef.Allocate(10), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(msRef.Deallocate(address), Exception);

    BOOST_CHECK_THROW(msRef.Deinitialize(), Exception);
    deinitialize = true;

    thread.join();
}


BOOST_AUTO_TEST_CASE(Thread_safety_non_thread_private)
{
    constexpr U32 numAllocations = 500;
    constexpr U32 allocationSize = sizeof(I32);
    constexpr U32 numThreads = 4;
    constexpr MemorySize memorySize = allocationSize * numAllocations * numThreads * 1_B;

    memoryStack ms{memorySize};
    ms.Initialize();

    std::array<std::atomic_bool, numThreads> threadReady;
    std::array<I32, numThreads> sum;
    std::fill(sum.begin(), sum.end(), 0);

    for (U32 i = 0; i < numThreads; ++i)
        threadReady[i] = false;

    std::atomic_bool kickoff = false;

    std::vector<std::thread> threads;
    for (U32 i = 0; i < numThreads; ++i)

        threads.emplace_back([i, &ms, &sum, &threadReady, &kickoff]() {
            threadReady[i] = true;
            std::array<I32*, numAllocations> values;

            while (!kickoff)
                std::this_thread::yield();

            for (U32 j = 0; j < numAllocations; ++j)
            {
                values[j] = static_cast<I32*>(ms.Allocate(allocationSize));
                *(values[j]) = j + 1;
            }
            for (U32 j = 0; j < numAllocations; ++j)
                sum[i] += *(values[j]);

            for (U32 j = 0; j < numAllocations; ++j)
                ms.Deallocate(values[j]);

        });

    bool allThreadReady = false;
    while (!allThreadReady)
    {
        allThreadReady = threadReady[0];
        for (U32 i = 1; i < numThreads; ++i)
            allThreadReady = allThreadReady && threadReady[i];
        if (!allThreadReady)
            std::this_thread::yield();
    }



    kickoff = true;
    for (U32 i = 0; i < numThreads; ++i)
        threads[i].join();

    I32 expectedResult = (numAllocations * numAllocations + numAllocations) / 2;

    for (U32 i = 0; i < numThreads; ++i)
        BOOST_CHECK(sum[i] == expectedResult);


    ms.Deinitialize();
}
