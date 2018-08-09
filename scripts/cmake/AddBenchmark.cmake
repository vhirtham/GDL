#[==[
This function does all the necessary steps to set up a benchmark
#]==]

function(addBenchmark BenchmarkName)
    ### Add prefix "Benchmark_" to avoid target naming conflicts (for example with tests or other GDL targets)
    set(BenchmarkName "Benchmark_${BenchmarkName}")

    ### Separate passed inputs
    SeparateInputs("${ARGN}")

    ### Add inputs which are relevant for benchmarks
    set(Sources
        ${BenchmarkName}.cpp
        ${Sources})

    set(Libraries
        benchmark
        ${Libraries})

    ### Create executable
    AddExecutable(${BenchmarkName} "${Sources}" "${Libraries}" "${Definitions}")
    TargetDefaultBuildSetup(${BenchmarkName})

endfunction()
