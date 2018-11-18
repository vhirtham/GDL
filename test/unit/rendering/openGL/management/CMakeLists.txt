addTest(programDataManagerGL
    rendering/openGL/management/programDataManagerGL.cpp
    rendering/openGL/core/programGL.cpp
    rendering/openGL/core/shaderGL.cpp
    rendering/openGL/core/renderWindowGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(managedUniformBufferObjectGL
    rendering/openGL/management/managedUniformBufferObjectGL.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    rendering/openGL/core/programGL.cpp
    rendering/openGL/core/shaderGL.cpp
    rendering/openGL/core/renderWindowGL.cpp
    rendering/openGL/core/uniformBufferObjectGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )
