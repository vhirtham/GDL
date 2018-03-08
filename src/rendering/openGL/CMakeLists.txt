if(ENABLE_OPENGL)
    add_library(GDL_OpenGL
        programGL
        renderWindowGL.cpp
        shaderGL.cpp
        )
    add_library(GDL::OpenGL ALIAS GDL_OpenGL)
    target_link_libraries(GDL_OpenGL
        PUBLIC
        OpenGL::GL
        OpenGL::GLU
        GLUT::GLUT
        GLEW::GLEW
        )

    target_include_directories(GDL_OpenGL
        PUBLIC
            ${GDL_SOURCE_DIR})
endif()
