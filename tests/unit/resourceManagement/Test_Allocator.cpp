//#include <boost/test/unit_test.hpp>
#include "resourceManagement/memory/PoolAllocator.h"
#include <vector>

struct test
{
    int mA;
    double mB;
    test(int a)
        : mA(a)
        , mB(static_cast<double>(a) / 3.)
    {
    }
};

using namespace GDL;
// BOOST_AUTO_TEST_CASE(PoolAllocator)
int main()
{
    MemoryManager::Instance(1024);

    std::vector<test, GDL::custom_allocator<test>> v;
    std::vector<F32, GDL::custom_allocator<F32>> v2;

    v.push_back(42);
    v2.push_back(3.14);
    v.push_back(56);
    v.push_back(11);
    v.push_back(22);
    v.reserve(20);
    bool caught = false;
    try
    {
        v.reserve(200);
    }
    catch (Exception e)
    {
        e.what();
        caught = true;
    }

    if (caught == false)
        throw Exception(__PRETTY_FUNCTION__, "Exception not caught as expected");
    std::cout << "End of program" << std::endl;
    //    v.push_back(33);
    //    v.push_back(44);
}
