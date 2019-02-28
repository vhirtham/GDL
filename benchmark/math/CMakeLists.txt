addBenchmark(Mat4)
addBenchmark(Mat)
addBenchmark(solver3)

if(Eigen3_FOUND)
    target_compile_definitions(Benchmark_Mat4
        PRIVATE
            -DEIGEN3_FOUND)
    target_compile_definitions(Benchmark_Mat
        PRIVATE
            -DEIGEN3_FOUND)
    target_compile_definitions(Benchmark_solver3
        PRIVATE
            -DEIGEN3_FOUND)

endif()
