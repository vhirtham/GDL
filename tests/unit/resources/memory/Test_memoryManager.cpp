#include <boost/test/unit_test.hpp>

#include "gdl/resources/memory/memoryManager.h"


using namespace GDL;


BOOST_AUTO_TEST_CASE(Initialize_Deinitialize)
{
    MemoryManager& mM = MemoryManager::Instance();
    mM.CreateGeneralPurposeMemory(1000);
    mM.Initialize();
    mM.Deinitialize();
}
