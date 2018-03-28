//#include <boost/test/unit_test.hpp>
#include "gdl/resources/memory/PoolAllocator.h"
#include <vector>

#include <new>

class GlobalNewAllocationCounter
{
    friend void* operator new(std::size_t size);
    friend void* operator new(std::size_t size, const std::nothrow_t&) noexcept;
    friend void* operator new[](std::size_t size);
    friend void* operator new[](std::size_t size, const std::nothrow_t&) noexcept;
    friend void operator delete(void* ptr) noexcept;
    friend void operator delete(void* ptr, const std::nothrow_t&)noexcept;
    friend void operator delete[](void* ptr) noexcept;
    friend void operator delete[](void* ptr, const std::nothrow_t&) noexcept;

    int mAllocationsOnConstruction = -1;
    int mDeallocationsOnConstruction = -1;

    static int mTotalAllocations;
    static int mTotalDeallocations;

public:
    GlobalNewAllocationCounter()
        : mAllocationsOnConstruction(mTotalAllocations)
        , mDeallocationsOnConstruction(mTotalDeallocations)
    {
    }

    int GetNumAllocations() const
    {
        return mTotalAllocations - mAllocationsOnConstruction;
    }
    int GetNumDeallocations() const
    {
        return mTotalDeallocations - mDeallocationsOnConstruction;
    }

    static int GetTotalNumAllocations()
    {
        return mTotalAllocations;
    }
    static int GetTotalNumDeallocations()
    {
        return mTotalDeallocations;
    }
};

int GlobalNewAllocationCounter::mTotalAllocations = 0;
int GlobalNewAllocationCounter::mTotalDeallocations = 0;

void* operator new(std::size_t size)
{
    ++GlobalNewAllocationCounter::mTotalAllocations;
    void* p = malloc(size);
    if (!p)
        throw std::bad_alloc();
    return p;
}

void* operator new[](std::size_t size)
{
    ++GlobalNewAllocationCounter::mTotalAllocations;
    void* p = malloc(size);
    if (!p)
        throw std::bad_alloc();
    return p;
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    ++GlobalNewAllocationCounter::mTotalAllocations;
    return malloc(size);
}
void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    ++GlobalNewAllocationCounter::mTotalAllocations;
    return malloc(size);
}


void operator delete(void* ptr) noexcept
{
    ++GlobalNewAllocationCounter::mTotalDeallocations;
    free(ptr);
}
void operator delete(void* ptr, const std::nothrow_t&)noexcept
{
    ++GlobalNewAllocationCounter::mTotalDeallocations;
    free(ptr);
}
void operator delete[](void* ptr) noexcept
{
    ++GlobalNewAllocationCounter::mTotalDeallocations;
    free(ptr);
}
void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
    ++GlobalNewAllocationCounter::mTotalDeallocations;
    free(ptr);
}



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



using namespace GDL;
// BOOST_AUTO_TEST_CASE(PoolAllocator)
int main()
{
    std::cout << "Num GLOBAL allocations CP START: " << GlobalNewAllocationCounter::GetTotalNumAllocations()
              << std::endl;
    std::cout << "Num GLOBAL deallocations CP START: " << GlobalNewAllocationCounter::GetTotalNumDeallocations();
    //    int allocs = number_of_allocs;
    MemoryManager::Instance(1024);
    std::cout << "Num GLOBAL allocations CP START: " << GlobalNewAllocationCounter::GetTotalNumAllocations()
              << std::endl;
    std::cout << "Num GLOBAL deallocations CP START: " << GlobalNewAllocationCounter::GetTotalNumDeallocations();
    GlobalNewAllocationCounter ac;
    std::cout << "Num allocations start: " << ac.GetNumAllocations() << std::endl;
    {
        std::cout << "Num allocations CP 1: " << ac.GetNumAllocations() << std::endl;

        std::vector<test, GDL::custom_allocator<test>> v;
        std::vector<test2<GDL::custom_allocator>, GDL::custom_allocator<test2<GDL::custom_allocator>>> v3;
        std::vector<F32, GDL::custom_allocator<F32>> v2;

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

        test2<std::allocator> test;
        std::cout << "Num allocations CP 2: " << ac.GetNumAllocations() << std::endl;
        test.mVec.push_back(42);
        std::cout << "Num allocations CP 3: " << ac.GetNumAllocations() << std::endl;
        int* a1 = new int(10);
        std::cout << "Num allocations CP new a1: " << ac.GetNumAllocations() << std::endl;
        std::cout << "Num deallocations CP new a1: " << ac.GetNumDeallocations() << std::endl;
        delete a1;
        std::cout << "Num allocations CP delete a1: " << ac.GetNumAllocations() << std::endl;
        std::cout << "Num deallocations CP delete a1: " << ac.GetNumDeallocations() << std::endl;

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
    std::cout << "Num allocations CP END: " << ac.GetNumAllocations() << std::endl;
    std::cout << "Num deallocations CP END: " << ac.GetNumDeallocations() << std::endl;
    std::cout << "Num GLOBAL allocations CP END: " << GlobalNewAllocationCounter::GetTotalNumAllocations() << std::endl;
    std::cout << "Num GLOBAL deallocations CP END: " << GlobalNewAllocationCounter::GetTotalNumDeallocations()
              << std::endl;
}
