#include "gdl/resources/memory/memoryManager.h"

using namespace GDL;


//! @brief Adds memory systems to the memory manager
//! @return Memory manager
MemoryManager& SetupMemoryManager()
{
    MemoryManager& memoryManager = MemoryManager::Instance();
#ifndef NO_GENERAL_PURPOSE_MEMORY
    memoryManager.CreateGeneralPurposeMemory(1024 * 1024);
#endif
#ifndef NO_MEMORY_POOL
    memoryManager.CreateMemoryPool(32, 1000);
    memoryManager.CreateMemoryPool(64, 1000);
    memoryManager.CreateMemoryPool(128, 1000);
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
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL))
    GetMemoryManager().Initialize();
#endif
}

void DeinitializeMemoryManager()
{
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL))
    GetMemoryManager().Deinitialize();
#endif
}
