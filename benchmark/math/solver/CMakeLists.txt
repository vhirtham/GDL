addBenchmark(gauss)

if(Eigen3_FOUND)
    target_compile_definitions(Benchmark_gauss
        PRIVATE
            -DEIGEN3_FOUND)

endif()
