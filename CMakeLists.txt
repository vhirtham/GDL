project(GDL)
cmake_minimum_required(VERSION 3.9)
set(CMAKE_VERBOSE_MAKEFILE OFF)


# COMPILER SETUP
# --------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -Wall -Wno-long-long -pedantic -pthread -D_REENTRANT ")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

# Options
# -------
option(ENABLE_BENCHMARKS "Enables the benchmarks" FALSE)
option(ENABLE_CUSTOM_APPS "Enables applications that are located in the applications/custom directory" FALSE)
option(ENABLE_TESTS "Enables the testfiles" TRUE)
option(ENABLE_ADVANCED_SSE "Enables advanced vector extensions (SIMD) like _m256 and _m512 registers" FALSE)

if(ENABLE_ADVANCED_SSE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -march=native -mavx2 ") #Definition needed to replace pure AVX classes by backup versions
    add_definitions(-DENABLE_SSE4)
    add_definitions(-DENABLE_AVX)
endif()



if(ENABLE_TESTS)
    enable_testing()
endif()


# ADD CUSTOM SCRIPTS
# ------------------
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts/cmake ${CMAKE_MODULE_PATH})
include(IncludeScripts)


# EXTERNAL LIBRARIES
# ------------------
externalDependencies()



# SUBDIRECTORIES
# --------------
add_subdirectory(applications)
add_subdirectory(src)
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()
if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmark)
endif()


