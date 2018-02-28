if(ENABLE_OPENGL)
    add_executable(TriangleOpenGL opengl/TriangleOpenGL)

    target_link_libraries(TriangleOpenGL
        OpenGL::GL
        GLUT::GLUT
        GLEW::GLEW
        )
endif()
