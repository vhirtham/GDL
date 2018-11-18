addTest(thread)

addTest(spinlock)

addTest(threadPool
    resources/cpu/threadPoolQueue.cpp
    resources/memory/generalPurposeMemory.cpp
    resources/memory/heapMemory.cpp
    resources/memory/memoryManager.cpp
    resources/memory/memoryPool.cpp
    resources/memory/memoryStack.cpp
    )


addTest(traceBuffer)
