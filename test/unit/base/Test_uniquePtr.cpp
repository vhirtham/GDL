#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/utility/heapAllocationCounter.h"
#include "gdl/base/uniquePtr.h"

using namespace GDL;

#include <iostream>
#include <memory>

template <typename _t>
class A
{
public:
    A()
    {
        std::cout << "Non array version" << std::endl;
    }
};

template <typename _t>
class A<_t[]>
{
public:
    A()
    {
        std::cout << "Array [] version" << std::endl;
    }
};

// INFO: https://stackoverflow.com/questions/16596950/why-is-make-uniquetn-disallowed

BOOST_AUTO_TEST_CASE(Make_Unique_Array)
{
    HeapAllocationCounter hac;
    {
        // GeneralPurposeAllocator<F32[]> gpa;
        // MakeUnique<F32[]>(3);
        // std::allocator<float[]> a;
    }
    A<F32[]> a;
    std::make_unique<F32>(3);
}
