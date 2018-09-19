#include <boost/test/unit_test.hpp>

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/functions/alignment.h"
#include "gdl/resources/cpu/utility/deadlockTerminationTimer.h"
#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"
#include "gdl/resources/memory/utility/heapAllocationCounter.h"



#include "memoryManagerSetup.h"
#include "tests/tools/ExceptionChecks.h"

#include <array>
#include <atomic>
#include <map>
#include <memory>
#include <set>
#include <thread>
#include <vector>

#include "gdl/base/uniquePtr.h"

using namespace GDL;



//! Uncomment next line if this test shoul be run with valgrind
// #define DISABLE_HEAP_ALLOCATION_COUNTER



// Helper structs and functions ---------------------------------------------------------------------------------------

constexpr U32 alignment = 32;
struct alignas(alignment) AlignedStruct
{
    AlignedStruct(I32 val)
        : mMember{val}
    {
    }
    I32 mMember;
};

template <template <typename> class _allocator>
void CheckNoAllocations([[maybe_unused]] const HeapAllocationCounter& hac)
{
    EXCEPTION(true, "Default version should not be used.");
}

template <>
void CheckNoAllocations<GeneralPurposeAllocator>(const HeapAllocationCounter& hac)
{
#ifndef NO_GENERAL_PURPOSE_MEMORY
    BOOST_CHECK(hac.CheckNumCallsExpected(0, 0));
#else
    BOOST_CHECK(!hac.CheckNumCallsExpected(0, 0));
#endif
}

template <>
void CheckNoAllocations<PoolAllocator>(const HeapAllocationCounter& hac)
{
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL) && defined(NO_MEMORY_STACK) &&                     \
      defined(NO_THREAD_PRIVATE_MEMORY_STACK))
    BOOST_CHECK(hac.CheckNumCallsExpected(0, 0));
#else
    BOOST_CHECK(!hac.CheckNumCallsExpected(0, 0));
#endif
}


template <>
void CheckNoAllocations<StackAllocator>(const HeapAllocationCounter& hac)
{
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_STACK))
    BOOST_CHECK(hac.CheckNumCallsExpected(0, 0));
#else
    BOOST_CHECK(!hac.CheckNumCallsExpected(0, 0));
#endif
}

template <>
void CheckNoAllocations<ThreadPrivateStackAllocator>(const HeapAllocationCounter& hac)
{
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_THREAD_PRIVATE_MEMORY_STACK))
    BOOST_CHECK(hac.CheckNumCallsExpected(0, 0));
#else
    BOOST_CHECK(!hac.CheckNumCallsExpected(0, 0));
#endif
}



// Vector -------------------------------------------------------------------------------------------------------------

template <template <typename> class _allocator>
void VectorTest()
{
    HeapAllocationCounter hac;
    constexpr U32 numElements = 100;

    std::vector<I32, _allocator<I32>> v;

    for (U32 i = 0; i < numElements; ++i)
        v.push_back(static_cast<I32>(i) * 2);
    for (U32 i = 0; i < numElements; ++i)
        BOOST_CHECK(v[i] == static_cast<I32>(i) * 2);

    v.clear();
    v.shrink_to_fit();

    // Alignment test
    std::vector<AlignedStruct, _allocator<AlignedStruct>> v2;
    for (U32 i = 0; i < numElements; ++i)
        v2.push_back(static_cast<I32>(i));
    for (U32 i = 0; i < numElements; ++i)
        BOOST_CHECK(IsAligned(&v2[i], alignment));

    v2.clear();
    v2.shrink_to_fit();

    CheckNoAllocations<_allocator>(hac);
}


BOOST_AUTO_TEST_CASE(Vector)
{
    InitializeMemoryManager();

    BOOST_CHECK_NO_THROW(VectorTest<GeneralPurposeAllocator>());
    BOOST_CHECK_NO_THROW(VectorTest<StackAllocator>());
    BOOST_CHECK_NO_THROW(VectorTest<ThreadPrivateStackAllocator>());
    DeinitializeMemoryManager();
}



// Map ----------------------------------------------------------------------------------------------------------------

template <template <typename> class _allocator>
void MapTest()
{
    HeapAllocationCounter hac;
    constexpr U32 numElements = 100;

    std::map<U32, I32, std::less<U32>, _allocator<std::pair<const U32, I32>>> m;

    for (U32 i = 0; i < numElements; ++i)
        m.emplace(i, static_cast<I32>(i));

    for (U32 i = 0; i < numElements; ++i)
    {
        const auto& entry = m.find(i);
        BOOST_CHECK(entry != m.end());
        BOOST_CHECK(entry->second == static_cast<I32>(i));
    }

    m.clear();

    // Alignment
    std::map<U32, AlignedStruct, std::less<U32>, _allocator<std::pair<const U32, AlignedStruct>>> m2;
    for (U32 i = 0; i < numElements; ++i)
        m2.emplace(i, static_cast<I32>(i));
    for (const auto& it : m2)
        BOOST_CHECK(IsAligned(&it.second, alignment));

    m2.clear();

    CheckNoAllocations<_allocator>(hac);
}

BOOST_AUTO_TEST_CASE(Map)
{
    InitializeMemoryManager();

    BOOST_CHECK_NO_THROW(MapTest<GeneralPurposeAllocator>());
    BOOST_CHECK_NO_THROW(MapTest<PoolAllocator>());
    BOOST_CHECK_NO_THROW(MapTest<StackAllocator>());
    BOOST_CHECK_NO_THROW(MapTest<ThreadPrivateStackAllocator>());

    DeinitializeMemoryManager();
}



// Thread safety ------------------------------------------------------------------------------------------------------


//! @brief This test creates a specified number of threads. Each thread waits until the main threads gives the signal to
//! start. After the signal is given, each thread creates a set with the template types allocator and inserts a given
//! number of U64 elements. After insertion, the sum of all members is calculated and added to a global sum. The result
//! is compared to an expected value

template <template <typename> class _allocator>
void ThreadSafetyTest()
{
    DeadlockTerminationTimer dtm;

    constexpr U32 numThreads = 4;
    constexpr U32 numInsertionsPerThread = 100;

    std::atomic_bool startTest = false;
    std::atomic_bool closeThreads = false;
    std::atomic_bool exceptionInThreads = false;
    std::atomic<U32> threadsReady = 0;
    std::atomic<U32> threadsFinished = 0;
    std::atomic<U64> sumOfAllSetValues = 0;

    std::vector<std::thread> threads;



    // Create threads
    for (U32 i = 0; i < numThreads; ++i)
        threads.emplace_back([&]() {
            try
            {
#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
                GetMemoryManager().CreatePrivateMemoryStackForThisThread(1_MB);
#endif
                ++threadsReady;
                while (startTest == false)
                    std::this_thread::yield();

                std::set<U64, std::less<U64>, _allocator<U64>> s;
                for (U32 i = 1; i <= numInsertionsPerThread; ++i)
                    s.emplace(static_cast<U64>(i));

                U64 sum = 0;
                for (auto val : s)
                    sum += val;

                sumOfAllSetValues += sum;

                s.clear();
                ++threadsFinished;

                while (closeThreads == false)
                    std::this_thread::yield();

#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
                GetMemoryManager().DeletePrivateMemoryStackForThisThread();
#endif
            }
            catch (Exception&)
            {
                exceptionInThreads = true;
            }
        });


    while (threadsReady != numThreads)
        std::this_thread::yield();

    // Start the test
    startTest = true;
    HeapAllocationCounter hac;

    while (threadsFinished != numThreads)
        std::this_thread::yield();

    U64 expectedResult = (numInsertionsPerThread * numInsertionsPerThread + numInsertionsPerThread) / 2 * numThreads;
    BOOST_CHECK(sumOfAllSetValues == expectedResult);
    BOOST_CHECK(exceptionInThreads == false);
    CheckNoAllocations<_allocator>(hac);

    // Close all threads
    closeThreads = true;

    for (U32 i = 0; i < numThreads; ++i)
        threads[i].join();



    threads.clear();
    threads.shrink_to_fit();
}


BOOST_AUTO_TEST_CASE(Thread_Safety)
{
    InitializeMemoryManager();

    BOOST_CHECK_NO_THROW(ThreadSafetyTest<GeneralPurposeAllocator>());
    BOOST_CHECK_NO_THROW(ThreadSafetyTest<PoolAllocator>());
    BOOST_CHECK_NO_THROW(ThreadSafetyTest<StackAllocator>());
    BOOST_CHECK_NO_THROW(ThreadSafetyTest<ThreadPrivateStackAllocator>());

    DeinitializeMemoryManager();
}


// Pool Alloctor specific tests ---------------------------------------------------------------------------------------

#ifndef NO_MEMORY_POOL
BOOST_AUTO_TEST_CASE(Pool_throw_on_array_allocation)
{
    InitializeMemoryManager();

    std::vector<F32, PoolAllocator<F32>> v;
    GDL_CHECK_THROW_DEV(v.reserve(3), Exception);

    v.clear();
    v.shrink_to_fit();

    DeinitializeMemoryManager();
}
#endif

void TestDeleter(U32* p)
{
    GeneralPurposeAllocator<U32>().deallocate(p, 1);
}


BOOST_AUTO_TEST_CASE(Unique_ptr)
{
    InitializeMemoryManager();
    HeapAllocationCounter hac;
    {
        UniquePtr<U32> uptr = MakeUnique<U32>(123);
        BOOST_CHECK(*uptr == 123);
    }
#ifndef USE_STD_ALLOCATOR
    CheckNoAllocations<GeneralPurposeAllocator>(hac);
#else
    BOOST_CHECK(hac.CheckNumCallsExpected(1, 1));
#endif
    DeinitializeMemoryManager();
}
