addTest(programDataManagerGL
    rendering/openGL/management/programDataManagerGL.cpp
    rendering/openGL/core/program.cpp
    rendering/openGL/core/shader.cpp
    ${OpenGLTestDefaultLibs}
    )

addTest(managedUniformBufferObjectGL
    rendering/openGL/management/managedUniformBufferObjectGL.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    rendering/openGL/core/program.cpp
    rendering/openGL/core/shader.cpp
    rendering/openGL/core/uniformBufferObjectGL.cpp
    ${OpenGLTestDefaultLibs}
    )
