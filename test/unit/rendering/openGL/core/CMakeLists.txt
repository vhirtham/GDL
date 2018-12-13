addTest(contextGLUT
    ${OpenGLTestDefaultLibs}
    )

addTest(renderWindowGLUT
    ${OpenGLTestDefaultLibs}
    )

addTest(shader
    rendering/openGL/core/shader.cpp
    ${OpenGLTestDefaultLibs}
    )

addTest(program
    rendering/openGL/core/program.cpp
    rendering/openGL/core/shader.cpp    
    ${OpenGLTestDefaultLibs}
    )

addTest(uniformBufferObjectGL
    rendering/openGL/core/bufferObjectGL.cpp
    rendering/openGL/core/uniformBufferObjectGL.cpp
    ${OpenGLTestDefaultLibs}
    )

addTest(bufferObjectGL
    rendering/openGL/core/bufferObjectGL.cpp
    ${OpenGLTestDefaultLibs}
    )

addTest(vertexArrayObjectGL
    rendering/openGL/core/vertexArrayObjectGL.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    ${OpenGLTestDefaultLibs}
    )
