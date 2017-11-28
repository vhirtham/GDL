project(GDL)
cmake_minimum_required(VERSION 3.9)
#SET(CMAKE_VERBOSE_MAKEFILE ON)


# COMPILER SETUP
# --------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -Wno-long-long -pedantic -pthread -D_REENTRANT")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

# OPTIONS
# -------
OPTION(ENABLE_BENCHMARKS "Enables the benchmarks" FALSE)
OPTION(ENABLE_CUSTOM_APPS "Enables applications that are located in the applications/custom directory" FALSE)
OPTION(ENABLE_TESTS "Enables the testfiles" TRUE)
OPTION(ENABLE_AVX "Enables advanced vector extensions (SIMD) like _m256 and _m512 registers" TRUE)

IF(ENABLE_AVX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mavx2 ") #Definition needed to replace pure AVX classes by backup versions
ENDIF()



IF(ENABLE_TESTS)
    enable_testing()
ENDIF()


# ADD CUSTOM SCRIPTS
# ------------------
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts/cmake ${CMAKE_MODULE_PATH})
include(IncludeScripts)
externalDependencies()

# EXTERNAL LIBRARIES
# ------------------
FIND_PACKAGE(Boost REQUIRED unit_test_framework)





# SET INCLUDE DIRECTORIES
# -----------------------
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)


# SUBDIRECTORIES
# --------------
add_subdirectory(applications)
add_subdirectory(src)
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()
IF(ENABLE_BENCHMARKS)
    add_subdirectory(benchmark)
ENDIF()


