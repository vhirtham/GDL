#include <boost/test/unit_test.hpp>

#include "gdl/base/container/vector.h"
#include "gdl/resources/cpu/threadPool.h"
#include "gdl/resources/memory/memoryManager.h"
#include "gdl/resources/memory/utility/heapAllocationCounter.h"

using namespace GDL;

MemoryManager& SetupMemorySystem()
{
    MemoryManager& mm = MemoryManager::Instance();
    mm.CreateGeneralPurposeMemory(10_MB);
    mm.CreateMemoryPool(128_B, 1000);
    mm.EnableThreadPrivateMemory();
    return mm;
}

void WorkerThreadInitializeFunction()
{
    MemoryManager& mm = MemoryManager::Instance();
    mm.CreatePrivateMemoryStackForThisThread(1_MB);
}

void WorkerThreadMainLoopFunction(ThreadPool<2>& tp)
{
}

void WorkerThreadDeinitializeFunction()
{
    MemoryManager& mm = MemoryManager::Instance();
    mm.DeletePrivateMemoryStackForThisThread();
}

void WorkForMainThread(U32 val)
{
    int a = 0;
}

void WorkForWorkers(ThreadPool<2>& tp)
{
    constexpr U32 numIterations = 10000;
    U32 sum = 0;
    VectorTPS<U32> vec;
    for (U32 i = 0; i < numIterations; ++i)
        vec.push_back(i);

    for (U32 i = 0; i < vec.size(); ++i)
        sum += vec[i];

    tp.SubmitToQueue(0, &WorkForMainThread, sum);
}



void MainLoop()
{
    constexpr U32 numSubmits = 100;
    HeapAllocationCounter hac;
    ThreadPool<2> tp(0);
    tp.StartThreads(3, [&] { tp.TryExecuteTask(1); }, &WorkerThreadInitializeFunction,
                    &WorkerThreadDeinitializeFunction);


    for (U32 i = 0; i < numSubmits; ++i)
        tp.SubmitToQueue(1, &WorkForWorkers, std::ref(tp));


    tp.CloseAllThreads();
    tp.PropagateExceptions();
    tp.Deinitialize();

    // Per thread - 1 create thread, 1 create thread private memory , 1 put thread private memory into map
    hac.CheckNumCallsExpectedCustomAllocation(9, 9);
}

BOOST_AUTO_TEST_CASE(Engine_Loop)
{
    MemoryManager& mm = SetupMemorySystem();
    mm.Initialize();

    MainLoop();

    mm.Deinitialize();
}
