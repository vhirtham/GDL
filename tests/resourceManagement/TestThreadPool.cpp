#define BOOST_TEST_MODULE ThreadPool test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>


#include "src/resourceManagement/ThreadPool.h"


using namespace GDL;


BOOST_AUTO_TEST_CASE(General)
{
    ThreadPool TP(2);
    TP.submit([]() { std::cout << "Hello World"; });
    TP.submit([]() { std::cout << "Hello World"; });
    TP.submit([]() { std::cout << "Hello World"; });
    TP.submit([]() { std::cout << "Hello World"; });
    TP.submit([]() { std::cout << "Hello World"; });
    TP.submit([]() { std::cout << "Hello World"; });
    TP.submit([](int a, int b) { std::cout << a << "+" << b << "=" << a + b; }, 1, 2);
}
