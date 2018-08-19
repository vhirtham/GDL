#include "gdl/GDLTypedefs.h"

#include "gdl/GDLTypedefs.h"
#include "gdl/base/timer.h"
#include "gdl/resources/memory/memoryManager.h"
#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"


#include <iostream>
#include <memory>

using namespace GDL;

void PrintAndResetResults(size_t allocationSize, std::array<std::pair<U32, U32>, 5>& results)
{
    std::cout << "Allocation size: " << allocationSize << " Bytes" << std::endl;
    std::cout << "Allocator | Allocation | Deallocation | Total | Ratio" << std::endl;

    U32 totalSTD = results[0].first + results[0].second;
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

        U32 total = results[i].first + results[i].second;
        std::cout << results[i].first << " | " << results[i].second << " | " << results[i].first << " | " << total
                  << " | " << static_cast<F32>(total) / static_cast<F32>(totalSTD) << std::endl;

        results[i].first = 0;
        results[i].second = 0;
    }

    std::cout << std::endl;
}

template <template <typename> class _allocator, typename _type, U32 _numAllocations>
std::pair<U32, U32> AllocationBenchmark1()
{
    constexpr U32 numRuns = 10;
    Timer timer;
    std::array<_type*, _numAllocations> objects;

    _allocator<_type> allocator;

    U32 allocationTime = 0;
    U32 deallocationTime = 0;

    for (U32 run = 0; run < numRuns; ++run)
    {
        timer.Reset();
        for (U32 i = 0; i < _numAllocations; ++i)
            objects[i] = allocator.allocate(1);
        allocationTime += timer.GetMicroseconds();

        timer.Reset();
        for (U32 i = 0; i < _numAllocations; ++i)
            allocator.deallocate(objects[i], 1);
        deallocationTime += timer.GetMicroseconds();
    }

    return std::pair<U32, U32>{allocationTime, deallocationTime};
}

template <template <typename> class _allocator, typename _type, U32 _numAllocations>
std::pair<U32, U32> AllocationBenchmark2()
{
    constexpr U32 numRuns = 100;
    Timer timer;
    std::array<std::array<_type*, _numAllocations>, numRuns> objects;

    _allocator<_type> allocator;

    U32 allocationTime = 0;
    U32 deallocationTime = 0;

    for (U32 run = 0; run < numRuns; ++run)
    {
        timer.Reset();
        for (U32 i = 0; i < _numAllocations; ++i)
            objects[run][i] = allocator.allocate(1);
        allocationTime += timer.GetMicroseconds();
    }

    for (U32 run = 0; run < numRuns; ++run)
    {
        timer.Reset();
        for (U32 i = 0; i < _numAllocations; ++i)
            allocator.deallocate(objects[run][i], 1);
        deallocationTime += timer.GetMicroseconds();
    }

    return std::pair<U32, U32>{allocationTime, deallocationTime};
}

template <template <typename> class _allocator, typename _type, U32 _numAllocations>
std::pair<U32, U32> AllocationBenchmark3()
{
    constexpr U32 numRuns = 100;
    Timer timer;
    std::array<_type*, _numAllocations> objects;

    _allocator<_type> allocator;

    U32 allocationTime = 0;
    U32 deallocationTime = 0;


    for (U32 i = 0; i < _numAllocations; ++i)
        objects[i] = allocator.allocate(1);


    for (U32 run = 0; run < numRuns; ++run)
    {
        timer.Reset();
        for (U32 i = 0; i < _numAllocations; i += 100)
            allocator.deallocate(objects[i], 1);
        deallocationTime += timer.GetMicroseconds();

        timer.Reset();
        for (U32 i = 0; i < _numAllocations; i += 100)
            objects[i] = allocator.allocate(1);
        allocationTime += timer.GetMicroseconds();
    }


    for (U32 i = 0; i < _numAllocations; ++i)
        allocator.deallocate(objects[i], 1);


    return std::pair<U32, U32>{allocationTime, deallocationTime};
}

template <typename _type, U32 _numAllocations>
void RunAllocationBenchmarks1()
{
    std::array<std::pair<U32, U32>, 5> allocatorResults;

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
    std::array<std::pair<U32, U32>, 5> allocatorResults;

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
    std::array<std::pair<U32, U32>, 5> allocatorResults;

    allocatorResults[0] = AllocationBenchmark3<std::allocator, _type, _numAllocations>();
    allocatorResults[1] = AllocationBenchmark3<GeneralPurposeAllocator, _type, _numAllocations>();
    allocatorResults[2] = AllocationBenchmark3<PoolAllocator, _type, _numAllocations>();
    allocatorResults[3] = AllocationBenchmark3<StackAllocator, _type, _numAllocations>();
    allocatorResults[4] = AllocationBenchmark3<ThreadPrivateStackAllocator, _type, _numAllocations>();

    PrintAndResetResults(sizeof(_type), allocatorResults);
}

int main()
{
    MemoryManager& mm = MemoryManager::Instance();
    mm.CreateGeneralPurposeMemory(1000_MB);
    mm.CreateMemoryPool(32_B, 1000000);
    mm.CreateMemoryPool(64_B, 1000000);
    mm.CreateMemoryPool(128_B, 1000000);
    mm.CreateMemoryPool(256_B, 1000000);
    mm.CreateMemoryPool(512_B, 1000000);
    mm.CreateMemoryStack(1000_MB);
    mm.EnableThreadPrivateMemory();
    mm.Initialize();
    mm.CreatePrivateMemoryStackForThisThread(1000_MB);


    RunAllocationBenchmarks1<F32, 100000>();
    RunAllocationBenchmarks1<std::array<F32, 8>, 100000>();
    RunAllocationBenchmarks1<std::array<F32, 16>, 100000>();
    RunAllocationBenchmarks1<std::array<F32, 32>, 100000>();
    RunAllocationBenchmarks1<std::array<F32, 64>, 100000>();
    RunAllocationBenchmarks1<std::array<F32, 128>, 100000>();
    RunAllocationBenchmarks1<std::array<F32, 256>, 100000>();
    RunAllocationBenchmarks1<std::array<F32, 512>, 100000>();
    RunAllocationBenchmarks1<std::array<F32, 1024>, 100000>();


    //    RunAllocationBenchmarks3<F32, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 8>, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 16>, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 32>, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 64>, 10000>();
    //    RunAllocationBenchmarks3<std::array<F32, 128>, 10000>();

    mm.DeletePrivateMemoryStackForThisThread();
    mm.Deinitialize();
}
