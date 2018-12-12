addTest(contextGLUT
    ${OpenGLTestDefaultSources}
    )

addTest(renderWindowGLUT
    ${OpenGLTestDefaultSources}
    )

addTest(shader
    rendering/openGL/core/shader.cpp
    ${OpenGLTestDefaultSources}
    )

addTest(program
    rendering/openGL/core/program.cpp
    rendering/openGL/core/shader.cpp    
    ${OpenGLTestDefaultSources}
    )

addTest(uniformBufferObjectGL
    rendering/openGL/core/bufferObjectGL.cpp
    rendering/openGL/core/uniformBufferObjectGL.cpp
    ${OpenGLTestDefaultSources}
    )

addTest(bufferObjectGL
    rendering/openGL/core/bufferObjectGL.cpp
    ${OpenGLTestDefaultSources}
    )

addTest(vertexArrayObjectGL
    rendering/openGL/core/vertexArrayObjectGL.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    ${OpenGLTestDefaultSources}
    )
