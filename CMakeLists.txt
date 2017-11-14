project(GDL)
cmake_minimum_required(VERSION 3.9)


# OPTIONS
# -------
OPTION(ENABLE_CUSTOM_APPS "Enables applications that are located in the applications/custom directory" FALSE)
OPTION(ENABLE_TESTS "Enables the testfiles" TRUE)
OPTION(ENABLE_ADVANCED_SSE "Enables newer SSE features and bigger registers like _m256 and _m512" TRUE)


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

# COMPILER SETUP
# --------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -Wno-long-long -pedantic -pthread -D_REENTRANT")
IF(ENABLE_ADVANCED_SSE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native") #Definition needed to effect all template classes
ENDIF()




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


