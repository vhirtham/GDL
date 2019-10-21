project(GDL)
cmake_minimum_required(VERSION 3.9)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(GDL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GDL_SOURCE_DIR ${GDL_ROOT_DIR}/gdl)

set(GDL_COMPILE_FEATURES cxx_std_17)
set(GDL_COMPILE_DEFINITIONS "")


# COMPILER SETUP
# --------------
# https://stackoverflow.com/questions/32940860/gcc-optimization-levels-which-is-better
# optional:  -funroll-loops
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Wpedantic -Wmissing-braces -pthread ")

# Options
# -------
option(ENABLE_ALLOCATORS "Enable allocators which replace the std::allocator for all GDL containers" TRUE)
option(ENABLE_BENCHMARKS "Enables the benchmarks" FALSE)
option(ENABLE_CUSTOM_APPS "Enables applications that are located in the applications/custom directory" FALSE)
option(ENABLE_DEV_TOOLS "Enables extea tools for the development process" TRUE)
option(ENABLE_OPENGL "Enables OpenGL API" FALSE)
option(ENABLE_GUI_APPLICATIONS "Enables QT GUI applications" FALSE)
option(ENABLE_TESTS "Enables the testfiles" TRUE)
option(ENABLE_VULKAN "Enables Vulkan API" FALSE)



if(NOT ENABLE_ALLOCATORS)
    set(GDL_COMPILE_DEFINITIONS -DUSE_STD_ALLOCATOR ${GDL_COMPILE_DEFINITIONS})
    message(STATUS "Allocators disabled")
else()
    message(STATUS "Allocators enabled")
endif()

if(NOT ENABLE_DEV_TOOLS)
    set(GDL_COMPILE_DEFINITIONS -DNDEVEXCEPTION ${GDL_COMPILE_DEFINITIONS})
    message(STATUS "Development tools disabled")
else()
    message(STATUS "Development tools enabled")
endif()

if(ENABLE_TESTS)
    enable_testing()
endif()


# ADD CUSTOM SCRIPTS
# ------------------
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts/cmake ${CMAKE_MODULE_PATH})
include(IncludeScripts)


# SUBDIRECTORIES
# --------------
add_subdirectory(gdl)

add_subdirectory(applications)
if(ENABLE_TESTS)
    add_subdirectory(test)
endif()
if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmark)
endif()


