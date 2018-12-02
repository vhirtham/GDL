#include "gdl/base/uniquePtr.h"
#include "gdl/base/container/vector.h"
#include "gdl/resources/memory/memoryManager.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/utility/heapAllocationCounter.h"


using namespace GDL;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    MemoryManager& memoryManager = MemoryManager::Instance();

    memoryManager.CreateGeneralPurposeMemory(1_MB);
    memoryManager.CreateMemoryStack(1_MB);
    memoryManager.CreateMemoryPool(32_B, 1000);
    memoryManager.CreateMemoryPool(64_B, 1000);
    memoryManager.CreateMemoryPool(128_B, 1000, 32); // Memory pool with 32 byte aligned blocks
    memoryManager.EnableThreadPrivateMemory();

    memoryManager.Initialize();

    memoryManager.CreatePrivateMemoryStackForThisThread(1_MB);

    HeapAllocationCounter hac;

    // Container --------------------------------

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



    // Manual allocaion wit allocators ----------

    StackAllocator<F32> stackAllocator;

    F32* myFloatArray = stackAllocator.allocate(5);
    stackAllocator.deallocate(myFloatArray, 5);


    // Manual allocaion witout allocators -------

    ThreadPrivateMemoryStack* tpms = memoryManager.GetThreadPrivateMemoryStack();
    if (tpms == nullptr)
        return -1;

    I32* myIntArray = static_cast<I32*>(tpms->Allocate(sizeof(I32) * 5));
    tpms->Deallocate(myIntArray);

    // aligned allocation
    myIntArray = static_cast<I32*>(tpms->Allocate(sizeof(I32) * 5, 32));
    tpms->Deallocate(myIntArray);

    hac.PrintCalls();

    memoryManager.DeletePrivateMemoryStackForThisThread();
    memoryManager.Deinitialize();

    return 0;
}
