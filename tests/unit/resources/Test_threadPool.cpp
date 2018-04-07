#include <boost/test/unit_test.hpp>

#include "gdl/resources/cpu/threadPool.h"
#include <iostream>

using namespace GDL;

BOOST_AUTO_TEST_CASE(Construction)
{
    ThreadPoolNEW tp(10);
}

BOOST_AUTO_TEST_CASE(Deadlock_submit_notification_missing)
{
    ThreadPoolNEW tp(2);

    // Possible deadlock if queue is empty and all threads are waiting. Deadlock occurs if notification is missing
    // during submit call
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    for (U32 i = 0; i < 8; ++i)
        tp.submit([]() { std::this_thread::sleep_for(std::chrono::milliseconds(1)); });
}

BOOST_AUTO_TEST_CASE(Destruction_with_non_empty_queue)
{
    ThreadPoolNEW tp(4);

    for (U32 i = 0; i < 1000; ++i)
        tp.submit([]() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
}


BOOST_AUTO_TEST_CASE(Parent_thread_wait)
{
    ThreadPoolNEW tp(4);

    for (U32 i = 0; i < 100; ++i)
        tp.submit([]() { std::this_thread::sleep_for(std::chrono::milliseconds(1)); });

    tp.submit([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        tp.ParentThreadContinue();
    });
    tp.ParentThreadWait();
    BOOST_CHECK(tp.HasTasks() == false);
}
