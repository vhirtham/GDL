#include <boost/test/unit_test.hpp>


#include "gdl/base/fundamentalTypes.h"
#include "gdl/resources/cpu/thread.h"

#include <array>
#include <list>

using namespace GDL;


//! @brief Checks error free construction and destruction
BOOST_AUTO_TEST_CASE(Construction_Destruction)
{
    Thread thread([]() {
        U32 a = 0;
        for (U32 i = 0; i < 1000; ++i)
            a += i;
    });
}

//! @brief Checks if the threads run and process the passed function
BOOST_AUTO_TEST_CASE(Run)
{
    std::array<U32, 4> res{{0, 0, 0, 0}};
    std::list<Thread> threads;

    for (U32 i = 0; i < res.size(); ++i)
        threads.emplace_back([&res, i]() {
            for (U32 j = 0; j < 1000; ++j)
                res[i] += j;
        });

    // close all threads - main thread waits for them to join
    threads.clear();

    for (auto result : res)
        BOOST_CHECK(result == 499500);
}
