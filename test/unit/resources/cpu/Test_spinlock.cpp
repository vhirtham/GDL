#include <boost/test/unit_test.hpp>


#include "gdl/base/fundamentalTypes.h"
#include "gdl/resources/cpu/spinlock.h"

#include <array>
#include <atomic>
#include <mutex>
#include <thread>

using namespace GDL;



//! @brief Checks error free construction and destruction
BOOST_AUTO_TEST_CASE(thread_safety)
{
    constexpr U32 numThreads = 10;
    constexpr U32 numIterations = 10000;

    std::array<std::thread, numThreads> threads;

    U32 value = 0;
    SpinLock sp;

    for (U32 i = 0; i < numThreads; ++i)
        threads[i] = std::thread([&value, &sp]() {

            for (U32 j = 0; j < numIterations; ++j)
            {
                std::lock_guard<SpinLock> lock(sp);
                value++;
            }
        });

    for (U32 i = 0; i < numThreads; ++i)
        threads[i].join();

    BOOST_CHECK(value == numThreads * numIterations);
}
