addBenchmark(gauss)
addBenchmark(lu)
addBenchmark(qr)
addBenchmark(solver3)
addBenchmark(solver4)

target_compile_definitions(Benchmark_gauss
    PRIVATE
        ${SOLVER_BENCHMARK_DEFINITIONS})

target_compile_definitions(Benchmark_lu
    PRIVATE
        ${SOLVER_BENCHMARK_DEFINITIONS})

target_compile_definitions(Benchmark_qr
    PRIVATE
        ${SOLVER_BENCHMARK_DEFINITIONS})

if(Eigen3_FOUND)
    target_compile_definitions(Benchmark_solver3
        PRIVATE
            -DEIGEN3_FOUND)

    target_compile_definitions(Benchmark_solver4
        PRIVATE
            -DEIGEN3_FOUND)
endif()
