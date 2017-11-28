addBenchmark(Mat4)
addBenchmark(MatX)
if(Eigen3_FOUND)
    target_compile_definitions(Benchmark_MatX
        PRIVATE
            -DEIGEN3_FOUND)
endif()
