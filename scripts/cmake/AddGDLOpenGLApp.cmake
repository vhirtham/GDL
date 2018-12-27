#[==[
Creates an executable with GDL OpenGL support
#]==]

function(addGDLOpenGLApp AppName)

    ### Separate passed inputs
    if(ARGN)
        foreach(Input ${ARGN})
            string(FIND "${Input}" ".cpp" FindCppResult)
            if(${FindCppResult} EQUAL "-1")
                string(FIND ${Input} "-D" FindDefResult)
                if(NOT ${FindDefResult} EQUAL "-1")
                    set(Definitions
                        "${Definitions};${Input}")
                else()
                    set(Libraries
                        "${Libraries};${Input}")
                endif()
            else()
                set(Sources
                    "${Sources};${CMAKE_CURRENT_SOURCE_DIR}/${Input}")
            endif()
        endforeach()
    endif()

    ### Add inputs which are relevant for tests
    set(Sources
        ${Sources})

    set(Libraries
        ${Libraries}
        GDL::OpenGLCore)

    set(Definitions
        PRIVATE
        ${Definitions})


    AddExecutable(${AppName} "${Sources}" "${Libraries}" "${Definitions}")
    TargetDefaultBuildSetup(${AppName})

endfunction()
