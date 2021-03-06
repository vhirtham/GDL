#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/base/time.h"
#include "gdl/resources/cpu/threadPool.h"
#include "gdl/resources/cpu/utility/deadlockTerminationTimer.h"

#include <chrono>
#include <iostream>

using namespace GDL;
using namespace std::chrono_literals;


// Helper functions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void ParentOnlyTask(U32& counter, std::thread::id& parentThreadID, ThreadPool<1>& threadPool)
{
    if (parentThreadID == std::this_thread::get_id())
        ++counter;
    else
        threadPool.Submit([&] { ParentOnlyTask(counter, parentThreadID, threadPool); });
    std::this_thread::sleep_for(1ms);
}

void MemberOnlyTask(std::atomic<U32>& counter, std::thread::id& parentThreadID, ThreadPool<1>& threadPool)
{
    if (parentThreadID != std::this_thread::get_id())
        ++counter;
    else
        threadPool.Submit([&] { MemberOnlyTask(counter, parentThreadID, threadPool); });
    std::this_thread::sleep_for(1ms);
}


// Thread pool tests (single queue) %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//! @brief Tests the construction and deinitialization
//! @remark 100 Threads are spawned to test for a possible deadlock during deconstruction. There are possible paths were
//! a thread has checked the condition and is going to sleep while the notify_all message is transmitted. This results
//! in a deadlock if the thread is yet not asleep and notify_all is only called once.
BOOST_AUTO_TEST_CASE(Construction_And_Deinitialization)
{
    DeadlockTerminationTimer dtt;
    ThreadPool tp(100);
}


//! @brief Tests to check if threads are notified correctly when an empty queue is filled and all threads are sleeping.
BOOST_AUTO_TEST_CASE(Deadlock_submit_notification_missing)
{
    DeadlockTerminationTimer dtt;
    ThreadPool tp(2);

    // Possible deadlock if queue is empty and all threads are waiting. Deadlock occurs if notification is missing
    // during submit call
    std::this_thread::sleep_for(1ms);

    for (U32 i = 0; i < 8; ++i)
        tp.Submit([]() { std::this_thread::sleep_for(1ms); });

    while (tp.HasTasks())
        std::this_thread::sleep_for(1ms);
}


//! @brief Checks if the threadpool can be destroyed while it still contains tasks
BOOST_AUTO_TEST_CASE(Destruction_with_non_empty_queue)
{
    DeadlockTerminationTimer dtt;
    ThreadPool tp(4);
    for (U32 i = 0; i < 1000; ++i)
        tp.Submit([]() { std::this_thread::sleep_for(100ms); });
}


//! @brief Tests task processing by threads that are not managed by the threadpool
BOOST_AUTO_TEST_CASE(External_thread_process_tasks)
{
    DeadlockTerminationTimer dtt;
    ThreadPool tp(4);

    std::thread::id parentThreadID = std::this_thread::get_id();

    U32 counterParent = 0; // only parent thread should modify it (no atomic type needed)
    std::atomic<U32> counterMember = 0;

    for (U32 i = 0; i < 10; ++i)
        tp.Submit([&] { ParentOnlyTask(counterParent, parentThreadID, tp); });
    for (U32 i = 0; i < 100; ++i)
        tp.Submit([&] { MemberOnlyTask(counterMember, parentThreadID, tp); });


    while (tp.HasTasks() || counterParent < 10 || counterMember < 100)
        tp.TryExecuteTask();

    BOOST_CHECK(counterParent == 10);
    BOOST_CHECK(counterMember == 100);
}


//! @brief Checks the exception handling mechanism.
BOOST_AUTO_TEST_CASE(Exception_handling)
{
    DeadlockTerminationTimer dtt;
    ThreadPool tp(4);
    BOOST_CHECK_NO_THROW(tp.PropagateExceptions());

    tp.Submit([&] { throw Exception(__PRETTY_FUNCTION__, "test"); });
    while (tp.GetExceptionLogSize() == 0)
        std::this_thread::sleep_for(1ms);

    BOOST_CHECK_THROW(tp.PropagateExceptions(), Exception);

    tp.ClearExceptionLog();
    BOOST_CHECK_NO_THROW(tp.PropagateExceptions());

    tp.Submit([&] { throw int{1}; });
    std::this_thread::sleep_for(10ms);

    BOOST_CHECK_THROW(tp.Deinitialize(), Exception);

    tp.ClearExceptionLog();
}

//! @brief Tests if threads are started and closed correctly.
//! @remark This test didn't catch a bug were the CloseThreads function caused all threads to leave its main loop
//! instead of just the specified number of threads. This is still the case, but the LIFO test will fail/deadlock
//! in this case.
BOOST_AUTO_TEST_CASE(Start_and_Close_Threads) // Deadlock timer triggered on travis
{
    DeadlockTerminationTimer dtt;

    // with empty queue
    ThreadPool tp(2);
    BOOST_CHECK(tp.GetNumThreads() == 2);
    tp.StartThreads(3);
    BOOST_CHECK(tp.GetNumThreads() == 5);
    tp.CloseThreads(2);
    BOOST_CHECK(tp.GetNumThreads() == 3);
    tp.CloseAllThreads();
    BOOST_CHECK(tp.GetNumThreads() == 0);

    for (U32 i = 0; i < 1000; ++i)
        tp.Submit([]() { std::this_thread::sleep_for(1ms); });

    tp.StartThreads(1);
    tp.StartThreads(2);
    tp.StartThreads(3);
    std::this_thread::sleep_for(1ms);
    BOOST_CHECK(tp.GetNumThreads() == 6);
    BOOST_CHECK(tp.GetNumTasks() > 0);

    tp.CloseThreads(3);
    BOOST_CHECK(tp.GetNumThreads() == 3);
    BOOST_CHECK(tp.GetNumTasks() > 0);

    tp.CloseThreads(10);
    BOOST_CHECK(tp.GetNumThreads() == 0);
    BOOST_CHECK(tp.GetNumTasks() > 0);


    tp.StartThreads(3);
    BOOST_CHECK(tp.GetNumThreads() == 3);
    BOOST_CHECK(tp.GetNumTasks() > 0);

    tp.CloseAllThreads();
    BOOST_CHECK(tp.GetNumThreads() == 0);
    BOOST_CHECK(tp.GetNumTasks() > 0);
}


//! @brief Tests if threads with custom main loop work as expected
BOOST_AUTO_TEST_CASE(Start_Thread_With_Custom_Main_Loop)
{
    DeadlockTerminationTimer dtt;
    ThreadPool tp(0);

    for (U32 i = 0; i < 100; ++i)
        tp.Submit([]() { std::this_thread::sleep_for(1ms); });

    // Reproduce default behaviour %%%%%%%%%%%%%%
    tp.StartThreads(3, [&] { tp.TryExecuteTask(); });

    while (tp.HasTasks())
        std::this_thread::sleep_for(1ms);

    tp.CloseAllThreads();
    BOOST_CHECK(tp.GetNumThreads() == 0);
    BOOST_CHECK(tp.GetNumTasks() == 0);



    // Submit different main loops %%%%%%%%%%%%%%

    constexpr U32 numSubmits = 100;
    U32 ML_Counter_0 = 0;
    U32 ML_Counter_1 = 0;
    U32 ML_Counter_2 = 0;

    auto ML_function = [&tp](U32& counter) {
        tp.TryExecuteTask();
        ++counter;
    };

    tp.StartThreads(1, [&]() { ML_function(ML_Counter_0); });
    tp.StartThreads(1, [&]() { ML_function(ML_Counter_1); });
    tp.StartThreads(1, [&]() { ML_function(ML_Counter_2); });


    for (U32 i = 0; i < numSubmits; ++i)
        tp.Submit([]() { std::this_thread::sleep_for(1ms); });

    while (tp.HasTasks() || ML_Counter_0 + ML_Counter_1 + ML_Counter_2 < numSubmits)
        std::this_thread::sleep_for(1ms);


    BOOST_CHECK(ML_Counter_0 > 0);
    BOOST_CHECK(ML_Counter_1 > 0);
    BOOST_CHECK(ML_Counter_2 > 0);
    BOOST_CHECK(ML_Counter_0 + ML_Counter_1 + ML_Counter_2 >= numSubmits);
}


//! @brief Tests if the Initialization and Deinitialization functions are called
BOOST_AUTO_TEST_CASE(Initialization_and_Deinitialization_functions)
{
    DeadlockTerminationTimer dtt;
    ThreadPool tp(0);

    std::atomic_bool init = false;
    std::atomic_bool deinit = false;

    tp.StartThreads(1, [&]() { tp.TryExecuteTask(); }, [&]() { init = true; }, [&]() { deinit = true; });

    tp.Submit([]() {});
    tp.Submit([]() {});
    while (tp.HasTasks())
        std::this_thread::yield();
    BOOST_CHECK(init == true);
    BOOST_CHECK(deinit == false);

    tp.CloseAllThreads();
    BOOST_CHECK(init == true);
    BOOST_CHECK(deinit == true);
}

//! @brief Tests if the Initialization and Deinitialization functions are called
BOOST_AUTO_TEST_CASE(Exceptions_in_Initialization_and_Deinitialization_functions)
{
    DeadlockTerminationTimer dtt;
    ThreadPool tp(0);

    std::atomic_bool init = false;
    std::atomic_bool mainLoopRun = false;
    std::atomic_bool deinit = false;

    tp.StartThreads(1, [&]() { tp.TryExecuteTask(); },
                    [&]() { throw Exception(__PRETTY_FUNCTION__, "You shall not initialize!"); },
                    [&]() { deinit = true; });

    tp.Submit([&]() { mainLoopRun = true; });
    tp.Submit([]() {});
    while (deinit == false)
        std::this_thread::yield();
    BOOST_CHECK(init == false);
    BOOST_CHECK(mainLoopRun == false);
    BOOST_CHECK(deinit == true);
    BOOST_CHECK(tp.GetNumTasks() == 2);
    BOOST_CHECK_THROW(tp.PropagateExceptions(), Exception);
    tp.ClearExceptionLog();
    tp.CloseAllThreads();

    deinit = false;
    tp.StartThreads(1, [&]() { tp.TryExecuteTask(); }, [&]() { init = true; },
                    [&]() { throw Exception(__PRETTY_FUNCTION__, "You shall not deinitialize!"); });

    while (tp.HasTasks())
        std::this_thread::yield();
    tp.CloseAllThreads();
    BOOST_CHECK_THROW(tp.PropagateExceptions(), Exception);
    tp.ClearExceptionLog();
    BOOST_CHECK(init == true);
    BOOST_CHECK(mainLoopRun == true);
    BOOST_CHECK(deinit == false);
    BOOST_CHECK(tp.GetNumTasks() == 0);
    //    BOOST_CHECK(init == true);
    //    BOOST_CHECK(deinit == true);
}

// Thread pool tests (multiple queue) %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//! @brief Tests the construction and deinitialization
BOOST_AUTO_TEST_CASE(MultiQueue_Construction_And_Deinitialization)
{
    DeadlockTerminationTimer dtt;
    constexpr I32 numQueues = 4;

    ThreadPool<numQueues> tp;
    tp.StartThreads(100, [&]() {
        for (I32 i = 0; i < numQueues; ++i)
            if (tp.TryExecuteTask(i))
                continue;
    });
}



//! @brief Tests to check if threads are notified correctly when an empty queue is filled. It also checks if all tasks
//! are processed.
BOOST_AUTO_TEST_CASE(MultiQueue_Submit_And_Process)
{
    DeadlockTerminationTimer dtt;
    constexpr I32 numQueues = 3;
    ThreadPool<numQueues> tp;

    tp.StartThreads(2, [&]() {
        for (I32 i = 0; i < numQueues; ++i)
            if (tp.TryExecuteTask(i))
                continue;
    });

    std::this_thread::sleep_for(1ms);

    for (U32 i = 0; i < numQueues; ++i)
    {
        for (U32 j = 0; j < 8; ++j)
            tp.Submit(static_cast<I32>(i), []() { std::this_thread::sleep_for(1ms); });

        while (tp.HasTasks(0) || tp.HasTasks(1) || tp.HasTasks(2))
            std::this_thread::sleep_for(1ms);
    }
}



//! @brief Tests if tasks are put into the right queue and processed as expected
BOOST_AUTO_TEST_CASE(MultiQueue_Submit_And_Process_Specific_Queues)
{
    DeadlockTerminationTimer dtt;
    constexpr U32 numQueues = 3;
    ThreadPool<numQueues> tp;

    tp.Submit(2, []() { std::this_thread::sleep_for(1ms); });
    for (U32 i = 0; i < 2; ++i)
        tp.Submit(0, []() { std::this_thread::sleep_for(1ms); });
    for (U32 i = 0; i < 3; ++i)
        tp.Submit(1, []() { std::this_thread::sleep_for(1ms); });

    BOOST_CHECK(tp.GetNumTasks(0) == 2);
    BOOST_CHECK(tp.GetNumTasks(1) == 3);
    BOOST_CHECK(tp.GetNumTasks(2) == 1);

    tp.StartThreads(1, [&tp]() { tp.TryExecuteTask(1); });
    while (tp.HasTasks(1))
        std::this_thread::sleep_for(1ms);
    BOOST_CHECK(tp.GetNumTasks(0) == 2);
    BOOST_CHECK(tp.GetNumTasks(1) == 0);
    BOOST_CHECK(tp.GetNumTasks(2) == 1);
    tp.CloseAllThreads();

    tp.StartThreads(1, [&tp]() { tp.TryExecuteTask(0); });
    while (tp.HasTasks(0))
        std::this_thread::sleep_for(1ms);
    BOOST_CHECK(tp.GetNumTasks(0) == 0);
    BOOST_CHECK(tp.GetNumTasks(1) == 0);
    BOOST_CHECK(tp.GetNumTasks(2) == 1);
    tp.CloseAllThreads();


    tp.StartThreads(1, [&tp]() { tp.TryExecuteTask(2); });
    while (tp.HasTasks(2))
        std::this_thread::sleep_for(1ms);

    BOOST_CHECK(tp.GetNumTasks(0) == 0);
    BOOST_CHECK(tp.GetNumTasks(1) == 0);
    BOOST_CHECK(tp.GetNumTasks(2) == 0);
}


//! @brief Tests if the last created  threads are destroyed first
//! @remark This could also be tested with a single queue, but it is simpler with multiple queues
BOOST_AUTO_TEST_CASE(MultiQueue_LIFO_ThreadDestruction)
{
    DeadlockTerminationTimer dtt;
    constexpr U32 numQueues = 4;
    ThreadPool<numQueues> tp;

    for (U32 i = 0; i < numQueues; ++i)
        tp.StartThreads(1, [&tp, i]() { tp.TryExecuteTask(static_cast<I32>(i)); });

    for (U32 i = 0; i < numQueues; ++i)
        tp.Submit(static_cast<I32>(i), []() { std::this_thread::sleep_for(1ms); });

    while (tp.GetNumTasks(0) > 0 || tp.GetNumTasks(1) > 0 || tp.GetNumTasks(2) > 0 || tp.GetNumTasks(3) > 0)
        std::this_thread::sleep_for(1ms);

    for (U32 i = 0; i < numQueues; ++i)
        BOOST_CHECK(tp.GetNumTasks(static_cast<I32>(i)) == 0);



    tp.CloseThreads(1);
    for (U32 i = 0; i < numQueues; ++i)
        tp.Submit(static_cast<I32>(i), []() { std::this_thread::sleep_for(1ms); });

    while (tp.GetNumTasks(0) > 0 || tp.GetNumTasks(1) > 0 || tp.GetNumTasks(2) > 0)
        std::this_thread::sleep_for(1ms);

    for (U32 i = 0; i < numQueues - 1; ++i)
        BOOST_CHECK(tp.GetNumTasks(static_cast<I32>(i)) == 0);

    BOOST_CHECK(tp.GetNumTasks(numQueues - 1) == 1);



    tp.CloseThreads(2);
    for (U32 i = 0; i < numQueues; ++i)
        tp.Submit(static_cast<I32>(i), []() { std::this_thread::sleep_for(1ms); });

    while (tp.GetNumTasks(0) > 0)
        std::this_thread::sleep_for(1ms);


    BOOST_CHECK(tp.GetNumTasks(0) == 0);
    BOOST_CHECK(tp.GetNumTasks(1) == 1);
    BOOST_CHECK(tp.GetNumTasks(2) == 1);
    BOOST_CHECK(tp.GetNumTasks(3) == 2);
}
