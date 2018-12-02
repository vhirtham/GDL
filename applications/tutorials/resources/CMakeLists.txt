add_executable(Tutorial_1-1
    tutorial_1.1_memoryManagement.cpp)

target_link_libraries(Tutorial_1-1
    GDL::Resources
    )

TargetDefaultBuildSetup(Tutorial_1-1)


add_executable(Tutorial_1-2
    tutorial_1.2_multiThreading.cpp)

target_link_libraries(Tutorial_1-2
    GDL::Resources
    )

TargetDefaultBuildSetup(Tutorial_1-2)
