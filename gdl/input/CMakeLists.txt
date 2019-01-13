Set(ConfigDependentSources "")
if(${GLUT_FOUND})
    Set(ConfigDependentSources
        ${ConfigDependentSources}
        keyMappingGLUT.cpp
        )
endif()

if(${GLFW_FOUND})
    Set(ConfigDependentSources
        ${ConfigDependentSources}
        keyMappingGLFW.cpp
        )
endif()


addGDLLib(Input
    inputState.cpp
    ${ConfigDependentSources}
    #GDL::Resources
)
