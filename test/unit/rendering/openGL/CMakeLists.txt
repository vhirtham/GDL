
# Compile cpp files that are shared by all OpenGL tests into a library
# --------------------------------------------------------------------

Set(ConfigDependentSources "")
if(${GLUT_FOUND})
    Set(ConfigDependentSources
        ${ConfigDependentSources}
        ${GDL_SOURCE_DIR}/rendering/openGL/core/contextManagerGLUT.cpp
        ${GDL_SOURCE_DIR}/rendering/openGL/core/renderWindowGLUT.cpp
        )
endif()

if(${GLFW_FOUND})
    Set(ConfigDependentSources
        ${ConfigDependentSources}
        ${GDL_SOURCE_DIR}/rendering/openGL/core/contextManagerGLFW.cpp
        ${GDL_SOURCE_DIR}/rendering/openGL/core/renderWindowGLFW.cpp
        )
endif()


add_library(OpenGLTestDefaultSourcesLib
    ${GDL_SOURCE_DIR}/rendering/openGL/core/debugMessageHandler.cpp
    ${GDL_SOURCE_DIR}/rendering/openGL/core/glewController.cpp
    ${ConfigDependentSources}
    )
TargetDefaultBuildSetup(OpenGLTestDefaultSourcesLib)


# Collect libraries that are needed by all OpenGL tests
# -----------------------------------------------------
set(OpenGLTestDefaultLibs
    OpenGLTestDefaultSourcesLib
    GDL::Input
    GDL::Resources
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    GLEW::GLEW
    ${GLFW_LIBRARIES}
    )

add_subdirectory(core)
#add_subdirectory(management)
