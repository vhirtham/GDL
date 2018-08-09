
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
    )

addTest(allocator
    resources/memory/generalPurposeMemory.cpp
    resources/memory/memoryManager.cpp
    )

addTest(allocator-no_custom_memory_model
    resources/memory/generalPurposeMemory.cpp
    resources/memory/memoryManager.cpp
    -DNO_GENERAL_PURPOSE_MEMORY
    )
