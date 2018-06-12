#include <boost/test/unit_test.hpp>

#include "gdl/base/timer.h"

#include <thread>


#include <iostream>
using namespace GDL;

//! @brief Tests if the timer is working as expected (accuracy not checked in detail)
BOOST_AUTO_TEST_CASE(GetMilliseconds)
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    auto duration = timer.GetMilliseconds();
    BOOST_CHECK(duration >= 20 && duration <= 25);

    timer.Reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    duration = timer.GetMilliseconds();
    BOOST_CHECK(duration >= 40 && duration <= 45);

    timer.Reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    duration = timer.GetMilliseconds();
    BOOST_CHECK(duration >= 10 && duration <= 15);
}
