#[==[
This function does all the necessary steps to set up a GDL library
#]==]

function(AddGDLLib libName)

    ### Separate sources from libs
    if(ARGN)
        foreach(input ${ARGN})
            string(FIND "${input}" ".cpp" IsCpp)
            if(${IsCpp} EQUAL "-1")
                set(additionalLibs
                    "${additionalLibs};${input}")
            else()
                set(libSources
                    "${libSources};${CMAKE_CURRENT_SOURCE_DIR}/${input}")
            endif()
        endforeach()
    endif()

    ### Add "GDL_" to library name to avoid naming conflicts with external libs (for example OpenGL and GDL_OpenGL)
    set(gdlLibName "GDL_${libName}")

    ### Create library
    add_library(${gdlLibName}
        ${libSources}
        )

    target_link_libraries(${gdlLibName}
        PUBLIC
            ${additionalLibs}
        )

    TargetDefaultBuildSetup(${gdlLibName})

    ### Set alias
    add_library(GDL::${libName} ALIAS ${gdlLibName})

endfunction()
