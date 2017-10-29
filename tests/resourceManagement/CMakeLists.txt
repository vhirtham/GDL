add_unit_test(TEST_ThreadPool
    TestThreadPool.cpp
    ../../src/resourceManagement/ThreadPool.cpp)

ADD_EXECUTABLE(TEST_ThreadPool TestThreadPool.cpp
        ../../src/resourceManagement/ThreadPool.cpp
        ../../src/resourceManagement/ThreadSafeQueue.cpp)
target_link_libraries(TEST_ThreadPool
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
ADD_TEST(Unit::ThreadPool ${CMAKE_CURRENT_BUILD_DIR}/TESTThreadPool)

