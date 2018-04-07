#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/utility/globalNewCounter.h"
#include "gdl/resources/cpu/threadPool.h"
#include <iostream>

using namespace GDL;

BOOST_AUTO_TEST_CASE(Construction)
{
    ThreadPoolNEW tp(4);
}


BOOST_AUTO_TEST_CASE(Submit_tasks)
{
    ThreadPoolNEW tp(4);
    GlobalNewCounter gnc;
    for (U32 i = 0; i < 8; ++i)
        tp.submit([]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << "Hello" << std::endl;
        });
    tp.submit([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        tp.ParentThreadContinue();
    });
    gnc.PrintCalls();
    tp.ParentThreadWait();
    gnc.PrintCalls();
}
