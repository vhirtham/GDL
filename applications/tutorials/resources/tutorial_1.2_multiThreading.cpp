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
    {
        Thread thread(HelloFromThread);
    }


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
}
