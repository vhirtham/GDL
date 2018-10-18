#include <boost/test/unit_test.hpp>

#include "gdl/base/timer.h"
#include "gdl/base/container/queue.h"
#include "gdl/base/container/map.h"
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

//! @brief Calculates the expected result. This is basically the gaussian formula, but since 0 is part of the
//! summation, we had to substract numValues from the original formula.
//! @param numValues: Number of Values that are summed up
constexpr U32 CalculateExpectedResult(U32 numValues)
{
    return (numValues * numValues - numValues) / 2;
}



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

    tp.Submit(0, &MainThreadAddToTotalSum, sum);
}



//! @brief Fills a queue ( general purpose memory) with numbers from 0 until numIterations and calculates the sum.
//! Enques a task to
//! add the result to the global sum into the main thread exclusive queue.
//! @param tp: Reference to thread pool
//! @param numValues: Number of values that should be added to the queue
void WorkerThreadSumWithQueue(ThreadPool<2>& tp, U32 numValues)
{
    Queue<U32> queue;
    for (U32 i = 0; i < numValues; ++i)
        queue.emplace(i);

    U32 sum = 0;
    for (U32 i = 0; i < numValues; ++i)
    {
        sum += queue.front();
        queue.pop();
    }

    tp.Submit(0, &MainThreadAddToTotalSum, sum);
}



//! @brief Fills a map (memory pool) with numbers from 0 until numIterations and calculates the sum. Enques a task to
//! add the result to the global sum into the main thread exclusive queue.
//! @param tp: Reference to thread pool
//! @param numValues: Number of values that should be added to the map
void WorkerThreadSumWithMap(ThreadPool<2>& tp, U32 numValues)
{
    Map<U32, U32> map;
    for (U32 i = 0; i < numValues; ++i)
        map.emplace(i, i);

    U32 sum = 0;
    for (auto[key, value] : map)
        sum += (key + value) / 2;

    tp.Submit(0, &MainThreadAddToTotalSum, sum);
}



//! @brief The tests main loop. Have a lok at the test description above the BOOST_AUTO_TEST_CASE macro for more details
void MainLoop()
{
    constexpr U32 numIterationsPerSubmitVector = 10000;
    constexpr U32 numIterationsPerSubmitMap = 100;
    constexpr U32 numIterationsPerSubmitQueue = 100;
    constexpr U32 numSubmits = 100;
    constexpr U32 numThreads = 3;
    constexpr Seconds testDuration{1};

    HeapAllocationCounter hac;
    ThreadPool<2> tp;
    tp.StartThreads(numThreads, [&] { tp.TryExecuteTask(1); }, &WorkerThreadInitializeFunction,
                    &WorkerThreadDeinitializeFunction);



    Timer timer;
    U32 numMainLoopIterations = 0;
    while (timer.GetElapsedTime<Milliseconds>() < testDuration)
    {
        ++numMainLoopIterations;

        for (U32 i = 0; i < numSubmits; ++i)
        {
            tp.Submit(1, &WorkerThreadSumWithVectorTPS, std::ref(tp), numIterationsPerSubmitVector);
            tp.Submit(1, &WorkerThreadSumWithMap, std::ref(tp), numIterationsPerSubmitMap);
            tp.Submit(1, &WorkerThreadSumWithQueue, std::ref(tp), numIterationsPerSubmitQueue);
        }

        U32 numResults = 0;
        GetCurrentTotalSum() = 0;
        while (numResults != numSubmits * 3)
        {
            if (tp.TryExecuteTask(0))
            {
                ++numResults;
                continue;
            }

            tp.TryExecuteTask(1);
            tp.PropagateExceptions();
        }

        constexpr U32 expectedSumSubmitVector = CalculateExpectedResult(numIterationsPerSubmitVector);
        constexpr U32 expectedSumSubmitMap = CalculateExpectedResult(numIterationsPerSubmitMap);
        constexpr U32 expectedSumSubmitQueue = CalculateExpectedResult(numIterationsPerSubmitQueue);
        constexpr U32 expectedTotalSum =
                (expectedSumSubmitVector + expectedSumSubmitMap + expectedSumSubmitQueue) * numSubmits;
        BOOST_CHECK(GetCurrentTotalSum() == expectedTotalSum);
    }

    tp.CloseAllThreads();
    tp.Deinitialize();

    // New calls per thread: 1 create thread - 1 create thread private memory - 1 add thread private memory into map
    constexpr U32 expectedAllocs = 3 * numThreads;

    std::cout << "Test accomplished " << numMainLoopIterations << " main loop iterations." << std::endl;
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
