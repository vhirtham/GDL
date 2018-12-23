addGDLLib(OpenGLCore
    bufferObject.cpp
    contextGLUT.cpp
    contextManagerGLFW.cpp
    debugMessageHandler.cpp
    glewController.cpp
    program.cpp
    renderWindowGLFW.cpp
    renderWindowGLUT.cpp
    shader.cpp
    vertexArrayObject.cpp

    GDL::Resources
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    ${GLFW_LIBRARIES}
    ${GLFW_DEPENDENCIES}
)

