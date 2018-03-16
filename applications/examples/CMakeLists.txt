if(ENABLE_OPENGL)
    add_executable(TriangleOpenGL opengl/TriangleOpenGL)

    target_link_libraries(TriangleOpenGL
        GDL::OpenGL
        )

    target_compile_features(TriangleOpenGL PUBLIC cxx_std_17)

endif()
