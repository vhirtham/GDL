#include <boost/test/unit_test.hpp>

#include "gdl/GDLTypedefs.h"
#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/memoryManager.h"


#include <iostream>
#include <vector>

using namespace GDL;



BOOST_AUTO_TEST_CASE(Vector)
{
    auto& mM = MemoryManager::Instance();
    mM.CreateGeneralPurposeMemory(10 * sizeof(F32));
    mM.Initialize();

    std::vector<F32, GeneralPurposeAllocator<F32>> test;
    test.resize(7);
    for (U32 i = 0; i < test.size(); ++i)
        test[i] = i * 2;
    for (U32 i = 0; i < test.size(); ++i)
        std::cout << test[i] << std::endl;
    test.clear();
    test.shrink_to_fit();

    mM.Deinitialize();
}
