#include <boost/test/unit_test.hpp>

#include "gdl/resources/cpu/threadPool.h"
#include <iostream>

using namespace GDL;

//! @brief Tests the construction and deinitialization
//! @remark 100 Threads are spawned to test for a possible deadlock during deconstruction. There are possible paths were
//! a thread has checked the condition and is going to sleep while the notify_all message is transmitted. This results
//! in a deadlock if the thread is yet not asleep and notify_all is only called once.
BOOST_AUTO_TEST_CASE(Construction_And_Deinitialization)
{
    ThreadPoolNEW tp(2);
}

BOOST_AUTO_TEST_CASE(Deadlock_submit_notification_missing)
{
    ThreadPoolNEW tp(2);

    // Possible deadlock if queue is empty and all threads are waiting. Deadlock occurs if notification is missing
    // during submit call
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    for (U32 i = 0; i < 8; ++i)
        tp.Submit([]() { std::this_thread::sleep_for(std::chrono::milliseconds(1)); });
}

BOOST_AUTO_TEST_CASE(Destruction_with_non_empty_queue)
{
    ThreadPoolNEW tp(4);

    for (U32 i = 0; i < 1000; ++i)
        tp.Submit([]() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
}

//! @brief The ThisThreadWaitFor function is tested by simply waiting for the thread pool to run out of tasks. Internal
//! resubmission of the condition check is also tested.
BOOST_AUTO_TEST_CASE(This_thread_wait_for)
{
    ThreadPoolNEW tp(4);

    U32 numFunctionCalls = 0;
    tp.SubmitThreadWaitCondition([&]() {
        ++numFunctionCalls;
        return !tp.HasTasks();
    });

    BOOST_CHECK(numFunctionCalls == 1); // No resubmission, hence one check!

    numFunctionCalls = 0;

    tp.SubmitThreadWaitCondition([&]() {
        ++numFunctionCalls;
        if (numFunctionCalls == 1)
            for (U32 i = 0; i < 100; ++i)
                tp.Submit([]() { std::this_thread::sleep_for(std::chrono::milliseconds(1)); });
        return !tp.HasTasks();
    });

    BOOST_CHECK(numFunctionCalls == 2); // Single resubmission
    BOOST_CHECK(tp.HasTasks() == false);

    numFunctionCalls = 0;


    tp.SubmitThreadWaitCondition([&]() {
        ++numFunctionCalls;
        if (numFunctionCalls < 5)
            for (U32 i = 0; i < 100; ++i)
                tp.Submit([]() { std::this_thread::sleep_for(std::chrono::milliseconds(1)); });
        return !tp.HasTasks();
    });

    BOOST_CHECK(numFunctionCalls == 5); // 4 resubmission (call of ThisThreadWaitFor() + refilling submits)
    BOOST_CHECK(tp.HasTasks() == false);
}
