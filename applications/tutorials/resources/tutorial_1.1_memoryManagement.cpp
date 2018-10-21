#include "gdl/base/uniquePtr.h"
#include "gdl/base/container/vector.h"
#include "gdl/resources/memory/memoryManager.h"


using namespace GDL;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    MemoryManager& memoryManager = MemoryManager::Instance();

    memoryManager.CreateGeneralPurposeMemory(1_MB);
    memoryManager.CreateMemoryStack(1_MB);
    memoryManager.CreateMemoryPool(32_B, 1000);
    memoryManager.CreateMemoryPool(64_B, 1000);

    memoryManager.Initialize();

    Vector<U32> myVec;
    myVec.push_back(1);
    myVec.clear();
    myVec.shrink_to_fit();

    VectorS<U32> myVecS;
    myVecS.push_back(1);
    myVecS.clear();
    myVecS.shrink_to_fit();

    UniquePtrP<U32> myPtr = MakeUniqueP<U32>(5);
    myPtr.reset(MakeUniqueP<U32>(77).release());
    // myPtr.reset(MakeUniqueS<U32>(2).release()); //<--- Compiles but will cause troubles. Only debug builds throw!
    myPtr.reset(nullptr);


    memoryManager.Deinitialize();

    return 0;
}
