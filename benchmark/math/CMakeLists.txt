addBenchmark(Mat4)
addBenchmark(MatX)
target_compile_definitions(Benchmark_MatX
    PRIVATE
        -DEIGEN3_FOUND)
