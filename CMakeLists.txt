project(GDL)
cmake_minimum_required(VERSION 3.1)
aux_source_directory(. SRC_LIST)

# EXTERNAL LIBRARIES
# ------------------
FIND_PACKAGE(Boost REQUIRED unit_test_framework)

# COMPILER SETUP
# --------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -Wall  -Wno-long-long -pedantic -pthread")




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
add_subdirectory(tests)
add_subdirectory(tutorials)
