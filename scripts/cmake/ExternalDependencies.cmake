#[==[
    Find all external dependencies that are required by library and the enabled options.
#]==]

include(FindGLFW)



### BOOST ####################################################################################
if(ENABLE_TESTS)
    FIND_PACKAGE(Boost REQUIRED unit_test_framework)
endif()


### Google Benchmark ##########################################################################
if(ENABLE_BENCHMARKS)
    find_package(benchmark REQUIRED)
    if(benchmark_FOUND)
        message(STATUS "Benchmark found")
        find_package(Eigen3)
        if(Eigen3_FOUND)
            message(STATUS "Eigen3 found - Only for comparison in benchmarks")
        endif()
    else()
        message(STATUS "Google benchmark NOT found - Benchmarks won't be build!")
    endif()
endif()



### OpenGL ####################################################################################
if(ENABLE_OPENGL)
    find_package(OpenGL REQUIRED)
    find_package(GLEW)
    find_package(GLUT)
    findGLFW()

    if(GLUT_FOUND)
        set(GDL_COMPILE_DEFINITIONS  ${GDL_COMPILE_DEFINITIONS} -DGLUT_FOUND)
        message("GLUT found")
    endif()
    if(GLFW_FOUND)
        set(GDL_COMPILE_DEFINITIONS ${GDL_COMPILE_DEFINITIONS} -DGLFW_FOUND)
        message("GLFW found")
    endif()
    if(!${GLUT_FOUND} AND !${GLFW_FOUND})
        message(FATAL_ERROR "ERROR: Could not find GLUT or GLFW")
    endif()
endif()


### QT5 Widgets  ##################################################################################

if(ENABLE_GUI_APPLICATIONS)
    find_package(Qt5Widgets)
    if(Qt5Widgets_FOUND)
        message("Qt5Widgets found")
    endif(Qt5Widgets_FOUND )
endif(ENABLE_GUI_APPLICATIONS)


### Assimp ########################################################################################

find_package(assimp)
if(assimp_FOUND)
    message("Assimp found")
else()
    message("Assimp NOT found")
endif()
