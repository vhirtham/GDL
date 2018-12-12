set(OpenGLTestDefaultSources
    rendering/openGL/core/contextGLUT.cpp
    rendering/openGL/core/GLEWController.cpp
    rendering/openGL/core/renderWindowGLUT.cpp
    GDL::Resources
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

add_subdirectory(core)
add_subdirectory(management)
