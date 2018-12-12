addTest(renderWindowGLUT
    rendering/openGL/core/renderWindowGLUT.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(shader
    rendering/openGL/core/shader.cpp
    rendering/openGL/core/renderWindowGLUT.cpp
    GDL::Resources
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(program
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

addTest(uniformBufferObjectGL
    rendering/openGL/core/renderWindowGLUT.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    rendering/openGL/core/uniformBufferObjectGL.cpp
    GDL::Resources
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(bufferObjectGL
    rendering/openGL/core/renderWindowGLUT.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    GDL::Resources
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(vertexArrayObjectGL
    rendering/openGL/core/renderWindowGLUT.cpp
    rendering/openGL/core/vertexArrayObjectGL.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    GDL::Resources
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )
