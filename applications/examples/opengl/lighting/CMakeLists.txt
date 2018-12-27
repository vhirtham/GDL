add_executable(DiffusePerVertexShading
    diffusePerVertexShading.cpp)

target_link_libraries(DiffusePerVertexShading
    GDL::OpenGLCore
    )

TargetDefaultBuildSetup(DiffusePerVertexShading)
