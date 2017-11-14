ADD_EXECUTABLE(TEST_Mat4 TestMat4.cpp)

target_link_libraries(TEST_Mat4
    Boost::unit_test_framework
    )

target_include_directories(TEST_Mat4
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src)

ADD_TEST(Unit::Mat4 TEST_Mat4)
