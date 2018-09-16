addBenchmark(threadPool
    resources/cpu/threadPoolQueue.cpp
    resources/memory/generalPurposeMemory.cpp
    resources/memory/heapMemory.cpp
    resources/memory/memoryManager.cpp
    resources/memory/memoryPool.cpp
    resources/memory/memoryStack.cpp
    )

addBenchmark(allocators
    resources/memory/memoryManager.cpp
    resources/memory/generalPurposeMemory.cpp
    resources/memory/memoryPool.cpp
    resources/memory/memoryStack.cpp
    resources/memory/heapMemory.cpp
    )
