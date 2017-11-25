add_executable(BM_Mat4 math/BM_Mat4.cpp)
target_link_libraries(BM_Mat4 benchmark)

add_executable(BM_MatX math/BM_MatX.cpp)
target_link_libraries(BM_MatX benchmark)
target_compile_features(BM_MatX PRIVATE cxx_std_17)
