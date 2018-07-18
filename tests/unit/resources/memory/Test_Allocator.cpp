//#include <boost/test/unit_test.hpp>
#include "gdl/resources/memory/utility/globalNewCounter.h"
#include "gdl/resources/memory/PoolAllocator.h"
#include <vector>
#include <map>
#include <new>



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

template <template <typename> class Allocator = std::allocator>
struct test2
{
    std::vector<int, Allocator<int>> mVec;
};

template <typename TAllocator>
void someFunc(const std::vector<GDL::F32, TAllocator>& A)
{
    std::cout << "can do it" << std::endl;
}

using namespace GDL;
// BOOST_AUTO_TEST_CASE(PoolAllocator)
int main()
{
    std::cout << "Num GLOBAL allocations CP START: " << GlobalNewCounter::GetTotalNumNewCalls() << std::endl;
    std::cout << "Num GLOBAL deallocations CP START: " << GlobalNewCounter::GetTotalNumDeleteCalls();
    //    int allocs = number_of_allocs;
    MemoryManager::Instance(1024);
    std::cout << "Num GLOBAL allocations CP START: " << GlobalNewCounter::GetTotalNumNewCalls() << std::endl;
    std::cout << "Num GLOBAL deallocations CP START: " << GlobalNewCounter::GetTotalNumDeleteCalls();
    GlobalNewCounter ac;
    std::cout << "Num allocations start: " << ac.GetNumNewCalls() << std::endl;
    {
        std::cout << "Num allocations CP 1: " << ac.GetNumNewCalls() << std::endl;

        std::vector<test, GDL::custom_allocator<test>> v;
        std::vector<test2<GDL::custom_allocator>, GDL::custom_allocator<test2<GDL::custom_allocator>>> v3;
        std::vector<F32, GDL::custom_allocator<F32>> v2;

        std::map<I32, I32, std::less<I32>, MyAlloc<std::pair<I32, I32>>> m;

        m.insert(std::pair<I32, I32>(1, 1));
        // m.emplace(1,1);
        v.push_back(42);
        v2.push_back(3.14);
        v.push_back(56);
        v.push_back(11);
        v.push_back(22);
        v.reserve(20);
        v3.emplace_back();
        v3.back().mVec.push_back(1);
        v3.back().mVec.push_back(2);
        v3.emplace_back(v3.back());

        someFunc(v2);

        test2<std::allocator> test;
        std::cout << "Num allocations CP 2: " << ac.GetNumNewCalls() << std::endl;
        test.mVec.push_back(42);
        std::cout << "Num allocations CP 3: " << ac.GetNumNewCalls() << std::endl;
        int* a1 = new int(10);
        std::cout << "Num allocations CP new a1: " << ac.GetNumNewCalls() << std::endl;
        std::cout << "Num deallocations CP new a1: " << ac.GetNumDeleteCalls() << std::endl;
        delete a1;
        std::cout << "Num allocations CP delete a1: " << ac.GetNumNewCalls() << std::endl;
        std::cout << "Num deallocations CP delete a1: " << ac.GetNumDeleteCalls() << std::endl;

        //        bool caught = false;
        //        try
        //        {
        //            v.reserve(200);
        //        }
        //        catch (Exception e)
        //        {
        //            e.what();
        //            caught = true;
        //        }

        //        if (caught == false)
        //            throw Exception(__PRETTY_FUNCTION__, "Exception not caught as expected");
        //        std::cout << "End of program" << std::endl;
        //        v.push_back(33);
        //        v.push_back(44);
    }
    std::cout << "Num allocations CP END: " << ac.GetNumNewCalls() << std::endl;
    std::cout << "Num deallocations CP END: " << ac.GetNumDeleteCalls() << std::endl;
    std::cout << "Num GLOBAL allocations CP END: " << GlobalNewCounter::GetTotalNumNewCalls() << std::endl;
    std::cout << "Num GLOBAL deallocations CP END: " << GlobalNewCounter::GetTotalNumDeleteCalls() << std::endl;
}
