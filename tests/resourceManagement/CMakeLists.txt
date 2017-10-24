ADD_EXECUTABLE(TEST_ThreadPool TestThreadPool.cpp)
target_link_libraries(TEST_ThreadPool
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
ADD_TEST(Unit::ThreadPool ${CMAKE_CURRENT_BUILD_DIR}/TESTThreadPool)

