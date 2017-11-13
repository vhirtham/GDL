add_unit_test(TEST_ThreadPool
    TestThreadPool.cpp
    ../../src/resourceManagement/ThreadPool.cpp)

ADD_EXECUTABLE(TEST_ThreadPool TestThreadPool.cpp
        ../../../src/resourceManagement/Thread.cpp
        ../../../src/resourceManagement/ThreadPool.cpp
        ../../../src/resourceManagement/ThreadSafeQueue.cpp)
target_link_libraries(TEST_ThreadPool
    Boost::unit_test_framework
    )
target_include_directories(TEST_ThreadPool
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src)
ADD_TEST(Unit::ThreadPool TEST_ThreadPool)

