addTest(renderWindowGL
    rendering/openGL/renderWindowGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(shaderGL
    rendering/openGL/shaderGL.cpp
    rendering/openGL/renderWindowGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )

addTest(programGL
    rendering/openGL/programGL.cpp
    rendering/openGL/shaderGL.cpp
    rendering/openGL/renderWindowGL.cpp
    # External libs
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    )
