project(GDL)
cmake_minimum_required(VERSION 3.1)
aux_source_directory(. SRC_LIST)


# OPTIONS
# -------
OPTION(ENABLE_CUSTOM_APPS "Enables applications that are located in the applications/custom directory" FALSE)
OPTION(ENABLE_TESTS "Enables the testfiles" TRUE)

# ADD CUSTOM SCRIPTS
# ------------------
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts/cmake ${CMAKE_MODULE_PATH})
include(IncludeScripts)

# EXTERNAL LIBRARIES
# ------------------
FIND_PACKAGE(Boost REQUIRED unit_test_framework)

# COMPILER SETUP
# --------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall  -Wno-long-long -pedantic -pthread -D_REENTRANT")




# SET INCLUDE DIRECTORIES
# -----------------------
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# SET LIBRARY DIRECTORIES
# -----------------------
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# SUBDIRECTORIES
# --------------
add_subdirectory(applications)
add_subdirectory(src)
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()


# This makes qtcreator show header-only classes...
file(GLOB_RECURSE HeaderFiles "*.h")
add_custom_target(headers SOURCES ${HeaderFiles})
