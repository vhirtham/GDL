#include <boost/test/unit_test.hpp>

#include "gdl/base/timer.h"
#include "gdl/base/container/vector.h"
#include "gdl/resources/cpu/threadPool.h"
#include "gdl/resources/memory/memoryManager.h"
#include "gdl/resources/memory/utility/heapAllocationCounter.h"

using namespace GDL;


// Memory setup -------------------------------------------------------------------------------------------------------

//! @brief Initializes the memory manager
void InitializeMemoryManager()
{
    MemoryManager& mm = MemoryManager::Instance();
    mm.CreateGeneralPurposeMemory(10_MB);
    mm.CreateMemoryPool(128_B, 10000);
    mm.EnableThreadPrivateMemory();
    mm.Initialize();
    mm.CreatePrivateMemoryStackForThisThread(1_MB);
}

//! @brief Deinitializes the memory manager
void DeinitializeMemoryManager()
{
    MemoryManager& mm = MemoryManager::Instance();
    mm.DeletePrivateMemoryStackForThisThread();
    mm.Deinitialize();
}



// Worker thread functions --------------------------------------------------------------------------------------------

//! @brief Initialization function for worker threads
void WorkerThreadInitializeFunction()
{
    MemoryManager& mm = MemoryManager::Instance();
    mm.CreatePrivateMemoryStackForThisThread(1_MB);
}



//! @brief Deinitialization function for worker threads
void WorkerThreadDeinitializeFunction()
{
    MemoryManager& mm = MemoryManager::Instance();
    mm.DeletePrivateMemoryStackForThisThread();
}



// Tasks and helper functions -----------------------------------------------------------------------------------------


//! @brief Gets a reference to a static sum variable
//! @return Reference to a static sum variable
U32& GetCurrentTotalSum()
{
    static U32 totalSum = 0;
    return totalSum;
}


//! @brief Adds a value to the total sum;
//! @param val: Value that is added
void MainThreadAddToTotalSum(U32 val)
{
    GetCurrentTotalSum() += val;
}


//! @brief Fills a thread private vector with numbers from 0 until numIterations and calculates the sum. Enques a task
//! to add the result to the global sum into the main thread exclusive queue.
//! @param tp: Reference to thread pool
//! @param numValues: Number of values that should be added to the vector
void WorkerThreadSumWithVectorTPS(ThreadPool<2>& tp, U32 numValues)
{
    VectorTPS<U32> vec;
    for (U32 i = 0; i < numValues; ++i)
        vec.push_back(i);

    U32 sum = 0;
    for (auto value : vec)
        sum += value;

    tp.SubmitToQueue(0, &MainThreadAddToTotalSum, sum);
}


//! @brief The tests main loop. Have a lok at the test description above the BOOST_AUTO_TEST_CASE macro for more details
void MainLoop()
{
    constexpr U32 numIterationsPerSubmit = 10000;
    constexpr U32 numSubmits = 100;
    constexpr U32 numThreads = 3;
    constexpr Seconds testDuration{1};

    HeapAllocationCounter hac;
    ThreadPool<2> tp(0);
    tp.StartThreads(numThreads, [&] { tp.TryExecuteTask(1); }, &WorkerThreadInitializeFunction,
                    &WorkerThreadDeinitializeFunction);



    Timer timer;
    while (timer.GetElapsedTime<Milliseconds>() < testDuration)
    {


        for (U32 i = 0; i < numSubmits; ++i)
            tp.SubmitToQueue(1, &WorkerThreadSumWithVectorTPS, std::ref(tp), numIterationsPerSubmit);

        U32 numResults = 0;
        GetCurrentTotalSum() = 0;
        while (numResults != numSubmits)
        {
            if (tp.TryExecuteTask(0))
            {
                ++numResults;
                continue;
            }

            tp.TryExecuteTask(1);
            tp.PropagateExceptions();
        }

        constexpr U32 expectedSumSubmit =
                (numIterationsPerSubmit * numIterationsPerSubmit - numIterationsPerSubmit) / 2;
        constexpr U32 expectedTotalSum = expectedSumSubmit * numSubmits;
        BOOST_CHECK(GetCurrentTotalSum() == expectedTotalSum);
    }

    tp.CloseAllThreads();
    tp.Deinitialize();

    // New calls per thread: 1 create thread - 1 create thread private memory - 1 add thread private memory into map
    constexpr U32 expectedAllocs = 3 * numThreads;
    BOOST_CHECK(hac.CheckNumCallsExpectedCustomAllocation(expectedAllocs, expectedAllocs));
}



// Test main ----------------------------------------------------------------------------------------------------------

//! @brief This test uses different memory systems and a threadpool to do some simple sum calculations. Two queues are
//! used. One is used exclusively by the main thread (index 0) while the other (index 1) is shared amongst all threads.
//! The shared queue is filled with tasks that store increasing numbers in different container types and calculate the
//! sum. Afterwards they submit a task to the main thread exclusive queue which adds the resulting sum to a total sum.
//! This test is repeated until a certain amount of time has passed.
BOOST_AUTO_TEST_CASE(ResourceManagement)
{
    InitializeMemoryManager();

    MainLoop();

    DeinitializeMemoryManager();
}
