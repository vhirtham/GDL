#include "gdl/resources/memory/memoryManager.h"
#include "gdl/resources/memory/memorySize.h"

using namespace GDL;


//! @brief Adds memory systems to the memory manager
//! @return Memory manager
MemoryManager& SetupMemoryManager()
{
    MemoryManager& memoryManager = MemoryManager::Instance();
#ifndef NO_GENERAL_PURPOSE_MEMORY
    memoryManager.CreateGeneralPurposeMemory(1_MiB);
#endif
#ifndef NO_MEMORY_POOL
    memoryManager.CreateMemoryPool(32_B, 1000);
    memoryManager.CreateMemoryPool(64_B, 1000);
    memoryManager.CreateMemoryPool(128_B, 1000);
#endif
#ifndef NO_MEMORY_STACK
    memoryManager.CreateMemoryStack(1_MiB);
#endif
#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
    memoryManager.EnableThreadPrivateMemory();
#endif
    return memoryManager;
}


//! @brief Helper function which gets the memory menager and ensures it is only set up once
//! @return Memory manager
MemoryManager& GetMemoryManager()
{
    static MemoryManager& memoryManager = SetupMemoryManager();
    return memoryManager;
}


void InitializeMemoryManager()
{
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL) && defined(NO_MEMORY_STACK) &&                     \
      defined(NO_THREAD_PRIVATE_MEMORY_STACK))

    GetMemoryManager().Initialize();

#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
    GetMemoryManager().CreatePrivateMemoryStackForThisThread(1_MB);
#endif

#endif
}

void DeinitializeMemoryManager()
{
#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
    GetMemoryManager().DeletePrivateMemoryStackForThisThread();
#endif


#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL) && defined(NO_MEMORY_STACK) &&                     \
      defined(NO_THREAD_PRIVATE_MEMORY_STACK))
    GetMemoryManager().Deinitialize();
#endif
}
