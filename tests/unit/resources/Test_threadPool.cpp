#include <boost/test/unit_test.hpp>

#include "gdl/base/Exception.h"
#include "gdl/resources/cpu/threadPool.h"
#include <iostream>

using namespace GDL;


// Helper functions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void ParentOnlyTask(U32& counter, std::thread::id& parentThreadID, ThreadPoolNEW& threadPool)
{
    if (parentThreadID == std::this_thread::get_id())
        ++counter;
    else
        threadPool.Submit([&] { ParentOnlyTask(counter, parentThreadID, threadPool); });
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void MemberOnlyTask(std::atomic<U32>& counter, std::thread::id& parentThreadID, ThreadPoolNEW& threadPool)
{
    if (parentThreadID != std::this_thread::get_id())
        ++counter;
    else
        threadPool.Submit([&] { MemberOnlyTask(counter, parentThreadID, threadPool); });
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}


// Thread pool tests %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


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



BOOST_AUTO_TEST_CASE(External_thread_process_tasks)
{
    ThreadPoolNEW tp(4);

    std::thread::id parentThreadID = std::this_thread::get_id();

    U32 counterParent = 0; // only parent thread should modify it (no atomic type needed)
    std::atomic<U32> counterMember = 0;

    for (U32 i = 0; i < 10; ++i)
        tp.Submit([&] { ParentOnlyTask(counterParent, parentThreadID, tp); });
    for (U32 i = 0; i < 100; ++i)
        tp.Submit([&] { MemberOnlyTask(counterMember, parentThreadID, tp); });

    while (tp.HasTasks())
        tp.TryExecuteTask();

    tp.Deinitialize(); // <-- Avoid that member threads are still processing before checking the numbers

    BOOST_CHECK(counterParent == 10);
    BOOST_CHECK(counterMember == 100);
}

BOOST_AUTO_TEST_CASE(Exception_handling)
{
    ThreadPoolNEW tp(4);
    BOOST_CHECK_NO_THROW(tp.CheckExceptions());

    tp.Submit([&] { throw Exception(__PRETTY_FUNCTION__, "test"); });
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    BOOST_CHECK_THROW(tp.CheckExceptions(), Exception);

    tp.ClearExceptionLog();
    BOOST_CHECK_NO_THROW(tp.CheckExceptions());

    tp.Submit([&] { throw int{1}; });
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    BOOST_CHECK_THROW(tp.Deinitialize(), Exception);

    tp.ClearExceptionLog();
}
