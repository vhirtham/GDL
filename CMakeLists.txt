project(GDL)
cmake_minimum_required(VERSION 3.9)



# COMPILER SETUP
# --------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -Wno-long-long -pedantic -pthread -D_REENTRANT")
IF(ENABLE_AVX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native") #Definition needed to effect all template classes
ENDIF()


# OPTIONS
# -------
OPTION(ENABLE_BENCHMARKS "Enables the benchmarks. This needs a copy of the nonius headers in the folder: extern/nonius" FALSE)
OPTION(ENABLE_CUSTOM_APPS "Enables applications that are located in the applications/custom directory" FALSE)
OPTION(ENABLE_TESTS "Enables the testfiles" TRUE)
OPTION(ENABLE_AVX "Enables advanced vector extensions (SIMD) like _m256 and _m512 registers" TRUE)


IF(ENABLE_TESTS)
    enable_testing()
ENDIF()


# ADD CUSTOM SCRIPTS
# ------------------
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts/cmake ${CMAKE_MODULE_PATH})
include(IncludeScripts)

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
IF(ENABLE_BENCHMARKS) # check if nonius is present
    #INCLUDE_DIRECTORIES("extern") # remove this and make an extra script to create benchmarks that adds the dependency via target_include_directories
    add_subdirectory(benchmark)
ENDIF()


