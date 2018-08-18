#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"


using namespace GDL;

#ifndef TEST_THREAD_SAFETY


#include "memoryManagerSetup.h"

BOOST_AUTO_TEST_CASE(General)
{
    MemoryManager& memoryManager = GetMemoryManager();


// Custom memory enabled
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL) && defined(NO_MEMORY_STACK) &&                     \
      defined(NO_THREAD_PRIVATE_MEMORY_STACK))

// Already created/enabled exceptions
#ifndef NO_GENERAL_PURPOSE_MEMORY
    BOOST_CHECK_THROW(memoryManager.CreateGeneralPurposeMemory(1_MB), Exception);
#endif
#ifndef NO_MEMORY_STACK
    BOOST_CHECK_THROW(memoryManager.CreateMemoryStack(1_MB), Exception);
#endif
#ifndef NO_MEMORY_POOL
    BOOST_CHECK_THROW(memoryManager.CreateMemoryPool(32_B, 100), Exception);
#endif
#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
    BOOST_CHECK_THROW(memoryManager.EnableThreadPrivateMemory(), Exception);
    // Not Initialized
    BOOST_CHECK_THROW(memoryManager.CreatePrivateMemoryStackForThisThread(1_MB), Exception);
#endif



    // Initialize
    BOOST_CHECK_NO_THROW(memoryManager.Initialize());



// Addition and deletion of thread private stacks
#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
    BOOST_CHECK_THROW(memoryManager.DeletePrivateMemoryStackForThisThread(), Exception);
    BOOST_CHECK(memoryManager.GetThreadPrivateMemoryStack() == nullptr);
    BOOST_CHECK_NO_THROW(memoryManager.CreatePrivateMemoryStackForThisThread(1_MB));
#endif

    BOOST_CHECK_THROW(memoryManager.CreatePrivateMemoryStackForThisThread(1_MB), Exception);

#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
    BOOST_CHECK_THROW(memoryManager.Deinitialize(), Exception);
    BOOST_CHECK_NO_THROW(memoryManager.DeletePrivateMemoryStackForThisThread());
#endif

    BOOST_CHECK_THROW(memoryManager.DeletePrivateMemoryStackForThisThread(), Exception);



    // Deinitialize
    BOOST_CHECK_NO_THROW(memoryManager.Deinitialize());



// Only heap allocations
#else

    // No memory added
    BOOST_CHECK_THROW(memoryManager.Initialize(), Exception);
    BOOST_CHECK_THROW(memoryManager.Deinitialize(), Exception);



    // Setup yet not finished
    memoryManager.CreateGeneralPurposeMemory(1_MB);
    memoryManager.CreateMemoryStack(1_MB);
    memoryManager.CreateMemoryPool(32_B, 100);
    memoryManager.EnableThreadPrivateMemory();



    // These checked functions end the setup process and make the memory manager uninitializeable;
    BOOST_CHECK(memoryManager.GetGeneralPurposeMemory() == nullptr);
    BOOST_CHECK(memoryManager.GetMemoryStack() == nullptr);
    BOOST_CHECK(memoryManager.GetMemoryPool(12, 4) == nullptr);
    BOOST_CHECK(memoryManager.GetThreadPrivateMemoryStack() == nullptr);



    // Memory was requested before initialization (fallback to heap allocation)
    BOOST_CHECK_THROW(memoryManager.Initialize(), Exception);
    BOOST_CHECK_THROW(memoryManager.Deinitialize(), Exception);
#endif



    // Exceptions because memory setup is already finished
    BOOST_CHECK_THROW(memoryManager.CreateGeneralPurposeMemory(1_MB), Exception);
    BOOST_CHECK_THROW(memoryManager.CreateMemoryStack(1_MB), Exception);
    BOOST_CHECK_THROW(memoryManager.CreateMemoryPool(32_B, 100), Exception);
    BOOST_CHECK_THROW(memoryManager.EnableThreadPrivateMemory(), Exception);
}

#else // TEST_THREAD_SAFETY

#include "gdl/GDLTypedefs.h"
#include "gdl/resources/cpu/utility/deadlockTerminationTimer.h"
#include "gdl/resources/memory/memoryManager.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>


struct SynchronizationData
{
    std::atomic<U32> startTestCounter = 0;
    std::atomic<U32> threadReadyCounter = 0;
    std::atomic<U32> exceptionCounter = 0;
};



template <U32 _testNumber, typename _functionType>
void MemoryManagerFunctionTest(_functionType function, SynchronizationData& sd)
{
    while (sd.startTestCounter != _testNumber)
        std::this_thread::yield();
    try
    {
        function();
    }
    catch (Exception&)
    {
        ++sd.exceptionCounter;
    }
    ++sd.threadReadyCounter;
}



BOOST_AUTO_TEST_CASE(Thread_Safety)
{
    DeadlockTerminationTimer dtt;
    MemoryManager& mm = MemoryManager::Instance();

    constexpr U32 numThreads = 10;
    constexpr U32 numTestedFunctions = 6;

    SynchronizationData sd;

    std::vector<std::thread> threads;

    // Add threads
    for (U32 i = 0; i < numThreads; ++i)
        threads.emplace_back([&]() {

            ++sd.threadReadyCounter;

            MemoryManagerFunctionTest<1>([&]() { mm.CreateGeneralPurposeMemory(1_MB); }, sd);
            MemoryManagerFunctionTest<2>([&]() { mm.CreateMemoryStack(1_MB); }, sd);
            MemoryManagerFunctionTest<3>([&]() { mm.CreateMemoryPool(32_B, 100); }, sd);
            MemoryManagerFunctionTest<4>([&]() { mm.EnableThreadPrivateMemory(); }, sd);
            MemoryManagerFunctionTest<5>([&]() { mm.Initialize(); }, sd);
            MemoryManagerFunctionTest<6>([&]() { mm.Deinitialize(); }, sd);

            // the thread private memory stack creation and destruction is already tested in the allocator test

        });



    // Wait until all threads are ready for the first test
    while (sd.threadReadyCounter != numThreads)
        std::this_thread::yield();



    for (U32 i = 0; i < numTestedFunctions; ++i)
    {
        // start next test
        sd.threadReadyCounter = 0;
        ++sd.startTestCounter;

        // wait until test is finished
        while (sd.threadReadyCounter != numThreads)
            std::this_thread::yield();

        // Check test that only one thread succeeded. The others should throw
        BOOST_CHECK(sd.exceptionCounter == numThreads - 1);
        if (sd.exceptionCounter != numThreads - 1)
            std::cout << "Failed test number = " << std::to_string(i + 1) << std::endl;
        sd.exceptionCounter = 0;
    }


    for (U32 i = 0; i < numThreads; ++i)
        threads[i].join();
}

#endif // TEST_THREAD_SAFETY
