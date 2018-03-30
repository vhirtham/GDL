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

addTest(managedUniformBufferObject
    rendering/openGL/management/managedUniformBufferObject.cpp
    rendering/openGL/core/programGL.cpp
    rendering/openGL/core/shaderGL.cpp
    rendering/openGL/core/renderWindowGL.cpp
    rendering/openGL/core/uniformBufferObject.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )
