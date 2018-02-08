set(GDL_RESOURCE_MANAGEMENT_SOURCES
    resourceManagement/Thread.cpp
    resourceManagement/ThreadPool.cpp
    resourceManagement/ThreadSafeQueue.cpp
    )

add_library(GDLResourceManagement
    ${GDL_RESOURCE_MANAGEMENT_SOURCES})

target_include_directories(GDLResourceManagement
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(GDLResourceManagement
    PUBLIC
        cxx_std_14)

createObjectLibs("${GDL_RESOURCE_MANAGEMENT_SOURCES}")

