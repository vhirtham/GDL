#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/time.h"
#include "gdl/base/timer.h"
#include "gdl/resources/memory/memoryManager.h"
#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"


#include <iostream>
#include <memory>

using namespace GDL;
using namespace std::chrono_literals;

void PrintAndResetResults(size_t allocationSize, std::array<std::pair<Nanoseconds, Nanoseconds>, 5>& results)
{
    std::cout << "Allocation size: " << allocationSize << " Bytes" << std::endl;
    std::cout << "Allocator | Allocation | Deallocation | Total | Ratio" << std::endl;

    Nanoseconds totalSTD = results[0].first + results[0].second;
    for (U32 i = 0; i < 5; ++i)
    {
        switch (i)
        {
        case 0:
            std::cout << "std::allocator     ";
            break;
        case 1:
            std::cout << "GeneralPurpose     ";
            break;
        case 2:
            std::cout << "Pool               ";
            break;
        case 3:
            std::cout << "Stack              ";
            break;
        case 4:
            std::cout << "ThreadPrivateStack ";
            break;
        }

        Nanoseconds total = results[i].first + results[i].second;
        std::cout << results[i].first.count() << " | " << results[i].second.count() << " | " << total.count() << " | "
                  << static_cast<F32>(total.count()) / static_cast<F32>(totalSTD.count()) << std::endl;
    }

    std::cout << std::endl;
}

template <template <typename> class _allocator, typename _type, U32 _numAllocations>
std::pair<Nanoseconds, Nanoseconds> AllocationBenchmark1()
{
    constexpr U32 numRuns = 1;
    Timer timer;
    std::array<_type*, _numAllocations> objects;

    _allocator<_type> allocator;

    Nanoseconds allocationTime = 0ns;
    Nanoseconds deallocationTime = 0ns;

    for (U32 run = 0; run < numRuns; ++run)
    {
        timer.Reset();
        for (U32 i = 0; i < _numAllocations; ++i)
            objects[i] = allocator.allocate(1);
        allocationTime += timer.GetElapsedTime<Nanoseconds>();

        timer.Reset();
        for (U32 i = 0; i < _numAllocations; ++i)
            allocator.deallocate(objects[i], 1);
        deallocationTime += timer.GetElapsedTime<Nanoseconds>();
    }

    std::cout << allocationTime.count() << std::endl;
    return std::pair<Nanoseconds, Nanoseconds>{allocationTime, deallocationTime};
}

template <template <typename> class _allocator, typename _type, U32 _numAllocations>
std::pair<Nanoseconds, Nanoseconds> AllocationBenchmark2()
{
    constexpr U32 numRuns = 100;
    Timer timer;
    std::array<std::array<_type*, _numAllocations>, numRuns> objects;

    _allocator<_type> allocator;

    Nanoseconds allocationTime = 0ns;
    Nanoseconds deallocationTime = 0ns;

    for (U32 run = 0; run < numRuns; ++run)
    {
        timer.Reset();
        for (U32 i = 0; i < _numAllocations; ++i)
            objects[run][i] = allocator.allocate(1);
        allocationTime += timer.GetElapsedTime<Nanoseconds>();
    }

    for (U32 run = 0; run < numRuns; ++run)
    {
        timer.Reset();
        for (U32 i = 0; i < _numAllocations; ++i)
            allocator.deallocate(objects[run][i], 1);
        deallocationTime += timer.GetElapsedTime<Nanoseconds>();
    }

    return std::pair<Nanoseconds, Nanoseconds>{allocationTime, deallocationTime};
}

template <template <typename> class _allocator, typename _type, U32 _numAllocations>
std::pair<Nanoseconds, Nanoseconds> AllocationBenchmark3()
{
    constexpr U32 numRuns = 100;
    Timer timer;
    std::array<_type*, _numAllocations> objects;

    _allocator<_type> allocator;

    Nanoseconds allocationTime = 0ns;
    Nanoseconds deallocationTime = 0ns;


    for (U32 i = 0; i < _numAllocations; ++i)
        objects[i] = allocator.allocate(1);


    //    allocator.deallocate(objects[0], 1);
    //    allocator.deallocate(objects[2], 1);
    //    allocator.deallocate(objects[4], 1);
    //    allocator.deallocate(objects[6], 1);
    //    allocator.deallocate(objects[8], 1);
    //    allocator.deallocate(objects[10], 1);
    //    allocator.deallocate(objects[12], 1);
    //    allocator.deallocate(objects[14], 1);
    //    allocator.deallocate(objects[16], 1);
    //    allocator.deallocate(objects[18], 1);

    for (U32 run = 0; run < numRuns; ++run)
    {
        timer.Reset();
        for (U32 i = 20; i < _numAllocations; ++i)
            allocator.deallocate(objects[i], 1);
        deallocationTime += timer.GetElapsedTime<Nanoseconds>();

        timer.Reset();
        for (U32 i = 20; i < _numAllocations; ++i)
            objects[i] = allocator.allocate(1);
        allocationTime += timer.GetElapsedTime<Nanoseconds>();
    }


    //    objects[0] = allocator.allocate(1);
    //    objects[2] = allocator.allocate(1);
    //    objects[4] = allocator.allocate(1);
    //    objects[6] = allocator.allocate(1);
    //    objects[8] = allocator.allocate(1);
    //    objects[10] = allocator.allocate(1);
    //    objects[12] = allocator.allocate(1);
    //    objects[14] = allocator.allocate(1);
    //    objects[16] = allocator.allocate(1);
    //    objects[18] = allocator.allocate(1);

    for (U32 i = 0; i < _numAllocations; ++i)
        allocator.deallocate(objects[i], 1);


    return std::pair<Nanoseconds, Nanoseconds>{allocationTime, deallocationTime};
}

template <typename _type, U32 _numAllocations>
void RunAllocationBenchmarks1()
{
    std::array<std::pair<Nanoseconds, Nanoseconds>, 5> allocatorResults;

    allocatorResults[0] = AllocationBenchmark1<std::allocator, _type, _numAllocations>();
    allocatorResults[1] = AllocationBenchmark1<GeneralPurposeAllocator, _type, _numAllocations>();
    allocatorResults[2] = AllocationBenchmark1<PoolAllocator, _type, _numAllocations>();
    allocatorResults[3] = AllocationBenchmark1<StackAllocator, _type, _numAllocations>();
    allocatorResults[4] = AllocationBenchmark1<ThreadPrivateStackAllocator, _type, _numAllocations>();

    PrintAndResetResults(sizeof(_type), allocatorResults);
}

template <typename _type, U32 _numAllocations>
void RunAllocationBenchmarks2()
{
    std::array<std::pair<Nanoseconds, Nanoseconds>, 5> allocatorResults;

    allocatorResults[0] = AllocationBenchmark2<std::allocator, _type, _numAllocations>();
    allocatorResults[1] = AllocationBenchmark2<GeneralPurposeAllocator, _type, _numAllocations>();
    allocatorResults[2] = AllocationBenchmark2<PoolAllocator, _type, _numAllocations>();
    allocatorResults[3] = AllocationBenchmark2<StackAllocator, _type, _numAllocations>();
    allocatorResults[4] = AllocationBenchmark2<ThreadPrivateStackAllocator, _type, _numAllocations>();

    PrintAndResetResults(sizeof(_type), allocatorResults);
}


template <typename _type, U32 _numAllocations>
void RunAllocationBenchmarks3()
{
    std::array<std::pair<Nanoseconds, Nanoseconds>, 5> allocatorResults;

    allocatorResults[0] = AllocationBenchmark3<std::allocator, _type, _numAllocations>();
    allocatorResults[1] = AllocationBenchmark3<GeneralPurposeAllocator, _type, _numAllocations>();
    allocatorResults[2] = AllocationBenchmark3<PoolAllocator, _type, _numAllocations>();
    allocatorResults[3] = AllocationBenchmark3<StackAllocator, _type, _numAllocations>();
    allocatorResults[4] = AllocationBenchmark3<ThreadPrivateStackAllocator, _type, _numAllocations>();

    PrintAndResetResults(sizeof(_type), allocatorResults);
}

int main()
{
    constexpr U32 maxNumAllocations = 10000;

    MemoryManager& mm = MemoryManager::Instance();
    mm.CreateGeneralPurposeMemory(200_MB);
    mm.CreateMemoryPool(32_B, maxNumAllocations);
    mm.CreateMemoryPool(64_B, maxNumAllocations);
    mm.CreateMemoryPool(128_B, maxNumAllocations);
    mm.CreateMemoryPool(256_B, maxNumAllocations);
    mm.CreateMemoryPool(512_B, maxNumAllocations);
    mm.CreateMemoryStack(200_MB);
    mm.EnableThreadPrivateMemory();
    mm.Initialize();
    mm.CreatePrivateMemoryStackForThisThread(200_MB);


    RunAllocationBenchmarks1<std::array<U8, 256>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 8>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 16>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 32>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 64>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 128>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 256>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 512>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 1024>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 2048>, maxNumAllocations>();
    //    RunAllocationBenchmarks1<std::array<U8, 4096>, maxNumAllocations>();


    //    RunAllocationBenchmarks3<F32, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 8>, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 16>, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 32>, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 64>, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 128>, 10000>();

    mm.DeletePrivateMemoryStackForThisThread();
    mm.Deinitialize();
}
