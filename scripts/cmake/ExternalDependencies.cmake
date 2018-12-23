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
        find_package(benchmark REQUIRED)
        if(benchmark_FOUND)
            message(STATUS "Google benchmark found")
            find_package(Eigen3)
            if(Eigen3_FOUND)
                message(STATUS "Eigen3 found - Only for comparison in benchmarks")
                set(Eigen3_FOUND ${Eigen3_FOUND} PARENT_SCOPE)
            endif()
        else()
            message(STATUS "Google benchmark NOT found - Benchmarks won't be build!")
        endif()
    endif()

    ### OpenGL ####################################################################################
    if(ENABLE_OPENGL)
        find_package(OpenGL REQUIRED)
        find_package(GLEW REQUIRED)
        find_package(GLUT REQUIRED)
        findGLFW()

        if(GLUT_FOUND)
            set(GLUT_FOUND ${GLUT_FOUND} CACHE INTERNAL "")
        endif()
        if(!${GLUT_FOUND} AND !${GLFW_FOUND})
            message(FATAL_ERROR "ERROR: Could not find GLUT or GLFW")
        endif()
    endif()

endfunction()
