#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/utility/heapAllocationCounter.h"
#include "gdl/base/string.h"

#include <iostream>
#include <cstdio>

using namespace GDL;


BOOST_AUTO_TEST_CASE(To_String)
{

    MemoryManager& mm = MemoryManager::Instance();
    mm.CreateGeneralPurposeMemory(1_MB);
    mm.Initialize();
    HeapAllocationCounter hac;
    {
        std::cout << ToString(3.15678f, "%7.2f") << ";" << std::endl;
        std::cout << ToString(-10030500000000001.) << ";" << std::endl;
        std::cout << ToString(0.0000030500000000001) << ";" << std::endl;
        std::cout << ToString(10030500000000000.f, "%7.5e") << ";" << std::endl;
        String test("A number = " + ToString(3.156e6f));
        std::cout << test << ";" << std::endl;
        std::to_string(2);
    }
    hac.PrintCalls();
    mm.Deinitialize();
}
