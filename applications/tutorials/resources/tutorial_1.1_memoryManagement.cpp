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

    memoryManager.Deinitialize();

    return 0;
}
