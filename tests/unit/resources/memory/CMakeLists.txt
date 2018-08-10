
addTest(globalNewCounter)

addTest(generalPurposeMemory
    resources/memory/generalPurposeMemory.cpp
    )
addTest(memoryPool
    resources/memory/memoryPool.cpp
    )
addTest(memoryStack
    resources/memory/memoryStack.cpp
    )

addTest(memoryManager
    resources/memory/generalPurposeMemory.cpp
    resources/memory/memoryManager.cpp
    resources/memory/memoryPool.cpp
    )

addTest(allocators
    resources/memory/generalPurposeMemory.cpp
    resources/memory/memoryManager.cpp
    resources/memory/memoryPool.cpp
    )

addTest(allocators-no_custom_memory_model
    resources/memory/generalPurposeMemory.cpp
    resources/memory/memoryManager.cpp
    resources/memory/memoryPool.cpp
    -DNO_GENERAL_PURPOSE_MEMORY
    )
