
addTest(heapAllocationCounter)

addTest(generalPurposeMemory
    resources/memory/generalPurposeMemory.cpp
    )
addTest(memoryPool
    resources/memory/memoryPool.cpp
    )
addTest(memoryStack
    resources/memory/memoryStack.cpp
    )

set(MemoryManagerSources
    resources/memory/generalPurposeMemory.cpp
    resources/memory/heapMemory.cpp
    resources/memory/memoryManager.cpp
    resources/memory/memoryPool.cpp
    resources/memory/memoryStack.cpp
    )

addTest(memoryManager
    ${MemoryManagerSources}
    )

addTest(allocators
    ${MemoryManagerSources}
    )

addTest(allocators-no_pool_memory
    ${MemoryManagerSources}
    -DNO_MEMORY_POOL
    )

addTest(allocators-no_custom_memory_model
    ${MemoryManagerSources}
    -DNO_GENERAL_PURPOSE_MEMORY
    -DNO_MEMORY_POOL
    )
