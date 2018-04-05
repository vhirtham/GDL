#include <boost/test/unit_test.hpp>

#include "gdl/resources/cpu/threadPool.h"
#include <iostream>

using namespace GDL;

BOOST_AUTO_TEST_CASE(Construction)
{
    ThreadPoolNEW tp(30);
}


BOOST_AUTO_TEST_CASE(Submit_tasks)
{
    ThreadPoolNEW tp(4);
    tp.submit([]() { std::cout << "Hello" << std::endl; });
    tp.submit([]() { std::cout << "Good Bye" << std::endl; });
}
