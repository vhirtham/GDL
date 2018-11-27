#include <boost/test/unit_test.hpp>

#include "gdl/base/time.h"
#include "gdl/base/timer.h"

#include <thread>


#include <iostream>
using namespace GDL;
using namespace std::chrono_literals;

//! @brief Tests if the timer is working as expected (accuracy not checked in detail)
BOOST_AUTO_TEST_CASE(GetMilliseconds)
{
    Timer timer;
    std::this_thread::sleep_for(20ms);
    Milliseconds duration = timer.GetElapsedTime<Milliseconds>();
    BOOST_CHECK(duration >= 20ms);

    timer.Reset();
    std::this_thread::sleep_for(40ms);
    duration = timer.GetElapsedTime<Milliseconds>();
    BOOST_CHECK(duration >= 40ms);

    timer.Reset();
    std::this_thread::sleep_for(10ms);
    duration = timer.GetElapsedTime<Milliseconds>();
    BOOST_CHECK(duration >= 10ms);
}
