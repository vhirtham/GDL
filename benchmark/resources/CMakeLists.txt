addBenchmark(threadPool
    resources/cpu/threadPoolQueue.cpp
    )

addBenchmark(allocators
    resources/memory/memoryManager.cpp
    resources/memory/generalPurposeMemory.cpp
    resources/memory/memoryPool.cpp
    resources/memory/memoryStack.cpp
    resources/memory/heapMemory.cpp
    )
