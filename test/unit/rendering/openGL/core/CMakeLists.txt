addTest(renderWindowGL
    rendering/openGL/core/renderWindowGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(shaderGL
    rendering/openGL/core/shaderGL.cpp
    rendering/openGL/core/renderWindowGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(programGL
    rendering/openGL/core/programGL.cpp
    rendering/openGL/core/shaderGL.cpp
    rendering/openGL/core/renderWindowGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(uniformBufferObjectGL
    rendering/openGL/core/renderWindowGL.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    rendering/openGL/core/uniformBufferObjectGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(bufferObjectGL
    rendering/openGL/core/renderWindowGL.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(vertexArrayObjectGL
    rendering/openGL/core/renderWindowGL.cpp
    rendering/openGL/core/vertexArrayObjectGL.cpp
    rendering/openGL/core/bufferObjectGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )
