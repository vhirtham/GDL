#[==[
    Find all external dependencies that are required by library and the enabled options.
#]==]

function(externalDependencies)

    ### BOOST ####################################################################################
    if(ENABLE_TESTS)
        FIND_PACKAGE(Boost REQUIRED unit_test_framework)
    endif()


    ### Google Benchmark ##########################################################################
    if(ENABLE_BENCHMARKS)
        find_package(benchmark)
        if(benchmark_FOUND)
            message(STATUS "Google benchmark found")
            find_package(Eigen3)
            if(Eigen3_FOUND)
                message(STATUS "Eigen3 found - Only for comparison in benchmarks")
            endif()
        else()
            message(STATUS "Google benchmark NOT found - Benchmarks won't be build!")
        endif()
    endif()


endfunction()
