addTest(ThreadPool
    resources/cpu/Thread.cpp
    resources/cpu/ThreadPool.cpp
    resources/cpu/ThreadSafeQueue.cpp)

addTest(threadPool
    resources/cpu/thread.cpp
    resources/cpu/threadPool.cpp
    resources/cpu/threadPoolQueue.cpp
    )

addTest(Allocator)


addTest(allocators
    resources/memory/memoryManager.cpp
    )
addTest(globalNewCounter)
