add_subdirectory(cookbook)

add_executable(FragmentShaderIntegrationGL
    fragmentShaderIntegrationGL.cpp)

target_link_libraries(FragmentShaderIntegrationGL
    GDL::OpenGLCore
    GDL::OpenGLManagement
    )

TargetDefaultBuildSetup(FragmentShaderIntegrationGL)
