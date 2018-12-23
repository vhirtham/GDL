
# Compile cpp files that are shared by all OpenGL tests into a library
# --------------------------------------------------------------------
add_library(OpenGLTestDefaultSourcesLib
    ${GDL_SOURCE_DIR}/rendering/openGL/core/contextManagerGLFW.cpp
    ${GDL_SOURCE_DIR}/rendering/openGL/core/contextGLUT.cpp
    ${GDL_SOURCE_DIR}/rendering/openGL/core/debugMessageHandler.cpp
    ${GDL_SOURCE_DIR}/rendering/openGL/core/glewController.cpp
    ${GDL_SOURCE_DIR}/rendering/openGL/core/renderWindowGLUT.cpp
    ${GDL_SOURCE_DIR}/rendering/openGL/core/renderWindowGLFW.cpp
    )
TargetDefaultBuildSetup(OpenGLTestDefaultSourcesLib)


# Collect libraries that are needed by all OpenGL tests
# -----------------------------------------------------
set(OpenGLTestDefaultLibs
    OpenGLTestDefaultSourcesLib
    GDL::Resources
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    ${GLFW_LIBRARIES}
    ${GLFW_DEPENDENCIES}
    )

add_subdirectory(core)
#add_subdirectory(management)
