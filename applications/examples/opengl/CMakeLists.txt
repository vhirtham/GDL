add_subdirectory(cookbook)
add_subdirectory(basics)
add_subdirectory(lighting)

add_executable(FragmentShaderIntegrationGL
    fragmentShaderIntegrationGL.cpp)

target_link_libraries(FragmentShaderIntegrationGL
    GDL::OpenGLCore
    )

TargetDefaultBuildSetup(FragmentShaderIntegrationGL)
