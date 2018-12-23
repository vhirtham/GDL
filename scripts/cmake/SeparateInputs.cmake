#[==[
Creates an executable and adds the passed sources, libs and definitions
#]==]

function(SeparateInputs Inputs)
    set(Definitions "PUBLIC")

    foreach(Input ${Inputs})
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
                "${Sources};${GDL_SOURCE_DIR}/${Input}")
        endif()
    endforeach()
    set(Sources ${Sources} PARENT_SCOPE)
    set(Definitions ${Definitions} PARENT_SCOPE)
    set(Libraries ${Libraries} PARENT_SCOPE)
endfunction()
