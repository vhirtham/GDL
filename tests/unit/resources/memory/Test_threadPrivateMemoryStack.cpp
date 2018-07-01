#include <boost/test/unit_test.hpp>
#include "tests/tools/ExceptionChecks.h"

#include "gdl/base/Exception.h"
#include "gdl/resources/memory/threadPrivateMemoryStack.h"

#include <atomic>
#include <memory>
#include <thread>

using namespace GDL;

BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    BOOST_CHECK_NO_THROW(threadPrivateMemoryStack(100));

    BOOST_CHECK_THROW(threadPrivateMemoryStack(0), Exception);
    BOOST_CHECK_THROW(threadPrivateMemoryStack(100, 0), Exception);
}


BOOST_AUTO_TEST_CASE(Non_owning_thread_access)
{
    std::atomic_bool constructed = false;
    std::atomic_bool initialize = false;
    std::atomic_bool deinitialize = false;

    std::unique_ptr<threadPrivateMemoryStack> tpms{nullptr};

    std::thread thread{[&]() {
        tpms.reset(new threadPrivateMemoryStack{100});
        constructed = true;

        while (!initialize)
            std::this_thread::yield();
        (*tpms).Initialize();

        while (!deinitialize)
            std::this_thread::yield();

        (*tpms).Deinitialize();
    }};

    while (!constructed)
        std::this_thread::yield();

    threadPrivateMemoryStack& tpmsRef = *tpms;

    BOOST_CHECK_THROW(tpmsRef.Initialize(), Exception);
    initialize = true;

    BOOST_CHECK_THROW(tpmsRef.Deinitialize(), Exception);
    deinitialize = true;

    thread.join();
}

// ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//! ---- CONTINUE Test if exceptions are triggered correctly!
// ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
