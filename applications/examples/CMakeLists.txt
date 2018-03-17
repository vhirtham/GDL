if(ENABLE_OPENGL)
    add_executable(TriangleOpenGL
        opengl/TriangleOpenGL.cpp)

    target_link_libraries(TriangleOpenGL
        GDL::OpenGL
        )

    TargetDefaultBuildSetup(TriangleOpenGL)

endif()
