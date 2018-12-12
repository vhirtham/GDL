addTest(programDataManagerGL
    rendering/openGL/management/programDataManagerGL.cpp
    rendering/openGL/core/program.cpp
    rendering/openGL/core/shader.cpp
    rendering/openGL/core/renderWindowGLUT.cpp
    GDL::Resources
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(managedUniformBufferObjectGL
    rendering/openGL/management/managedUniformBufferObjectGL.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    rendering/openGL/core/program.cpp
    rendering/openGL/core/shader.cpp
    rendering/openGL/core/renderWindowGLUT.cpp
    rendering/openGL/core/uniformBufferObjectGL.cpp
    GDL::Resources
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )
