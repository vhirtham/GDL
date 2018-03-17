if(ENABLE_OPENGL)

    addGDLLib(OpenGL
        programGL.cpp
        renderWindowGL.cpp
        shaderGL.cpp

        # External libs
        OpenGL::GL
        OpenGL::GLU
        GLUT::GLUT
        GLEW::GLEW
        )

endif()
