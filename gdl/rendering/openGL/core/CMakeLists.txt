Set(ConfigDependentSources "")

if(${GLUT_FOUND})
    Set(ConfigDependentSources
        ${ConfigDependentSources}
        contextManagerGLUT.cpp
        renderWindowGLUT.cpp
        )
endif()

if(${GLFW_FOUND})
    Set(ConfigDependentSources
        ${ConfigDependentSources}
        contextManagerGLFW.cpp
        renderWindowGLFW.cpp
        )
endif()


addGDLLib(OpenGLCore
    bufferObject.cpp
    debugMessageHandler.cpp
    glewController.cpp
    program.cpp
    shader.cpp
    vertexArrayObject.cpp
    ${ConfigDependentSources}
    GDL::Resources

    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    ${GLFW_LIBRARIES}
)

