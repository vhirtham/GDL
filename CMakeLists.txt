project(GDL)
cmake_minimum_required(VERSION 3.1)
aux_source_directory(. SRC_LIST)

# EXTERNAL LIBRARIES
# ------------------


# COMPILER SETUP
# --------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall  -Wno-long-long -pedantic")




# SET INCLUDE DIRECTORIES
# -----------------------
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})


# SUBDIRECTORIES
# --------------
add_subdirectory(applications)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(tutorials)
