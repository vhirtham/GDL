#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/memoryPool.h"



using namespace GDL;


BOOST_AUTO_TEST_CASE(Construction_destruction)
{
    MemoryPool mp(16, 3);
}


BOOST_AUTO_TEST_CASE(Allocation_and_Deallocation)
{
    MemoryPool mp(16, 5);
    void* elem1 = mp.Allocate(8);
    void* elem2 = mp.Allocate(8);
    void* elem3 = mp.Allocate(8);
    void* elem4 = mp.Allocate(8);
    void* elem5 = mp.Allocate(8);
    mp.CheckConsistency();
    mp.Deallocate(elem5);
    mp.CheckConsistency();
    mp.Allocate(8);
    mp.CheckConsistency();
}

BOOST_AUTO_TEST_CASE(Thread_Safety)
{
}
