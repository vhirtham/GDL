addTest(bufferObjects
    rendering/openGL/core/bufferObject.cpp
    rendering/openGL/core/uniformBufferObject.cpp
    ${OpenGLTestDefaultLibs}
    )


addTest(contextGLUT
    ${OpenGLTestDefaultLibs}
    )


addTest(program
    rendering/openGL/core/program.cpp
    rendering/openGL/core/shader.cpp
    ${OpenGLTestDefaultLibs}
    )


addTest(renderWindowGLUT
    ${OpenGLTestDefaultLibs}
    )


addTest(shader
    rendering/openGL/core/shader.cpp
    ${OpenGLTestDefaultLibs}
    )


addTest(vertexArrayObjectGL
    rendering/openGL/core/vertexArrayObjectGL.cpp
    rendering/openGL/core/bufferObject.cpp
    ${OpenGLTestDefaultLibs}
    )
