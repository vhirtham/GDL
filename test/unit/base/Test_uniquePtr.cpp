#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/utility/heapAllocationCounter.h"
#include "gdl/base/uniquePtr.h"



using namespace GDL;

BOOST_AUTO_TEST_CASE(Make_Unique_Array)
{
    HeapAllocationCounter hac;
    {
        // GeneralPurposeAllocator<F32[]> gpa;
        // MakeUnique<F32[]>(3);
        // std::allocator<float[]> a;
    }
}
