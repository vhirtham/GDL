add_subdirectory(sse)

addTest(approx)
addTest(freeFunctions)
addTest(insideTolerance)
addTest(outputFile)

addTest(string
    resources/memory/generalPurposeMemory.cpp
    resources/memory/heapMemory.cpp
    resources/memory/memoryManager.cpp
    resources/memory/memoryPool.cpp
    resources/memory/memoryStack.cpp)

addTest(timer)
addTest(transpose)
