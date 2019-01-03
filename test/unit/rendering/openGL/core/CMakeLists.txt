addTest(bufferObjects
    rendering/openGL/core/bufferObject.cpp
    ${OpenGLTestDefaultLibs}
    )


if(${GLUT_FOUND})
    addTest(GLUT
        ${OpenGLTestDefaultLibs}
        )
endif()

if(${GLFW_FOUND})
    addTest(GLFW
        ${OpenGLTestDefaultLibs}
        )
endif()

addTest(program
    rendering/openGL/core/program.cpp
    rendering/openGL/core/shader.cpp
    ${OpenGLTestDefaultLibs}
    )


addTest(shader
    rendering/openGL/core/shader.cpp
    ${OpenGLTestDefaultLibs}
    )

addTest(texture
    rendering/openGL/core/texture.cpp
    rendering/textures/textureData.cpp
    ${OpenGLTestDefaultLibs}
    )


addTest(vertexArrayObject
    rendering/openGL/core/vertexArrayObject.cpp
    rendering/openGL/core/bufferObject.cpp
    ${OpenGLTestDefaultLibs}
    )
