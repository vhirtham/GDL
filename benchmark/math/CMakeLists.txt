add_subdirectory(solver)

addBenchmark(mat4)
addBenchmark(mat)


if(Eigen3_FOUND)
    target_compile_definitions(Benchmark_mat4
        PRIVATE
            -DEIGEN3_FOUND)
    target_compile_definitions(Benchmark_mat
        PRIVATE
            -DEIGEN3_FOUND)
endif()
