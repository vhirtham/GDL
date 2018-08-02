#include <boost/test/unit_test.hpp>

#include "memoryManagerSetup.h"


using namespace GDL;


BOOST_AUTO_TEST_CASE(Initialize_Deinitialize)
{
    MemoryManager& memoryManager = GetMemoryManager();

    memoryManager.Initialize();
    memoryManager.Deinitialize();
}
