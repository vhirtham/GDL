#include "gdl/resources/cpu/thread.h"
#include "gdl/resources/cpu/threadPool.h"

#include <iostream>

using namespace GDL;


void HelloFromThread()
{
    std::cout << "Hello from thread with ID " << std::this_thread::get_id() << "!" << std::endl;
}


void PrintValue(I32 value)
{
    std::cout << "value: " << value << std::endl;
}

void SetValueTo1337(I32& value)
{
    value = 1337;
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Thread class -------------------------------------------------------------------------------

    {
        Thread thread(HelloFromThread);
    }



    // Thread pool --------------------------------------------------------------------------------

    ThreadPool<> pool;

    pool.StartThreads(2);

    for (U32 i = 0; i < 2; ++i)
        pool.Submit(HelloFromThread);
    std::this_thread::sleep_for(std::chrono::milliseconds{10});

    pool.Submit(PrintValue, 42);
    std::this_thread::sleep_for(std::chrono::milliseconds{10});

    I32 referencedValue = 0;
    pool.Submit(SetValueTo1337, std::ref(referencedValue));
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
    std::cout << "Referenced value: " << referencedValue << std::endl;

    std::cout << "Running threads: " << pool.GetNumThreads() << std::endl;
    pool.CloseThreads(1);
    std::cout << "Running threads: " << pool.GetNumThreads() << std::endl;
    pool.CloseAllThreads();
    std::cout << "Running threads: " << pool.GetNumThreads() << std::endl;

    U32 counter = 0;
    auto threadMainFunc = [&]() { ++counter; };
    pool.StartThreads(1, threadMainFunc);
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
    pool.CloseAllThreads();

    std::cout << "Thread main loop iterations: " << counter << std::endl;


    auto threadInitFunc = [&]() { std::cout << "Start thread" << std::endl; };
    auto threadDeinitFunc = [&]() { std::cout << "Stop thread" << std::endl; };
    pool.StartThreads(1, threadMainFunc, threadInitFunc, threadDeinitFunc);
    pool.CloseAllThreads();

    std::cout << "Exception log size: " << pool.GetExceptionLogSize() << std::endl;

    pool.PropagateExceptions();
    pool.Deinitialize();


    // Thread pool with multiple queues -----------------------------------------------------------

    ThreadPool<2> pool2Queues;

    auto prioritizeFirstQueue = [&]() {
        if (pool2Queues.HasTasks(0))
            pool2Queues.TryExecuteTask(0);
        else
            pool2Queues.TryExecuteTask(1);
    };

    pool2Queues.StartThreads(2, prioritizeFirstQueue);

    for (U32 i = 0; i < 10; ++i)
        pool2Queues.Submit(1, []() {
            std::cout << "Task from second queue processed" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds{1});
        });

    for (U32 i = 0; i < 10; ++i)
        pool2Queues.Submit(0, []() {
            std::cout << "Task from first queue processed" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds{1});
        });

    while (pool2Queues.HasTasks(0) || pool2Queues.HasTasks(1))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{10});
    }

    pool2Queues.Deinitialize();
}
