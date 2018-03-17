function(addBenchmark BenchmarkName)
    ### Add prefix "Benchmark_" to avoid target naming conflicts (for example with tests or other GDL targets)
    set(BenchmarkName "Benchmark_${BenchmarkName}")

    ### Create executable
    add_executable(${BenchmarkName}
        ${BenchmarkName}.cpp)

    ### link necessary libs
    target_link_libraries(${BenchmarkName}
        benchmark)

    TargetDefaultBuildSetup(${BenchmarkName})
endfunction()
