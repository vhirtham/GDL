add_executable(BM_Mat4 math/BM_Mat4.cpp)
target_link_libraries(BM_Mat4 benchmark)
target_include_directories(BM_Mat4 PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_executable(BM_MatX math/BM_MatX.cpp)
target_link_libraries(BM_MatX benchmark)
#target_compile_features(BM_MatX PRIVATE cxx_std_17)
target_compile_definitions(BM_MatX PRIVATE -DEIGEN3_FOUND)
target_include_directories(BM_MatX PUBLIC ${PROJECT_SOURCE_DIR}/src)
