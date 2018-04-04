function(addBenchmark BenchmarkName)
    ### Add prefix "Benchmark_" to avoid target naming conflicts (for example with tests or other GDL targets)
    set(BenchmarkName "Benchmark_${BenchmarkName}")

    ### Separate sources from libs
    if(ARGN)
        foreach(input ${ARGN})
            string(FIND "${input}" ".cpp" IsCpp)
            if(${IsCpp} EQUAL "-1")
                set(AdditionalLibs
                    "${AdditionalLibs};${input}")
            else()
                set(AdditionalSources
                    "${AdditionalSources};${GDL_SOURCE_DIR}/gdl/${input}")
            endif()
        endforeach()
    endif()

    ### Create executable
    add_executable(${BenchmarkName}
        "${BenchmarkName}.cpp"
        ${AdditionalSources}
        )

    ### link necessary libs
    target_link_libraries(${BenchmarkName}
        benchmark
        ${AdditionalLibs}
        )

    TargetDefaultBuildSetup(${BenchmarkName})
endfunction()
